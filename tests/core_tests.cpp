#include "msdos/fat12.hpp"
#include "msdos/fcb_host.hpp"
#include "ports.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
template<class F> void rejects(F&& action, const char* message) {
    try { action(); } catch (const std::exception&) { return; }
    throw std::runtime_error(message);
}

void registers_and_memory() {
    msdos::Machine m;
    using msdos::Reg;
    m.set(Reg::AX, 0x1234); m.set(Reg::AH, 0xab); m.set(Reg::AL, 0xcd);
    check(m.get(Reg::AX) == 0xabcd, "byte registers must alias word registers");
    m.set(Reg::AX, 0x12345); check(m.get(Reg::AX) == 0x2345, "word writes must truncate");
    m.write16(0xffff, 0xf, 0xabcd);
    check(m.read8(0xffff, 0xf) == 0xcd && m.read8(0, 0) == 0xab, "20-bit address wrap");
    m.write16(0x2000, 0xffff, 0x1234);
    check(m.read8(0x3000, 0) == 0x12, "word memory transfer follows physical bus");
    m.write8(0x1234, 0x5678, 0x98); check(m.read8(0x179b, 8) == 0x98, "segment aliases");
    m.set(Reg::SS, 0x3000); m.set(Reg::SP, 0); m.push(0xbeef);
    check(m.get(Reg::SP) == 0xfffe && m.pop() == 0xbeef && m.get(Reg::SP) == 0, "stack wrapping");
    rejects([&] { m.read(32, 0, 0); }, "invalid operand width must fail");
}

bool even_parity(unsigned n) {
    unsigned count = 0;
    for (unsigned bit = 0; bit < 8; ++bit) count += (n >> bit) & 1;
    return count % 2 == 0;
}

void arithmetic() {
    msdos::Machine m;
    for (unsigned a = 0; a < 256; ++a) for (unsigned b = 0; b < 256; ++b) {
        for (unsigned carry = 0; carry < 2; ++carry) {
            const int sa = a < 128 ? static_cast<int>(a) : static_cast<int>(a) - 256;
            const int sb = b < 128 ? static_cast<int>(b) : static_cast<int>(b) - 256;
            const auto sum = a + b + carry;
            check(m.add(a, b, 8, carry != 0) == (sum & 255), "8-bit addition result");
            check(m.flags.carry == (sum > 255), "addition carry");
            check(m.flags.overflow == (sa + sb + static_cast<int>(carry) < -128 || sa + sb + static_cast<int>(carry) > 127), "addition overflow");
            check(m.flags.auxiliary == ((a & 15) + (b & 15) + carry > 15), "addition auxiliary carry");
            check(m.flags.parity == even_parity(sum) && m.flags.zero == ((sum & 255) == 0) &&
                m.flags.sign == ((sum & 128) != 0), "addition status flags");
            const int difference = static_cast<int>(a) - static_cast<int>(b) - static_cast<int>(carry);
            check(m.sub(a, b, 8, carry != 0) == (static_cast<unsigned>(difference) & 255), "8-bit subtraction result");
            check(m.flags.carry == (a < b + carry), "subtraction borrow");
            check(m.flags.overflow == (sa - sb - static_cast<int>(carry) < -128 || sa - sb - static_cast<int>(carry) > 127), "subtraction overflow");
            check(m.flags.auxiliary == ((a & 15) < (b & 15) + carry), "subtraction auxiliary borrow");
        }
    }
    check(m.add(0xffff, 1, 16) == 0 && m.flags.carry && m.flags.zero, "16-bit carry");
    check(m.add(0x7fff, 1, 16) == 0x8000 && m.flags.overflow, "16-bit signed overflow");
    check(m.sub(0x8000, 1, 16) == 0x7fff && m.flags.overflow, "16-bit subtraction overflow");
    m.flags.carry = true;
    check(m.inc(255, 8) == 0 && m.flags.carry, "INC preserves carry");
    m.flags.carry = false;
    check(m.dec(0, 16) == 0xffff && !m.flags.carry, "DEC preserves carry");
    check(m.neg(0x8000, 16) == 0x8000 && m.flags.overflow && m.flags.carry, "NEG minimum signed value");
    check(m.neg(0, 8) == 0 && !m.flags.carry, "NEG zero");
}

void shifts_and_strings() {
    msdos::Machine m;
    using msdos::Reg; using msdos::Shift;
    m.flags.carry = true; m.flags.zero = true;
    check(m.shift(0x80, 0, 8, Shift::Shl) == 0x80 && m.flags.carry && m.flags.zero, "zero shift preserves flags");
    check(m.shift(0x80, 1, 8, Shift::Shl) == 0 && m.flags.carry && m.flags.overflow, "SHL flags");
    check(m.shift(0x80, 1, 8, Shift::Sar) == 0xc0 && !m.flags.overflow, "arithmetic right shift");
    check(m.shift(1, 32, 16, Shift::Shl) == 0, "8086 does not mask counts to five bits");
    m.flags.carry = true;
    check(m.shift(0x80, 1, 8, Shift::Rcl) == 1 && m.flags.carry, "rotate through carry");
    m.flags.carry = true;
    check(m.shift(1, 1, 8, Shift::Rcr) == 0x80 && m.flags.carry, "rotate right through carry");
    m.set(Reg::DS, 0x1000); m.set(Reg::ES, 0x2000); m.set(Reg::SI, 7); m.set(Reg::DI, 9);
    m.write8(0x1000, 7, 0x62); m.movs(8);
    check(m.read8(0x2000, 9) == 0x62 && m.get(Reg::SI) == 8 && m.get(Reg::DI) == 10, "MOVSB direction up");
    m.flags.direction = true; m.set(Reg::SI, 0); m.write16(0x1000, 0, 0x1234); m.lods(16);
    check(m.get(Reg::AX) == 0x1234 && m.get(Reg::SI) == 0xfffe, "LODSW direction down and wrapping");
    m.set(Reg::DI, 0); m.stos(16);
    check(m.read16(0x2000, 0) == 0x1234 && m.get(Reg::DI) == 0xfffe, "STOSW direction down");
}

void hexchk() {
    msdos::Machine m;
    using msdos::Reg;
    for (unsigned ch = 0; ch < 256; ++ch) {
        m.set(Reg::AX, 0xaa00 | ch);
        msdos::v125::hexchk(m);
        // The original subtraction sequence also accepts ':' through '@'.
        // Preserve that historical quirk rather than silently repairing it.
        const bool valid = ch >= '0' && ch <= 'F';
        check(m.flags.carry == !valid, "lifted HEXCHK carry for every input byte");
        if (valid) check(m.get(Reg::AL) == (ch <= '9' ? ch - '0' : ch - 'A' + 10), "HEXCHK decoded digit");
        check(m.get(Reg::AH) == 0xaa && m.get(Reg::SP) == 0xfffe, "HEXCHK register/stack contract");
    }
}

std::uint16_t bit_entry(const std::vector<std::uint8_t>& bytes, std::size_t cluster) {
    unsigned value = 0;
    for (unsigned bit = 0; bit < 12; ++bit) {
        const auto location = cluster * 12 + bit;
        value |= ((bytes[location / 8] >> (location % 8)) & 1U) << bit;
    }
    return static_cast<std::uint16_t>(value);
}

void fat12() {
    std::vector<std::uint8_t> table(6144, 0xa5);
    msdos::Machine m;
    using msdos::Reg;
    m.set(Reg::DS, 0x1000); m.set(Reg::SS, 0x1000);
    for (unsigned value = 0; value < 4096; ++value) {
        for (unsigned cluster : {2U, 3U}) {
            const auto before = bit_entry(table, cluster ^ 1U);
            msdos::fat12_set(table, cluster, static_cast<std::uint16_t>(value));
            check(bit_entry(table, cluster) == value && msdos::fat12_get(table, cluster) == value, "FAT12 packing matches independent bit oracle");
            check(bit_entry(table, cluster ^ 1U) == before, "FAT12 neighboring nibble preserved");
        }
    }
    for (unsigned cluster = 0; cluster < 4096; ++cluster) {
        const auto value = (cluster * 197U) & 0xfffU;
        msdos::fat12_set(table, cluster, static_cast<std::uint16_t>(value));
    }
    for (unsigned i = 0; i < table.size(); ++i) m.write8(0x1000, static_cast<std::uint16_t>(0x2000 + i), table[i]);
    for (unsigned cluster = 0; cluster < 4096; ++cluster) {
        m.set(Reg::BX, cluster); m.set(Reg::DX, cluster ^ 0xfffU); m.set(Reg::SI, 0x2000);
        msdos::v125::fat_pack(m);
        msdos::fat12_set(table, cluster, static_cast<std::uint16_t>(cluster ^ 0xfffU));
    }
    for (unsigned i = 0; i < table.size(); ++i)
        check(m.read8(0x1000, static_cast<std::uint16_t>(0x2000 + i)) == table[i], "lifted PACK matches C++ FAT12 for every cluster");
    rejects([&] { msdos::fat12_get(table, std::numeric_limits<std::size_t>::max()); }, "cluster overflow rejected");
    rejects([&] { msdos::fat12_set(table, 2, 0x1000); }, "oversized FAT value rejected");
    std::array<std::uint8_t, 1> truncated{};
    rejects([&] { msdos::fat12_get(truncated, 0); }, "truncated entry rejected");
}

void vectors_and_dsum() {
    msdos::Machine m;
    using msdos::Reg;
    for (unsigned vector = 0; vector < 256; ++vector) {
        m.set(Reg::AL, vector); m.set(Reg::DS, 0xabcd); m.set(Reg::DX, 0x1234);
        msdos::v125::setvect(m);
        check(m.read16(0, static_cast<std::uint16_t>(vector * 4)) == 0x1234 &&
            m.read16(0, static_cast<std::uint16_t>(vector * 4 + 2)) == 0xabcd, "interrupt vector far pointer");
    }
    const std::array<unsigned, 12> months{31,28,31,30,31,30,31,31,30,31,30,31};
    m.set(Reg::DS, 0x1000);
    for (unsigned i = 0; i < months.size(); ++i) m.write8(0x1000, static_cast<std::uint16_t>(0x4000 + i), months[i]);
    unsigned sum = 65000;
    for (unsigned count = 0; count <= months.size(); ++count) {
        m.set(Reg::CX, count); m.set(Reg::DX, 65000); m.set(Reg::SI, 0x4000); m.set(Reg::AX, 0xffff);
        msdos::v200::dsum(m);
        check(m.get(Reg::DX) == (sum & 0xffff) && m.get(Reg::CX) == 0 && m.get(Reg::SI) == 0x4000 + count, "DSUM accumulated days and register contract");
        if (count < months.size()) sum += months[count];
    }
}

void decimal_adjust() {
    msdos::Machine m;using msdos::Reg;
    for (unsigned value=0;value<256;++value) for (unsigned base:{1U,10U,16U,255U}) {
        m.set(Reg::AX,0xa500U|value);m.aam(static_cast<std::uint8_t>(base));
        check(m.get(Reg::AH)==value/base && m.get(Reg::AL)==value%base,"AAM quotient/remainder");
        check(m.flags.zero==((value%base)==0) && m.flags.parity==even_parity(value%base),"AAM status flags");
    }
    for(unsigned value=0;value<65536;++value) {
        m.set(Reg::AX,value);m.aad();const auto expected=((value&255)+(value>>8)*10)&255;
        check(m.get(Reg::AX)==expected && m.flags.zero==(expected==0) && m.flags.sign==((expected&128)!=0) &&
            m.flags.parity==even_parity(expected),"AAD exhaustive byte wrapping and flags");
    }
    m.set(Reg::AX,0xbeef);rejects([&]{m.aam(0);},"AAM zero base must fault");
    check(m.get(Reg::AX)==0xbeef,"AAM fault preserves input");
}

void original_command_routines() {
    msdos::Machine m;using msdos::Reg;m.set(Reg::DS,0x2000);m.set(Reg::SS,0x3000);
    for(unsigned ch=0;ch<256;++ch) {
        m.set(Reg::AX,0xa500|ch);msdos::v125::command_delim(m);
        check(m.flags.zero==(ch==' ' || ch=='=' || ch==',' || ch==9),"COMMAND delimiters for every byte");
        check(m.get(Reg::AX)==(0xa500|ch),"DELIM preserves AX");
        m.write8(0x2000,0x4000,ch);m.set(Reg::SI,0x4000);m.set(Reg::AX,0xa500);
        msdos::v125::command_indig(m);const bool digit=ch>='0' && ch<='9';
        check(m.flags.carry==!digit && m.get(Reg::SI)==(digit ? 0x4001 : 0x4000),"INDIG carry and pointer");
        if(digit) check(m.get(Reg::AL)==ch-'0',"INDIG decimal value");
    }
    const std::string command=" ,=\tNAME";
    for(unsigned i=0;i<command.size();i++) m.write8(0x2000,static_cast<std::uint16_t>(0x4000+i),command[i]);
    m.set(Reg::SI,0x4000);msdos::v125::command_scanoff(m);
    check(m.get(Reg::SI)==0x4004 && m.get(Reg::AL)=='N' && !m.flags.zero,"SCANOFF positions at first non-delimiter");
    for(unsigned value=0;value<100;value++) {
        const std::string input=std::to_string(value)+":";
        for(unsigned i=0;i<input.size();i++) m.write8(0x2000,static_cast<std::uint16_t>(0x4000+i),input[i]);
        m.set(Reg::SI,0x4000);msdos::v125::command_getnum(m);
        check(m.get(Reg::AH)==value && !m.flags.carry && !m.flags.zero &&
            m.get(Reg::SI)==0x4000+input.size()-1,"GETNUM consumes one or two digits and preserves separator");
    }
    std::string output;
    m.interrupt_handler=[&](msdos::Machine& cpu,std::uint8_t number){
        check(number==0x21 && cpu.get(Reg::AH)==2,"COMMAND output uses original DOS console call");
        output+=static_cast<char>(cpu.get(Reg::DL));
    };
    for(unsigned value=0;value<100;value++) for(bool suppress:{false,true}) {
        output.clear();m.set(Reg::AX,value);m.set(Reg::BX,suppress ? ('0'-' ')<<8 : 0);m.set(Reg::DX,0x1234);
        msdos::v125::command_out2(m);
        const char tens=suppress && value<10 ? ' ' : static_cast<char>('0'+value/10);
        check(output==std::string{tens,static_cast<char>('0'+value%10)} && m.get(Reg::DX)==0x1234 &&
            m.get(Reg::BH)==0,"OUT2 formatting, leading zero suppression and register preservation");
    }
    output.clear();m.set(Reg::CX,0x0102);m.set(Reg::DX,0x0304);m.set(Reg::BL,':');
    msdos::v125::command_show(m);check(output==" 1:02:03.04","SHOW time format");
    output.clear();m.set(Reg::CX,0x0c1f);m.set(Reg::DX,0x1352);m.set(Reg::BL,'-');
    msdos::v125::command_show(m);check(output=="12-31-1982","SHOW date format");
}

void complete_hex2bin() {
    const std::string hex = ":040100000102A0FF59\r\n:00000001FF\r\n";
    msdos::Machine m;
    msdos::FcbHost host({reinterpret_cast<const std::uint8_t*>(hex.data()), hex.size()});
    host.prepare_psp(m); host.attach(m); msdos::v125::hex2bin(m);
    check(!host.had_error() && host.created_output(), "whole source utility completes");
    check(host.output() == std::vector<std::uint8_t>({1,2,0xa0,0xff}), "default -100H load offset");
    msdos::Machine halted;
    halted.set(msdos::Reg::AX, 0xa546); halted.halted = true;
    msdos::v125::hexchk(halted);
    check(halted.get(msdos::Reg::AX) == 0xa546, "halted entry skips the source routine body");
}
void original_fcb_positions() {
    msdos::Machine m;using msdos::Reg;
    m.set(Reg::DS,0x2000);
    for(bool extended:{false,true}) {
        const std::uint16_t base=extended ? 0x107 : 0x100;
        m.write8(0x2000,0x100,extended ? 255 : 0);
        for(unsigned block:{0U,1U,255U,256U,511U,512U,65535U}) for(unsigned nr=0;nr<256;nr++) {
            m.write16(0x2000,base+12,block);m.write8(0x2000,base+32,nr);m.set(Reg::DX,0x100);
            msdos::v125::getrec(m);const auto record=block*128U+(nr&127U);
            check((m.get(Reg::AX)|(static_cast<unsigned>(m.get(Reg::DX))<<16))==record && m.get(Reg::CX)==1 && m.get(Reg::DI)==base,"original GETREC for block/record boundaries and extended FCB");
            for(unsigned size:{1U,63U,64U,128U}) {
                m.write16(0x2000,base+14,size);m.write8(0x2000,base+36,0xa5);m.set(Reg::DX,0x100);
                msdos::v125::setrndrec(m);
                check(m.read16(0x2000,base+33)==(record&65535) && m.read8(0x2000,base+35)==((record>>16)&255),"original SETRNDREC three-byte record field");
                check(m.read8(0x2000,base+36)==(size<64 ? record>>24 : 0xa5),"original SETRNDREC fourth byte depends on record size");
            }
        }
        m.write16(0x2000,base+33,0x5678);m.write16(0x2000,base+35,0x1234);m.set(Reg::DX,0x100);m.set(Reg::CX,42);
        msdos::v125::getrrpos(m);check(m.get(Reg::AX)==0x5678 && m.get(Reg::DX)==0x1234 && m.get(Reg::CX)==42,"original GETRRPOS full random position");
        m.set(Reg::DX,0x100);msdos::v125::getrrpos1(m);check(m.get(Reg::CX)==1,"GETRRPOS1 sets one-record count");
    }
}
} // namespace

int main() {
    try {
        registers_and_memory(); arithmetic(); shifts_and_strings(); decimal_adjust(); hexchk(); fat12(); vectors_and_dsum(); original_fcb_positions(); original_command_routines(); complete_hex2bin();
        std::cout << "Passed: arithmetic flags, memory, strings, HEXCHK, FAT12/PACK, vectors, DSUM, COMMAND routines, complete HEX2BIN\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAILED: " << error.what() << '\n'; return 1;
    }
}

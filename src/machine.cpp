#include "msdos/machine.hpp"

#include <string>

namespace msdos {

Machine::Machine() : memory_(1U << 20U, 0) {
    set(Reg::SP, 0xfffe);
}

unsigned Machine::width(Reg reg) {
    return reg >= Reg::AL ? 8U : 16U;
}

std::uint32_t Machine::mask(unsigned bits) {
    if (bits != 8 && bits != 16) throw std::invalid_argument("operand width must be 8 or 16");
    return (1U << bits) - 1U;
}

std::uint16_t Machine::get(Reg reg) const {
    auto index = static_cast<unsigned>(reg);
    if (reg < Reg::AL) return registers_[index];
    index -= static_cast<unsigned>(Reg::AL);
    return static_cast<std::uint16_t>((registers_[index % 4] >> (index >= 4 ? 8U : 0U)) & 0xffU);
}

void Machine::set(Reg reg, std::uint32_t value) {
    auto index = static_cast<unsigned>(reg);
    if (reg < Reg::AL) {
        registers_[index] = static_cast<std::uint16_t>(value);
        return;
    }
    index -= static_cast<unsigned>(Reg::AL);
    const unsigned shift = index >= 4 ? 8U : 0U;
    auto& full = registers_[index % 4];
    full = static_cast<std::uint16_t>((full & ~(0xffU << shift)) | ((value & 0xffU) << shift));
}

std::size_t Machine::address(std::uint16_t segment, std::uint16_t offset) {
    return ((static_cast<std::size_t>(segment) << 4U) + offset) & 0xfffffU;
}

std::uint8_t Machine::read8(std::uint16_t segment, std::uint16_t offset) const {
    return memory_[address(segment, offset)];
}

std::uint16_t Machine::read16(std::uint16_t segment, std::uint16_t offset) const {
    // A word straddling offset ffff follows the physical bus, not offset zero.
    const auto location = address(segment, offset);
    return static_cast<std::uint16_t>(memory_[location] |
        (static_cast<unsigned>(memory_[(location + 1) & 0xfffffU]) << 8U));
}

void Machine::write8(std::uint16_t segment, std::uint16_t offset, std::uint32_t value) {
    memory_[address(segment, offset)] = static_cast<std::uint8_t>(value);
}

void Machine::write16(std::uint16_t segment, std::uint16_t offset, std::uint32_t value) {
    const auto location = address(segment, offset);
    memory_[location] = static_cast<std::uint8_t>(value);
    memory_[(location + 1) & 0xfffffU] = static_cast<std::uint8_t>(value >> 8U);
}

std::uint16_t Machine::read(unsigned bits, std::uint16_t segment, std::uint16_t offset) const {
    mask(bits);
    return bits == 8 ? read8(segment, offset) : read16(segment, offset);
}

void Machine::write(unsigned bits, std::uint16_t segment, std::uint16_t offset, std::uint32_t value) {
    mask(bits);
    if (bits == 8) write8(segment, offset, value);
    else write16(segment, offset, value);
}

void Machine::result_flags(std::uint32_t value, unsigned bits) {
    value &= mask(bits);
    flags.zero = value == 0;
    flags.sign = (value & (1U << (bits - 1))) != 0;
    unsigned byte = value & 0xffU;
    byte ^= byte >> 4U;
    byte ^= byte >> 2U;
    byte ^= byte >> 1U;
    flags.parity = (byte & 1U) == 0;
}

std::uint16_t Machine::add(std::uint32_t a, std::uint32_t b, unsigned bits, bool carry) {
    const auto limit = mask(bits);
    a &= limit;
    b &= limit;
    const auto result = a + b + static_cast<unsigned>(carry);
    flags.carry = result > limit;
    flags.auxiliary = ((a ^ b ^ result) & 0x10U) != 0;
    flags.overflow = ((~(a ^ b) & (a ^ result)) & (1U << (bits - 1))) != 0;
    result_flags(result, bits);
    return static_cast<std::uint16_t>(result & limit);
}

std::uint16_t Machine::sub(std::uint32_t a, std::uint32_t b, unsigned bits, bool borrow) {
    const auto limit = mask(bits);
    a &= limit;
    b &= limit;
    const auto result = a - b - static_cast<unsigned>(borrow);
    flags.carry = a < b + static_cast<unsigned>(borrow);
    flags.auxiliary = ((a ^ b ^ result) & 0x10U) != 0;
    flags.overflow = (((a ^ b) & (a ^ result)) & (1U << (bits - 1))) != 0;
    result_flags(result, bits);
    return static_cast<std::uint16_t>(result & limit);
}

std::uint16_t Machine::logic(std::uint32_t a, std::uint32_t b, unsigned bits, Logic op) {
    const auto result = op == Logic::And ? a & b : op == Logic::Or ? a | b : a ^ b;
    flags.carry = flags.overflow = false;
    // AF is undefined by the ISA; preserve it, rather than invent a contract.
    result_flags(result, bits);
    return static_cast<std::uint16_t>(result & mask(bits));
}

std::uint16_t Machine::inc(std::uint32_t value, unsigned bits) {
    const bool carry = flags.carry;
    const auto result = add(value, 1, bits);
    flags.carry = carry;
    return result;
}

std::uint16_t Machine::dec(std::uint32_t value, unsigned bits) {
    const bool carry = flags.carry;
    const auto result = sub(value, 1, bits);
    flags.carry = carry;
    return result;
}

std::uint16_t Machine::neg(std::uint32_t value, unsigned bits) {
    return sub(0, value, bits);
}

void Machine::lahf() {
    set(Reg::AH, (flags.sign ? 0x80U : 0U) | (flags.zero ? 0x40U : 0U) |
        (flags.auxiliary ? 0x10U : 0U) | (flags.parity ? 4U : 0U) | 2U | (flags.carry ? 1U : 0U));
}

void Machine::sahf() {
    const auto value = get(Reg::AH);
    flags.sign = (value & 0x80U) != 0;
    flags.zero = (value & 0x40U) != 0;
    flags.auxiliary = (value & 0x10U) != 0;
    flags.parity = (value & 4U) != 0;
    flags.carry = (value & 1U) != 0;
}

void Machine::daa() {
    const auto old = get(Reg::AL);
    const bool old_carry = flags.carry;
    unsigned value = old;
    flags.auxiliary = (old & 0xfU) > 9 || flags.auxiliary;
    if (flags.auxiliary) value += 6;
    flags.carry = old > 0x99 || old_carry;
    if (flags.carry) value += 0x60;
    set(Reg::AL, value);
    result_flags(value, 8);
}

void Machine::aam(std::uint8_t base) {
    if (!base) throw std::runtime_error("AAM divide by zero");
    const auto value = get(Reg::AL);
    set(Reg::AH, value / base);
    set(Reg::AL, value % base);
    result_flags(get(Reg::AL), 8);
}

void Machine::aad(std::uint8_t base) {
    const auto value = get(Reg::AL) + get(Reg::AH) * base;
    set(Reg::AX, value & 255U);
    result_flags(get(Reg::AL), 8);
}

std::uint16_t Machine::shift(std::uint32_t value, unsigned count, unsigned bits, Shift op) {
    const auto limit = mask(bits);
    const auto high_bit = 1U << (bits - 1);
    value &= limit;
    if (count == 0) return static_cast<std::uint16_t>(value);
    const auto original = value;
    // 8086 uses the full count, without the 286+'s five-bit count mask.
    for (unsigned i = 0; i < count; ++i) {
        const bool old_carry = flags.carry;
        const bool right = op == Shift::Shr || op == Shift::Sar || op == Shift::Ror || op == Shift::Rcr;
        flags.carry = (value & (right ? 1U : high_bit)) != 0;
        if (right) {
            const auto top = op == Shift::Sar ? value & high_bit :
                op == Shift::Ror ? (flags.carry ? high_bit : 0U) :
                op == Shift::Rcr ? (old_carry ? high_bit : 0U) : 0U;
            value = (value >> 1U) | top;
        } else {
            const auto bottom = op == Shift::Rol ? static_cast<unsigned>(flags.carry) :
                op == Shift::Rcl ? static_cast<unsigned>(old_carry) : 0U;
            value = ((value << 1U) | bottom) & limit;
        }
    }
    if (op == Shift::Shl || op == Shift::Shr || op == Shift::Sar) result_flags(value, bits);
    if (count == 1) {
        if (op == Shift::Shr) flags.overflow = (original & high_bit) != 0;
        else if (op == Shift::Sar) flags.overflow = false;
        else if (op == Shift::Ror || op == Shift::Rcr)
            flags.overflow = ((value ^ (value << 1U)) & high_bit) != 0;
        else flags.overflow = ((value & high_bit) != 0) != flags.carry;
    }
    // OF after a multi-bit shift is undefined; preserve its prior value.
    return static_cast<std::uint16_t>(value);
}

void Machine::push(std::uint16_t value) {
    set(Reg::SP, get(Reg::SP) - 2U);
    write16(get(Reg::SS), get(Reg::SP), value);
}

std::uint16_t Machine::pop() {
    const auto value = read16(get(Reg::SS), get(Reg::SP));
    set(Reg::SP, get(Reg::SP) + 2U);
    return value;
}

void Machine::advance(Reg reg, unsigned bits) {
    set(reg, get(reg) + (flags.direction ? 0U - bits / 8 : bits / 8));
}

void Machine::lods(unsigned bits, Reg segment) {
    set(bits == 8 ? Reg::AL : Reg::AX, read(bits, get(segment), get(Reg::SI)));
    advance(Reg::SI, bits);
}

void Machine::stos(unsigned bits) {
    write(bits, get(Reg::ES), get(Reg::DI), get(bits == 8 ? Reg::AL : Reg::AX));
    advance(Reg::DI, bits);
}

void Machine::movs(unsigned bits, Reg segment) {
    write(bits, get(Reg::ES), get(Reg::DI), read(bits, get(segment), get(Reg::SI)));
    advance(Reg::SI, bits);
    advance(Reg::DI, bits);
}

void Machine::cmps(unsigned bits, Reg segment) {
    sub(read(bits, get(segment), get(Reg::SI)), read(bits, get(Reg::ES), get(Reg::DI)), bits);
    advance(Reg::SI, bits);
    advance(Reg::DI, bits);
}

void Machine::scas(unsigned bits) {
    sub(get(bits == 8 ? Reg::AL : Reg::AX), read(bits, get(Reg::ES), get(Reg::DI)), bits);
    advance(Reg::DI, bits);
}

void Machine::interrupt(std::uint8_t number) {
    if (!interrupt_handler) throw std::runtime_error("no interrupt backend attached");
    interrupt_handler(*this, number);
}

} // namespace msdos

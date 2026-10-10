#include "ports.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
using msdos::Machine;using msdos::Reg;
void check(bool condition,const char* message) {if(!condition) throw std::runtime_error(message);}
constexpr std::uint16_t output_segment=0x4000,output_offset=0x100;
constexpr std::uint16_t path_segment=0x4200,path_offset=0x200;
using Routine=void(*)(Machine&);
struct Call {
    Machine m;
    std::array<std::uint16_t,12> before{};
    unsigned interrupts=0;
    Call(std::initializer_list<std::uint16_t> arguments) {
        for(unsigned i=0;i<12;i++) m.set(static_cast<Reg>(i),0x1100+i*0x111);
        m.set(Reg::SS,0x3000);m.set(Reg::SP,0x9000-arguments.size()*2);
        unsigned offset=m.get(Reg::SP);
        for(const auto word:arguments) {m.write16(m.get(Reg::SS),static_cast<std::uint16_t>(offset),word);offset+=2;}
        for(unsigned i=0;i<12;i++) before[i]=m.get(static_cast<Reg>(i));
        for(unsigned i=0;i<24;i++) m.write8(output_segment,output_offset+i,0xa5);
        m.write16(m.get(Reg::SS),0x9000,0xbeef);
    }
    void run(Routine routine,const std::function<void(Machine&)>& handler={},unsigned expected_interrupt=0x21) {
        m.interrupt_handler=[&](Machine& cpu,std::uint8_t number){
            check(number==expected_interrupt,"mapper must call the expected DOS/BIOS interrupt");interrupts++;
            check(static_cast<bool>(handler),"unexpected DOS service call");handler(cpu);
        };
        routine(m);
        for(unsigned i=1;i<12;i++) if(static_cast<Reg>(i)!=Reg::SP)
            check(m.get(static_cast<Reg>(i))==before[i],"far mapper must restore all non-AX registers");
        check(m.get(Reg::SP)==0x9000,"far RET must remove return address and exact parameter bytes");
        check(m.read16(m.get(Reg::SS),0x9000)==0xbeef,"far RET must preserve caller stack canary");
    }
    void untouched() {check(m.read16(output_segment,output_offset)==0xa5a5,"failure must leave output pointer unchanged");}
};

void pointer_calls() {
    const std::array<std::pair<Routine,unsigned>,4> calls{{
        {msdos::v400::dosmkdir,0x39},{msdos::v400::dosrmdir,0x3a},
        {msdos::v400::doschdir,0x3b},{msdos::v400::dosdelete,0x41}}};
    for(const auto& [routine,service]:calls) for(bool failure:{false,true}) {
        Call call({0,0,path_offset,path_segment});
        call.run(routine,[&](Machine& cpu){
            check(cpu.get(Reg::AH)==service && cpu.get(Reg::DS)==path_segment && cpu.get(Reg::DX)==path_offset,
                "directory/delete mapper must pass original far pathname");
            cpu.set(Reg::AX,failure ? 5 : 0x9876);cpu.flags.carry=failure;
        });
        check(call.m.get(Reg::AX)==(failure ? 5 : 0) && call.interrupts==1,"directory/delete return codes");
    }
}

void file_io() {
    for(const auto routine:{msdos::v400::dosread,msdos::v400::doswrite}) for(bool failure:{false,true}) {
        Call call({output_offset,output_segment,123,path_offset,path_segment,7});
        call.run(routine,[&](Machine& cpu){
            check(cpu.get(Reg::AH)==(routine==msdos::v400::dosread ? 0x3f : 0x40) &&
                cpu.get(Reg::BX)==7 && cpu.get(Reg::CX)==123 && cpu.get(Reg::DS)==path_segment &&
                cpu.get(Reg::DX)==path_offset,"read/write DOS arguments and far buffer");
            cpu.set(Reg::AX,failure ? 6 : 99);cpu.flags.carry=failure;
        });
        check(call.m.get(Reg::AX)==(failure ? 6 : 0),"read/write return code");
        if(failure) call.untouched();else check(call.m.read16(output_segment,output_offset)==99,"read/write short transfer count");
    }
    for(bool failure:{false,true}) {
        Call call({output_offset,output_segment,2,0x1234,0xffff,7});
        call.run(msdos::v400::doschgfileptr,[&](Machine& cpu){
            check(cpu.get(Reg::AX)==0x4202 && cpu.get(Reg::BX)==7 && cpu.get(Reg::CX)==0xffff &&
                cpu.get(Reg::DX)==0x1234,"seek keeps signed 32-bit distance and origin");
            cpu.set(Reg::AX,failure ? 6 : 0x5678);cpu.set(Reg::DX,0x1234);cpu.flags.carry=failure;
        });
        check(call.m.get(Reg::AX)==(failure ? 6 : 0),"seek error code");
        if(failure) call.untouched();else check(call.m.read16(output_segment,output_offset)==0x5678 &&
            call.m.read16(output_segment,output_offset+2)==0x1234,"seek writes both pointer halves");
    }
    for(unsigned handle:{0U,1U,7U,0x7fffU,0x8000U,0xffffU}) for(bool failure:{false,true}) {
        Call call({static_cast<std::uint16_t>(handle)});
        const bool file=handle!=0 && handle<=0x8000; // original NEG/JNS also treats 8000h as a file
        call.run(msdos::v400::dosclose,[&](Machine& cpu){
            check(cpu.get(Reg::AX)==0x3e00 && cpu.get(Reg::BX)==handle,"close forwards file handles");
            cpu.set(Reg::AX,failure ? 6 : 0x9876);cpu.flags.carry=failure;
        });
        check(call.interrupts==(file ? 1U : 0U) && call.m.get(Reg::AX)==(file && failure ? 6U : 0U),
            "close preserves historical dummy-device and signed boundary handling");
    }
}

void queries_and_attributes() {
    Call version({output_offset,output_segment});
    version.run(msdos::v400::dosgetversion,[](Machine& cpu){
        check(cpu.get(Reg::AH)==0x30,"get version DOS service");cpu.set(Reg::AX,0x0104);
    });
    check(version.m.get(Reg::AX)==0 && version.m.read16(output_segment,output_offset)==0x0401,"version mapper reverses major/minor bytes");
    for(unsigned verify:{0U,1U,0x80U,0xffU}) {
        Call query({output_offset,output_segment});
        query.run(msdos::v400::dosqverify,[&](Machine& cpu){check(cpu.get(Reg::AH)==0x54,"query verify service");cpu.set(Reg::AL,verify);});
        check(query.m.read16(output_segment,output_offset)==(verify<128 ? verify : 0xff00|verify) && query.m.get(Reg::AX)==0,
            "query verify preserves original CBW sign extension");
    }
    for(unsigned verify=0;verify<256;verify++) {
        Call setting({static_cast<std::uint16_t>(verify)});const bool accepted=verify<=1 || verify>=128;
        setting.run(msdos::v400::dossetverify,[&](Machine& cpu){
            check(cpu.get(Reg::AH)==0x2e && cpu.get(Reg::AL)==verify,"set verify passes raw low byte");
        });
        check(setting.interrupts==(accepted ? 1U : 0U) && setting.m.get(Reg::AX)==(accepted ? 0 : 2),
            "set verify retains signed JG historical behavior");
    }
    for(bool failure:{false,true}) {
        Call query({0,0,output_offset,output_segment,path_offset,path_segment});
        query.run(msdos::v400::dosqfilemode,[&](Machine& cpu){
            check(cpu.get(Reg::AX)==0x4300 && cpu.get(Reg::DS)==path_segment && cpu.get(Reg::DX)==path_offset,"get attributes arguments");
            cpu.set(Reg::CX,0x27);cpu.set(Reg::AX,5);cpu.flags.carry=failure;
        });
        check(query.m.get(Reg::AX)==(failure ? 2 : 0),"get attributes original error remapping");
        if(failure) query.untouched();else check(query.m.read16(output_segment,output_offset)==0x27,"get attributes output");
        Call setting({0,0,0x27,path_offset,path_segment});
        setting.run(msdos::v400::dossetfilemode,[&](Machine& cpu){
            check(cpu.get(Reg::AX)==0x4301 && cpu.get(Reg::CX)==0x27 && cpu.get(Reg::DS)==path_segment && cpu.get(Reg::DX)==path_offset,
                "set attributes arguments");cpu.set(Reg::AX,5);cpu.flags.carry=failure;
        });
        check(setting.m.get(Reg::AX)==(failure ? 5 : 0),"set attributes DOS error code");
    }
    for(unsigned descriptor:{0U,0x7fU,0x80U,0x8080U}) for(bool failure:{false,true}) {
        Call query({output_offset,output_segment,static_cast<std::uint16_t>(output_offset+2),output_segment,7});
        query.run(msdos::v400::dosqhandtype,[&](Machine& cpu){
            check(cpu.get(Reg::AX)==0x4400 && cpu.get(Reg::BX)==7,"handle type query arguments");
            cpu.set(Reg::AX,failure ? 6 : 0x1234);cpu.set(Reg::DX,descriptor);cpu.flags.carry=failure;
        });
        check(query.m.get(Reg::AX)==(failure ? 6 : 0x1234) && query.m.read16(output_segment,output_offset)==descriptor &&
            query.m.read16(output_segment,output_offset+2)==((descriptor&0x80)!=0),
            "handle query writes outputs and retains DOS AX even on error, like the source");
    }
    for(unsigned drive:{0U,1U,26U,0xffffU}) {
        Call call({static_cast<std::uint16_t>(drive)});
        call.run(msdos::v400::dosselectdisk,[&](Machine& cpu){
            check(cpu.get(Reg::AH)==0x0e && cpu.get(Reg::DX)==((drive-1)&0xffff),"disk mapper decrements one-based drive with word wrapping");
        });check(call.m.get(Reg::AX)==0,"select disk return code");
    }
}

void memory_calls() {
    // Full byte-size domain, including zero/full segment and rounding overflow.
    Call alloc({1,output_offset,output_segment,0});
    Call resize({0x5000,0});
    for(unsigned size=0;size<65536;size++) {
        alloc.m.set(Reg::SP,0x8ff8);alloc.m.write16(alloc.m.get(Reg::SS),0x8ffe,size);
        alloc.before[static_cast<unsigned>(Reg::SP)]=0x8ff8;
        alloc.run(msdos::v400::dosallocseg,[&](Machine& cpu){
            const unsigned rounded=(size+15)&0xfff0;
            check(cpu.get(Reg::AH)==0x48 && cpu.get(Reg::BX)==(rounded ? rounded/16 : 4096),"allocation paragraph rounding across all sizes");
            cpu.set(Reg::AX,0x5000);cpu.flags.carry=false;
        });
        check(alloc.m.get(Reg::AX)==0 && alloc.m.read16(output_segment,output_offset)==0x5000,"allocation selector output");
        resize.m.set(Reg::SP,0x8ffc);resize.m.write16(resize.m.get(Reg::SS),0x8ffe,size);
        resize.run(msdos::v400::dosreallocseg,[&](Machine& cpu){
            check(cpu.get(Reg::AH)==0x4a && cpu.get(Reg::ES)==0x5000 && cpu.get(Reg::BX)==(size ? size/16 : 4096),
                "resize retains original truncation rather than allocation rounding");cpu.flags.carry=false;
        });check(resize.m.get(Reg::AX)==0,"resize return code");
    }
    Call failed({0,output_offset,output_segment,17});
    failed.run(msdos::v400::dosallocseg,[](Machine& cpu){cpu.set(Reg::AX,8);cpu.flags.carry=true;});
    check(failed.m.get(Reg::AX)==8,"allocation preserves insufficient-memory error");failed.untouched();
    Call resize_failure({0x5000,17});
    resize_failure.run(msdos::v400::dosreallocseg,[](Machine& cpu){cpu.set(Reg::AX,8);cpu.flags.carry=true;});
    check(resize_failure.m.get(Reg::AX)==8,"resize preserves DOS error");
    for(bool failure:{false,true}) {
        Call free({0x5000});free.run(msdos::v400::dosfreeseg,[&](Machine& cpu){
            check(cpu.get(Reg::AH)==0x49 && cpu.get(Reg::ES)==0x5000,"free segment arguments");
            cpu.set(Reg::AX,9);cpu.flags.carry=failure;
        });check(free.m.get(Reg::AX)==(failure ? 6 : 0),"free maps errors to invalid selector");
    }
}

void date_time() {
    Call get({output_offset,output_segment});
    get.run(msdos::v400::dosgetdatetime,[&](Machine& cpu){
        if(get.interrupts==1) {check(cpu.get(Reg::AH)==0x2c,"get time first");cpu.set(Reg::CX,0x0c22);cpu.set(Reg::DX,0x384e);}
        else {check(cpu.get(Reg::AH)==0x2a,"get date second");cpu.set(Reg::CX,2026);cpu.set(Reg::DX,0x0a06);cpu.set(Reg::AL,2);}
    });
    const std::array<unsigned,11> expected{12,34,56,78,6,10,0xea,7,0x68,1,2};
    for(unsigned i=0;i<expected.size();i++) check(get.m.read8(output_segment,output_offset+i)==expected[i],"date/time structure and original fixed GMT offset");
    check(get.m.get(Reg::AX)==0 && get.interrupts==2 && get.m.read8(output_segment,output_offset+11)==0xa5,"date/time output bounds");
    for(bool time_error:{false,true}) for(bool date_error:{false,true}) {
        Call set({output_offset,output_segment});
        for(unsigned i=0;i<expected.size();i++) set.m.write8(output_segment,output_offset+i,expected[i]);
        set.run(msdos::v400::dossetdatetime,[&](Machine& cpu){
            if(set.interrupts==1) {check(cpu.get(Reg::AH)==0x2d && cpu.get(Reg::CX)==0x0c22 && cpu.get(Reg::DX)==0x384e,"set time fields");cpu.set(Reg::AL,time_error ? 255 : 0);}
            else {check(cpu.get(Reg::AH)==0x2b && cpu.get(Reg::CX)==2026 && cpu.get(Reg::DX)==0x0a06,"set date fields even after time error");cpu.set(Reg::AL,date_error ? 255 : 0);}
        });
        check(set.interrupts==2 && set.m.get(Reg::AX)==(time_error || date_error ? 2 : 0),"date/time checks both return values");
    }
}

void messages_io_control_and_exit() {
    for(bool failure:{false,true}) {
        Call message({path_offset,path_segment,123,7});
        message.run(msdos::v400::dosputmessage,[&](Machine& cpu){
            check(cpu.get(Reg::AH)==0x40 && cpu.get(Reg::BX)==7 && cpu.get(Reg::CX)==123 &&
                cpu.get(Reg::DS)==path_segment && cpu.get(Reg::DX)==path_offset,"message write arguments");
            cpu.set(Reg::AX,failure ? 6 : 99);cpu.flags.carry=failure;
        });check(message.m.get(Reg::AX)==(failure ? 6 : 0),"message maps short write to original success");
    }
    for(unsigned handle:{0U,7U,0xffe4U,0xffe5U,0xfffeU,0xffffU,0x7fffU,0x8000U})
        for(unsigned error:{0U,1U,5U}) {
            Call ioctl({static_cast<std::uint16_t>(handle),8,0x20,0,0,output_offset,output_segment});
            ioctl.run(msdos::v400::dosdevioctl,[&](Machine& cpu){
                const bool converted=static_cast<std::int16_t>(handle)>=static_cast<std::int16_t>(0xffe5);
                const auto expected=converted ? static_cast<std::uint16_t>(0U-handle-1U) : handle;
                check(cpu.get(Reg::BX)==expected && cpu.get(Reg::AX)==0x4408,"IOCTL retains signed handle comparison and category");
                cpu.set(Reg::AX,error ? error : 0x80);cpu.flags.carry=error!=0;
            });
            check(ioctl.m.get(Reg::AX)==(error==5 ? 5 : 0),"IOCTL treats error 1 as network continuation");
            check(ioctl.m.read8(output_segment,output_offset)==(error==5 ? 0xa5 : error==1 ? 1 : 0x80) &&
                ioctl.m.read8(output_segment,output_offset+1)==0xa5,"IOCTL writes one result byte only on accepted status");
        }
    Call invalid({7,8,0x21,0,0,output_offset,output_segment});
    invalid.run(msdos::v400::dosdevioctl);
    check(invalid.m.get(Reg::AX)==1 && invalid.interrupts==0,"IOCTL rejects unsupported function before interrupt");invalid.untouched();
    for(unsigned action:{0U,1U,2U,0x7fffU,0x8000U,0xffffU}) {
        Call exit({0x1234,static_cast<std::uint16_t>(action)});
        exit.run(msdos::v400::dosexit,[](Machine& cpu){check(cpu.get(Reg::AX)==0x4c34,"exit forwards low byte of result code");});
        const bool accepted=static_cast<std::int16_t>(action)<=1;
        check(exit.interrupts==(accepted ? 1U : 0U) && exit.m.get(Reg::AX)==(accepted ? 0U : action),"exit retains signed action validation");
    }
    Call terminated({0xabcd,1});
    terminated.m.interrupt_handler=[](Machine& cpu,std::uint8_t number){
        check(number==0x21 && cpu.get(Reg::AX)==0x4ccd,"non-returning termination boundary");cpu.halted=true;
    };
    msdos::v400::dosexit(terminated.m);
    check(terminated.m.halted && terminated.m.get(Reg::SP)<terminated.before[static_cast<unsigned>(Reg::SP)],
        "termination stops execution before mapper epilogue");
}

void file_timestamps() {
    for(bool failure:{false,true}) {
        Call call({22,output_offset,output_segment,1,7});
        call.m.write16(output_segment,output_offset+8,0x5d46);call.m.write16(output_segment,output_offset+10,0x6543);
        call.run(msdos::v400::dossetfileinfo,[&](Machine& cpu){
            check(cpu.get(Reg::AX)==0x5701 && cpu.get(Reg::BX)==7 && cpu.get(Reg::DX)==0x5d46 &&
                cpu.get(Reg::CX)==0x6543,"set file information passes last-write DOS date/time");
            cpu.set(Reg::AX,failure ? 6 : 0x9876);cpu.flags.carry=failure;
        });check(call.m.get(Reg::AX)==(failure ? 6 : 0),"set file information preserves error code");
    }
}

void device_configuration() {
    for(unsigned equipment:{0U,0xffffU,0x4303U,0x8600U}) for(unsigned item=0;item<4;item++) {
        Call call({0,static_cast<std::uint16_t>(item),output_offset,output_segment});
        call.run(msdos::v400::dosdevconfig,[&](Machine& cpu){cpu.set(Reg::AX,equipment);},0x11);
        const unsigned expected=item==0 ? equipment>>14 : item==1 ? (equipment>>9)&7 : item==2 ? ((equipment>>6)&3)+1 : (equipment>>1)&1;
        check(call.m.get(Reg::AX)==0 && call.m.read8(output_segment,output_offset)==expected &&
            call.m.read8(output_segment,output_offset+1)==0xa5,"equipment bit extraction and byte output");
    }
    for(unsigned item:{4U,6U,0x7fffU,0x8000U,0xffffU}) {
        Call call({0,static_cast<std::uint16_t>(item),output_offset,output_segment});call.run(msdos::v400::dosdevconfig);
        check(call.m.get(Reg::AX)==2 && call.interrupts==0,"invalid device query");call.untouched();
    }
    Call reserved({1,0,output_offset,output_segment});reserved.run(msdos::v400::dosdevconfig);
    check(reserved.m.get(Reg::AX)==2 && reserved.interrupts==0,"reserved parameter validation");reserved.untouched();
    Call model({0,5,output_offset,output_segment});model.m.write8(0xf000,0xfffe,0xfc);
    model.run(msdos::v400::dosdevconfig);
    check(model.m.get(Reg::AX)==0 && model.interrupts==0 && model.m.read8(output_segment,output_offset)==
        (model.before[static_cast<unsigned>(Reg::BX)]&0xff),"model query retains original BL output rather than computed AL");
}

void character_conversion() {
    for(unsigned length:{1U,256U,0U}) {
        Call call({path_offset,path_segment,output_offset,output_segment,static_cast<std::uint16_t>(length)});
        call.m.write16(output_segment,output_offset,0x1234);
        const unsigned iterations=length ? length : 65536;
        for(unsigned i=0;i<iterations;i++) call.m.write8(path_segment,static_cast<std::uint16_t>(path_offset+i),i&0xff);
        call.run(msdos::v400::doscasemap);
        for(unsigned i=0;i<iterations;i++) {
            const unsigned c=i&0xff,expected=c>='a' && c<='z' ? c-'a'+'A' : c;
            check(call.m.read8(path_segment,static_cast<std::uint16_t>(path_offset+i))==expected,"case mapper preserves every non-lowercase byte");
        }
        check(call.interrupts==0 && call.m.get(Reg::AX)==(0x1200|((iterations-1)&0xff)),
            "case mapper retains original AX and zero-length LOOP wrap behavior");
    }
    constexpr unsigned vector_segment=0x7000,vector_offset=0xfffd;
    for(unsigned length:{1U,5U,0U}) for(bool failure:{false,true}) {
        Call call({output_offset,output_segment,0,0,static_cast<std::uint16_t>(length)});
        const unsigned iterations=length ? length : 65536;
        for(unsigned i=0;i<iterations;i++) call.m.write8(vector_segment,static_cast<std::uint16_t>(vector_offset+i),i*17U);
        call.run(msdos::v400::dosgetdbcsev,[&](Machine& cpu){
            check(cpu.get(Reg::AX)==0x6300,"DBCS vector service");cpu.set(Reg::DS,vector_segment);cpu.set(Reg::SI,vector_offset);
            cpu.set(Reg::AX,failure ? 2 : 0xabcd);cpu.flags.carry=failure;
        });
        if(failure) {check(call.m.get(Reg::AX)==2,"DBCS error code");call.untouched();}
        else {
            for(unsigned i=0;i<iterations;i++) check(call.m.read8(output_segment,static_cast<std::uint16_t>(output_offset+i))==(i*17U&0xff),
                "DBCS byte copy including source/destination offset wrapping");
            check(call.m.get(Reg::AX)==0,"DBCS success");
            if(length) check(call.m.read8(output_segment,output_offset+length)==0xa5,"DBCS output bounds");
        }
    }
}

void video_services() {
    for(unsigned row=0;row<256;row++) for(unsigned column:{0U,80U,81U,127U,128U,255U}) {
        Call call({0,static_cast<std::uint16_t>(column),static_cast<std::uint16_t>(row)});
        const bool valid=static_cast<std::int8_t>(row)<=25 && static_cast<std::int8_t>(column)<=80;
        call.run(msdos::v400::viosetcurpos,[&](Machine& cpu){
            check(cpu.get(Reg::AH)==2 && cpu.get(Reg::BH)==0 && cpu.get(Reg::DH)==row && cpu.get(Reg::DL)==column,
                "video cursor arguments");
            for(Reg reg:{Reg::BX,Reg::CX,Reg::DX,Reg::SI,Reg::DI,Reg::BP,Reg::ES,Reg::DS}) cpu.set(reg,0x9876);
        },0x10);
        check(call.interrupts==(valid ? 1U : 0U) && call.m.get(Reg::AX)==(valid ? 0 : 2),"cursor signed byte bounds and BIOS register restoration");
    }
    for(unsigned lines:{0U,25U,26U,127U,128U,255U}) for(unsigned right:{80U,81U,128U}) for(unsigned bottom:{25U,26U,128U}) {
        Call call({0,output_offset,output_segment,static_cast<std::uint16_t>(lines),static_cast<std::uint16_t>(right),
            static_cast<std::uint16_t>(bottom),2,3});call.m.write16(output_segment,output_offset,0x4f07);
        const bool valid=static_cast<std::int8_t>(lines)<=25 && static_cast<std::int8_t>(right)<=80 && static_cast<std::int8_t>(bottom)<=25;
        call.run(msdos::v400::vioscrollup,[&](Machine& cpu){
            check(cpu.get(Reg::AX)==(0x0600|lines) && cpu.get(Reg::CX)==0x0302 && cpu.get(Reg::DH)==bottom &&
                cpu.get(Reg::DL)==right && cpu.get(Reg::BH)==7,"scroll rectangle, signed bounds and original low-byte fill attribute");
        },0x10);check(call.interrupts==(valid ? 1U : 0U) && call.m.get(Reg::AX)==(valid ? 0 : 2),"scroll validation");
    }
    Call text({0,output_offset,output_segment,79,3,2,path_offset,path_segment});
    text.m.write8(output_segment,output_offset,0x1e);text.m.write8(path_segment,path_offset,'A');text.m.write8(path_segment,path_offset+1,'B');
    text.run(msdos::v400::viowrtcharstratt,[&](Machine& cpu){
        const unsigned step=text.interrupts;
        if(step==1 || step==3 || step==5) {
            check(cpu.get(Reg::AH)==2 && cpu.get(Reg::DH)==(step==1 ? 3 : 4) && cpu.get(Reg::DL)==(step==1 ? 79 : step==3 ? 0 : 1),
                "video text cursor movement and line wrap");
        }else check(cpu.get(Reg::AH)==9 && cpu.get(Reg::AL)==(step==2 ? 'A' : 'B') && cpu.get(Reg::BL)==0x1e &&
            cpu.get(Reg::BH)==0 && cpu.get(Reg::CX)==1,"video text character and attribute");
    },0x10);
    check(text.interrupts==5 && text.m.get(Reg::AX)==2,"video text retains original success fall-through to error code 2");
    Call end({0,output_offset,output_segment,79,24,1,path_offset,path_segment});
    end.m.write8(output_segment,output_offset,7);end.m.write8(path_segment,path_offset,'X');
    end.run(msdos::v400::viowrtcharstratt,[](Machine&){},0x10);
    check(end.interrupts==2 && end.m.get(Reg::AX)==1,"video text full-screen boundary");
    Call invalid({0,output_offset,output_segment,81,0,1,path_offset,path_segment});
    invalid.run(msdos::v400::viowrtcharstratt,{},0x10);
    check(invalid.interrupts==0 && invalid.m.get(Reg::AX)==2,"video text rejects invalid column");
}
}
int main() {
    try {pointer_calls();file_io();queries_and_attributes();memory_calls();date_time();
        messages_io_control_and_exit();file_timestamps();device_configuration();character_conversion();video_services();
        std::cout<<"Passed: all 30 DOS 4.0 mapper modules, far ABI, output bounds, DOS/BIOS boundaries, success/error paths and historical quirks\n";return 0;
    }catch(const std::exception& error){std::cerr<<"FAILED: "<<error.what()<<'\n';return 1;}
}

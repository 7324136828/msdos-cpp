#include "msdos/machine.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace msdos::native_fixture {
void nested_calls(Machine&);
void conditional_return(Machine&);
void table_jump(Machine&);
void invalid_return(Machine&);
void interrupt_halt(Machine&);
void call5_halt(Machine&);
void repeat_copy(Machine&);
void nonlocal_return(Machine&);
}

namespace {
using msdos::Machine;
using msdos::Reg;
namespace fixture=msdos::native_fixture;
void check(bool passed,const char* message) {
    if(!passed) throw std::runtime_error(message);
}
template<class F> void rejects(F&& action,const char* message) {
    try {action();} catch(const std::runtime_error&) {return;}
    throw std::runtime_error(message);
}
void prepare(Machine& m) {
    m.set(Reg::SS,0x3000);m.set(Reg::SP,0x9000);
    m.write16(0x3000,0x9000,0xbeef);
}
void stack_restored(const Machine& m) {
    check(m.get(Reg::SP)==0x9000 && m.read16(0x3000,0x9000)==0xbeef,"caller stack and canary preserved");
}

void nested_and_shared_returns() {
    Machine m;prepare(m);fixture::nested_calls(m);
    check(m.get(Reg::AX)==31,"nested CALL and shared RET follow source continuations");stack_restored(m);
    m.set(Reg::AX,0);m.set(Reg::BX,0x1234);fixture::conditional_return(m);
    check(m.get(Reg::BX)==0x1234 && m.flags.zero,"conditional branch to shared return");stack_restored(m);
    m.set(Reg::AX,1);fixture::conditional_return(m);
    check(m.get(Reg::BX)==77 && !m.flags.zero,"conditional return fallthrough");stack_restored(m);
}

void indirect_targets() {
    Machine m;prepare(m);
    for(unsigned selector:{0U,2U}) {
        m.set(Reg::SI,selector);fixture::table_jump(m);
        check(m.get(Reg::AX)==(selector ? 22 : 11),"relocated table dispatch reaches native code");stack_restored(m);
    }
    m.set(Reg::SI,4);
    rejects([&]{fixture::table_jump(m);},"invalid indirect jump must fail closed");
    Machine bad_return;prepare(bad_return);
    rejects([&]{fixture::invalid_return(bad_return);},"invalid return address must fail closed");
}

void interrupt_termination() {
    Machine m;prepare(m);unsigned calls=0;
    m.interrupt_handler=[&](Machine& state,std::uint8_t interrupt) {
        check(interrupt==0x21 && state.get(Reg::AX)==7,"expected interrupt input");++calls;state.halted=true;
    };
    fixture::interrupt_halt(m);
    check(calls==1 && m.get(Reg::AX)==7 && m.get(Reg::SP)==0x8ffe,"INT termination skips next statement and return cleanup");

    Machine initial;prepare(initial);initial.set(Reg::AX,0x1234);initial.halted=true;
    initial.interrupt_handler=[](Machine&,std::uint8_t){throw std::runtime_error("initial halt must skip interrupts");};
    fixture::interrupt_halt(initial);
    check(initial.get(Reg::AX)==0x1234 && initial.get(Reg::SP)==0x8ffe,"initial halt preserves source state after entry wrapper");

    Machine legacy;prepare(legacy);legacy.set(Reg::AX,0x5a07);
    legacy.interrupt_handler=[](Machine& state,std::uint8_t interrupt) {
        check(interrupt==0x21 && state.get(Reg::AH)==2,"CALL-5 uses source service number");state.halted=true;
    };
    fixture::call5_halt(legacy);
    check(legacy.get(Reg::AX)==0x5a07 && legacy.get(Reg::SP)==0x8ffe,"CALL-5 termination restores bridge AH and skips following source");
}

void repeated_strings() {
    Machine m;prepare(m);m.set(Reg::DS,0x1000);m.set(Reg::ES,0x2000);
    m.set(Reg::SI,0x10);m.set(Reg::DI,0x20);m.set(Reg::CX,0);m.write8(0x2000,0x20,0xa5);
    fixture::repeat_copy(m);
    check(m.get(Reg::SI)==0x10 && m.get(Reg::DI)==0x20 && m.read8(0x2000,0x20)==0xa5,"zero REP count performs no transfer");stack_restored(m);
    const std::array<std::uint8_t,3> bytes{0x21,0x43,0x65};
    for(unsigned i=0;i<bytes.size();++i) m.write8(0x1000,static_cast<std::uint16_t>(0x10+i),bytes[i]);
    m.set(Reg::CX,bytes.size());fixture::repeat_copy(m);
    check(m.get(Reg::CX)==0 && m.get(Reg::SI)==0x13 && m.get(Reg::DI)==0x23,"REP normal count advances pointers");
    for(unsigned i=0;i<bytes.size();++i) check(m.read8(0x2000,static_cast<std::uint16_t>(0x20+i))==bytes[i],"REP preserves copied bytes");stack_restored(m);
    m.flags.direction=true;m.set(Reg::SI,0);m.set(Reg::DI,0);m.set(Reg::CX,2);
    m.write8(0x1000,0,0x76);m.write8(0x1000,0xffff,0x98);fixture::repeat_copy(m);
    check(m.get(Reg::SI)==0xfffe && m.get(Reg::DI)==0xfffe && m.read8(0x2000,0)==0x76 && m.read8(0x2000,0xffff)==0x98,
        "backward REP preserves 16-bit pointer wrap");stack_restored(m);
}

void stack_reset_unwinds_source_calls() {
    Machine m;prepare(m);fixture::nonlocal_return(m);
    check(m.get(Reg::AX)==42 && m.get(Reg::BP)==0x8ffe,"SP reset bypasses both nested continuations");stack_restored(m);
}
}

int main() {
    try {
        nested_and_shared_returns();indirect_targets();interrupt_termination();repeated_strings();stack_reset_unwinds_source_calls();
        std::cout<<"Passed: native branches, nested/shared returns, table targets, termination, REP, nonlocal stack reset\n";
        return 0;
    } catch(const std::exception& error) {
        std::cerr<<"FAILED: "<<error.what()<<'\n';return 1;
    }
}

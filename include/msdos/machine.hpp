#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace msdos {

enum class Reg { AX, CX, DX, BX, SP, BP, SI, DI, ES, CS, SS, DS,
                 AL, CL, DL, BL, AH, CH, DH, BH };
enum class Logic { And, Or, Xor };
enum class Shift { Shl, Shr, Sar, Rol, Ror, Rcl, Rcr };

struct Flags {
    bool carry = false;
    bool parity = false;
    bool auxiliary = false;
    bool zero = false;
    bool sign = false;
    bool overflow = false;
    bool direction = false;
    bool interrupts = true;
};

// DOS register/memory/flag state for source-lifted native C++ functions.
// Control flow is compiled C++; this class has no instruction executor.
// Unsigned arithmetic and explicit 8/16-bit truncation preserve DOS behavior.
class Machine {
public:
    Machine();
    Flags flags;
    bool halted = false;
    std::function<void(Machine&, std::uint8_t)> interrupt_handler;

    std::uint16_t get(Reg reg) const;
    void set(Reg reg, std::uint32_t value);
    static unsigned width(Reg reg);
    static std::uint32_t mask(unsigned bits);

    std::uint8_t read8(std::uint16_t segment, std::uint16_t offset) const;
    std::uint16_t read16(std::uint16_t segment, std::uint16_t offset) const;
    void write8(std::uint16_t segment, std::uint16_t offset, std::uint32_t value);
    void write16(std::uint16_t segment, std::uint16_t offset, std::uint32_t value);
    std::uint16_t read(unsigned bits, std::uint16_t segment, std::uint16_t offset) const;
    void write(unsigned bits, std::uint16_t segment, std::uint16_t offset, std::uint32_t value);

    std::uint16_t add(std::uint32_t a, std::uint32_t b, unsigned bits, bool carry = false);
    std::uint16_t sub(std::uint32_t a, std::uint32_t b, unsigned bits, bool borrow = false);
    std::uint16_t logic(std::uint32_t a, std::uint32_t b, unsigned bits, Logic op);
    std::uint16_t shift(std::uint32_t value, unsigned count, unsigned bits, Shift op);
    std::uint16_t inc(std::uint32_t value, unsigned bits);
    std::uint16_t dec(std::uint32_t value, unsigned bits);
    std::uint16_t neg(std::uint32_t value, unsigned bits);
    void lahf();
    void sahf();
    void daa();
    void aam(std::uint8_t base = 10);
    void aad(std::uint8_t base = 10);
    void push(std::uint16_t value);
    std::uint16_t pop();
    void lods(unsigned bits, Reg segment = Reg::DS);
    void stos(unsigned bits);
    void movs(unsigned bits, Reg segment = Reg::DS);
    void cmps(unsigned bits, Reg segment = Reg::DS);
    void scas(unsigned bits);
    void interrupt(std::uint8_t number);

private:
    std::uint16_t registers_[12]{};
    std::vector<std::uint8_t> memory_;
    static std::size_t address(std::uint16_t segment, std::uint16_t offset);
    void result_flags(std::uint32_t value, unsigned bits);
    void advance(Reg reg, unsigned bits);
};

} // namespace msdos

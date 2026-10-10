#include "msdos/fcb_host.hpp"
#include <algorithm>

namespace msdos {
FcbHost::FcbHost(std::span<const std::uint8_t> input) : input_(input.begin(), input.end()) {}

void FcbHost::attach(Machine& machine) {
    machine.interrupt_handler = [this](Machine& m, std::uint8_t number) { interrupt(m, number); };
}

void FcbHost::prepare_psp(Machine& m, std::string_view offset) {
    if (offset.size() > 8) throw std::invalid_argument("offset must fit the DOS eight-character argument field");
    for (const auto reg : {Reg::DS, Reg::ES, Reg::CS, Reg::SS}) m.set(reg, 0x1000);
    m.set(Reg::SP, 0xfffe);
    m.write16(0x1000, 0, 0x20cd);
    m.write16(0x1000, 2, 0xa000);
    m.write16(0x1000, 6, 0xfff0);
    m.write8(0x1000, 0x5c, 0);
    constexpr std::string_view name = "INPUT      "; // No extension: test original default-to-HEX path.
    for (std::uint16_t i = 0; i < name.size(); ++i) m.write8(0x1000, static_cast<std::uint16_t>(0x5d + i), name[i]);
    // HEX2BIN reads its offset from the second default FCB at PSP:6d.
    for (std::uint16_t i = 0; i < 8; ++i)
        m.write8(0x1000, static_cast<std::uint16_t>(0x6d + i), i < offset.size() ? offset[i] : ' ');
    dma_segment_ = 0x1000;
    dma_offset_ = 0x80;
}

void FcbHost::sequential_read(Machine& m, std::uint16_t segment, std::uint16_t fcb) {
    if (!opened_) { m.set(Reg::AL, 0xff); return; }
    const auto field = [fcb](unsigned n) { return static_cast<std::uint16_t>(fcb + n); };
    const auto size = m.read16(segment, field(14));
    if (!size) throw std::runtime_error("FCB record size is zero");
    const auto block = m.read16(segment, field(12));
    const auto record = m.read8(segment, field(32));
    const auto position = (static_cast<std::size_t>(block) * 128 + record) * size;
    if (static_cast<unsigned>(dma_offset_) + size > 0x10000U) { m.set(Reg::AL, 2); return; }
    if (position >= input_.size()) { m.set(Reg::AL, 1); return; }
    const auto count = std::min<std::size_t>(size, input_.size() - position);
    for (std::uint16_t i = 0; i < size; ++i)
        m.write8(dma_segment_, static_cast<std::uint16_t>(dma_offset_ + i), i < count ? input_[position + i] : 0);
    const unsigned next = record + 1U;
    m.write8(segment, field(32), next & 127U);
    m.write16(segment, field(12), block + (next >> 7U));
    m.set(Reg::AL, count == size ? 0 : 3);
}

void FcbHost::interrupt(Machine& m, std::uint8_t number) {
    if (number == 0x20) { m.halted = true; return; }
    if (number != 0x21) throw std::runtime_error("unsupported interrupt in HEX2BIN backend");
    const auto segment = m.get(Reg::DS), fcb = m.get(Reg::DX);
    const auto field = [fcb](unsigned n) { return static_cast<std::uint16_t>(fcb + n); };
    switch (m.get(Reg::AH)) {
    case 9: {
        std::uint16_t pointer = fcb;
        for (unsigned i = 0; i < 0x10000; ++i) {
            const auto ch = m.read8(segment, pointer++);
            if (ch == '$') return;
            messages_ += static_cast<char>(ch);
        }
        throw std::runtime_error("unterminated DOS message");
    }
    case 15: // FCB open.
        opened_ = true;
        m.write16(segment, field(12), 0);
        m.write16(segment, field(14), 128);
        m.write16(segment, field(16), static_cast<std::uint16_t>(input_.size()));
        m.write16(segment, field(18), static_cast<std::uint16_t>(input_.size() >> 16U));
        m.set(Reg::AL, 0);
        return;
    case 16: // FCB close.
        m.set(Reg::AL, 0);
        return;
    case 20: // FCB sequential read.
        sequential_read(m, segment, fcb);
        return;
    case 21: { // FCB sequential write, used by TRANS.
        if (!created_) { m.set(Reg::AL, 1); return; }
        const auto size = m.read16(segment, field(14));
        const auto block = m.read16(segment, field(12));
        const auto record = m.read8(segment, field(32));
        const auto position = (static_cast<std::size_t>(block) * 128 + record) * size;
        if (!size || static_cast<unsigned>(dma_offset_) + size > 0x10000U) { m.set(Reg::AL, 2); return; }
        if (position + size > 4 * 1024 * 1024) throw std::runtime_error("translated output exceeds backend limit");
        if (output_.size() < position + size) output_.resize(position + size);
        for (std::size_t i = 0; i < size; ++i)
            output_[position + i] = m.read8(dma_segment_, static_cast<std::uint16_t>(dma_offset_ + i));
        const unsigned next = record + 1U;
        m.write8(segment, field(32), next & 127U);
        m.write16(segment, field(12), block + (next >> 7U));
        m.set(Reg::AL, 0);
        return;
    }
    case 22: // FCB create.
        created_ = true;
        output_.clear();
        m.write16(segment, field(12), 0);
        m.write16(segment, field(14), 128);
        m.set(Reg::AL, 0);
        return;
    case 19: // FCB delete: the in-memory output does not yet exist.
        m.set(Reg::AL, 0xff);
        return;
    case 26: // Set DTA.
        dma_segment_ = segment;
        dma_offset_ = fcb;
        return;
    case 38: { // DOS 1.x program-base setup; only the size field is needed here.
        const auto allocation = m.get(Reg::DX);
        if (allocation > 0x9000) throw std::runtime_error("load segment outside conventional memory");
        m.write16(allocation, 6, 0xfff0);
        return;
    }
    case 40: { // FCB random block write.
        if (!created_) { m.set(Reg::AL, 1); return; }
        const auto size = m.read16(segment, field(14));
        const std::uint32_t record = m.read16(segment, field(33)) |
            (static_cast<std::uint32_t>(m.read8(segment, field(35))) << 16U);
        const auto length = static_cast<std::size_t>(size) * m.get(Reg::CX);
        const auto position = static_cast<std::size_t>(record) * size;
        if (static_cast<std::size_t>(dma_offset_) + length > 0x10000U) {
            m.set(Reg::AL, 2); m.set(Reg::CX, 0); return;
        }
        if (position + length > 0x10000U) throw std::runtime_error("output exceeds 16-bit HEX2BIN limit");
        output_.resize(position + length);
        for (std::size_t i = 0; i < length; ++i)
            output_[position + i] = m.read8(dma_segment_, static_cast<std::uint16_t>(dma_offset_ + i));
        m.set(Reg::AL, 0);
        return;
    }
    default: throw std::runtime_error("unsupported DOS function in HEX2BIN backend");
    }
}
} // namespace msdos

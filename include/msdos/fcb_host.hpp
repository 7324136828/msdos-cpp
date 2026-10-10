#pragma once
#include "msdos/machine.hpp"
#include <span>
#include <string>
#include <vector>

namespace msdos {
// The DOS calls needed by the original HEX2BIN utility, with in-memory files.
// The command-line frontend alone touches the host filesystem.
class FcbHost {
public:
    explicit FcbHost(std::span<const std::uint8_t> input);
    void attach(Machine& machine);
    void prepare_psp(Machine& machine, std::string_view offset = "");
    const std::vector<std::uint8_t>& output() const { return output_; }
    const std::string& messages() const { return messages_; }
    bool created_output() const { return created_; }
    bool had_error() const { return !messages_.empty(); }

private:
    std::vector<std::uint8_t> input_, output_;
    std::string messages_;
    std::uint16_t dma_segment_ = 0x1000, dma_offset_ = 0x80;
    bool opened_ = false, created_ = false;
    void interrupt(Machine& m, std::uint8_t number);
    void sequential_read(Machine& m, std::uint16_t segment, std::uint16_t fcb);
};
} // namespace msdos

#include "msdos/fat12.hpp"
#include <stdexcept>

namespace msdos {
static std::size_t entry_offset(std::size_t size, std::size_t cluster) {
    // Check before multiplication/addition, including size_t overflow cases.
    if (cluster >= size || size < 2) throw std::out_of_range("FAT12 cluster outside table");
    if (cluster / 2 >= size - 1 - cluster) throw std::out_of_range("truncated FAT12 entry");
    const auto offset = cluster + cluster / 2;
    return offset;
}

std::uint16_t fat12_get(std::span<const std::uint8_t> table, std::size_t cluster) {
    const auto offset = entry_offset(table.size(), cluster);
    const auto word = static_cast<std::uint16_t>(table[offset] |
        (static_cast<unsigned>(table[offset + 1]) << 8U));
    return static_cast<std::uint16_t>((word >> ((cluster & 1U) ? 4U : 0U)) & 0xfffU);
}

void fat12_set(std::span<std::uint8_t> table, std::size_t cluster, std::uint16_t value) {
    if (value > 0xfff) throw std::invalid_argument("FAT12 values must fit in 12 bits");
    const auto offset = entry_offset(table.size(), cluster);
    const auto word = static_cast<unsigned>(table[offset]) |
        (static_cast<unsigned>(table[offset + 1]) << 8U);
    const auto replacement = (cluster & 1U) ? (word & 0xfU) | (static_cast<unsigned>(value) << 4U) :
        (word & 0xf000U) | value;
    table[offset] = static_cast<std::uint8_t>(replacement);
    table[offset + 1] = static_cast<std::uint8_t>(replacement >> 8U);
}
} // namespace msdos

#pragma once
#include <cstddef>
#include <cstdint>
#include <span>

namespace msdos {
// Port of the packed 12-bit table algorithm documented in v1.25/MSDOS.ASM.
std::uint16_t fat12_get(std::span<const std::uint8_t> table, std::size_t cluster);
void fat12_set(std::span<std::uint8_t> table, std::size_t cluster, std::uint16_t value);
} // namespace msdos

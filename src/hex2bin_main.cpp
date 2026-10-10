#include "msdos/fcb_host.hpp"
#include "ports.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

int main(int argc, char** argv) {
    if (argc == 2 && std::string_view(argv[1]) == "--help") {
        std::cout << "Usage: hex2bin INPUT.HEX OUTPUT.COM [OFFSET]\n"
            "Source-lifted MS-DOS 1.25 HEX2BIN; default offset -100 hex.\n"
            "OFFSET uses DOS uppercase hexadecimal spelling, e.g. +0 or -100.\n"
            "Preserves original behavior: record types/checksums are not validated.\n";
        return 0;
    }
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: hex2bin INPUT.HEX OUTPUT.COM [OFFSET]\n";
        return 2;
    }
    try {
        const std::filesystem::path inputPath(argv[1]), outputPath(argv[2]);
        if (std::filesystem::weakly_canonical(inputPath) == std::filesystem::weakly_canonical(outputPath) ||
            (std::filesystem::exists(outputPath) && std::filesystem::equivalent(inputPath, outputPath)))
            throw std::runtime_error("input and output must be different files");
        const std::string offset = argc == 4 ? argv[3] : "";
        const auto digits = offset.starts_with('+') || offset.starts_with('-') ? offset.substr(1) : offset;
        if (argc == 4 && (offset.size() > 8 || digits.empty() ||
            !std::all_of(digits.begin(), digits.end(), [](char ch) { return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F'); })))
            throw std::runtime_error("OFFSET must contain 1-7 uppercase hex digits, with an optional sign (8 characters total)");
        std::ifstream input(inputPath, std::ios::binary);
        if (!input) throw std::runtime_error("cannot open input file");
        const std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(input)), {});
        if (input.bad()) throw std::runtime_error("failed reading input file");
        msdos::Machine machine;
        msdos::FcbHost host(bytes);
        host.prepare_psp(machine, offset);
        host.attach(machine);
        msdos::v125::hex2bin(machine);
        // Like the original, errors detected during parsing may still produce partial output.
        // Backend failures throw before touching the destination.
        if (host.created_output()) {
            std::ofstream output(outputPath, std::ios::binary | std::ios::trunc);
            if (!output) throw std::runtime_error("cannot create output file");
            const auto& result = host.output();
            output.write(reinterpret_cast<const char*>(result.data()), static_cast<std::streamsize>(result.size()));
            output.close();
            if (!output) throw std::runtime_error("failed writing output file");
        }
        if (host.had_error()) { std::cerr << host.messages() << '\n'; return 1; }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "hex2bin: " << error.what() << '\n';
        return 1;
    }
}

#include "msdos/fcb_host.hpp"
#include "ports.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

int main(int argc, char** argv) {
    if (argc == 2 && std::string_view(argv[1]) == "--help") {
        std::cout << "Usage: trans INPUT.Z80 OUTPUT.ASM\nSource-lifted MS-DOS 1.25 Z80-to-8086 translator.\n"
            "Input is DOS text (CRLF; control-Z terminates). Output retains 128-byte record padding.\n";
        return 0;
    }
    if (argc != 3) { std::cerr << "Usage: trans INPUT.Z80 OUTPUT.ASM\n"; return 2; }
    try {
        const std::filesystem::path inputPath(argv[1]), outputPath(argv[2]);
        if (std::filesystem::weakly_canonical(inputPath) == std::filesystem::weakly_canonical(outputPath) ||
            (std::filesystem::exists(outputPath) && std::filesystem::equivalent(inputPath, outputPath)))
            throw std::runtime_error("input and output must be different files");
        std::ifstream input(inputPath, std::ios::binary);
        if (!input) throw std::runtime_error("cannot open input file");
        const std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(input)), {});
        if (input.bad()) throw std::runtime_error("failed reading input file");
        msdos::Machine machine;
        msdos::FcbHost host(bytes);
        host.prepare_psp(machine); host.attach(machine);
        msdos::v125::trans(machine);
        if (host.messages().find("Translation complete") == std::string::npos)
            throw std::runtime_error(host.messages().empty() ? "translation failed" : host.messages());
        std::ofstream output(outputPath, std::ios::binary | std::ios::trunc);
        if (!output) throw std::runtime_error("cannot create output file");
        const auto& result = host.output();
        output.write(reinterpret_cast<const char*>(result.data()), static_cast<std::streamsize>(result.size()));
        output.close();
        if (!output) throw std::runtime_error("failed writing output file");
        std::cout << host.messages();
        return 0;
    } catch (const std::exception& error) { std::cerr << "trans: " << error.what() << '\n'; return 1; }
}

# Agent Guide: MS-DOS-cpp (MSVC++ on Windows)

## Overview

This project is a C++ rewrite and porting environment for the original Microsoft MS-DOS source releases (v1.25, v2.0, v4.0), adapted to build and run with Microsoft Visual C++ (MSVC) in the current Windows environment while preserving complete functional equivalence with the original project.

The project consists of two core layers:
1. **Host C++20 Binaries (MSVC)**:
   - `msdos_core` (static library: CPU state machine, FCB host adapter, FAT12 driver, lifted routines from DOS 1.25, 2.0, and 30 DOS 4.0 mapper modules).
   - Host utilities: `hex2bin.exe` (HEX to COM converter) and `trans.exe` (Z80 to 8086 assembly translator).
   - Test suites: `msdos_tests.exe` (CPU/FAT12 core tests), `msdos_v4_tests.exe` (DOS 4.0 mapper contract tests), `msdos_native_flow_tests.exe` (translated control-flow tests), and `msdos_boot_tests.exe` (FAT12 boot kernel memory/filesystem tests).
   - CTest test runner orchestrating native executables and Node.js test suites (`boot_image_layout`, `conversion`, `hex2bin_cli`, `trans_cli`).
   - DOS 4.0 mapper audit target (`v4_audit`).
2. **Freestanding 16-Bit Real-Mode Boot Kernel**:
   - `boot/kernel.cpp`, `boot/boot.asm`, `boot/entry.asm`, `boot/demo.asm`, `boot/rwtest.asm`.
   - Produces a 1.44 MB bootable raw FAT12 floppy image (`build/boot/msdos-cpp.img`).
   - **Important Toolchain Note**: Modern MSVC (`cl.exe`) dropped 16-bit real-mode code generation in the 1990s. The freestanding real-mode kernel is built via `tools/build-boot.mjs` using `nasm` and GNU `g++ -m16 -march=i386` + `ld` + `objcopy` (provided by Strawberry MinGW at `C:\Strawberry\c\bin`), matching the original project design.

---

## Toolchain & Environment Specifications

- **OS**: Windows 11 / Windows Server x64
- **MSVC Compiler**: Microsoft Visual C++ 19.51 (Visual Studio Build Tools 2026 Dev18, toolset 14.51.36231)
  - Location: `C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools`
  - Environment Initialization: `C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat x64`
- **Build System**: CMake 3.29.2 + Ninja 1.11.1 (`C:\Strawberry\c\bin\ninja.exe`)
- **Runtime Environment**: Node.js v20+ (`C:\Program Files\nodejs\node.exe`)
- **16-bit Kernel Toolchain**: NASM 2.16+, GCC 13.2+ (`-m16`), GNU ld, objcopy (`C:\Strawberry\c\bin`)
- **Guest Verification**: Oracle VirtualBox 7.2.20 (`C:\Program Files\Oracle\VirtualBox\VBoxManage.exe`)

---

## Why MSVC Requires Special Handling on this System

1. **Default Path Collision**:
   `cl.exe` is not placed on the default system `PATH`. Running raw `cmake -B build` will locate Strawberry Perl's `c++.exe` (GCC 13.2.0) rather than MSVC.
2. **CMake Generator Version Gap**:
   CMake 3.29 only recognizes Visual Studio generators up to `Visual Studio 17 2022`. Visual Studio Build Tools 2026 is Dev18. Attempting `cmake -G "Visual Studio 17 2022"` errors with "could not find any instance of Visual Studio".
3. **Correct MSVC Pipeline**:
   The correct way to build with MSVC is using the **Ninja** generator inside an initialized MSVC environment (`vcvarsall.bat x64`).

---

## Quick Start & Build Scripts

Two automated wrappers are provided in the repository root to locate the MSVC toolchain dynamically via `vswhere.exe` and execute all actions cleanly.

### Batch Script (`build-msvc.bat`)

```cmd
:: Build everything (config, build, ctest, audit, boot image):
build-msvc.bat all

:: Individual tasks:
build-msvc.bat config      :: Configure CMake with MSVC + Ninja (Release)
build-msvc.bat build       :: Build msdos_core, hex2bin, trans, tests
build-msvc.bat test        :: Run all 8 CTest groups
build-msvc.bat audit       :: Audit DOS 4.0 mapper files and validation
build-msvc.bat boot        :: Build 1.44 MB bootable FAT12 floppy image
build-msvc.bat bootsmoke   :: Run VirtualBox headless boot verification
build-msvc.bat clean       :: Remove build directory
```

### PowerShell Script (`build-msvc.ps1`)

```powershell
# Run the entire pipeline:
.\build-msvc.ps1 -Action All

# Run individual actions:
.\build-msvc.ps1 -Action Config
.\build-msvc.ps1 -Action Build
.\build-msvc.ps1 -Action Test
.\build-msvc.ps1 -Action Audit
.\build-msvc.ps1 -Action BootImage
.\build-msvc.ps1 -Action BootSmoke
.\build-msvc.ps1 -Action Regenerate
.\build-msvc.ps1 -Action Clean
```

### Manual CMake Commands (from MSVC Command Prompt)

If working inside a Developer Command Prompt or PowerShell with `vcvarsall.bat x64`:

```powershell
# 1. Regenerate C++ from original assembly sources (optional, already generated)
node tools/convert.mjs generate

# 2. Configure CMake with Ninja
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# 3. Build all host targets
cmake --build build

# 4. Run full test suite
ctest --test-dir build --output-on-failure

# 5. Validate DOS 4.0 mapper modules
cmake --build build --target v4_audit

# 6. Build the freestanding boot floppy image
cmake --build build --target boot_image

# 7. (Optional) Run guest VirtualBox smoke test
cmake --build build --target boot_smoke_virtualbox
```

---

## MSVC Compiler Flags & Optimizations

In `CMakeLists.txt`, MSVC targets are configured with:

```cmake
if(MSVC)
    add_compile_options(/W4 /permissive- /utf-8 /wd4702 /wd4267 /wd4244 /EHsc /Zc:__cplusplus)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
endif()
```

- `/W4`: Highest practical warning level.
- `/permissive-`: Enforces standard C++ compliance and disables non-standard Microsoft extensions.
- `/utf-8`: Treats source and execution character sets as UTF-8 (prevents C4819 multibyte encoding issues).
- `/wd4702`: Disables warning C4702 (*unreachable code*). Translated MASM control flow generates unconditional jump and return structures that produce spurious unreachable code warnings in MSVC.
- `/wd4267` & `/wd4244`: Disables standard size truncation warnings (`size_t` to `uint32_t` / `uint16_t`) in the unit test fixtures on 64-bit architectures.
- `/EHsc`: Enables standard C++ stack unwinding for exception handling.
- `/Zc:__cplusplus`: Ensures the `__cplusplus` preprocessor macro correctly reports the C++20 standard value.

---

## Validation & Verification Results

All tests and targets verified passing on the current system with MSVC:

1. **CTest Suite (8/8 Passed)**:
   - `core`: Tests CPU semantics, registers, 20-bit address wrapping, flags, 8-bit arithmetic exhaustively, and FAT12 cluster packing.
   - `v4_mapper`: Tests all 30 DOS 4.0 mapper modules, stack canary preservation, far-call cleanup, memory allocation, and DOS/BIOS interrupts.
   - `native_control_flow`: Tests nested returns, indirect dispatch tables, REP loops, and nonlocal stack unwinds.
   - `boot_kernel`: Tests the C++ freestanding kernel filesystem operations against `build/boot/test-fixture.img`.
   - `boot_image_layout`: Validates FAT12 image structure, duplicate FAT tables, and directory entries.
   - `conversion`: Validates assembly-to-C++ parser, macro expander, and source integrity.
   - `hex2bin_cli`: Tests the MSVC-compiled `hex2bin.exe` binary against Intel HEX test vectors.
   - `trans_cli`: Tests the MSVC-compiled `trans.exe` binary against Z80 assembly translation test vectors.
2. **DOS 4.0 Mapper Audit (`v4_audit`)**:
   - 973 source files audited.
   - 30 complete translated mapper modules validated against runtime contracts.
   - 0 reference mismatches; `validation_passed: true`.
3. **FAT12 Floppy Image (`boot_image`)**:
   - Image: `build/boot/msdos-cpp.img` (1,474,560 bytes).
   - SHA-256: `670c6ae20e6e64fa2ecb1b8a992e5d445161d4efd069fd5670dd999c4148b8d7`.
   - Embedded files: `DEMO.COM`, `RWTEST.COM`, `README.TXT`, `COMP.COM`, `CHKDSK.COM`, `EDLIN.COM`.
4. **VirtualBox Smoke Test (`boot_smoke_virtualbox`)**:
   - Boots `msdos-cpp.img` in a dedicated headless VirtualBox VM with 16 MB RAM and floppy controller.
   - Executes guest commands: `DIR`, `TYPE README.TXT`, `DEMO README.TXT`, `RWTEST`, `COPY`, `REN`, `DEL`, `VER`, `HELP`, `CLS`, `HISTORY`.
   - Result: 100% passed; screenshots saved to `reports/`.

---

## Troubleshooting & Tips

- **VirtualBox "A VM named MS-DOS-cpp already exists"**:
  If a previous run left a VM registered to a different path or aborted, unregister it with:
  ```powershell
  & 'C:\Program Files\Oracle\VirtualBox\VBoxManage.exe' unregistervm MS-DOS-cpp --delete
  ```
- **Cleaning the Workspace**:
  Run `build-msvc.bat clean` or `.\build-msvc.ps1 -Action Clean` to delete `build/` and start fresh.


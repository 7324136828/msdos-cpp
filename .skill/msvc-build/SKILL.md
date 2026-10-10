---
name: msvc-build
description: Build, test, audit, and run the msdos-cpp project using MSVC++ (cl.exe), Ninja, and Windows toolchains.
---

# MSVC++ Build & Verification Skill for MS-DOS-cpp

Use this skill whenever building, testing, verifying, or debugging the `msdos-cpp` project using Microsoft Visual C++ (MSVC) in the current Windows environment.

## Environment Architecture

- **Host Compiler**: Microsoft Visual C++ (MSVC 19.51 / Visual Studio Build Tools 2026 Dev18, toolset 14.51).
- **Environment Setup**: `C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat x64`.
- **Build Generator**: Ninja (`C:\Strawberry\c\bin\ninja.exe`) driven by CMake 3.29.2.
- **Freestanding 16-Bit Boot Kernel**: Built using NASM (`nasm.exe`) and GCC (`g++ -m16 -march=i386`) from `C:\Strawberry\c\bin`. MSVC does not generate 16-bit real-mode code.

## Primary Workflows

### 1. Automated Full Build & Verification

Use the provided `build-msvc.bat` or `build-msvc.ps1` helper:

```cmd
build-msvc.bat all
```

Or in PowerShell:

```powershell
.\build-msvc.ps1 -Action All
```

This runs the five essential stages:
1. `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release`
2. `cmake --build build`
3. `ctest --test-dir build --output-on-failure`
4. `cmake --build build --target v4_audit`
5. `cmake --build build --target boot_image`

### 2. Running Individual Tasks

- **Configure only**:
  ```cmd
  build-msvc.bat config
  ```
- **Compile only**:
  ```cmd
  build-msvc.bat build
  ```
- **Run CTest suite**:
  ```cmd
  build-msvc.bat test
  ```
- **Audit DOS 4.0 mapper files**:
  ```cmd
  build-msvc.bat audit
  ```
- **Build FAT12 boot floppy image (`build/boot/msdos-cpp.img`)**:
  ```cmd
  build-msvc.bat boot
  ```
- **Run VirtualBox headless guest smoke test**:
  ```cmd
  build-msvc.bat bootsmoke
  ```
- **Clean build directory**:
  ```cmd
  build-msvc.bat clean
  ```

### 3. Manual MSVC Environment Setup

If running commands directly in a shell, first initialize the x64 MSVC environment:

```cmd
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
```

In PowerShell:

```powershell
$vcvars = "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"
$envLines = cmd.exe /c "`"$vcvars`" x64 >nul && set"
foreach ($line in $envLines) {
    if ($line -match "^([^=]+)=(.*)$") {
        [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2], "Process")
    }
}
```

### 4. CMake Presets

`CMakePresets.json` defines the `msvc-x64` preset:

```cmd
cmake --preset msvc-x64
cmake --build --preset msvc-x64
ctest --preset msvc-x64
```

## Compiler Flags in `CMakeLists.txt`

When `MSVC` is active, the following compile flags are applied:
- `/W4`: Warning Level 4.
- `/permissive-`: Strict C++ standards conformance.
- `/utf-8`: UTF-8 source and execution encoding.
- `/wd4702`: Suppresses unreachable code warnings from translated assembly jumps.
- `/wd4267 /wd4244`: Suppresses 64-bit to 32/16-bit integer conversion warnings in test suites.
- `/EHsc`: Standard C++ exception unwinding.
- `/Zc:__cplusplus`: Correct C++20 `__cplusplus` reporting.

## VirtualBox Test Notes

- The smoke test creates a dedicated VM named `MS-DOS-cpp`.
- If an older VM with the same name exists from a previous workspace, delete/unregister it with:
  ```powershell
  & 'C:\Program Files\Oracle\VirtualBox\VBoxManage.exe' unregistervm MS-DOS-cpp --delete
  ```
- Run VirtualBox tests under the same Windows account where VirtualBox Manager is registered.


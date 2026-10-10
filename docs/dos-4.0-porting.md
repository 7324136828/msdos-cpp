# DOS 4.0 transcription status

The audit accounts for all 1,281 files supplied in the DOS 4.0 source tree and compares their SHA-256 hashes with the sibling original tree when it is available. No reference files are missing or changed. There are 973 ASM/INC/C/H files and 103 additional macro, declaration, assembly configuration, and message source inputs, plus seven runtime/help resources to preserve or rebuild. Thirty complete MAPPER module bodies now generate buildable C++; 943 standard source files and the 103 additional inputs remain pending. The [complete file list](../reports/v4-missing.md) and [machine-readable report](../reports/v4-audit.json) identify every file and its status.

## Translated modules

| Area | Original modules |
| --- | --- |
| Directories and files | MKDIR, RMDIR, CHDIR, DELETE, CLOSE, READ, WRITE, LSEEK, SEL_DISK |
| File metadata and device control | QFILEMOD, SFILEMOD, QHANDTYP, SFILEINF, IOCTL |
| Memory | ALLOCSEG, REALLSEG, FREESEG |
| Version, verification, date/time | GETVER, QVERIFY, SVERIFY, GET_TOD, SET_TOD |
| Process and messages | EXIT, PUTMSG |
| Device queries and characters | DEVCONFG, DBCS, CASEMAP |
| Video | SCURPOS, SCROLLUP, WCHSTRA |

These are the supplied CP/DOS API mappers: they translate far-call parameters into DOS or BIOS interrupts. Translating their complete bodies does not implement the interrupt services they call. The generated files live under `generated/v4.0/mapper/` and compile into the host `msdos_core` library. The boot disk still contains the DOS 1.25 compatibility kernel.

The converter processes whole mapper files, expands the actual `MACROS.INC`, derives structure field offsets, translates LDS/LES pointers, and preserves far returns and callee stack cleanup. Unused EXTRN declarations are accepted as declarations; using an unresolved external symbol still fails conversion. It does not silently supply implementations for missing dependencies. Source/include hashes and original line references appear in each generated file.

The generated bodies execute as compiled C++ statements and branches, with no per-instruction ticking or program-counter executor. `Machine` retains DOS state and interrupt boundaries. These mapper bodies have no internal CALLs, so their translated returns use ordinary C++ `return` after DOS ABI cleanup. See [native execution](native-cpp-execution.md) for the handling of other modules with dynamic DOS calls.

## Calling a translated mapper

Exports in `generated/ports.hpp` take a `Machine&`. Set up the original parameters on SS:SP before calling the export; the wrapper supplies the far return address. Parameters must be pushed in the original calling sequence, with each far pointer pushed segment first, then offset. The mapper removes its own parameters on return. AX and flags reflect the original body; a successful return does not universally mean AX is zero.

For example, the version mapper expects a far pointer to a two-byte output word:

```cpp
msdos::Machine m;
m.set(msdos::Reg::SS, 0x3000);
m.set(msdos::Reg::SP, 0x9000);
m.push(0x4000); // output segment
m.push(0x0100); // output offset
m.interrupt_handler = [](msdos::Machine& cpu, std::uint8_t interrupt) {
    if (interrupt != 0x21 || cpu.get(msdos::Reg::AH) != 0x30)
        throw std::runtime_error("unexpected service");
    cpu.set(msdos::Reg::AX, 0x0004); // DOS 4.0: AL=major, AH=minor
};
msdos::v400::dosgetversion(m);
// Output word is 0x0400; SP is back at 0x9000.
```

An interrupt backend must supply the documented service results and memory side effects. For terminating calls, it can set `m.halted`; execution then stops before the mapper epilogue. This host API is not a decoder or binary loader for arbitrary DOS 4.0 programs.

## Validation and retained source behavior

`tests/v4_mapper_tests.cpp` exercises every translated export with controlled DOS/BIOS responses. It checks far-call cleanup, register preservation, success/error paths, pointer buffers, allocation sizes, signed bounds, and 16-bit offset wrapping. This establishes those C++ contracts; original assembled routine comparison and integration with a DOS 4.0 kernel remain unfinished.

The transcription deliberately retains observable source behavior, including:

- SVERIFY and several video bounds use signed byte comparisons.
- ALLOCSEG rounds up; REALLSEG truncates paragraph counts. Zero has the original full-segment meaning.
- GET_TOD reports the original fixed 360-minute GMT offset.
- DEVCONFG's model query computes AL but ultimately writes the existing BL.
- IOCTL uses a signed handle comparison and continues after DOS error 1.
- CASEMAP returns residual AX rather than a normalized success code. CASEMAP and DBCS run 65,536 iterations for a zero length because their original loops have no initial zero-count check.
- WCHSTRA falls through to error code 2 after completing its output and returns 1 at the screen boundary.

Corrections to historical behavior should be separate, explicitly tested changes. They should not be hidden inside a transcription.

## Reproduce the audit

```powershell
node tools/convert.mjs generate
cmake --build build
ctest --test-dir build --output-on-failure
cmake --build build --target v4_audit
```

The CMake audit target builds the mapper tests and passes their actual executable path to the auditor. With `BUILD_TESTING=OFF`, it inventories files without runtime validation. Direct use of `node tools/audit-v4.mjs --validate` defaults to the test executable in `build/`; `--test-executable <path>` supports another build directory.

The audit rejects stale generated C++, modules omitted from the CMake source list, and missing or changed reference copies. It keeps include files and additional source inputs visible as pending work; expanding the macros used by a mapper does not count the entire include file as a completed C++ module. Supplied binaries and build/resource files are recorded separately in the JSON report.

The remaining kernel, COMMAND, BIOS, drivers, utilities, SELECT, and memory manager require module-specific dependencies, configurations, and observable behavior tests before they can be counted as complete translations.

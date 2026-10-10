# Remaining work

The requested outcome is a bootable C++ operating system that runs DOS programs, with all supplied versions ported starting with 1.25. The bootable disk and specified guest smoke tests now pass in VirtualBox. The complete source rewrite remains unfinished.

## Acceptance gates

1. **Passed for the smoke-test scope:** VirtualBox and v86 boot the generated image; prompt, DIR, TYPE, repeated native DEMO launches, CALL 5, FCB writes, INT 25h and shell COPY/REN/DEL pass. See `reports/boot-runtime-virtualbox.json` and `reports/boot-smoke-virtualbox.txt`. Expand testing for callbacks and additional historical programs.
2. Validate the C++ translations against original assembled routines or binaries in an independent 8086 emulator. Source hashes and successful compilation are necessary but insufficient.
3. Complete the DOS 1.25 kernel services, devices, storage writes, memory/PSP setup, interrupt ABI, process termination, and original COMMAND behavior. Test actual historical applications and disk images.
4. Translate the remaining 1.25 tools and drivers, including ASM, IO, and COMMAND; resolve code pointers, overlays, source macros/conditionals, and original address dependencies explicitly.
5. Port DOS 2.0 kernel modules and COMMAND, including directories, handle APIs, EXE relocation/loading, EXEC, memory management, redirection, and pipelines.
6. Port DOS 4.0 boot/BIOS, kernel, utilities, SELECT, networking, and memory-manager code with module-specific hardware targets and build configurations. The [file audit](reports/v4-missing.md) lists 943 pending ASM/INC/C/H files and 103 additional source inputs after 30 complete mapper modules were transcribed. Seven runtime/help resources are tracked separately.
7. Treat the Ozzie material as its own historical variant. Inventory supplied sources versus binary-only components before claiming a complete source rewrite.

## Current status

| Component | Current result | Missing acceptance evidence/work |
| --- | --- | --- |
| Reference material | All four version trees copied, original hashes inventoried | Sources may not cover every supplied binary |
| Source lifting | Complete HEX2BIN and TRANS; PACK, SETVECT, DSUM, FCB records, COMMAND scanning/digits/formatting routines; 30 complete DOS 4.0 mapper modules with expanded original macros | Full MASM/SCP preprocessing, exact address layout, wider instruction set, differential execution |
| Generated execution | All 38 units compile to direct C++ statements, branches, and loops; per-instruction ticking and PC dispatch removed | DOS state adapter remains; ordinary typed APIs can be introduced for additional hand-written ports |
| Host utilities | Build and tested observable contracts | Original binary comparison, full behavior coverage |
| Boot loader and ABI | Boots in VirtualBox and v86; CHS cylinder handling and kernel ES restoration corrected | Broader interrupt/callback ABI testing |
| FAT12 and root files | Reads/writes, allocation, gap filling, resize/reclamation, both FAT copies, persistence tests | General media geometry, change detection, critical-error callbacks, multiple drives, failure recovery |
| INT 21h | Branches for all original 0–46 functions; FCB CRUD/records/parser; named devices; session clock | Exact ABI/console/device/process semantics; see the [service matrix](docs/dos-1.25-services.md) |
| Native COM programs | DEMO and RWTEST execute in VirtualBox and v86, including repeated launches, CALL 5 and INT 25h flags-stack cleanup | INT 26h native writes, callback behavior and broad application compatibility |
| Command prompt | DIR/TYPE/COPY/REN/DEL, REM/PAUSE, DATE/TIME/VERIFY, COM and BAT with arguments | Original COMMAND source port; COPY modes/concatenation; exact batch and editing behavior |
| DOS 2.0 | One date-table routine translated | Kernel/BIOS/COMMAND/utility rewrite |
| DOS 4.0 | All 1,281 original/reference file hashes match; 30 complete mapper module bodies compile and pass host contract tests | 943 ASM/INC/C/H files and 103 additional source inputs; kernel/BIOS/COMMAND/driver port; original-binary comparison; guest integration |
| Ozzie | Reference inventory | Implementation |

Every automatic conversion must preserve widths, wrapping, aliasing, and observable flags. Hardware and binary-layout dependencies need explicit adapters or a faithful real-mode build; unsupported operations must not silently become no-ops.

## Verification environment

Windows MinGW GCC 13.2, CMake 3.29.2, NASM 2.16.01 and Node 24.18.0 were used. All eight CTest groups pass. DOS 4.0 mapper validation uses controlled DOS/BIOS responses, rather than an implementation of the DOS 4.0 services. The installed VirtualBox 7.2.20r175154 passes all 12 guest smoke checks for the DOS 1.25 compatibility image, with screenshots and a serial transcript saved under `reports/`. v86 also passes the same checks using workspace-local, hash-checked assets. QEMU/Bochs are not installed; those backends remain untested. Neither guest test establishes full DOS compatibility or completion of the source rewrite.

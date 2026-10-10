# DOS 1.25 implementation status

This C++ compatibility kernel has dispatch branches for every function (0–46)
in `reference/v1.25/source/MSDOS.ASM`'s DISPATCH table. **A dispatch branch is not
full compatibility or a completed source rewrite.** The table below describes
implemented behavior and its limits. Native DEMO and RWTEST execution passes
in VirtualBox and v86; only the recorded smoke-test scope is established.

| AH (hex) | Original service | Current C++ behavior |
| --- | --- | --- |
| 00 | ABORT | Return to the C++ shell; flush writes after program return. Original INT 22h exit callback behavior is incomplete. |
| 01–02 | CONIN, CONOUT | Character input with echo and byte-preserving output. Full Ctrl-C, Ctrl-P and console editing behavior is incomplete. |
| 03–05 | READER, PUNCH, LIST | BIOS COM1 input/output and LPT1 output. COM1 is also used for the debug terminal. Peripheral behavior is not guest-tested. |
| 06 | RAWIO | Direct output; nonblocking input updates ZF. |
| 07–08 | RAWINP, IN | Input without echo; full Ctrl-C differences are incomplete. |
| 09 | PRTBUF | `$`-terminated byte-preserving string output, bounded to one segment. |
| 0A | BUFIN | Buffered input, backspace and CR termination. Original template/function-key editing is incomplete. |
| 0B–0C | CONSTAT, FLUSHKB | Input status; flush and optional redispatch to input functions. |
| 0D–0E | DSKRESET, SELDSK | Persist dirty FAT/root metadata; one drive (A), returns one available drive. Media-change detection is incomplete. |
| 0F–10 | OPEN, CLOSE | Normal/extended FCB files and named CON/AUX/NUL/PRN devices. LST/COM1/LPT1 aliases are recognized. Exact reserved-field and dirty-FCB behavior is incomplete. |
| 11–12 | SRCHFRST, SRCHNXT | Wildcards, hidden/system attribute filtering, per-FCB search state, drive + directory entry results, extended result prefix. Named-device search is incomplete. |
| 13 | DELETE | Wildcard deletion and cluster reclamation; reject read-only files, invalid FAT links and cyclic chains. |
| 14–15 | SEQRD, SEQWRT | Record-sized sequential reads/writes, partial-record padding, extent/current-record updates. |
| 16 | CREATE | Create or truncate, optional extended-FCB attributes, initialize record size; device names do not become disk files. |
| 17 | RENAME | Wildcard substitution, collision preflight, persistent directory updates. |
| 18 | INUSE | Returns zero, as in the original DOS 1.25 compatibility handler. |
| 19–1A | GETDRV, SETDMA | A as current drive; segmented DTA. |
| 1B–1C | GETFATPT, GETFATPTDL | FAT pointer, cluster count, sector size and sectors per cluster for the fixed floppy. |
| 1D–1E | GETRDONLY, SETATTRIB | Return zero, as in the original DOS 1.25 compatibility handler; not later DOS attribute APIs. |
| 1F | GETDSKPT | Original 20-byte DPB shape for the generated floppy geometry. |
| 20 | USERCODE | Returns zero, as in the original compatibility handler. |
| 21–22 | RNDRD, RNDWRT | Single random record; sequential position set to that record, RR unchanged. |
| 23–24 | FILESIZE, SETRNDREC | Round file bytes up to records; derive RR from extent/NR; four-byte RR when record size is below 64. |
| 25 | SETVECT | Write the requested real-mode interrupt vector. Custom DOS exit/Ctrl-C/critical-error callbacks are not fully supported. |
| 26 | NEWBASE | Copy/prepare a PSP in a specified segment, including vectors and CALL 5. Full process relocation/ownership semantics are incomplete. |
| 27–28 | BLKRD, BLKWRT | Random block transfers with CX count and position updates; zero-count write resizes/truncates. |
| 29 | MAKEFCB | DS:SI to ES:DI parser, wildcard expansion, default-preservation flags, drive validation and delimiter position. |
| 2A–2D | GETDATE, SETDATE, GETTIME, SETTIME | RTC seed, BIOS-tick session clock, weekday/leap-date validation and midnight rollover. Setters update the session clock; they do not program the RTC. More than one day between clock queries is not tracked reliably. |
| 2E | VERIFY | Read-back verification of filesystem sector writes. |

The additional AH=30h version query returns 1.25 as a compatibility extension;
it was not part of the original 1.25 table. Unknown functions currently print a
diagnostic and return to the shell, rather than emulate later DOS versions.

INT 20h, INT 21h and CP/M CALL 5 entry/exit are assembled native boundaries.
INT 25h/26h absolute sector access checks drive, disk bounds and DMA wrap; the
native return preserves the extra flags word on the caller's stack. Host tests
exercise both C++ handlers. VirtualBox and v86 guest tests additionally verify
INT 20h/21h, repeated COM returns, CALL 5 and INT 25h flags-stack cleanup.
INT 26h native writes and arbitrary guest callbacks remain untested.

Storage is deliberately limited to the generated 1.44 MB FAT12 image: 65 reserved
sectors, two nine-sector FATs, 224 root entries, one sector per cluster. Writes
report I/O failures; they are not atomic under media failure or power loss. There
is no critical-error retry callback, directory support, multiple-drive support,
or general DOS media/BIOS adaptation yet.

The C++ shell implements DIR (wildcards, /W, /P), TYPE, simple binary COPY,
DEL/ERASE, REN/RENAME, REM, PAUSE, DATE, TIME, VERIFY, native COM launch, and BAT
files with %0–%9 substitution (four nesting levels). AUTOEXEC.BAT is run if found.
The shell also has a modern VGA terminal with CLS/CLEAR, VER, HISTORY, eight
commands of session history, Tab completion, and cursor/insertion editing.
[Terminal controls](terminal.md) document its shortcuts and display limits.
This is not a complete translation of COMMAND 1.17: COPY concatenation and /A,
/B switches, exact batch chaining, original prompt/date syntax, command-overlay
loading, and the original COMMAND template/function-key editing are still missing.

Automatically lifted GETREC, SETRNDREC and GETRRPOS live in
`generated/v1.25/fcb_records.cpp`. The native compatibility kernel uses its own
freestanding implementations; the portable lifted code retains the source
register/flag operations and is tested separately.

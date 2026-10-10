/* The PE/COFF container is stripped to a flat real-mode image by objcopy. */
ENTRY(_start)
SECTIONS {
    . = 0;
    .text : { *(.text.start) *(.text*) }
    .rodata : { *(.rdata*) *(.rodata*) }
    .data : { *(.data*) }
    _bss_start = .;
    .bss : { *(.bss*) *(COMMON) }
    _bss_end = .;
    ASSERT(_bss_start <= 32768, "kernel code/data exceed the reserved boot sectors")
    ASSERT(_bss_end <= 49152, "kernel BSS overlaps interrupt stack reserve")
    /DISCARD/ : { *(.eh_frame*) *(.pdata*) *(.xdata*) *(.comment*) *(.note*) }
}

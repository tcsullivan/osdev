OUTPUT_ARCH(i386)
OUTPUT(stage2.elf)
ENTRY(main)

MEMORY
{
    MEM  (rwx) : org = 0x10000,  len = 8K
}

SECTIONS
{
    .text : {
        *(.entry)
        *(.text*)
        *(.rodata*)
        *(.data*)
        *(.bss*)
    } > MEM

    /DISCARD/ : {
        *(.dynsym)
        *(.dynstr)
        *(.dynamic)
        *(.eh_frame*)
        *(.gnu.hash)
        *(.comment)
        *(.note*)
    }
}


OUTPUT_ARCH(i386)
OUTPUT(boot.elf)
ENTRY(boot)

MEMORY
{
    MEM  (rx) : org = 0x3E,  len = 448
    BOOT (rx) : org = 0x1FE, len = 2
}

SECTIONS
{
    .text : {
        *(.text)
        *(.rodata)
    } > MEM

    .bootable : {
        *(.bootable)
    } > BOOT

    /DISCARD/ : {
        *(.dynsym)
        *(.dynstr)
        *(.dynamic)
        *(.eh_frame*)
        *(.gnu.hash)
        *(.comment)
    }
}


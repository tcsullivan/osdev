#include <stdint.h>

__attribute__((section(".bootable")))
static const uint8_t bootable[2] = { 0x55, 0xAA };

__attribute__((naked))
void boot()
{
    asm("cli");

    asm("int $0x10" :: "a" (0x0E00 | 'A'), "b" (0x0007), "c" (1)); 

    while (1);
}


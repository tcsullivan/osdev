/*
 * 0x 0000 7C00 = stage1 (512 bytes)
 *    0001 0000 = stage2 (8k)
 *    000B 8000 = video mem...
 *    0010 0000 = 1M
 */

#include "idt.h"
#include "port.h"
#include "printf.h"
#include "tinyalloc.h"
#include "videoram.h"

#include <stdint.h>

#define MEM_HIGH1_START ((void *)0x00100000)

static const char msg[] = "Welcome to stage2!";

// mem_low: kB starting at zero (usually zero to 0xA0000, 640kB)
// mem_high1: kB start at 0x0010'0000 (around 15MB)
// mem_high2: 64kB start at 0x0100'0000 (the rest)
__attribute__((section(".entry")))
int main(uint32_t mem_low, uint32_t mem_high1, uint32_t mem_high2)
{
    kputs("Welcome!\n");

    ta_init(MEM_HIGH1_START, (void *)(MEM_HIGH1_START + mem_high1 * 1024), 256, 16, 4);

    char *buf = ta_alloc(64);
    snprintf(buf, 64, "Total Mem: %lu kB\n", mem_low + mem_high1 + (mem_high2 * 64));
    kputs(buf);
    snprintf(buf, 64, "Heap given %lu kB\n", ta_free_bytes() / 1024);
    kputs(buf);

    idtInitialize();
    kputs("Interrupt table installed.");

    // PIT
    uint32_t divisor = (1193180 / 50) & 0xFFFF;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);

    asm("sti");

    ta_free(buf);
    while (1);
    return 0;
}


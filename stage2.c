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

#include <stdarg.h>
#include <stdint.h>

#define MEM_HIGH1_START ((void *)0x00100000)

#define KPRINTF_BUFFER_SIZE (512)
static char *kprintf_buffer = NULL;
int kprintf(const char *s, ...)
{
    va_list args;
    va_start(args, s);
    int result = vsnprintf(kprintf_buffer, KPRINTF_BUFFER_SIZE, s, args);
    va_end(args);

    kputs(kprintf_buffer);
    return result;
}

volatile static uint32_t pit_millis = 0;
void pit(void) {
    ++pit_millis;
}

uint32_t millis(void) {
    return pit_millis;
}

// mem_low: kB starting at zero (usually zero to 0xA0000, 640kB)
// mem_high1: kB start at 0x0010'0000 (around 15MB)
// mem_high2: 64kB start at 0x0100'0000 (the rest)
__attribute__((section(".entry")))
int main(uint32_t mem_low, uint32_t mem_high1, uint32_t mem_high2)
{
    kputs("Welcome!\n");

    ta_init(MEM_HIGH1_START, (void *)(MEM_HIGH1_START + mem_high1 * 1024), 256, 16, 4);

    kprintf_buffer = ta_alloc(KPRINTF_BUFFER_SIZE);
    if (kprintf_buffer == NULL) {
        kputs("Failed to allocate memory!");
        return 0;
    }
        
    kprintf("Total Mem: %lu kB\n", mem_low + mem_high1 + (mem_high2 * 64));
    kprintf("Heap given %lu kB\n", ta_free_bytes() / 1024);

    idtInitialize();
    kprintf("Interrupt table installed.\n");

    // PIT
    uint32_t divisor = (1193180 / 1000) & 0xFFFF;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
    irqInstallHandler(0, pit);

    asm("sti");

    uint32_t target = 0;
    while (1) {
        uint32_t m = millis();
        if (m > target) {
            kprintf("tick: %lu\n", m);
            target += 1000;
        }
    }

    ta_free(kprintf_buffer);
    while (1);
    return 0;
}


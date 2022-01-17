/*
 * 0x 0000 7C00 = stage1 (512 bytes)
 *    0001 0000 = stage2 (8k)
 *    000B 8000 = video mem...
 *    0010 0000 = 1M
 */

#include "printf.h"

#include <stdint.h>

void outb(uint16_t port, uint8_t val) {
    asm("out %0, %1" :: "Nd" (port), "a" (val));
}
#define TERM_WIDTH  (80)
#define TERM_HEIGHT (25)
#define TERM_BASE   ((uint16_t *)0xB8000)

volatile static uint16_t *term = TERM_BASE + TERM_WIDTH;
static uint8_t termColor = 0x07;

static const char msg[] = "Welcome to stage2!";

void termCursor(unsigned int pos) {
    outb(0x03D4, 0x0F);
    outb(0x03D5, pos & 0xFF);
    outb(0x03D4, 0x0E);
    outb(0x03D5, pos >> 8);
}

void kputs(const char *s) {
    uint16_t tc = termColor << 8;
    while (*s) {
        if (*s == '\n') {
            ++s;
            term += TERM_WIDTH;
            term = TERM_BASE + (term - TERM_BASE) / TERM_WIDTH * TERM_WIDTH;
        } else {
            *term++ = tc | *s++;
        }
    }
    termCursor(term - TERM_BASE);
}

// mem_low: kB starting at zero (usually zero to 0xA0000, 640kB)
// mem_high1: kB start at 0x0010'0000 (around 15MB)
// mem_high2: 64kB start at 0x0100'0000 (the rest)
__attribute__((section(".entry")))
int main(uint32_t mem_low, uint32_t mem_high1, uint32_t mem_high2)
{
    kputs("Welcome!\n");

    char buf[64];
    snprintf(buf, sizeof(buf), "Mem: %lu, %lu, %lu.\n", mem_low, mem_high1, mem_high2);
    kputs(buf);
    snprintf(buf, sizeof(buf), "Total Mem: %lu kB", mem_low + mem_high1 + (mem_high2 * 64));
    kputs(buf);

    while (1);
    return 0;
}


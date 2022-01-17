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
    while (*s)
        *term++ = tc | *s++;
    termCursor(term - TERM_BASE);
}

__attribute__((section(".entry")))
void stage2()
{
    char buf[20];
    snprintf(buf, 20, "Hello %d.", 42);
    kputs(msg);
    kputs(buf);

    while (1);
}


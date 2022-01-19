#include "port.h"

#include <stdint.h>

#define TERM_WIDTH  (80)
#define TERM_HEIGHT (25)
#define TERM_BASE   ((uint16_t *)0xB8000)

volatile static uint16_t *term = TERM_BASE + TERM_WIDTH;
static uint8_t termColor = 0x07;

static void termCursor(unsigned int pos) {
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





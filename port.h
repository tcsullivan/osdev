#include <stdint.h>

inline static void outb(uint16_t port, uint8_t val) {
    asm("out %0, %1" :: "Nd" (port), "a" (val));
}

inline static uint8_t inb(uint16_t port) {
    uint8_t ret = 0;
    asm("in %0, %1" : "=a" (ret) : "Nd" (port));
    return ret;
}


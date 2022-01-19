#include <stdint.h>

inline static void outb(uint16_t port, uint8_t val) {
    asm("out %0, %1" :: "Nd" (port), "a" (val));
}


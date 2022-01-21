#include <stdint.h>

#define IDT_ADDR (0x00000800)

typedef void (*irq_handler_t)(void);

void idtInitialize(void);
void irqInstallHandler(unsigned int n, irq_handler_t handler);


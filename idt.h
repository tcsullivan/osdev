#include <stdint.h>

typedef struct {
   uint16_t base_lo;
   uint16_t sel;
   uint8_t unused;
   uint8_t flags;
   uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct {
   uint16_t limit;
   uint32_t base;
} __attribute__((packed)) idtr_t;

static idt_entry_t *idt_table/*[256]*/ = (idt_entry_t *)0x00000800;
static const idtr_t idt_idtr = {
    .limit = 256 * sizeof(idt_entry_t) - 1,
    .base = 0x00000800
};

__attribute__((naked)) static void isr_unused0(void) { asm("cli; push 0; push 0; jmp isr_common"); }
__attribute__((naked)) static void isr_unused1(void) { asm("cli; push 0; push 1; jmp isr_common"); }
__attribute__((naked)) static void isr_unused2(void) { asm("cli; push 0; push 2; jmp isr_common"); }
__attribute__((naked)) static void isr_unused3(void) { asm("cli; push 0; push 3; jmp isr_common"); }
__attribute__((naked)) static void isr_unused4(void) { asm("cli; push 0; push 4; jmp isr_common"); }
__attribute__((naked)) static void isr_unused5(void) { asm("cli; push 0; push 5; jmp isr_common"); }
__attribute__((naked)) static void isr_unused6(void) { asm("cli; push 0; push 6; jmp isr_common"); }
__attribute__((naked)) static void isr_unused7(void) { asm("cli; push 0; push 7; jmp isr_common"); }
__attribute__((naked)) static void isr_unused8(void) { asm("cli; push 0; jmp isr_common"); }
__attribute__((naked)) static void isr_unused9(void) { asm("cli; push 0; push 9; jmp isr_common"); }
__attribute__((naked)) static void isr_unused10(void) { asm("cli; push 10; jmp isr_common"); }
__attribute__((naked)) static void isr_unused11(void) { asm("cli; push 11; jmp isr_common"); }
__attribute__((naked)) static void isr_unused12(void) { asm("cli; push 12; jmp isr_common"); }
__attribute__((naked)) static void isr_unused13(void) { asm("cli; push 13; jmp isr_common"); }
__attribute__((naked)) static void isr_unused14(void) { asm("cli; push 14; jmp isr_common"); }
__attribute__((naked)) static void isr_unused15(void) { asm("cli; push 0; push 15; jmp isr_common"); }
__attribute__((naked)) static void isr_unused16(void) { asm("cli; push 0; push 16; jmp isr_common"); }
__attribute__((naked)) static void isr_unused17(void) { asm("cli; push 0; push 17; jmp isr_common"); }
__attribute__((naked)) static void isr_unused18(void) { asm("cli; push 0; push 18; jmp isr_common"); }
__attribute__((naked)) static void isr_unused19(void) { asm("cli; push 0; push 19; jmp isr_common"); }
__attribute__((naked)) static void isr_unused20(void) { asm("cli; push 0; push 20; jmp isr_common"); }
__attribute__((naked)) static void isr_unused21(void) { asm("cli; push 0; push 21; jmp isr_common"); }
__attribute__((naked)) static void isr_unused22(void) { asm("cli; push 0; push 22; jmp isr_common"); }
__attribute__((naked)) static void isr_unused23(void) { asm("cli; push 0; push 23; jmp isr_common"); }
__attribute__((naked)) static void isr_unused24(void) { asm("cli; push 0; push 24; jmp isr_common"); }
__attribute__((naked)) static void isr_unused25(void) { asm("cli; push 0; push 25; jmp isr_common"); }
__attribute__((naked)) static void isr_unused26(void) { asm("cli; push 0; push 26; jmp isr_common"); }
__attribute__((naked)) static void isr_unused27(void) { asm("cli; push 0; push 27; jmp isr_common"); }
__attribute__((naked)) static void isr_unused28(void) { asm("cli; push 0; push 28; jmp isr_common"); }
__attribute__((naked)) static void isr_unused29(void) { asm("cli; push 0; push 29; jmp isr_common"); }
__attribute__((naked)) static void isr_unused30(void) { asm("cli; push 0; push 30; jmp isr_common"); }
__attribute__((naked)) static void isr_unused31(void) { asm("cli; push 0; push 31; jmp isr_common"); }
__attribute__((naked)) static void isr_unused32(void) { asm("cli; push 0; push 32; jmp isr_common"); }
__attribute__((naked)) static void isr_unused33(void) { asm("cli; push 0; push 33; jmp isr_common"); }
__attribute__((naked)) static void isr_unused34(void) { asm("cli; push 0; push 34; jmp isr_common"); }
__attribute__((naked)) static void isr_unused35(void) { asm("cli; push 0; push 35; jmp isr_common"); }
__attribute__((naked)) static void isr_unused36(void) { asm("cli; push 0; push 36; jmp isr_common"); }
__attribute__((naked)) static void isr_unused37(void) { asm("cli; push 0; push 37; jmp isr_common"); }
__attribute__((naked)) static void isr_unused38(void) { asm("cli; push 0; push 38; jmp isr_common"); }
__attribute__((naked)) static void isr_unused39(void) { asm("cli; push 0; push 39; jmp isr_common"); }
__attribute__((naked)) static void isr_unused40(void) { asm("cli; push 0; push 40; jmp isr_common"); }
__attribute__((naked)) static void isr_unused41(void) { asm("cli; push 0; push 41; jmp isr_common"); }
__attribute__((naked)) static void isr_unused42(void) { asm("cli; push 0; push 42; jmp isr_common"); }
__attribute__((naked)) static void isr_unused43(void) { asm("cli; push 0; push 43; jmp isr_common"); }
__attribute__((naked)) static void isr_unused44(void) { asm("cli; push 0; push 44; jmp isr_common"); }
__attribute__((naked)) static void isr_unused45(void) { asm("cli; push 0; push 45; jmp isr_common"); }
__attribute__((naked)) static void isr_unused46(void) { asm("cli; push 0; push 46; jmp isr_common"); }
__attribute__((naked)) static void isr_unused47(void) { asm("cli; push 0; push 47; jmp isr_common"); }

static void isr_common_main(uint32_t n, uint32_t e)
{
    static char buf[20];

    n &= 0xFF;
    e &= 0xFF;

    sprintf(buf, "!%u!", n);
    kputs(buf);

    if (n >= 32) {
        outb(0x20, 0x20);
        if (n >= 40)
            outb(0xA0, 0x20);
    }
}

__attribute__((naked))
static void isr_common(uint32_t n, uint32_t e)
{
    asm("call isr_common_main; add esp, 8; sti; iret");
}

static void idtSetEntry(unsigned int n, void (*handler)(void))
{
    idt_table[n].base_lo = (uint32_t)handler & 0xFFFF;
    idt_table[n].base_hi = (uint32_t)handler >> 16;
    idt_table[n].sel = 0x08;
    idt_table[n].flags = 0x8E;
}

void idtInitialize(void)
{
    char *idt_table_cptr = (char *)idt_table;
    for (unsigned int i = 0; i <= idt_idtr.limit; ++i)
        *idt_table_cptr++ = 0;

    idtSetEntry(0, isr_unused0);
    idtSetEntry(1, isr_unused1);
    idtSetEntry(2, isr_unused2);
    idtSetEntry(3, isr_unused3);
    idtSetEntry(4, isr_unused4);
    idtSetEntry(5, isr_unused5);
    idtSetEntry(6, isr_unused6);
    idtSetEntry(7, isr_unused7);
    idtSetEntry(8, isr_unused8);
    idtSetEntry(9, isr_unused9);
    idtSetEntry(10, isr_unused10);
    idtSetEntry(11, isr_unused11);
    idtSetEntry(12, isr_unused12);
    idtSetEntry(13, isr_unused13);
    idtSetEntry(14, isr_unused14);
    idtSetEntry(15, isr_unused15);
    idtSetEntry(16, isr_unused16);
    idtSetEntry(17, isr_unused17);
    idtSetEntry(18, isr_unused18);
    idtSetEntry(19, isr_unused19);
    idtSetEntry(20, isr_unused20);
    idtSetEntry(21, isr_unused21);
    idtSetEntry(22, isr_unused22);
    idtSetEntry(23, isr_unused23);
    idtSetEntry(24, isr_unused24);
    idtSetEntry(25, isr_unused25);
    idtSetEntry(26, isr_unused26);
    idtSetEntry(27, isr_unused27);
    idtSetEntry(28, isr_unused28);
    idtSetEntry(29, isr_unused29);
    idtSetEntry(30, isr_unused30);
    idtSetEntry(31, isr_unused31);
    idtSetEntry(32, isr_unused32);
    idtSetEntry(33, isr_unused33);
    idtSetEntry(34, isr_unused34);
    idtSetEntry(35, isr_unused35);
    idtSetEntry(36, isr_unused36);
    idtSetEntry(37, isr_unused37);
    idtSetEntry(38, isr_unused38);
    idtSetEntry(39, isr_unused39);
    idtSetEntry(40, isr_unused40);
    idtSetEntry(41, isr_unused41);
    idtSetEntry(42, isr_unused42);
    idtSetEntry(43, isr_unused43);
    idtSetEntry(44, isr_unused44);
    idtSetEntry(45, isr_unused45);
    idtSetEntry(46, isr_unused46);
    idtSetEntry(47, isr_unused47);

    // IRQ remapping
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    asm("lidt [eax]" :: "a" (&idt_idtr));
}


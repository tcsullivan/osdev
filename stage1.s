org 0x3E
bits 16

boot:
    cli
.initSegments:
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
.clearVideo:
    mov ax, 0x0003
    int 0x10
.welcomeMessage:
    mov ax, 0x1301
    mov bx, 0x0007
    mov cx, msg_end - msg
    xor dx, dx
    mov bp, msg
    int 0x10
.checkLowMemory:
    clc
    int 0x12
    jc .loadStage2
    mov [mem_low], ax
.checkHighMemory:
    xor cx, cx
    clc
    mov ax, 0xE801
    int 0x15
    jc .loadStage2
    cmp ah, 0x86
    je .loadStage2
    cmp ah, 0x80
    je .loadStage2
    jcxz .checkHighMemoryAx
    mov ax, cx
    mov bx, dx
.checkHighMemoryAx:
    mov [mem_high1], ax
    mov [mem_high2], bx
.loadStage2:
    mov ax, 0x1000 ; for stage2
    mov es, ax
    mov ax, 0x0208 ; 8 sectors = 4k
    xor bx, bx
    mov cx, 0x0002
    xor dh, dh
    mov dl, 0x00   ; floppy 0
    int 0x13
    jc .fail
    cmp ax, 0x0008
    jne .fail
.enableA20Line:
    in al, 0x92
    or al, 2
    out 0x92, al
.prepareGDT:
    nop
    nop
    lgdt [ds:gdtr]
    nop
    nop
.enterProtectedMode:
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp 0x08:stage2Entry + 0x7C00
.fail:
    jmp $

msg:     db "Loading stage2...",0x0D,0x0A
msg_end:

mem_low: dw 0
mem_high1: dw 0
mem_high2: dw 0

align 16

gdt:
    ; 0x00: 0 0 0
    dq 0x0000000000000000
    ; 0x08: code 0 f 9A
    dq 0x00CF9A000000FFFF
    ; 0x10: data 0 f 92
    dq 0x00CF92000000FFFF
gdtr:
    dw gdtr - gdt - 1
    dd gdt + 0x7C00

bits 32

stage2Entry:
    mov dx, 0x10
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx
    mov ss, dx
    mov esp, 0x00010000
    mov ebp, 0x00010000
    xor eax, eax
    mov ax, [0x7C00 + mem_high2]
    push eax
    mov ax, [0x7C00 + mem_high1]
    push eax
    mov ax, [0x7C00 + mem_low]
    push eax
    mov eax, 0x00010000
    call eax
    cli
    hlt


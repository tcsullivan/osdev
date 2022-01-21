org 0x3E
bits 16

boot:
    cli
.initSegments:
    mov ax, 0x07C0
    mov ds, ax
;    mov es, ax
    mov fs, ax
    mov gs, ax
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00
    mov bp, sp
.clearVideo:
    mov ax, 0x0003
    int 0x10
.loadStage1:
    mov ax, 0x07E0
    mov es, ax
    mov ax, 1
    xor bx, bx
    call floppyread
    mov ax, 0x07C0
    mov es, ax
.welcomeMessage:
    mov ax, 0x1301
    mov bx, 0x0007
    mov cx, msg_end - msg
    xor dx, dx
    mov bp, msg
    nop
    nop
    int 0x10

    call getmeminfo

.loadRootDir:
    mov bx, [ds:0x0E]
    xor ax, ax
    mov al, [ds:0x10]
    mul word [ds:0x16]
    add ax, bx          ; ax = first_root_dir_sector
    push ax

    mov bx, 0x0800
    mov es, bx
    xor bx, bx
    call floppyread     ; root dir to 0x8000

    xor di, di
.findStage2:
    push di
    mov si, kernel
    mov cx, 11
    cld
    repe cmpsb
    jecxz .foundStage2  ; found KERNEL.SYS
    pop di
    add di, 32
    cmp di, 0x0200
    jne .findStage2
    mov ax, 0x0E45      ; error, no KERNEL.SYS
    mov bx, 0x0007
    int 0x10
.foundStage2:
    pop di              ; di -> start of file entry
    add di, 26
    mov cx, [es:di]       ; cx = first cluster
.rootDirSectors:
    mov ax, [ds:0x11]
    shl ax, 5           ; mul word 32
    add ax, 511
    shr ax, 9           ; div word 512
    pop bx              ; bx = first root dir sector
    add ax, bx          ; ax = first data sector
    xchg ax, cx         ; ax = first cluster, cx = first data sector

    xor di, di          ; es:di = kernel addr
.readCluster:
    push ax             ; current cluster
    sub ax, 2
    xor bx, bx
    mov bl, [ds:0x0D]
    mul bx
    add ax, cx          ; ax = lba

    mov bx, 0x1000
    mov es, bx
    mov bx, di          ; es:bx = load addr
    call floppyread     ; loaded cluster. now, find next in FAT:

    pop ax              ; ax = current cluster
    push ax

    mov bx, 3
    mul bx
    shr ax, 1           ; ax = fat offset

    xor dx, dx
    mov bx, 512
    div bx              ; dx = ent_offset
    add ax, [ds:0x0E]   ; ax = fat sector

    mov bx, 0x0800
    mov es, bx
    xor bx, bx
    call floppyread     ; fat table to 0x8000

    xchg di, dx
    mov ax, [es:di]     ; ax = table value
    mov di, dx
    pop bx              ; bx = current cluster

    and bx, 1
    jnz .readClusterA
    and ax, 0x0FFF
    jmp .readClusterB
.readClusterA:
    shr ax, 4
.readClusterB:
    cmp ax, 0x0FF8
    jge .enableA20Line

    add di, 0x0200
    jmp .readCluster

.enableA20Line:
    in al, 0x92
    or al, 2
    out 0x92, al
.prepareGDT:
    lgdt [ds:gdtr]
.enterProtectedMode:
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp 0x08:stage2Entry + 0x7C00
.fail:
    mov ax, 0x07C0
    mov es, ax
    mov ax, 0x1301
    mov bx, 0x0007
    mov cx, err_end - err
    mov dx, 0x0100
    mov bp, err
    int 0x10
    jmp $

; ax = LBA
; es:bx = store addr
floppyread:
    pusha

    xor dx, dx
    div word [ds:0x18]
    and dl, 0x3F
    inc dl
    mov cl, dl          ; cx = sector
    xor dx, dx
    div word [ds:0x1A]  ; ax = cylinder/track
                        ; dx = head

    mov ch, al
    mov dh, dl
    mov dl, 0x00        ; 0x00 = 1st floppy drive
    mov ax, 0x0201      ; read one sector

    int 0x13
    jc .fail
    cmp ax, 0x0001  ; confirm one sector read
    jne .fail
    popa
    ret
.fail:
    jmp $


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bootsig:
    times 446-($-$$) db 0
    db 0x55
    db 0xAA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

getmeminfo:
    pusha
.checkLowMemory:
    clc
    int 0x12
    jc .checkHighMemory
    mov [mem_low], ax
.checkHighMemory:
    xor cx, cx
    clc
    mov ax, 0xE801
    int 0x15
    jc .end
    cmp ah, 0x86
    je .end
    cmp ah, 0x80
    je .end
    jcxz .checkHighMemoryAx
    mov ax, cx
    mov bx, dx
.checkHighMemoryAx:
    mov [mem_high1], ax
    mov [mem_high2], bx
.end:
    popa
    ret

msg:        db "Loading stage2...",0x0D,0x0A
msg_end:
err:        db "Error!"
err_end:

mem_low:    dw 0
mem_high1:  dw 0
mem_high2:  dw 0

kernel:     db "KERNEL  SYS"

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
    nop
    nop
    nop
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


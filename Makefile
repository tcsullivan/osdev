all:
	@nasm stage1.s -o stage1.bin
	@gcc -m32 -masm=intel \
	    -ggdb -O0 \
        -I. -DPRINTF_DISABLE_SUPPORT_FLOAT -DPRINTF_DISABLE_SUPPORT_LONG_LONG \
	    -ffreestanding -nostdlib -fno-pie -no-pie \
	    -ffunction-sections -fdata-sections -Wl,-gc-sections \
	    -Wl,-Tstage2.ld \
	    stage2.c printf.c idt.c videoram.c tinyalloc.c
	@objcopy -O binary -j .text stage2.elf stage2.bin
	@dd if=stage1.bin of=disk.img conv=notrunc bs=1 seek=62

disk:
	@dd if=/dev/zero of=disk.img bs=1k count=1440
	@mkfs.fat -F 12 -R 2 disk.img
	@dd if=stage1.bin of=disk.img conv=notrunc bs=1 seek=62
	@sudo mount -oloop disk.img /mnt
	@sudo cp stage2.bin /mnt/KERNEL.SYS
	@sync
	@sudo umount /mnt


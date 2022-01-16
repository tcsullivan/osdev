all:
	gcc -m32 -maddress-mode=short \
	    -ffreestanding -fno-pie -nostdlib -no-pie \
	    -Wl,--no-dynamic-linker -Wl,-Tlinker_boot.ld -Wl,--build-id=none \
	    boot.c
	objcopy -O binary boot.elf boot.bin
	dd if=/dev/zero of=disk.img bs=1k count=1440
	mkfs.fat -F 12 -R 2 disk.img
	dd if=boot.bin  of=disk.img conv=notrunc bs=1 seek=62


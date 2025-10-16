CFLAGS = -m32 -ffreestanding -nostdlib -fno-pic -fno-pie -fno-stack-protector
LDFLAGS = -m elf_i386

all: kernel.bin

boot.o: boot.asm
	nasm -f elf32 boot.asm -o boot.o

kernel.o: kernel.c
	gcc $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: boot.o kernel.o linker.ld
	ld $(LDFLAGS) -T linker.ld -o kernel.bin boot.o kernel.o

iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Kernel" { multiboot /boot/kernel.bin }' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso iso

clean:
	rm -rf *.o *.bin iso myos.iso

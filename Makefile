
CC = gcc
CFLAGS = -m32 -Wall -nostdlib -fno-builtin 

ASM = nasm
ASMPARAMS = -f elf32

LINKER = ld
LDPARAMS = -m elf_i386 

objects = loader.o gdt.o kernel.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.asm
	$(ASM) $(ASMPARAMS) -o $@ $<

kernel: link.ld $(objects)
	$(LINKER) $(LDPARAMS) -T $< -o $@ $(objects)

clean:
	rm -rf *.o kernel

clean-all:
	$(MAKE) clean 
	$(MAKE) clean -C tests

run:
	qemu-system-i386 -kernel kernel

test:
	$(MAKE) -C tests

test-run:
	$(MAKE) run-all -C tests
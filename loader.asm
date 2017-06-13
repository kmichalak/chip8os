;;kernel.asm

%define MAGIC 		0x1BADB002
%define FLAGS 		0x00
%define CHECKSUM 	-(MAGIC + FLAGS)
%define STACKSIZE	8192

bits 32						;nasm directive - generate code to run 
							;on a processor operating in 32 bits
section .text
	align 4

multiboot_header:
	dd MAGIC				;dd MAGIC
	dd FLAGS
	dd CHECKSUM				;dd CHECKSUM

global start				;set the 'start' symbol as a global
extern kmain

start:
	cli						;block interrupts
	mov esp, stack_space + STACKSIZE ;set stack pointer
	call kmain
	hlt						;halt the CPU

section .bss

stack_space:
	resb STACKSIZE				;8KB for stack
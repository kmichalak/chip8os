;;kernel.asm

%define MAGIC 0x1BADB002
%define FLAGS 0x00
%define CHECKSUM -(MAGIC + 0x00)

bits 32						;nasm directive - generate code to run 
							;on a processor operating in 32 bits
section .text
	align 4
	dd 0x1BADB002			;dd MAGIC
	dd 0x00
	dd -(0x1BADB002 + 0x00)	;dd CHECKSUM

global start				;set the 'start' symbol as a global
extern kmain

start:
	cli						;block interrupts
	mov esp, stack_space	;set stack pointer
	call kmain
	hlt						;halt the CPU

section .bss
resb 8192					;8KM for stack
stack_space:
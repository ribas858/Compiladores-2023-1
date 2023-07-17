section .data

	a dd 2

section .bss

	b resd 1

section .text
	global _start

_start:
	mov dword [b], 2
		
mov eax, 1
mov ebx, 0
int 80h

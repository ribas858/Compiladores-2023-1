section .data

	id dd 1

section .bss

	var resd 1

section .text
	global _start

_start:
	mov eax, [id]
mov [var], eax
	cmp eax, [id]
		
mov eax, 1
mov ebx, 0
int 80h
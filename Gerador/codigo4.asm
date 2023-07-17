%include "io.mac"

section .data

	b dd 45

section .bss

	a resd 1

section .text
	global _start:

_start:
	mov eax, 2
	mov ebx, 5
	imul ebx
	
	mov ebx, 10
	add ebx, eax
	
	sub edx, edx
	push eax
	mov eax, 20
	mov ecx, 2
	idiv ecx
	mov ecx, eax
	pop eax
	
	mov [a], eax

	PutLInt [a]
	nwln
	PutLInt [b]
	nwln

mov eax, 1
mov ebx, 0
int 80h

%include "io.mac"

section .data

	b dd 1

section .bss

	a resd 1
c resd 1

section .text
	global _start

_start:
	mov eax, 2
	add eax, 3
	
	mov ebx, eax
	add ebx, [b]
	
	mov [a], ebx
mov eax, [a]
	sub eax, 1
	
	push eax
mov eax, [a]
	mov ebx, [a]
	imul ebx
mov ebx, eax
	pop eax

	mov [c], ebx
push eax
		while0:
		mov eax, [a]
		cmp eax, 10
		jge fim0
		mov ebx, [a]
	add ebx, 1
	
	mov [a], ebx
jmp while0
fim0:
pop eax
		
mov eax, 1
mov ebx, 0
int 80h

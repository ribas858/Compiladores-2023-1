section .data

	a dd 2
b dd 2

section .bss

	c resd 1
g resd 1

section .text
	global _start

_start:
	push eax
		mov eax, 2
		cmp eax, 2
		jnge if0
		mov ebx, 1
	add ebx, 1
	
	mov [c], ebx
if0:
pop eax
		push eax
		while1:
		mov eax, [a]
		cmp eax, 2
		jl fim1
		mov ebx, 2
	add ebx, 3
	
	mov [g], ebx
mov ebx, [a]
mov [g], ebx
		jmp while1
fim1:
pop eax
		
mov eax, 1
mov ebx, 0
int 80h

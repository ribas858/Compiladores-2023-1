%include "io.mac"

section .data

	fat dd 1
n dd 8

section .text
	global _start

_start:
	push eax
		while0:
		mov eax, [n]
		cmp eax, 0
		jle fim0
		push eax
mov eax, [fat]
	mov ebx, [n]
	imul ebx
mov ebx, eax
	pop eax

	mov [fat], ebx
mov ebx, [n]
	sub ebx, 1
	
	mov [n], ebx
jmp while0
fim0:
pop eax

PutLInt [fat]
nwln
		
mov eax, 1
mov ebx, 0
int 80h

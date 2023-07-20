%include "io.mac"

section .data

	a dd 100
c dd 5

section .bss

	e resd 1
var resd 1
e2 resd 1
e1 resd 1
b resd 1

section .text
	global _start

_start:
	mov eax, 1
	add eax, 1
	
	mov [e], eax
mov eax, 1
	add eax, 1
	
	mov [var], eax
mov eax, 1
	add eax, 1
	
	mov [e2], eax
mov eax, 1
	add eax, 1
	
	mov [e1], eax
mov eax, [a]
	mov ebx, [c]
	imul ebx
mov ebx, eax
	
	mov [b], ebx
sub edx, edx
	mov eax, [a]
	mov ebx, [c]
	idiv ebx
mov ebx, eax
	
	mov [c], ebx

mov eax, 1
mov ebx, 0
int 80h

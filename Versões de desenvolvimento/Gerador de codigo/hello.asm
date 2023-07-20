%include "io.mac"

section .data

    msg db 'Hello Word!' ,0dH,0ah
    
    num dd 50

section .bss

    buffer_int resb 10
    a resd 1

section .text
    global _start

_start:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg
    mov edx, 13
    int 80h


    
    



    mov eax, 1
    mov ebx, 0
    int 80h
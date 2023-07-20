section .data

	myNwln db 0dH, 0ah, 0
	size_ln EQU $-myNwln

fat dd 1
n dd 8

section .bss

	scan resb 255

	numero_buffer resb 10

	
section .text

str_to_int:
                enter 8,0
                mov dword [ebp-4], 10
                mov dword [ebp-8], -1
                mov esi, [ebp+8]
                mov ecx, [ebp+12]
                sub ecx, 1
                mov ebx, 0
                mov edi, 0
                mov byte bl, [esi]
                cmp byte bl, 0x2d
                jne go
                        sub ebx, ebx
                        inc ebx
                        push 1
                        jmp fim_go
                go:
                        push 0
                        sub ebx, ebx
        fim_go:
                cvsi:
                        mov al, [esi+ebx]
                        sub al, 0x30
                        cbw
                        cwde
                        inc ebx
                        add eax, edi
                        mov edi, eax
                        cmp ebx, ecx
                        je fim_si
                        mul dword [ebp-4]
                        mov edi, eax
                        jmp cvsi
        fim_si:
                pop eax
                cmp eax, 0
                je n_mul
                        mov eax, edi
                        imul dword [ebp-8]
                        jmp fim_mul
        n_mul:
                mov eax, edi
        fim_mul:
                leave
                ret 8


int_to_str:
                    enter 4,0
                    sub ecx, ecx
                    sub ebx, ebx
                    mov dword [ebp-4], 10
                    mov eax, [ebp+8]
                    mov edi, [ebp+12]
                    cmp eax, 0
                    jg continue
                            mov eax, -1
                            imul dword [ebp+8]
                            mov byte [edi], 0x2d
                            inc ebx
            continue:
                    cvic:
                            sub edx, edx
                            div dword [ebp-4]
                            cmp eax, 0
                            je fim
                            add edx, 0x30
                            push edx
                            inc ecx
                            jmp cvic
            fim:
                    add edx, 0x30
                    push edx
                    inc ecx
                    invert:
                            pop edx
                            mov [edi+ebx], edx
                            inc ebx
                    loop invert
                    mov byte [edi+ebx], 0
                    mov eax, ebx
                    leave
                    ret 8


meu_print_int:
                    enter 4, 0
                    push dword [ebp+8]
                    push dword [ebp+12]
                    call int_to_str
                    mov dword [ebp-4], eax
                    mov eax, 4
                    mov ebx, 1
                    mov ecx, [ebp+8]
                    mov edx, [ebp-4]
                    int 80h
                    leave
                    ret 8


barra_n:
                    enter 0, 0
                    mov eax, 4
                    mov ebx, 1
                    mov ecx, myNwln
                    mov edx, size_ln
                    int 80h
                    leave
                    ret

global _start

_start:
	
	pusha
    mov eax, 3
    mov ebx, 0
    mov ecx, scan
    mov edx, 255
    int 80h
    mov byte [scan+eax-1], 0
    push eax
    push scan
    call str_to_int
	mov [n], eax
	popa

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
		
pusha
push dword[fat]
push numero_buffer
call meu_print_int
call barra_n
popa

mov eax, 1
mov ebx, 0
int 80h

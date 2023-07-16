%include "io.mac"

section .data

    msg db 'Hello Word!' ,0dH,0ah
    
    num dd 50

section .bss

    buffer_int resb 10

section .text
    global _start


int_to_str: ; (1: argumento)-> [BUFFER STRING DE RETORNO(+12)]; (2: argumento)-> [NÚMERO(+8)];
            ; Retorna em EAX o tamanho da string;

        ; Variável Locais-> [Constante '10' (ebp-4)];

        enter 4,0               ; Salva o EBP original na pilha, e move o ESP(topo da pilha) para o EBP(base da pilha);
                                ; Criando assim uma pilha local;
                                ; Reserva 4 bytes para uma variável INT local

        sub ecx, ecx            ; Contador de dígitos ECX == 0
        sub ebx, ebx            ; Contador EBX == 0

        mov dword [ebp-4], 10   ; [ebp-4] = 10; Inicializa variável local com 10
        mov eax, [ebp+8]        ; EAX = [ebp+8]; Parametro 'NÚMERO' carregado em EAX
        mov edi, [ebp+12]       ; EDI = [ebp+12]; Parametro 'BUFFER' carregado em EDI

        cmp eax, 0              ; Compara EAX com 0
        jg continue             ; if EAX > 0 then -> Pula para 'continue'; else -> executa comandos a baixo

                mov eax, -1             ; EAX = -1
                imul dword [ebp+8]      ; Multiplica Parâmetro [ebp+8] (Número) por EAX(-1)
                                        ; Resultado é retornado em EAX
                mov byte [edi], 0x2d    ; Adiciona o caracter(byte) '-' (símbolo negativo) á primeira posição do BUFFER (EDI)
                inc ebx                 ; EBX++; Incrementa 1 posição no índice do buffer
        continue:
                cvic: ; Converte dígito por dígito e armazena na pilha

                        sub edx, edx            ; EDX = 0; Zera EDX para evitar erros no resto da divisão
                        div dword [ebp-4]       ; Divide EAX por 10; Resultado -> EAX; Resto -> EDX
                        cmp eax, 0              ; Compara EAX com 0
                        je fim                  ; if EAX == 0 then -> pula para o 'fim'; else -> continua

                        add edx, 0x30           ; Converte o dígito INT para CHAR
                        push edx                ; Salva na pilha o Dígito agora em CHAR
                        inc ecx                 ; ECX++ ; Contador de dígitos ECX
                        jmp cvic                ; Pula para 'cvic'

        fim:    
                add edx, 0x30           ; Converte o ÚLTIMO dígito INT para CHAR
                push edx                ; Salva na pilha o ÚLTIMO Dígito no formato CHAR
                inc ecx                 ; i++ ; Contador de dígitos ECX

                invert: ; Retira da pilha e armazena no BUFFER

                        pop edx                 ; Desempilha dígito
                        mov [edi+ebx], edx      ; Armazena dígito no BUFFER
                        
                        inc ebx                 ; EBX++ ; Incrementa 1 posição no índice do buffer
                loop invert                     ; Pula para 'invert' se ECX != 0

                mov byte [edi+ebx], 0   ; Byte nulo no fim da string

                

                mov eax, ebx            ; Retorna em EAX o tamanho da string

                leave                   ; Deleta as variaveis locais; mov ESP, EBP; pop EBP;
                ret 8                   ; Retorna ao programa principal, e desempilha os argumentos (2 x 4 bytes)
; ---------------------------------------------------------------------------------------------------------------------------------------------------------------

_start:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg
    mov edx, 13
    int 80h


    mov eax, 3
    sub eax, 1

    PutLInt eax
    nwln

    mov ebx, 1
    sub ebx, eax
    PutLInt ebx
    nwln

    mov ecx, 10
    sub ecx, eax
    PutLInt ecx
    nwln

    sub edx, edx
    push eax
    mov eax, 10
    mov esi, 2
    idiv esi
    mov esi, eax
    pop eax

    PutLInt esi
    nwln
    PutLInt eax
    nwln



    



    mov eax, 1
    mov ebx, 0
    int 80h
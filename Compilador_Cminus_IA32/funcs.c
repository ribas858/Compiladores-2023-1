#include "funcs.h"

bool vazia(char *str) {
    if(strlen(str) == 0) {
        return true;
    }
    return false;
}

bool igual(char *str1, char *str2) {
    if( strlen(str1) != strlen(str2) ) {
        return false;
    }
    if(strcmp(str1, str2)  == 0) {
        return true;
    }
    return false;
}

struct tbs * retorna_simbolo(struct tbs **lista, char *simbolo) {
    if(*lista) {
        struct tbs *aux = malloc(sizeof(struct tbs));
        aux = *lista;
        while (aux) {
            if (igual(aux->simbolo, simbolo)) {
                return aux;
            }
            aux = aux-> prox;
        }
    }
    return NULL;
}

void atrib_var(struct tbs **lista, char *sb1, char *sb2) {
    int pass = 0;
    tbs *simb1 = retorna_simbolo(lista, sb1);
    tbs *simb2 = retorna_simbolo(lista, sb2);

    if(!simb1) {
        print_erros(sb1, 2);
    } else {
        pass++;
    }
    if(!simb2) {
        print_erros(sb2, 2);
    } else {
        pass++;
    }

    if(pass == 2) {
        if(igual(simb1->tipo, simb2->tipo)) {
            simb1->valor = simb2->valor;
        } else {
            // ERRO de tipo
        }
    }
}

bool insere_simbolo(struct tbs **lista, char *simbolo, generico *expr, int opr_expr) {

    if(*lista) {
        struct tbs *aux = malloc(sizeof(struct tbs));
        aux = *lista;
        while (aux) {
            if (igual(aux->simbolo, simbolo)) {
                if( vazia(expr->tipo_regra) && expr->dado) {

                    if(igual(expr->dado_tipo, aux->tipo)) {
                        
                        return true;
                    }
                }
                else if(!vazia(expr->tipo_regra)) {

                    print_erros(aux->simbolo, 1);
                    return false;
                } else {

                    return true;
                }
            }
            aux = aux-> prox;
        }
    }
    
    if(vazia(expr->tipo_regra)) {
        print_erros(simbolo, 2);
        return false;
    }

    struct tbs *novoSimbolo = malloc(sizeof(tbs));
    if(novoSimbolo) {
        novoSimbolo->simbolo = simbolo;

        if(expr->dado) {
            novoSimbolo->valor = *(int *) expr->dado;
        }
        novoSimbolo->tipo = expr->tipo_regra;
        novoSimbolo->opr_expr = opr_expr;
        
        novoSimbolo->prox = NULL;

        if (*lista == NULL){
            *lista = novoSimbolo;
        } else {
            struct tbs *aux = malloc(sizeof(tbs));
            aux = *lista;
            while (aux->prox) {
                aux = aux->prox;
            }
            aux->prox = novoSimbolo;
        }

    } else {
        printf("Erro ao inserir simbolo\n");
        return false;
    }
    return true;
}

void printList(struct tbs *lista) {
    if(!lista) {
        printf("Tabela vazia...\n");
        return;
    }
    printf("\n");
    while(lista) {
        printf("Simbolo: %s\n", lista->simbolo);
        printf("Tipo: %s\n", lista->tipo);
        printf("Valor: %d\n", lista->valor);
        printf("Operandos Expr: %d\n\n", lista->opr_expr);
        lista = lista->prox;
    }
}

void negrito(int status) {
    static const char *s[] = {"\x1b[0m", "\x1b[1m"};
    printf("%s", s[!!status]);
}

void italico(int status) {
    static const char *s[] = {"\x1b[0m", "\x1b[3m"};
    printf("%s", s[!!status]);
}

void cor_terminal(int colorCode, int status) {
    static const char *s[] = {"\x1b[0m", "\x1b[3%dm"};
    printf(s[!!status], colorCode);
}

void print_erros(char *simbolo, int erro) {
    erro_count++;
    if(erro == 1) {
        cor_terminal(6, 1);
        printf("\n(linha:%d) : ", linha_count+1 ); cor_terminal(2, 0);

        negrito(1); cor_terminal(1, 1);
        printf("Erro semantico >>>>>>>>>>> "); cor_terminal(1, 0); negrito(0);

        cor_terminal(2, 1);
        printf(" Dupla declaracao "); cor_terminal(2, 0);

        negrito(1); cor_terminal(5, 1);
        printf(" '%s'\n", simbolo); cor_terminal(5, 0); negrito(0);
    }
    else if(erro == 2) {
        cor_terminal(6, 1);
        printf("\n(linha:%d) : ", linha_count+1 ); cor_terminal(2, 0);

        negrito(1); cor_terminal(1, 1);
        printf("Erro semantico >>>>>>>>>>> "); cor_terminal(1, 0); negrito(0);
        
        cor_terminal(2, 1);
        printf(" Variavel"); cor_terminal(2, 0);

        negrito(1); cor_terminal(5, 1);
        printf(" '%s' ", simbolo); cor_terminal(5, 0); negrito(0);

        cor_terminal(2, 1);
        printf("não declarada!!\n\n"); cor_terminal(2, 0);
    }
    else if(erro == 3) {
        int j = 0;
        char aux[strlen(simbolo)];
        printf("\n");
        for (int i=0; i<strlen(simbolo); i++) {
            if (simbolo[i] != ' ') {
                aux[j] = simbolo[i];
                j++;
            } else {
                if (strcmp(aux, "syntax") == 0) {
                    aux[j] = ' ';
                    j++;
                    
                } else {
                    
                    if(aux[j] != ',' && strlen(aux) == 14) {
                        aux[j] = '\0';
                    }
                    
                    if (strcmp(aux, "syntax error,") == 0 || strcmp(aux, "syntax error,V") == 0) {
                        cor_terminal(6, 1);
                        printf("\n(linha:%d) : ", linha_count+1 ); cor_terminal(2, 0);
                        cor_terminal(1, 1); negrito(1); printf("Erro de Sintatico :: "); negrito(0); cor_terminal(1, 0);
                    }
                    else if (strcmp(aux, "unexpected") == 0) {
                        cor_terminal(2, 1); printf("Token inesperado: "); cor_terminal(2, 0);
                    }
                    else if (strcmp(aux, "expecting") == 0) {
                        cor_terminal(2, 1); printf(" :: Era esperado: "); cor_terminal(2, 0);
                    }
                    else if (strcmp(aux, "or") == 0) {
                        cor_terminal(2, 1); printf(", ou "); cor_terminal(2, 0);
                    } else {
                        if(aux[j-1] == ',') {
                            aux[j-1] = '\0';
                        }
                        cor_terminal(4, 1); negrito(1); printf("%s", aux); cor_terminal(2, 0); negrito(0);
                    }    
                    memset(aux, 0, sizeof(aux));
                    j = 0;
                }   
            }
        }
        cor_terminal(4, 1); negrito(1); printf("%s\n", aux); cor_terminal(2, 0); negrito(0);
        printf("\n");
    }
}

char* retorna_nome(char* caminho, int cod) {
    char* name = malloc(sizeof(char) * strlen(caminho));

    if (cod == 1) {
        for (int i=0; i< strlen(caminho); i++) {
            if (caminho[i] == '.') {
                name[i] = '\0';
                break;
            }
            name[i] = caminho[i];
        }
        
    } else {
        int j = 0;
        for (int i=strlen(caminho)-1; i>-1; i--) {
            if (caminho[i] == '/') {
                name[j] = '\0';
                break;
            }
            name[j] = caminho[i];
            j++;
        }
        int tam = j;
        int i;
        char* name_ret = malloc(sizeof(char) * strlen(name));
        for( i=0; i<tam; i++) {
            name_ret[i] = name[j-1];
            j--;
        }
        free(name);
        name = name_ret;
    }
    return name;
}

void insere_reg(struct registrador **lista, char *reg) {
    struct registrador *novoReg = malloc(sizeof(registrador));
    
    if(novoReg) {
        novoReg->reg = reg;
        novoReg->ocup = 0;
        novoReg->valor = 0;
        novoReg->prox = NULL;

        if (*lista == NULL) {
            *lista = novoReg;
        } else {
            struct registrador *aux = malloc(sizeof(registrador));
            aux = *lista;
            while (aux->prox) {
                aux = aux->prox;
            }
            aux->prox = novoReg;
        }
    } else {
        printf("Erro ao inserir registrador\n");
    }
}

struct registrador * retorna_reg(struct registrador **lista, char *reg, int b, int modo) {
    if(modo == 1) {
        if(*lista) {
            struct registrador *aux = malloc(sizeof(struct registrador));
            aux = *lista;
            while (aux) {
                if (igual(aux->reg, reg)) {                                     // Retorna o registrador buscado em 'reg', ocupado ou livre
                    return aux;
                }
                aux = aux-> prox;
            }
        }
        return NULL;
    }
    else if (modo == 2) {                                                       
        if(*lista) {
            struct registrador *aux = malloc(sizeof(struct registrador));
            aux = *lista;
            while (aux) {
                if(aux->ocup == 1 && aux->valor == b ) {                        // Retorna um registrador ocupado, para leitura apenas
                    return aux;
                }
                else if(aux->ocup == 0) {                                       // Retorna qualquer registrador livre
                    return aux;
                }
                aux = aux-> prox;
            }
        }
        return NULL;
    }
    else if (modo == 3) {                                                       
        if(*lista) {
            struct registrador *aux = malloc(sizeof(struct registrador));
            aux = *lista;
            while (aux) {
                if(aux->ocup == 0) {                                            // Retorna qualquer registrador livre
                    return aux;
                }
                aux = aux-> prox;
            }
        }
        return NULL;
    }
    return NULL;
}

struct registrador* retorna_var_regs(struct registrador **lista, char *var, int valor, int modo) {
    if(modo == 1) {
        if(*lista) {
            struct registrador *aux = malloc(sizeof(struct registrador));
            aux = *lista;
            while (aux) {
                if(igual(aux->var, var)) {
                    return aux;
                }
                aux = aux-> prox;
            }
        }
    } else if (modo == 2) {
        if(*lista) {
            struct registrador *aux = malloc(sizeof(struct registrador));
            aux = *lista;
            while (aux) {
                if(igual(aux->var, var) && aux->valor == valor) {
                    return aux;
                }
                aux = aux-> prox;
            }
        }
    }
    return NULL;
}

void printRegs(struct registrador *lista, int modo) {
    if(modo == 1) {
        printf("\n");
        while(lista) {
            printf("Reg: %s\n", lista->reg);
            printf("Valor: %d\n", lista->valor);
            printf("Ocupado: %d\n\n", lista->ocup);
            lista = lista->prox;
        }
    }
    else if (modo == 2) {
        printf("Regs: [ ");
        while(lista) {
            if(lista->ocup == 1) {
                if(!lista->prox) {
                    cor_terminal(1, 1); printf("%s(%s)%d ]\n\n", lista->reg, lista->var, lista->valor); cor_terminal(1, 0);    
                } else {
                    cor_terminal(1, 1); printf("%s(%s)%d, ", lista->reg, lista->var, lista->valor); cor_terminal(1, 0);
                }
            } else {
                if(!lista->prox) {
                    cor_terminal(2, 1); printf("%s(%s)%d ]\n\n", lista->reg, lista->var, lista->valor); cor_terminal(2, 0);
                } else {
                    cor_terminal(2, 1); printf("%s(%s)%d, ", lista->reg, lista->var, lista->valor); cor_terminal(2, 0);
                }
            }
            lista = lista->prox;
        }
    }
}

char* expr_nasm(char op, struct expressao *a, struct expressao *b) {
    int b_is_registrador = 0;
    int original_ocupado;
    int a_is_numero = 0;
    int b_is_numero = 0;
    registrador *reg_aux = NULL;
    registrador *reg_a = NULL;
    registrador *reg_b = NULL;

    if(vazia(a->id)) {
        a_is_numero = 1;
    }

    if(vazia(b->id)) {
        b_is_numero = 1;
        reg_aux = retorna_reg(&lista_regs, "", b->numero, 2);

        if(reg_aux) {
            if(reg_aux->ocup == 0) {
                original_ocupado = reg_aux->ocup;
                reg_aux->ocup = 1;
            } else {
                b_is_registrador = 1;
                original_ocupado = 1;
            }
        } else {
            printf("REGISTRADOR NAO ENCONTRADO\n");
        }
    } else {
        reg_aux = retorna_reg(&lista_regs, "", 0, 3);        
    }
    
    if(reg_aux) {
        if(op == '+' || op == '-') {

            if(b_is_registrador == 1) {                                         // Se o valor de B esta em um registrador
                
                reg_a = retorna_reg(&lista_regs, "", 0, 3);                     // Retorna qualquer registrador livre
                reg_b = reg_aux;

                if(original_ocupado == 0) {
                    reg_b->ocup = 0;
                }
                
                if(reg_a) {
                    if(a_is_numero == 1) {
                        registrador *reg_var = retorna_var_regs(&lista_regs, id_expr, a->numero, 2);
                        if(reg_var) {
                            fprintf(temp, "%s", "mov ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_var->reg); 
                        } else {
                            fprintf(temp, "%s", "mov ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%d\n\t", a->numero);
                        }

                        if(op == '+') {
                            fprintf(temp, "%s", "add ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_b->reg);
                        } else {
                            fprintf(temp, "%s", "sub ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_b->reg);
                        }
                    } else {

                        fprintf(temp, "%s", "mov ");
                        fprintf(temp, "%s, ", reg_a->reg);
                        fprintf(temp, "[%s]\n\t", a->id);

                        if(op == '+') { 
                            fprintf(temp, "%s", "add ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_b->reg);
                        } else {
                            fprintf(temp, "%s", "sub ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_b->reg);
                        }
                    }
                
                    if(op == '+') { 
                        reg_a->valor = a->numero + b->numero;
                    } else {
                        reg_a->valor = a->numero - b->numero;
                    }
                    reg_a->ocup = 1;
                    strcpy(reg_a->var, id_expr);
                    
                    // printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg_a->reg;
                    
                } else {
                    // Todos os registradores ocupados
                }
            } else {
                reg_a = reg_aux;

                if(a_is_numero == 1) {
                    registrador *reg_var = retorna_var_regs(&lista_regs, id_expr, a->numero, 2);
                    
                    if(reg_var) {
                        fprintf(temp, "%s", "mov ");
                        fprintf(temp, "%s, ", reg_a->reg);
                        fprintf(temp, "%s\n\t", reg_var->reg);  
                    } else {
                        fprintf(temp, "%s", "mov ");
                        fprintf(temp, "%s, ", reg_a->reg);
                        fprintf(temp, "%d\n\t", a->numero);
                    }
                } else {
                    fprintf(temp, "%s", "mov ");
                    fprintf(temp, "%s, ", reg_a->reg);
                    fprintf(temp, "[%s]\n\t", a->id);
                }

                if(b_is_numero == 1) {
                    registrador *reg_var = retorna_var_regs(&lista_regs, id_expr, b->numero, 2);

                    if(op == '+') { 
                        if(reg_var) {
                            fprintf(temp, "%s", "add ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_var->reg);
                        } else {
                            fprintf(temp, "%s", "add ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%d\n\t", b->numero);
                        }
                    } else {
                        if(reg_var) {
                            fprintf(temp, "%s", "sub ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_var->reg);
                        } else {
                            fprintf(temp, "%s", "sub ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%d\n\t", b->numero);
                        }
                    }
                } else {
                    if(op == '+') { 
                        fprintf(temp, "%s", "add ");
                        fprintf(temp, "%s, ", reg_a->reg);
                        fprintf(temp, "[%s]\n\t", b->id);
                    } else {
                        fprintf(temp, "%s", "sub ");
                        fprintf(temp, "%s, ", reg_a->reg);
                        fprintf(temp, "[%s]\n\t", b->id);
                    }
                }

                if(op == '+') {
                    reg_a->valor = a->numero + b->numero;
                } else {
                    reg_a->valor = a->numero - b->numero;
                }
                reg_a->ocup = 1;
                strcpy(reg_a->var, id_expr);
                // printRegs(lista_regs, 2);
                fprintf(temp, "\n\t");
                return reg_a->reg;
            }
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        else if(op == '*' || op == '/') {
            registrador *reg_edx = NULL;
            int ocup_edx = 0;

            if(op == '/') {
                
                if(igual(reg_aux->reg, "edx")) {
                    if(reg_aux->ocup == 1 ){
                        ocup_edx = 1;
                        fprintf(temp, "%s", "push edx\n\t");
                    } else {
                        reg_aux->ocup = 1;
                    }
                    fprintf(temp, "%s", "sub edx, edx\n\t");
                    
                    reg_edx = reg_edx;
                    reg_aux = retorna_reg(&lista_regs, "", b->numero, 2);
                
                } else {
                    reg_edx = retorna_reg(&lista_regs, "edx", 0, 1);
                    if(reg_edx->ocup == 1 ){ 
                        ocup_edx = 1;
                        fprintf(temp, "%s", "push edx\n\t");
                    } else {
                        reg_edx->ocup = 1;
                    }

                    fprintf(temp, "%s", "sub edx, edx\n\t");         
                }

            }

            if(!igual(reg_aux->reg, "eax")) {
                reg_b = reg_aux;

                reg_a = retorna_reg(&lista_regs, "eax", 0, 1);
                
                if(reg_a->ocup == 1) {
                    fprintf(temp, "%s", "push ");
                    fprintf(temp, "%s\n", reg_a->reg);
                }
            } else {
                reg_a = reg_aux;
                reg_a->ocup = 1;
                reg_b = retorna_reg(&lista_regs, "", 0, 3);
                reg_a->ocup = 0;

                if(original_ocupado == 0) {
                    reg_a->ocup = 0;
                }
            }

            if(b_is_registrador == 1 && reg_a->ocup == 0) {

                
                if (reg_a && reg_b) {

                    if(a_is_numero == 1) {
                        registrador *reg_var = retorna_var_regs(&lista_regs, id_expr, a->numero, 2);
                        if(reg_var) {
                            fprintf(temp, "%s", "mov ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_var->reg);
                        } else {
                            fprintf(temp, "%s", "mov ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%d\n\t", a->numero);
                        }
                    } else {
                        fprintf(temp, "%s", "mov ");
                        fprintf(temp, "%s, ", reg_a->reg);
                        fprintf(temp, "[%s]\n\t", a->id);
                    }

                    if(op == '*') {
                        fprintf(temp, "%s", "imul ");
                        fprintf(temp, "%s\n", reg_b->reg);
                        reg_a->valor = a->numero * b->numero;
                    } else {
                        fprintf(temp, "%s", "idiv ");
                        fprintf(temp, "%s\n", reg_b->reg);
                        reg_a->valor = a->numero / b->numero;
                    }

                    reg_a->ocup = 1;
                    strcpy(reg_a->var, id_expr);
                    // printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg_a->reg;
                }

            } else {
                
                if (reg_a && reg_b) {

                    if(a_is_numero == 1) {
                        registrador *reg_var = retorna_var_regs(&lista_regs, id_expr, a->numero, 2);
                        if(reg_var) {
                            fprintf(temp, "%s", "mov ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%s\n\t", reg_var->reg);   
                        } else {
                            fprintf(temp, "%s", "mov ");
                            fprintf(temp, "%s, ", reg_a->reg);
                            fprintf(temp, "%d\n\t", a->numero);
                        }
                    } else {
                        fprintf(temp, "%s", "mov ");
                        fprintf(temp, "%s, ", reg_a->reg);
                        fprintf(temp, "[%s]\n\t", a->id);
                    }

                    if(b_is_numero == 1) {
                        registrador *reg_var = retorna_var_regs(&lista_regs, id_expr, b->numero, 2);
                        if (reg_var) {
                            fprintf(temp, "%s", "mov ");
                            fprintf(temp, "%s, ", reg_b->reg);
                            fprintf(temp, "%s\n\t", reg_var->reg);
                        } else {
                            fprintf(temp, "%s", "mov ");
                            fprintf(temp, "%s, ", reg_b->reg);
                            fprintf(temp, "%d\n\t", b->numero);
                        }

                        if(op == '*') {
                            fprintf(temp, "%s", "imul ");
                            fprintf(temp, "%s\n", reg_b->reg);
                        } else {
                            fprintf(temp, "%s", "idiv ");
                            fprintf(temp, "%s\n", reg_b->reg);
                        }
                        
                        fprintf(temp, "%s", "mov ");
                        fprintf(temp, "%s, ", reg_b->reg);
                        fprintf(temp, "%s\n\t", reg_a->reg);
                    } else {

                        fprintf(temp, "%s", "mov ");
                        fprintf(temp, "%s, ", reg_b->reg);
                        fprintf(temp, "[%s]\n\t", b->id);

                        if(op == '*') {
                            fprintf(temp, "%s", "imul ");
                            fprintf(temp, "%s\n", reg_b->reg);
                        } else {
                            fprintf(temp, "%s", "idiv ");
                            fprintf(temp, "%s\n", reg_b->reg);
                        }
                        
                        fprintf(temp, "%s", "mov ");
                        fprintf(temp, "%s, ", reg_b->reg);
                        fprintf(temp, "%s\n\t", reg_a->reg);
                    }

                    if(reg_a->ocup == 1) {
                        fprintf(temp, "%s", "pop ");
                        fprintf(temp, "%s\n", reg_a->reg);
                    }

                    if(op == '*') {
                        reg_b->valor = a->numero * b->numero;
                    } else {
                        if(ocup_edx == 0) {
                            reg_edx->ocup = 0;
                        } else {
                            fprintf(temp, "%s", "pop edx\n\t");
                        }
                        reg_b->valor = a->numero / b->numero;
                    }

                    reg_b->ocup = 1;
                    strcpy(reg_b->var, id_expr);
                    // printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg_b->reg;
                }
            }
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    } else {
        // Todos os registradores ocupados
    }

    return "";
}

void vars_nasm(struct tbs *lista) {


    int bss = 0;
    struct tbs *aux = lista;
    fprintf(temp, "%s\n\n\t", "section .data");
    if (print_count > 0) {
        fprintf(temp, "%s\n", "myNwln db 0dH, 0ah, 0\n\tsize_ln EQU $-myNwln\n");
    }
    while(lista) {
        if(lista->opr_expr == 1) {
            fprintf(temp, "%s ", lista->simbolo);
            if( igual(lista->tipo, "int") ) {
                fprintf(temp, "%s ", "dd");
            }
            fprintf(temp, "%d\n", lista->valor);
        } else {
            bss = 1;
        }
        lista = lista->prox;
    }

    if(bss > 0 || scan_count > 0 || print_count > 0) {
        fprintf(temp, "%s", "\n");
        fprintf(temp, "%s\n\n\t", "section .bss");
        if (scan_count > 0) {
            fprintf(temp, "%s\n\n\t", "scan resb 255");
        }
        if (print_count > 0) {
            fprintf(temp, "%s\n\n\t", "numero_buffer resb 10");
        }
        while(aux) {
            if(aux->opr_expr != 1) {
                fprintf(temp, "%s ", aux->simbolo);
                if( igual(aux->tipo, "int") ) {
                    fprintf(temp, "%s\n", "resd 1");
                }
            }

            aux = aux->prox;
        }
    }
    fprintf(temp, "%s", "\n");

    if (scan_count > 0) {
        fprintf(temp, "%s\n\n", "section .text");

        fprintf(temp, "%s\n", "str_to_int:\n\
                enter 8,0\n\
                mov dword [ebp-4], 10\n\
                mov dword [ebp-8], -1\n\
                mov esi, [ebp+8]\n\
                mov ecx, [ebp+12]\n\
                sub ecx, 1\n\
                mov ebx, 0\n\
                mov edi, 0\n\
                mov byte bl, [esi]\n\
                cmp byte bl, 0x2d\n\
                jne go\n\
                        sub ebx, ebx\n\
                        inc ebx\n\
                        push 1\n\
                        jmp fim_go\n\
                go:\n\
                        push 0\n\
                        sub ebx, ebx\n\
        fim_go:\n\
                cvsi:\n\
                        mov al, [esi+ebx]\n\
                        sub al, 0x30\n\
                        cbw\n\
                        cwde\n\
                        inc ebx\n\
                        add eax, edi\n\
                        mov edi, eax\n\
                        cmp ebx, ecx\n\
                        je fim_si\n\
                        mul dword [ebp-4]\n\
                        mov edi, eax\n\
                        jmp cvsi\n\
        fim_si:\n\
                pop eax\n\
                cmp eax, 0\n\
                je n_mul\n\
                        mov eax, edi\n\
                        imul dword [ebp-8]\n\
                        jmp fim_mul\n\
        n_mul:\n\
                mov eax, edi\n\
        fim_mul:\n\
                leave\n\
                ret 8\n\n");
        if(print_count > 0) {
            fprintf(temp, "%s\n", "int_to_str:\n\
                    enter 4,0\n\
                    sub ecx, ecx\n\
                    sub ebx, ebx\n\
                    mov dword [ebp-4], 10\n\
                    mov eax, [ebp+8]\n\
                    mov edi, [ebp+12]\n\
                    cmp eax, 0\n\
                    jg continue\n\
                            mov eax, -1\n\
                            imul dword [ebp+8]\n\
                            mov byte [edi], 0x2d\n\
                            inc ebx\n\
            continue:\n\
                    cvic:\n\
                            sub edx, edx\n\
                            div dword [ebp-4]\n\
                            cmp eax, 0\n\
                            je fim\n\
                            add edx, 0x30\n\
                            push edx\n\
                            inc ecx\n\
                            jmp cvic\n\
            fim:\n\
                    add edx, 0x30\n\
                    push edx\n\
                    inc ecx\n\
                    invert:\n\
                            pop edx\n\
                            mov [edi+ebx], edx\n\
                            inc ebx\n\
                    loop invert\n\
                    mov byte [edi+ebx], 0\n\
                    mov eax, ebx\n\
                    leave\n\
                    ret 8\n\n");
            fprintf(temp, "%s\n", "meu_print_int:\n\
                    enter 4, 0\n\
                    push dword [ebp+8]\n\
                    push dword [ebp+12]\n\
                    call int_to_str\n\
                    mov dword [ebp-4], eax\n\
                    mov eax, 4\n\
                    mov ebx, 1\n\
                    mov ecx, [ebp+8]\n\
                    mov edx, [ebp-4]\n\
                    int 80h\n\
                    leave\n\
                    ret 8\n\n");
            fprintf(temp, "%s\n", "barra_n:\n\
                    enter 0, 0\n\
                    mov eax, 4\n\
                    mov ebx, 1\n\
                    mov ecx, myNwln\n\
                    mov edx, size_ln\n\
                    int 80h\n\
                    leave\n\
                    ret\n");
        }
        fprintf(temp, "%s\n\n", "global _start");
    } else {
        fprintf(temp, "%s\n\n", "section .text\n\tglobal _start");
    }

    fprintf(temp, "%s\n\t", "_start:");
}

struct labels* while_if_nasm(char* operador,  struct expressao *a_expr, struct expressao *b_expr, char *cond) {
    
    int a = a_expr->numero;
    int b = b_expr->numero;
    char *as = a_expr->id;
    char *bs = b_expr->id;

    struct labels *ret_labs = malloc(sizeof(labels));
    char if_str[3] = { "if" };
    char while_str[6] = { "while" };
    char loop_str[4] = { "fim" };

    struct registrador *reg = retorna_reg(&lista_regs, "", 0, 2);

    fprintf(temp, "%s", "push ");
    fprintf(temp, "%s\n\t\t", reg->reg);
    ret_labs->origin_ocup = reg->ocup;

    if(ret_labs->origin_ocup == 0) {
        reg->ocup = 1;
    }
    strcpy(ret_labs->reg_usado, reg->reg);
    
    char buffer0[10];
    if( igual(cond, "while")) {
        sprintf(buffer0, "%d", condicionais_count);
        
        strcat(while_str, buffer0);
        strcat(while_str, ":");
        fprintf(temp, "%s\n\t\t", while_str);
        strcpy(ret_labs->lab1, while_str);
    }

    if(igual(operador, "==")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(as)) {
            fprintf(temp, "%d\n\t\t", a);
        } else {
            fprintf(temp, "[%s]\n\t\t", as);
        }
  
        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(bs)) {
            fprintf(temp, "%d\n\t\t", b);
        } else {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", condicionais_count);

        fprintf(temp, "%s", "jne ");
        
        if (igual(cond, "while") ) {
            strcat(loop_str, buffer);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, "!=")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(as)) {
            fprintf(temp, "%d\n\t\t", a);
        } else {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(bs)) {
            fprintf(temp, "%d\n\t\t", b);
        } else {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", condicionais_count);

        fprintf(temp, "%s", "je ");
        
        if (igual(cond, "while") ) {
            strcat(loop_str, buffer);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, ">=")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(as)) {
            fprintf(temp, "%d\n\t\t", a);
        } else {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(bs)) {
            fprintf(temp, "%d\n\t\t", b);
        } else {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", condicionais_count);

        fprintf(temp, "%s", "jnge ");
        
        if (igual(cond, "while") ) {
            strcat(loop_str, buffer);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, "<=")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(as)) {
            fprintf(temp, "%d\n\t\t", a);
        } else {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(bs)) {
            fprintf(temp, "%d\n\t\t", b);
        } else {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", condicionais_count);

        fprintf(temp, "%s", "jg ");
        
        if (igual(cond, "while") ) {
            strcat(loop_str, buffer);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, ">")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(as)) {
            fprintf(temp, "%d\n\t\t", a);
        } else {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(bs)) {
            fprintf(temp, "%d\n\t\t", b);
        } else {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", condicionais_count);

        fprintf(temp, "%s", "jle ");
        
        if (igual(cond, "while") ) {
            strcat(loop_str, buffer);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, "<")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(as)) {
            fprintf(temp, "%d\n\t\t", a);
        } else {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(vazia(bs)) {
            fprintf(temp, "%d\n\t\t", b);
        } else {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", condicionais_count);

        fprintf(temp, "%s", "jge ");
        
        if (igual(cond, "while") ) {
            strcat(loop_str, buffer);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
 
    condicionais_count++;
    return ret_labs;
    
}

void while_if_nasm_2(char *cond) {
    if (igual(cond, "if")) {
        strcat(rot2, ":");
        fprintf(temp, "%s\n", rot2);
        
        struct registrador *reg_ret = retorna_reg(&lista_regs, ret_reg_usado, 0, 1);
        if(origin_oc != reg_ret->ocup) {
            reg_ret->ocup = origin_oc;
        }
        fprintf(temp, "%s", "pop ");
        fprintf(temp, "%s\n\t\t", reg_ret->reg);
    }
    else if (igual(cond, "while")) {
        fprintf(temp, "%s", "jmp ");
        rot1[strlen(rot1)-1] = '\0';
        fprintf(temp, "%s\n", rot1);

        strcat(rot2, ":");
        fprintf(temp, "%s\n", rot2);

        struct registrador *reg_ret = retorna_reg(&lista_regs, ret_reg_usado, 0, 1);
        if(origin_oc != reg_ret->ocup) {
            reg_ret->ocup = origin_oc;
        }
        fprintf(temp, "%s", "pop ");
        fprintf(temp, "%s\n\t\t", reg_ret->reg);

    }
}

void libera_regs(struct registrador *lista, char *var) {
    while(lista) {
        if(igual(lista->var, var)) {
            lista->ocup = 0;
            strcpy(lista->var, "");
        }
        lista = lista->prox;
    }
}
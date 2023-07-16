#include "funcs.h"

bool vazia(char *str) {
    if(strlen(str) == 0) {
        return true;
    }
    return false;
}

bool igual(char *str1, char *str2) {
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
            printf("Atribuicao com variavel: %s :: %s\n", simb1->tipo, simb2->tipo);
            simb1->valor = simb2->valor;
        } else {
            // ERRO de tipo
        }
    }
}

bool insere_simbolo(struct tbs **lista, char *simbolo, generico *expr) {

    // int a = 2+5- -3;
    // printf("AAAA: %d\n", a);

    // printf("simbolo: ");
    // if(!vazia(simbolo)) {
    //     printf("%s | ", simbolo);
    // } else {
    //     printf(" NULL | ");
    // }

    // printf("expr_dado: ");
    // if(expr->dado) {
    //     printf("%d | ", *( int*) expr->dado);
    // } else {
    //     printf(" NULL | ");
    // }
    // printf("expr_tipo_dado: ");
    // if(!vazia(expr->dado_tipo)) {
    //     printf("%s | ", expr->dado_tipo);
    // } else {
    //     printf(" NULL | ");
    // }
    // printf("expr_tipo_regra: ");
    // if(!vazia(expr->tipo_regra)) {
    //     printf("%s\n\n", expr->tipo_regra);
    // } else {
    //     printf("NULL\n\n");
    // }

    if(*lista) {
        struct tbs *aux = malloc(sizeof(struct tbs));
        aux = *lista;
        while (aux) {
            if (igual(aux->simbolo, simbolo)) {
                if( vazia(expr->tipo_regra) && expr->dado) {
                    // printf("AUX TIPO: %s\n", aux->tipo);
                    if(igual(expr->dado_tipo, aux->tipo)) {
                        // printf("Atualiza valor....\n");
                        aux->valor = *(int *) expr->dado;
                        return true;
                    }
                }
                else if(!vazia(expr->tipo_regra)) {
                    //printf("Erro semantico!!!! Declaracao dupla\n");
                    print_erros(aux->simbolo, 1);
                    return false;
                } else {
                    // Apenas Ocorrencia
                    return true;
                }
            }
            aux = aux-> prox;
        }
    }
    //printf("Nao achou na tabela....\n");

    
    if(vazia(expr->tipo_regra)) {
        //printf("Erro semantico!!!! Variavel não declarada\n");
        print_erros(simbolo, 2);
        return false;
    }


    struct tbs *novoSimbolo = malloc(sizeof(tbs));
    // printf("Passou maloc\n");
    if(novoSimbolo) {
        novoSimbolo->simbolo = simbolo;
        //printf("Passou maloc 1\n");

        if(expr->dado) {
            novoSimbolo->valor = *(int *) expr->dado;
        }
        //printf("Passou maloc 2\n");
        novoSimbolo->tipo = expr->tipo_regra;
        
        
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
    printf("\n");
    while(lista) {
        printf("Simbolo: %s\n", lista->simbolo);
        printf("Tipo: %s\n", lista->tipo);
        printf("Valor: %d\n\n", lista->valor);
        
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
                    
                    //cor_terminal(6, 1); printf("%s\n", aux); cor_terminal(6, 0);
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
                if (igual(aux->reg, reg)) {
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
                if(aux->ocup == 1 && aux->valor == b ) {
                    return aux;
                }
                else if(aux->ocup == 0) {
                    return aux;
                }
                aux = aux-> prox;
            }
        }
        return NULL;
    }
    else if (modo == 3) { // Caso imul
        if(*lista) {
            struct registrador *aux = malloc(sizeof(struct registrador));
            aux = *lista;
            while (aux) {
                if(aux->ocup == 0) {
                    return aux;
                }
                aux = aux-> prox;
            }
        }
        return NULL;
    }
}

void printRegs(struct registrador *lista) {
    printf("\n");
    while(lista) {
        printf("Reg: %s\n", lista->reg);
        printf("Valor: %d\n", lista->valor);
        printf("Ocupado: %d\n\n", lista->ocup);
        lista = lista->prox;
    }
}

void expr_nasm(char op, int a, int b) {
    
    registrador *reg = retorna_reg(&lista_regs, "", b, 2);
    
    if(reg) {
        if(op == '+') {
            printf("MAIS: Retorno: %s ocup:%d\n", reg->reg, reg->ocup);
            if(reg->valor == b) {
                printf("Caso reaproveita registrador..(mais)\n");
                registrador *reg_a = retorna_reg(&lista_regs, "", 0, 3);
                if(reg_a) {
                    fprintf(temp, "%s", "mov ");
                    fprintf(temp, "%s, ", reg_a->reg);
                    fprintf(temp, "%d\n", a);

                    fprintf(temp, "%s", "add ");
                    fprintf(temp, "%s, ", reg_a->reg);
                    fprintf(temp, "%s\n", reg->reg);
                    reg_a->valor = a + b;
                    reg_a->ocup = 1;
                }
            } else {
                printf("Caso normal...(mais)\n");
                fprintf(temp, "%s", "mov ");                               // 
                fprintf(temp, "%s, ", reg->reg);                         // Movemos para esse registrador o valor A
                fprintf(temp, "%d\n", a);
                fprintf(temp, "%s", "add ");
                fprintf(temp, "%s, ", reg->reg);
                fprintf(temp, "%d\n", b);
                reg->valor = a + b;
                reg->ocup = 1;
            }
            fprintf(temp, "\n");
        }
        else if (op == '-') {
            printf("MENOS: Retorno: %s ocup:%d valor:%d b:%d\n", reg->reg, reg->ocup, reg->valor, b);
            if(reg->valor == b) {
                printf("Caso reaproveita registrador..(menos)\n");
                registrador *reg_a = retorna_reg(&lista_regs, "", 0, 3);
                if(reg_a) {
                    fprintf(temp, "%s", "mov ");
                    fprintf(temp, "%s, ", reg_a->reg);
                    fprintf(temp, "%d\n", a);

                    fprintf(temp, "%s", "sub ");
                    fprintf(temp, "%s, ", reg_a->reg);
                    fprintf(temp, "%s\n", reg->reg);
                    reg_a->valor = a - b;
                    reg_a->ocup = 1;
                }
            } else {
                printf("Caso normal...(menos)\n");
                fprintf(temp, "%s", "mov ");                               // 
                fprintf(temp, "%s, ", reg->reg);                         // Movemos para esse registrador o valor A
                fprintf(temp, "%d\n", a);
                fprintf(temp, "%s", "sub ");
                fprintf(temp, "%s, ", reg->reg);
                fprintf(temp, "%d\n", b);
                reg->valor = a - b;
                reg->ocup = 1;
            }
            fprintf(temp, "\n");
        }

        else if (op == '*') {
            printf("MULT: Retorno: %s ocup:%d\n", reg->reg, reg->ocup);
            int original_ocup = reg->ocup;

            registrador *reg_eax;

            if(!igual(reg->reg, "eax")) {
                reg_eax = retorna_reg(&lista_regs, "eax", 0, 1);
            } else {
                reg_eax = reg;
                if(original_ocup == 0) {
                    reg_eax->ocup = 1;
                }

                reg = retorna_reg(&lista_regs, "", 0, 3);

                if (original_ocup == 0) {
                    reg_eax->ocup = 0;
                }
            }
            
            
            if(reg && reg_eax) {
                
                if(reg_eax->ocup == 1) {
                        printf("Caso EAX ocupado...(mult)\n");
                        fprintf(temp, "%s", "push eax\n");
                } else {
                    printf("Caso EAX normal...(mult)\n");
                }

                fprintf(temp, "%s", "mov ");
                fprintf(temp, "%s, ", reg_eax->reg);
                fprintf(temp, "%d\n", a);

                fprintf(temp, "%s", "mov ");
                fprintf(temp, "%s, ", reg->reg);
                fprintf(temp, "%d\n", b);
                
                fprintf(temp, "%s", "imul ");
                fprintf(temp, "%s\n", reg->reg);

                if(reg_eax->ocup == 1) {
                    fprintf(temp, "%s", "mov ");
                    fprintf(temp, "%s, ", reg->reg);
                    fprintf(temp, "%s\n", reg_eax->reg);

                    fprintf(temp, "%s", "pop eax\n");

                    reg->valor = a * b;
                    reg->ocup = 1;

                } else {
                    reg_eax->valor = a * b;
                    reg_eax->ocup = 1;
                }
            }
            fprintf(temp, "\n");
        }
        else if (op == '/') {
            // eax = eax * b
            printf("DIV: Retorno: %s ocup:%d\n", reg->reg, reg->ocup);

            registrador *reg_edx = NULL;
            int ocup_edx = 0;
            if(igual(reg->reg, "edx")) {
                if(reg->ocup == 1 ){
                    ocup_edx = 1;
                    fprintf(temp, "%s", "push edx\n");
                } else {
                    reg->ocup = 1;
                }
                fprintf(temp, "%s", "sub edx, edx\n");
                
                reg_edx = reg;
                reg = retorna_reg(&lista_regs, "", b, 2);

            } else {
                reg_edx = retorna_reg(&lista_regs, "edx", 0, 1);
                if(reg_edx->ocup == 1 ){
                    ocup_edx = 1;
                    fprintf(temp, "%s", "push edx\n");
                } else {
                    reg_edx->ocup = 1;
                }
                fprintf(temp, "%s", "sub edx, edx\n");
            }

            
            int original_ocup = reg->ocup;

            registrador *reg_eax;

            if(!igual(reg->reg, "eax")) {
                reg_eax = retorna_reg(&lista_regs, "eax", 0, 1);
            } else {
                reg_eax = reg;
                if(original_ocup == 0) {
                    reg_eax->ocup = 1;
                }

                reg = retorna_reg(&lista_regs, "", 0, 3);

                if (original_ocup == 0) {
                    reg_eax->ocup = 0;
                }
            }
            
            
            if(reg && reg_eax) {
                
                if(reg_eax->ocup == 1) {
                        printf("Caso EAX ocupado...(DIV)\n");
                        fprintf(temp, "%s", "push eax\n");
                } else {
                    printf("Caso EAX normal...(DIV)\n");
                }

                fprintf(temp, "%s", "mov ");
                fprintf(temp, "%s, ", reg_eax->reg);
                fprintf(temp, "%d\n", a);

                fprintf(temp, "%s", "mov ");
                fprintf(temp, "%s, ", reg->reg);
                fprintf(temp, "%d\n", b);
                
                fprintf(temp, "%s", "idiv ");
                fprintf(temp, "%s\n", reg->reg);

                if(reg_eax->ocup == 1) {
                    fprintf(temp, "%s", "mov ");
                    fprintf(temp, "%s, ", reg->reg);
                    fprintf(temp, "%s\n", reg_eax->reg);

                    fprintf(temp, "%s", "pop eax\n");

                    reg->valor = a / b;
                    reg->ocup = 1;

                } else {
                    reg_eax->valor = a / b;
                    reg_eax->ocup = 1;
                }
            }

            if(reg_edx) {
                if(ocup_edx == 0) {
                    reg_edx->ocup = 0;
                } else {
                    fprintf(temp, "%s", "pop edx\n");
                }
            }
        }
    } else {
        
    }
    
}
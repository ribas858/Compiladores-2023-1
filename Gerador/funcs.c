#include "funcs.h"

bool vazia(char *str) {
    if(strlen(str) == 0) {
        return true;
    }
    return false;
}

bool igual(char *str1, char *str2) {
    printf("str %s\n", str1);
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
            printf("Atribuicao com variavel: %s :: %s\n", simb1->tipo, simb2->tipo);
            simb1->valor = simb2->valor;
        } else {
            // ERRO de tipo
        }
    }
}

bool insere_simbolo(struct tbs **lista, char *simbolo, generico *expr, int opr_expr) {

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

struct registrador* retorna_valor_regs(struct registrador **lista, int valor) {
    if(*lista) {
        struct registrador *aux = malloc(sizeof(struct registrador));
        aux = *lista;
        while (aux) {
            if(aux->valor == valor) {                                            // Retorna o registrador de acordo com o valor
                return aux;
            }
            aux = aux-> prox;
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
                    cor_terminal(1, 1); printf("%s ]\n\n", lista->reg); cor_terminal(1, 0);    
                } else {
                    cor_terminal(1, 1); printf("%s, ", lista->reg); cor_terminal(1, 0);
                }
            } else {
                if(!lista->prox) {
                    cor_terminal(2, 1); printf("%s ]\n\n", lista->reg); cor_terminal(2, 0);
                } else {
                    cor_terminal(2, 1); printf("%s, ", lista->reg); cor_terminal(2, 0);
                }
            }
            lista = lista->prox;
        }
    }
}

char* expr_nasm(char op, int a, int b) {
    printf("CHAMOU EXPR NASM\n");
    registrador *reg = retorna_reg(&lista_regs, "", b, 2);                      // Retorna para 'reg' um registrador livre ou ocupado(se for para leitura)
                                                                                // OBS: Retornar um reg ocupado(para leitura), ja aproveita o valor do registrador
                                                                                // sem precisar procurar um novo disponivel
    
    if(reg) {
        if(op == '+') {
            printf("\nMAIS: Retorno: %s ocup:%d\n", reg->reg, reg->ocup);
            if(reg->valor == b) {                                               // Nesse caso o registrador ja tem um valor igual a um do operandos, ou seja, ele ja 
                                                                                // esta ocupado e pode ser usado para leitua, mas nao podemos altera-lo
                printf("Caso reaproveita registrador..(mais)\n");
                registrador *reg_a = retorna_reg(&lista_regs, "", 0, 3);        // Retorna qualquer registrador livre
                printf("%s + %s\n", reg_a->reg, reg->reg);
                if(reg_a) {
                    fprintf(temp, "%s", "mov ");                                //
                    fprintf(temp, "%s, ", reg_a->reg);                          //
                    fprintf(temp, "%d\n\t", a);                                 // mov reg_a, a

                    fprintf(temp, "%s", "add ");                                //
                    fprintf(temp, "%s, ", reg_a->reg);                          //
                    fprintf(temp, "%s\n\t", reg->reg);                          // add reg_a, reg

                    reg_a->valor = a + b;                                       // Salva valor
                    reg_a->ocup = 1;                                            // Ocupa o registrador
                    printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg_a->reg;
                } else {
                    // Todos os registradores ocupados
                }
            } else {                                                            // Caso onde não temos um registrador pra aproveitar, ocuparemos um novo
                printf("%s + %d\n", reg->reg, b);
                printf("Caso normal...(mais)\n");
                fprintf(temp, "%s", "mov ");                                    // 
                fprintf(temp, "%s, ", reg->reg);                                //
                fprintf(temp, "%d\n\t", a);                                       // mov reg, a

                fprintf(temp, "%s", "add ");                                    //
                fprintf(temp, "%s, ", reg->reg);                                //
                fprintf(temp, "%d\n\t", b);                                       // add reg, b

                reg->valor = a + b;                                             // Salva valor
                reg->ocup = 1;                                                  // Ocupa o registrador
                printRegs(lista_regs, 2);
                fprintf(temp, "\n\t");
                return reg->reg;
            }
            
        }
        else if (op == '-') {
            printf("\nMENOS: Retorno: %s ocup:%d valor:%d b:%d\n", reg->reg, reg->ocup, reg->valor, b);

            if(reg->valor == b) {                                               // Nesse caso o registrador ja tem um valor igual a um do operandos, ou seja, ele ja 
                                                                                // esta ocupado e pode ser usado para leitua, mas nao podemos altera-lo
                printf("Caso reaproveita registrador..(menos)\n");
                registrador *reg_a = retorna_reg(&lista_regs, "", 0, 3);        // Retorna qualquer registrador livre
                printf("%s - %s\n", reg_a->reg, reg->reg);
                if(reg_a) {
                    fprintf(temp, "%s", "mov ");                                //
                    fprintf(temp, "%s, ", reg_a->reg);                          //
                    fprintf(temp, "%d\n", a);                                   // mov reg_a, a

                    fprintf(temp, "%s", "sub ");                                //
                    fprintf(temp, "%s, ", reg_a->reg);                          //
                    fprintf(temp, "%s\n\t", reg->reg);                            // sub reg_a, reg

                    reg_a->valor = a - b;                                       // Salva valor
                    reg_a->ocup = 1;                                            // Ocupa o registrador
                    printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg_a->reg;
                } else {
                    // Todos os registradores ocupados
                }
            } else {                                                            // Caso onde não temos um registrador pra aproveitar, ocuparemos um novo
                printf("%s - %d\n", reg->reg, b);
                printf("Caso normal...(menos)\n");
                fprintf(temp, "%s", "mov ");                                    //
                fprintf(temp, "%s, ", reg->reg);                                //
                fprintf(temp, "%d\n\t", a);                                       // mov reg, a

                fprintf(temp, "%s", "sub ");                                    //
                fprintf(temp, "%s, ", reg->reg);                                //
                fprintf(temp, "%d\n\t", b);                                       // sub reg, b

                reg->valor = a - b;                                             // Salva valor
                reg->ocup = 1;                                                  // Ocupa o registrador
                printRegs(lista_regs, 2);
                fprintf(temp, "\n\t");
                return reg->reg;
            }
            
        }

        else if (op == '*') {
            printf("\nMULT: Retorno: %s ocup:%d\n", reg->reg, reg->ocup);
            
            int original_ocup = reg->ocup;                                      // Salva o status de 'reg'
            registrador *reg_eax;

            if(!igual(reg->reg, "eax")) {                                       // Se o registrador for diferente de EAX
                reg_eax = retorna_reg(&lista_regs, "eax", 0, 1);                // Buscamos EAX para 'reg_eax', estando ocupado ou nao
            } else {                                                            
                                                                                // Se o registrador for o EAX
                reg_eax = reg;                                                  // Atribuimos ele para 'reg_eax'
                if(original_ocup == 0) {                                        // Se originalmente o registrador encontrado estiver livre
                    reg_eax->ocup = 1;                                          // Marcamos ele como ocupado, pois precisamos buscar um segundo
                }                                                               // registrador, e ele nao pode ser selecionado novamente

                reg = retorna_reg(&lista_regs, "", 0, 3);                       // Buscamos um novo registrador qualquer que esteja livre  

                if (original_ocup == 0) {                                       // Se originalmente o 'reg_eax' estava livre
                    reg_eax->ocup = 0;                                          // Marcamos ele novamente como livre, pois ele foi alterado apenas para buscar
                }                                                               // um novo registrador
            }
            
            printf("%s * %s\n", reg_eax->reg, reg->reg);
            if(reg && reg_eax) {                                                // Se os dois registradores sao validos
                
                if(reg_eax->ocup == 1) {                                        // Se 'reg_eax' esta ocupado
                        printf("Caso EAX ocupado...(mult)\n");
                        fprintf(temp, "%s", "push eax\n\t");                      // Salvamos ele na pilha
                } else {
                    printf("Caso EAX normal...(mult)\n");
                }

                fprintf(temp, "%s", "mov ");                                    //
                fprintf(temp, "%s, ", reg_eax->reg);                            //
                fprintf(temp, "%d\n\t", a);                                       // mov reg_eax, a

                fprintf(temp, "%s", "mov ");                                    //
                fprintf(temp, "%s, ", reg->reg);                                //
                fprintf(temp, "%d\n\t", b);                                       // mov reg, b
                
                fprintf(temp, "%s", "imul ");                                   //
                fprintf(temp, "%s\n\t", reg->reg);                                // imul reg (reg_eax = reg_eax * reg)

                if(reg_eax->ocup == 1) {                                        // Se 'reg_eax' esta ocupado, salvamos o resultado da mult no 'reg'
                    fprintf(temp, "%s", "mov ");                                // para devolver o valor original de 'reg_eax'
                    fprintf(temp, "%s, ", reg->reg);                            //
                    fprintf(temp, "%s\n\t", reg_eax->reg);                        // mov reg, reg_eax

                    fprintf(temp, "%s", "pop eax\n\t");                           // Desempilha o valor original de EAX

                    reg->valor = a * b;                                         // Salva valor
                    reg->ocup = 1;                                              // Ocupa o registrador
                    printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg->reg;

                } else {                                                        // Se 'reg_eax' esta livre, apenas o ocupamos
                    reg_eax->valor = a * b;                                     // Salva valor
                    reg_eax->ocup = 1;                                          // Ocupa o registrador
                    printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg_eax->reg;
                }
            } else {
                if(!reg) {
                    // Todos os registradores ocupados
                }
            }
            
        }
        else if (op == '/') {
            // eax = eax * b
            printf("\nDIV: Retorno: %s ocup:%d\n", reg->reg, reg->ocup);

            registrador *reg_edx = NULL;
            int ocup_edx = 0;
            if(igual(reg->reg, "edx")) {                                        // Se o registrador for igual a EDX
                if(reg->ocup == 1 ){                                            // Se edx esta ocupado
                    ocup_edx = 1;                                               // Marcamos ele como ocupado originalmente
                    fprintf(temp, "%s", "push edx\n\t");                          // Salvamos ele na pilha
                } else {
                    reg->ocup = 1;                                              // Se originalmente ele estava livre, marcamos ele como ocupado
                }                                                               // para evitar seleciona-lo na busca por outro registrador
                fprintf(temp, "%s", "sub edx, edx\n\t");                          // Ocupado ou nao, ele foi salvo na pilha. Entao zeramos EDX
                                                                                // para evitar erros na operacao idiv
                reg_edx = reg;                                                  // Se o registrador encontrado for o EDX, atribuimos a 'reg_edx'
                reg = retorna_reg(&lista_regs, "", b, 2);                       // E entao buscamos um novo registrador para 'reg'
            
            } else {
                reg_edx = retorna_reg(&lista_regs, "edx", 0, 1);                // Se o registrador encontrado nao for o EDX, buscamos ele especificamente
                if(reg_edx->ocup == 1 ){                                        // Se edx esta ocupado     
                    ocup_edx = 1;                                               // Marcamos ele como ocupado originalmente
                    fprintf(temp, "%s", "push edx\n\t");                          // Salvamos ele na pilha
                } else {
                    reg_edx->ocup = 1;                                          // Se originalmente ele estava livre, marcamos ele como ocupado
                }                                                               // para evitar seleciona-lo na busca por outro registrador

                fprintf(temp, "%s", "sub edx, edx\n\t");                          // Ocupado ou nao, ele foi salvo na pilha. Entao zeramos EDX
                                                                                // para evitar erros na operacao idiv            
            }

            
            int original_ocup = reg->ocup;                                      // Salva o status de 'reg'

            registrador *reg_eax;

            if(!igual(reg->reg, "eax")) {                                       // Se o registrador for diferente de EAX
                reg_eax = retorna_reg(&lista_regs, "eax", 0, 1);                // Buscamos EAX para 'reg_eax', estando ocupado ou nao
            } else {
                                                                                // Se o registrador for o EAX
                reg_eax = reg;                                                  // Atribuimos ele para 'reg_eax'
                if(original_ocup == 0) {                                        // Se originalmente o registrador encontrado estiver livre
                    reg_eax->ocup = 1;                                          // Marcamos ele como ocupado, pois precisamos buscar um segundo
                }                                                               // registrador, e ele nao pode ser selecionado novamente

                reg = retorna_reg(&lista_regs, "", 0, 3);                       // Buscamos um novo registrador qualquer que esteja livre        

                if (original_ocup == 0) {                                       // Se originalmente o 'reg_eax' estava livre
                    reg_eax->ocup = 0;                                          // Marcamos ele novamente como livre, pois ele foi alterado apenas para buscar
                }                                                               // um novo registrador   
            }
            
            printf("%s / %s\n", reg_eax->reg, reg->reg);
            if(reg && reg_eax) {                                                // Se os dois registradores sao validos
                
                if(reg_eax->ocup == 1) {                                        // Se 'reg_eax' esta ocupado
                        printf("Caso EAX ocupado...(DIV)\n");
                        fprintf(temp, "%s", "push eax\n\t");                      // Salvamos ele na pilha
                } else {
                    printf("Caso EAX normal...(DIV)\n");
                }

                fprintf(temp, "%s", "mov ");                                    //
                fprintf(temp, "%s, ", reg_eax->reg);                            //
                fprintf(temp, "%d\n\t", a);                                       // mov reg_eax, a

                fprintf(temp, "%s", "mov ");                                    //
                fprintf(temp, "%s, ", reg->reg);                                //
                fprintf(temp, "%d\n\t", b);                                       // mov reg, b
                
                fprintf(temp, "%s", "idiv ");                                   //
                fprintf(temp, "%s\n\t", reg->reg);                                // idiv reg (reg_eax = reg_eax / reg)

                if(reg_eax->ocup == 1) {                                        // Se 'reg_eax' esta ocupado, salvamos o resultado da idiv no 'reg'
                    fprintf(temp, "%s", "mov ");                                // para devolver o valor original de 'reg_eax'
                    fprintf(temp, "%s, ", reg->reg);                            //
                    fprintf(temp, "%s\n\t", reg_eax->reg);                        // mov reg, reg_eax

                    fprintf(temp, "%s", "pop eax\n\t");                           // Desempilha o valor original de EAX

                    reg->valor = a / b;                                         // Salva o valor
                    reg->ocup = 1;                                              // Ocupa o registrador
                    printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg->reg;

                } else {                                                        // Se 'reg_eax' esta livre, apenas o ocupamos
                    reg_eax->valor = a / b;                                     // Salva o valor 
                    reg_eax->ocup = 1;                                          // Ocupa o registrador
                    printRegs(lista_regs, 2);
                    fprintf(temp, "\n\t");
                    return reg_eax->reg;
                }
            } else {
                if(!reg) {
                    // Todos os registradores ocupados
                }
            }

            if(ocup_edx == 0) {                                             // Se 'reg_edx' originalmente estava livre
                reg_edx->ocup = 0;                                          // Marcamos ele como livre
            } else {                                                        // Se ele estava ocupado
                fprintf(temp, "%s", "pop edx\n\t");                           // Devolvemos o seu valor original
            }

            
        }
        else if (op == ' ') {

        }
    } else {
        // Todos os registradores ocupados
    }
    return "";
}

void vars_nasm(struct tbs *lista) {
    int bss = 0;
    struct tbs *aux = lista;

    fprintf(temp, "%s\n\n\t", "section .data");
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

    if(bss > 0) {
        fprintf(temp, "%s", "\n");
        fprintf(temp, "%s\n\n\t", "section .bss");
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

    fprintf(temp, "%s\n\n", "section .text\n\tglobal _start");

    fprintf(temp, "%s\n\t", "_start:");
}


struct labels* while_if_nasm(char* operador, int a, int b, char* as, char* bs, int modo, int enquanto) {
    
    struct labels *ret_labs = malloc(sizeof(labels));
    char if_str[3] = { "if" };
    char while_str[6] = { "while" };
    char loop_str[4] = { "fim" };

    printf("%s %d %d\n", operador, a, b);

    struct registrador *reg = retorna_reg(&lista_regs, "", 0, 2);

    fprintf(temp, "%s", "push ");
    fprintf(temp, "%s\n\t\t", reg->reg);
    ret_labs->origin_ocup = reg->ocup;

    if(ret_labs->origin_ocup == 0) {
        reg->ocup = 1;
        printf("======================= ocupou\n");
    }
    strcpy(ret_labs->reg_usado, reg->reg);
    
    char buffer0[10];
    if( enquanto == 1) {
        sprintf(buffer0, "%d", ifs);
        
        strcat(while_str, buffer0);
        strcat(while_str, ":");
        printf("Passou... %s\n", while_str);
        fprintf(temp, "%s\n\t\t", while_str);
        strcpy(ret_labs->lab1, while_str);
        /////
    }

    if(igual(operador, "==")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 2) {
            fprintf(temp, "[%s]\n\t\t", as);
        }
        else if (modo == 3) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", as);
        }
        

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 2) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 3) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", ifs);

        fprintf(temp, "%s", "jne ");
        
        if (enquanto == 1 ) {
            strcat(loop_str, buffer);
            printf("%s\n", loop_str);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            printf("%s\n", if_str);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, "!=")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 2) {
            fprintf(temp, "[%s]\n\t\t", as);
        }
        else if (modo == 3) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 2) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 3) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", ifs);

        fprintf(temp, "%s", "je ");
        
        if (enquanto == 1 ) {
            strcat(loop_str, buffer);
            printf("%s\n", loop_str);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            printf("%s\n", if_str);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, ">=")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 2) {
            fprintf(temp, "[%s]\n\t\t", as);
        }
        else if (modo == 3) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 2) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 3) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", ifs);

        fprintf(temp, "%s", "jnge ");
        
        if (enquanto == 1 ) {
            strcat(loop_str, buffer);
            printf("%s\n", loop_str);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            printf("%s\n", if_str);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, "<=")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 2) {
            fprintf(temp, "[%s]\n\t\t", as);
        }
        else if (modo == 3) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 2) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 3) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", ifs);

        fprintf(temp, "%s", "jge ");
        
        if (enquanto == 1 ) {
            strcat(loop_str, buffer);
            printf("%s\n", loop_str);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            printf("%s\n", if_str);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, ">")) {
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 2) {
            fprintf(temp, "[%s]\n\t\t", as);
        }
        else if (modo == 3) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 2) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 3) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", ifs);

        fprintf(temp, "%s", "jl ");
        
        if (enquanto == 1 ) {
            strcat(loop_str, buffer);
            printf("%s\n", loop_str);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            printf("%s\n", if_str);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }
    }
    else if(igual(operador, "<")) {
        printf("Passou...MENOR\n");
        
        fprintf(temp, "%s", "mov ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 2) {
            fprintf(temp, "[%s]\n\t\t", as);
        }
        else if (modo == 3) {
            fprintf(temp, "%d\n\t\t", a);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", as);
        }

        fprintf(temp, "%s", "cmp ");
        fprintf(temp, "%s, ", reg->reg);
        if(modo == 1) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 2) {
            fprintf(temp, "%d\n\t\t", b);
        }
        else if (modo == 3) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        else if (modo == 4) {
            fprintf(temp, "[%s]\n\t\t", bs);
        }
        
        char buffer[10];
        sprintf(buffer, "%d", ifs);

        fprintf(temp, "%s", "jg ");
        
        if (enquanto == 1 ) {
            strcat(loop_str, buffer);
            printf("%s\n", loop_str);
            
            fprintf(temp, "%s\n\t\t", loop_str);
            strcpy(ret_labs->lab2, loop_str);
        } else {
            strcat(if_str, buffer);
            printf("%s\n", if_str);
            
            fprintf(temp, "%s\n\t\t", if_str);
            strcpy(ret_labs->lab2, if_str);
        }

        printf("l1 %s l2 %s\n", ret_labs->lab1, ret_labs->lab2);
    }
 
    ifs++;
    return ret_labs;
    
}

void if_part1(int tres, char* tres_str, char* quatro_str, int cinco, char* cinco_str, int modo, int enquanto) {
    if(modo == 1) {
        rotulo = while_if_nasm(quatro_str, tres, cinco, "", "", modo, enquanto);
        strcpy(rot1, rotulo->lab1);
        strcpy(rot2, rotulo->lab2);
        strcpy(ret_reg_usado, rotulo->reg_usado);
        origin_oc = rotulo->origin_ocup;
    }
    else if(modo == 2) {
        rotulo = while_if_nasm(quatro_str, 0, cinco, tres_str, "", modo, enquanto);
        strcpy(rot1, rotulo->lab1);
        strcpy(rot2, rotulo->lab2);
        strcpy(ret_reg_usado, rotulo->reg_usado);
        origin_oc = rotulo->origin_ocup;
    }
    else if(modo == 3) {
        rotulo = while_if_nasm(quatro_str, tres, 0, "", cinco_str, modo, enquanto);
        strcpy(rot1, rotulo->lab1);
        strcpy(rot2, rotulo->lab2);
        strcpy(ret_reg_usado, rotulo->reg_usado);
        origin_oc = rotulo->origin_ocup;
    }
    else if(modo == 4) {
        rotulo = while_if_nasm(quatro_str, 0, 0, tres_str, cinco_str, modo, enquanto);
        strcpy(rot1, rotulo->lab1);
        strcpy(rot2, rotulo->lab2);
        strcpy(ret_reg_usado, rotulo->reg_usado);
        origin_oc = rotulo->origin_ocup;
    }

}

void if_part2(int enquanto) {
    if (enquanto == 0) {
        strcat(rot2, ":");
        fprintf(temp, "%s\n", rot2);
        
        struct registrador *reg_ret = retorna_reg(&lista_regs, ret_reg_usado, 0, 1);
        if(origin_oc != reg_ret->ocup) {
            reg_ret->ocup = origin_oc;
        }
        fprintf(temp, "%s", "pop ");
        fprintf(temp, "%s\n\t\t", reg_ret->reg);
    }
    else if (enquanto == 1) {
        printf("l1r %s l2r %s\n", rot1, rot2);
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
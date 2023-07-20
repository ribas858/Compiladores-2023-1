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

void insere_simbolo(struct tbs **lista, char *simbolo, generico *expr) {

    printf("simbolo: ");
    if(!vazia(simbolo)) {
        printf("%s | ", simbolo);
    } else {
        printf(" NULL | ");
    }

    printf("expr_dado: ");
    if(expr->dado) {
        printf("%d | ", *( int*) expr->dado);
    } else {
        printf(" NULL | ");
    }
    printf("expr_tipo_dado: ");
    if(!vazia(expr->dado_tipo)) {
        printf("%s | ", expr->dado_tipo);
    } else {
        printf(" NULL | ");
    }
    printf("expr_tipo_regra: ");
    if(!vazia(expr->tipo_regra)) {
        printf("%s\n\n", expr->tipo_regra);
    } else {
        printf("NULL\n\n");
    }

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
                        return;
                    }
                }
                else if(!vazia(expr->tipo_regra)) {
                    //printf("Erro semantico!!!! Declaracao dupla\n");
                    print_erros(aux->simbolo, 1);
                    return;
                } else {
                    // Apenas Ocorrencia
                    return;
                }
            }
            aux = aux-> prox;
        }
    }
    //printf("Nao achou na tabela....\n");

    
    if(vazia(expr->tipo_regra)) {
        //printf("Erro semantico!!!! Variavel não declarada\n");
        print_erros(simbolo, 2);
        return;
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
    }
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
}

void yyerror(char const* s) {
    int j = 0;
    char aux[strlen(s)];
    printf("\n");
    for (int i=0; i<strlen(s); i++) {
        if (s[i] != ' ') {
            aux[j] = s[i];
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
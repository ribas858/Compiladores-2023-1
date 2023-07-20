#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

extern int linha_count;

typedef struct tbs {
    char *simbolo;
    char *tipo;
    int valor;

    struct tbs *prox;
} tbs;

typedef struct generico {
    void *dado;
    char *dado_tipo;
    char *tipo_regra;
} generico;

int yylex(void);
void yyerror(char const* s);
void insere_simbolo(struct tbs **lista, char *simbolo, generico *expr);
void printList(struct tbs *lista);
void negrito(int status);
void italico(int status);
void cor_terminal(int colorCode, int status);
void print_erros(char *simbolo, int erro);
bool vazia(char *str);
bool igual(char *str1, char *str2);
struct tbs * retorna_simbolo(struct tbs **lista, char *simbolo);
void atrib_var(struct tbs **lista, char *simb1, char *simb2);

#endif
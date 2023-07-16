#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

extern int linha_count;
int erro_count;
FILE* temp;

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

typedef struct registrador {
    char *reg;
    int ocup;
    int valor;
    struct registrador *prox;
} registrador;

registrador *lista_regs;

int yylex(void);
void yyerror(char const* s);
bool insere_simbolo(struct tbs **lista, char *simbolo, generico *expr);
void printList(struct tbs *lista);
void negrito(int status);
void italico(int status);
void cor_terminal(int colorCode, int status);
void print_erros(char *simbolo, int erro);
bool vazia(char *str);
bool igual(char *str1, char *str2);
struct tbs * retorna_simbolo(struct tbs **lista, char *simbolo);
void atrib_var(struct tbs **lista, char *simb1, char *simb2);
char* retorna_nome(char* caminho, int cod);
void expr_nasm(char op, int a, int b);

void insere_reg(struct registrador **lista, char *reg);
struct registrador * retorna_reg(struct registrador **lista, char *reg, int b, int modo);
void printRegs(struct registrador *lista);


#endif
#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

extern int linha_count;
extern int num_count;
int erro_count;

extern int scan_count;
extern int print_count;

FILE* temp;

char *id_expr;

int condicionais_count;

char rot1[30];
char rot2[30];
char ret_reg_usado[4];
int origin_oc;

typedef struct tbs {
    char *simbolo;
    char *tipo;
    int valor;
    int opr_expr;
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
    int usado;
    char var[100];
    struct registrador *prox;
} registrador;

typedef struct labels {
    char lab1[100];
    char lab2[100];
    char reg_usado[4];
    int origin_ocup;
}labels;
struct labels *rotulo;

typedef struct expressao {
    char *id;
    int numero;
} expressao;


registrador *lista_regs;

int yylex(void);
void yyerror(char const* s);
bool insere_simbolo(struct tbs **lista, char *simbolo, generico *expr, int opr_expr);
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
char* expr_nasm(char op, struct expressao *a, struct expressao *b);

void insere_reg(struct registrador **lista, char *reg);
struct registrador * retorna_reg(struct registrador **lista, char *reg, int b, int modo);
struct registrador* retorna_var_regs(struct registrador **lista, char *var, int valor, int modo);
void printRegs(struct registrador *lista, int modo);
void vars_nasm(struct tbs *lista);
struct labels* while_if_nasm(char* operador,  struct expressao *a, struct expressao *b, char *cond);

void if_part1(int tres, char* tres_str, char* quatro_str, int cinco, char* cinco_str, int modo, int enquanto);
void while_if_nasm_2(char *cond);
void libera_regs(struct registrador *lista, char *var);


#endif
%{
    /* definições */
    #include <stdio.h>
    int yylex(void);
    void yyerror(char const* s);
%}

%union {
    int numero;
    char simbolos;
}

%token<numero> NUMERO

%token PTV
%token P1
%token P2
%token INT
%token ID

%token OP_MAT
%token OP_COMP
%token ATRIB
%token OUTROS

%token PRINT
%token IF


%%

linha: | print PTV | if PTV | var PTV

expr: expr OP_MAT NUMERO
    | P1 NUMERO P2
    | NUMERO
    ;

print: PRINT P1 expr P2
    ;

if: IF P1 expr OP_COMP expr P2
    ;

var: INT ID
    | INT atrib
    ;

atrib: ID ATRIB expr
    ;




%%

extern FILE* yyin;

int main(int argc, char *argv[]) {

    yyin = fopen(argv[1], "r");

    if (!yyin) {
        printf("Arquivo invalido ou inexistente.\n");
        return 1;
    }

    yyparse();

    fclose(yyin);

    return 0;
}

void yyerror(char const* s) {
    printf("\n================>>>>>>ERRO: %s\n", s);
}
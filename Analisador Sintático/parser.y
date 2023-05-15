%{
    /* definições */
    #include <stdio.h>
    int yylex(void);
    void yyerror(char const* s);
%}

%union {
    int numero;
    char op_mat;
}

%token EOL
%token<op_mat> MAIS
%token<numero> NUMERO
%type<numero> int_exp


%%

input: | line input;

line: int_exp EOL { printf("%d\n", $1); } | EOL;

int_exp: NUMERO { $$ = $1; } | int_exp MAIS int_exp { $$ = $1 + $3; };




%%


int main() {
    yyparse();

    return 0;
}

void yyerror(char const* s) {
    printf("ERRO: %s\n", s);
}
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

%token EOL
%token<simbolos> MAIS
%token<numero> NUMERO

%token<simbolos> PTV
%token<simbolos> P1


%%

stmt: | linha stmt

linha: expr PTV | EOL;

expr: 
    NUMERO
    | expr MAIS expr
    | EOL
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
    printf("ERRO: %s\n", s);
}
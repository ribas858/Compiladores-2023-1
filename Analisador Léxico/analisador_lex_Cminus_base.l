%{
    /* definições */

%}

%%

-*[0-9][0-9]* {
        if (yytext[0] == '-') {
            printf("\nNúmero Negativo\n\n");
        } else {
            printf("\nNumero Positivo\n\n");
        }
    }
    
[A-Z_a-z][A-Z_a-z0-9]* {printf("\nIdentificador (Variável)\n\n");}



.+ {printf("erro\n");}

\n {return 0;}

%%

int yywrap() {}


int main() {
    printf("\nOlá, digite algo e o classificaremos para você.\n>> ");
    yylex();

    return 0;
}
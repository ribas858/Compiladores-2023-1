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

"/*"(.)*"*/" {printf("Comentário\n");}

-*[0-9][0-9]*\.[0-9][0-9]* {
        if (yytext[0] == '-') {
            printf("\nPonto Flutuante Negativo\n\n");
        } else {
            printf("\nPonto Flutuante Positivo\n\n");
        }
    }

.+ {printf("Não corresponde a gramática do C-Minus\n");}

\n { return 0; }

%%

int yywrap() {}

int main() {

    #if defined(__linux__)
        system("clear");
    #elif _WIN32
        system("cls");
    #endif

    printf("\nAnalisador Léxico 1.0 - C-Minus\n\n");

    printf("Olá, digite algo e o classificaremos em C-Minus para você.\n>> ");
    
    yylex();

    return 0;
}
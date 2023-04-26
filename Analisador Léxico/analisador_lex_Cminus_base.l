%{
    /* definições */

%}

%%

-*[0-9][0-9]* {
        if (yytext[0] == '-') {
            printf("\nNumero Negativo\n\n");
        } else {
            printf("\nNumero Positivo\n\n");
        }
    }
    
[A-Z_a-z][A-Z_a-z0-9]* {printf("\nIdentificador (Variavel)\n\n");}

"/*"(.)*"*/" {printf("\nComentario\n\n");}

-*[0-9][0-9]*\.[0-9][0-9]* {
        if (yytext[0] == '-') {
            printf("\nPonto Flutuante Negativo\n\n");
        } else {
            printf("\nPonto Flutuante Positivo\n\n");
        }
    }

.+ {printf("\nNao corresponde a gramatica do C-Minus\n\n");}

\n { return 0; }

%%

int yywrap() {}

int main() {

    #if defined(__linux__)
        system("clear");
    #elif _WIN32
        system("cls");
    #endif

    printf("\nAnalisador Lexico 1.0 - C-Minus\n\n");

    printf("Ola, digite algo e o classificaremos em C-Minus para voce.\n>> ");
    
    yylex();

    return 0;
}
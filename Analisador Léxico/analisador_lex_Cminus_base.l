%{
    /* definições */

%}

NUMERO [0-9][0-9]*
ID [A-Z_a-z][A-Z_a-z0-9]*

test "a"

%%

-?{NUMERO} {
        if (yytext[0] == '-') {
            printf("\nNumero Negativo\n\n");
        } else {
            printf("\nNumero Positivo\n\n");
        }
    }
    
{ID} {printf("\nIdentificador (Variavel)\n\n");}

"/*""*/" {printf("\nComentario\n\n");}

-?{NUMERO}\.{NUMERO} {
        if (yytext[0] == '-') {
            printf("\nPonto Flutuante Negativo\n\n");
        } else {
            printf("\nPonto Flutuante Positivo\n\n");
        }
    }

.+ {printf("\nNao corresponde a gramatica do C-Minus: %s\n\n", yytext);}


\n {return 0;}


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
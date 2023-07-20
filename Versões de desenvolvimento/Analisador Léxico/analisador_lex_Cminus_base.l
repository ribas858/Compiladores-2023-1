%{
    /* definições */

%}

LETRA [A-Z_a-z]
DIGITO [0-9]

NUMERO {DIGITO}{DIGITO}*
ID {LETRA}({LETRA}|{DIGITO})*

COMENTARIO "/*"([^*\n]|((\*)+[^*/\n]))*(\*+)(\/)" "*

%%

\n {printf("\nFim da Analise.\n"); return 0; }

else|if|int|return|void|while  {printf("\nToken Reservado: %s\n\n", yytext);}

\+|-|\*|\/ {printf("\nOperador Matematico: %s\n\n", yytext);}
\<|\<\=|\>|\>\=|\=\=|\!\= {printf("\nOperador de Comparacao: %s\n\n", yytext);}

\= {printf("\nOperador de Atribuicao: %s\n\n", yytext);}

;|,|\(|\)|\[|\]|\{|\}|\/\*|\*\/ {printf("\nOutros Tokens Especiais: %s\n\n", yytext);}

-?{NUMERO} {
        if (yytext[0] == '-') {
            printf("\nNumero Negativo: %s\n\n", yytext);
        } else {
            printf("\nNumero Positivo: %s\n\n", yytext);
        }
    }

-?{NUMERO}\.{NUMERO} {
        if (yytext[0] == '-') {
            printf("\nPonto Flutuante Negativo: %s\n\n", yytext);
        } else {
            printf("\nPonto Flutuante Positivo: %s\n\n", yytext);
        }
    }
    
{ID} {printf("\nIdentificador (Variavel): %s\n\n", yytext);}

{COMENTARIO} {printf("\nComentario: %s\n\n", yytext);}

.+ {printf("\nNao corresponde a gramatica do C-Minus: %s\n\n", yytext);}

%%

int yywrap() {}

int main() {

    #if defined(__linux__)
        system("clear");
    #elif _WIN32
        system("cls");
    #endif

    printf("\nAnalisador Lexico 1.1 - C-Minus\n\n");

    printf("Ola, digite algo e o classificaremos em C-Minus para voce.\n>> ");
    
    yylex();

    return 0;
}
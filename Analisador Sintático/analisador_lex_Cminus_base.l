%{
    /* definições */
    #include "parser.tab.h"

%}

LETRA [A-Z_a-z]
DIGITO [0-9]

NUMERO {DIGITO}{DIGITO}*
ID {LETRA}({LETRA}|{DIGITO})*

COMENTARIO "/*"([^*\n]|((\*)+[^*/\n]))*(\*+)(\/)" "*

%%

\n {return EOL; }

else|if|int|return|void|while  {printf("\nToken Reservado: %s\n\n", yytext);}

\+ { return MAIS; }

\-|\*|\/ {printf("\nOperador Matematico: %s\n\n", yytext);}
\<|\<\=|\>|\>\=|\=\=|\!\= {printf("\nOperador de Comparacao: %s\n\n", yytext);}

\= {printf("\nOperador de Atribuicao: %s\n\n", yytext);}

;|,|\(|\)|\[|\]|\{|\}|\/\*|\*\/ {printf("\nOutros Tokens Especiais: %s\n\n", yytext);}

-?{NUMERO} {
        if (yytext[0] == '-') {
            printf("\nNumero Negativo: %s\n\n", yytext);
        } else {
            //printf("\nNumero Positivo: %s\n\n", yytext);
            
            yylval.numero = atoi(yytext); 
            return NUMERO;
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

[ \t] {}

. {printf("\nNao corresponde a gramatica do C-Minus: %s\n\n", yytext);}

%%

int yywrap() {}
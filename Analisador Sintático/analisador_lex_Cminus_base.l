%{
    /* definições */
    #include "parser.tab.h"

%}

%option noyywrap

LETRA [A-Z_a-z]
DIGITO [0-9]
SIMBOLOS_ESPECIAIS \+\-|\*|\/|\<|\<\=|\>|\>\=|\=\=|\!\=|\=|\;|,|\(|\)|\[|\]|\{|\}|\/\*|\*\/

NUMERO {DIGITO}{DIGITO}*
ID {LETRA}({LETRA}|{DIGITO})*

COMENTARIO "/*"([^*\n]|((\*)+[^*/\n]))*(\*+)(\/)" "*



%%

else|if|int|return|void|while  {printf("\nToken Reservado: %s\n\n", yytext);}

\+ { printf("\nSoma: %s\n\n", yytext); return MAIS; }

\-|\*|\/ {printf("\nOperador Matematico: %s\n\n", yytext);}
\<|\<\=|\>|\>\=|\=\=|\!\= {printf("\nOperador de Comparacao: %s\n\n", yytext);}

\= {printf("\nOperador de Atribuicao: %s\n\n", yytext);}

; {printf("Ponto e virgula: %s\n\n", yytext); return PTV;}

\( {printf("Parenteses: %s\n\n", yytext); return P1;}

,|\)|\[|\]|\{|\}|\/\*|\*\/ {printf("\nOutros Tokens Especiais: %s\n\n", yytext);}

-?{NUMERO} {
        if (yytext[0] == '-') {
            printf("\nNumero Negativo: %s\n\n", yytext);
        } else {
            printf("\nNumero Positivo: %s\n\n", yytext);
            
            yylval.numero = atoi(yytext); 
            return NUMERO;
        }
    }

{COMENTARIO} {printf("\nComentario: %s\n\n", yytext);}

{ID} {printf("\nIdentificador (Variavel): %s\n\n", yytext);}

[ \t] {}

\n {return EOL;}

. {printf("\nNao corresponde a gramatica do C-Minus: %s %d\n\n", yytext, yyleng);}

%%
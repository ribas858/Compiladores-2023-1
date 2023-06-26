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

else|return|void|while  {printf("\nToken Reservado: %s\n\n", yytext);}

int {printf("Int: %s\n\n", yytext); yylval.string = strdup(yytext); return INT;}

if {printf("If: %s\n\n", yytext); yylval.string = strdup(yytext); return IF;}

print {printf("Print: %s\n\n", yytext); yylval.string = strdup(yytext); return PRINT;}

\+ {printf("\nOperador Matematico: %s\n\n", yytext); yylval.caracter = yytext[0]; return MAIS; }
\- {printf("\nOperador Matematico: %s\n\n", yytext); yylval.caracter = yytext[0]; return MENOS; }
\* {printf("\nOperador Matematico: %s\n\n", yytext); yylval.caracter = yytext[0]; return MULT; }
\/ {printf("\nOperador Matematico: %s\n\n", yytext); yylval.caracter = yytext[0]; return DIV; }


\<|\<\=|\>|\>\=|\=\=|\!\= {printf("\nOperador de Comparacao: %s\n\n", yytext); yylval.string = strdup(yytext); return OP_COMP;}

\= {printf("\nOperador de Atribuicao: %s\n\n", yytext); yylval.caracter = yytext[0]; return ATRIB;}

; {printf("Ponto e virgula: %s\n\n", yytext); yylval.caracter = yytext[0]; return PTV;}

\( {printf("Abre Parenteses: %s\n\n", yytext); yylval.caracter = yytext[0]; return P1;}
\) {printf("Fecha Parenteses: %s\n\n", yytext); yylval.caracter = yytext[0]; return P2;}

\,|\[|\]|\{|\}|\/\*|\*\/ {printf("\nOutros Tokens Especiais: %s\n\n", yytext); return OUTROS; }

-?{NUMERO} {
        if (yytext[0] == '-') {
            printf("\nNumero Negativo: %s\n\n", yytext);
            char s[yyleng-1];
            for (int i=1; i<yyleng; i++) {
                s[i-1] = yytext[i];
            }
            yylval.inteiro = atoi(s) * (-1);
            return NUMERO;
        } else {
            printf("\nNumero Positivo: %s\n\n", yytext);
            
            yylval.inteiro = atoi(yytext); 
            return NUMERO;
        }
    }

{COMENTARIO} {printf("\nComentario: %s\n\n", yytext);}

{ID} {printf("\nIdentificador (Variavel): %s\n\n", yytext); yylval.string = strdup(yytext); return ID;}

[ \t\n] {
    if( yytext[0] == '\n' ) {
        yylval.caracter = yytext[0];
        return BARRAN;
    }
}

. {printf("\nNao corresponde a gramatica do C-Minus: %s %d\n\n", yytext, yyleng);}

%%
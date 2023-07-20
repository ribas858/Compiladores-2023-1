%{
    /* definições */
    #include "parser.tab.h"
    int linha_count = 0;

%}

%option noyywrap

LETRA [A-Z_a-z]
DIGITO [0-9]

NUMERO {DIGITO}{DIGITO}*
ID {LETRA}({LETRA}|{DIGITO})*

COMENTARIO "/*"([^*\n]|((\*)+[^*/\n]))*(\*+)(\/)" "*

%%

int     {   //printf("Int: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return INT;
        }

void    {   //printf("Void: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return VOID;
        }

if      {   //printf("If: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return IF;
        }

else    {   //printf("Else: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return ELSE;
        }

while   {   //printf("While: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return WHILE;
        }

print   {   //printf("Print: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return PRINT;
        }

return  {   //printf("Return: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return RETURN;
        }

\+      {   //printf("\nOperador Matematico: %s\n\n", yytext); 
            yylval.caracter = yytext[0]; 
            return MAIS;
        }

\-      {   //printf("\nOperador Matematico: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return MENOS;
        }

\*      {   //printf("\nOperador Matematico: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return MULT;
        }

\/      {   //printf("\nOperador Matematico: %s\n\n", yytext); 
            yylval.caracter = yytext[0]; 
            return DIV;
        }

\<      {   //printf("\nOperador de Comparacao: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return MENOR;
        }

\>      {   //printf("\nOperador de Comparacao: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return MAIOR;
        }

\<\=    {   //printf("\nOperador de Comparacao: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return MENOR_IGUAL;
        }

\>\=    {   //printf("\nOperador de Comparacao: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return MAIOR_IGUAL;
        }

\=\=    {   //printf("\nOperador de Comparacao: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return IGUAL;
        }

\!\=    {   //printf("\nOperador de Comparacao: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return DIF;
        }

\=      {   //printf("\nOperador de Atribuicao: %s\n\n", yytext);
            yylval.caracter = yytext[0];
            return ATRIB;
        }

\;       {   //printf("Ponto e virgula: %s\n\n", yytext);
            yylval.caracter = yytext[0];
            return PTV;
        }

\(      {   //printf("Abre Parenteses: %s\n\n", yytext);
            yylval.caracter = yytext[0];
            return P1;
        }

\)      {   //printf("Fecha Parenteses: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return P2;
        }

\{      {   //printf("\nAbre Chaves: %s\n\n", yytext); 
            yylval.caracter = yytext[0]; 
            return CHV1;
        }

\}      {   //printf("\nFecha Chaves: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return CHV2;
        }

\[      {   //printf("\nAbre parenteses: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return PAR1;
        }

\]      {   //printf("\nFecha parenteses: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return PAR2;
        }

\,      {   //printf("\nVirgula: %s\n\n", yytext);
            yylval.caracter = yytext[0]; 
            return VG;
        }

-?{NUMERO} {    if ( yytext[0] == '-') {
                    //printf("\nNumero Negativo: %s\n\n", yytext);
                    char s[yyleng-1];
                    for (int i=1; i<yyleng; i++) {
                        s[i-1] = yytext[i];
                    }
                    yylval.inteiro = atoi(s) * (-1);
                    return NUMERO;
                } else {
                    //printf("\nNumero Positivo: %s\n\n", yytext);
                    
                    yylval.inteiro = atoi(yytext); 
                    return NUMERO;
                }
            }

{COMENTARIO} {  //printf("\nComentario: %s\n\n", yytext);
}

{ID}    {   //printf("\nIdentificador (Variavel): %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return ID;
        }

[ \t\n] {   if (yytext[0] == '\n') {
                linha_count++;
            }
            
        }

.       {   //printf("\nErro Lexico: Nao corresponde a gramatica do C-Minus: %s %d\n\n", yytext, yyleng);
}

%%
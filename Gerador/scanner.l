%{
    /* definições */
    #include "parser.tab.h"

    int linha_count = 0;
    int cap_menos = 0;
    int cap_parentese = 0;
    int num_count = 0;

%}

%option noyywrap


LETRA [A-Z_a-z]
DIGITO [0-9]

NUMERO {DIGITO}{DIGITO}*
ID {LETRA}({LETRA}|{DIGITO})*

COMENTARIO "/*"([^*\n]|((\*)+[^*/\n]))*(\*+)(\/)" "*

%%

int     {   // printf("Int: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return INT;
        }

void    {   //printf("Void: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return VOID;
        }

if      {   // printf("If: %s\n\n", yytext);
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

scan   {   //printf("Scan: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return SCAN;
        }

return  {   //printf("Return: %s\n\n", yytext);
            yylval.string = strdup(yytext);
            return RETURN;
        }

\+      {   //printf("\nOperador Matematico: %s\n\n", yytext); 
            yylval.caracter = yytext[0]; 
            return MAIS;
        }

\-      {   cap_menos++;
            if (cap_menos == 1) {
                //printf("\nOperador Matematico: %s\n\n", yytext);
                yylval.caracter = yytext[0];
                return MENOS;
            }
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
            //yylval.caracter = yytext[0]; 
            yylval.string = strdup(yytext);
            return MENOR;
        }

\>      {   //printf("\nOperador de Comparacao: %s\n\n", yytext);
            //yylval.caracter = yytext[0];
            yylval.string = strdup(yytext);
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
            cap_parentese = 1;
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

{NUMERO} {  printf("\nNumero: %s %d %d\n\n", yytext, cap_menos, cap_parentese);
            num_count++;
            if(cap_menos == 2 && cap_parentese == 1) {
                yylval.inteiro = atoi(yytext) * (-1);
                cap_menos = 0;
                cap_parentese = 0;
                
                return NUMERO;
            } else if ( (cap_menos == 2 && cap_parentese == 0) || cap_menos > 2) {
                printf(" ----->> yyval: %d\n", yylval.inteiro);
                yylval.caracter = '-';
                cap_menos = 0;
                cap_parentese = 0;
                return MENOS;
            } else {
                yylval.inteiro = atoi(yytext);
                cap_menos = 0;
                cap_parentese = 0;
                return NUMERO;
            }
        }

{COMENTARIO} {  //printf("\nComentario: %s\n\n", yytext);
}

{ID}    {   //printf("\nIdentificador (Variavel): %s\n\n", yytext);
            cap_menos = 0;
            cap_parentese = 0;
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
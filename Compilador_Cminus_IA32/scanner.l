%{
    #include "parser.tab.h"

    int linha_count = 0;
    int cap_menos = 0;
    int cap_parentese = 0;
    int num_count = 0;
    int scan_count = 0;
    int print_count = 0;

%}

%option noyywrap


LETRA [A-Z_a-z]
DIGITO [0-9]

NUMERO {DIGITO}{DIGITO}*
ID {LETRA}({LETRA}|{DIGITO})*

COMENTARIO "/*"([^*\n]|((\*)+[^*/\n]))*(\*+)(\/)" "*

%%

int     {   yylval.string = strdup(yytext);
            return INT;
        }

void    {   yylval.string = strdup(yytext);
            return VOID;
        }

if      {   yylval.string = strdup(yytext);
            return IF;
        }

else    {   yylval.string = strdup(yytext);
            return ELSE;
        }

while   {   yylval.string = strdup(yytext);
            return WHILE;
        }

print   {   print_count++;
            yylval.string = strdup(yytext);
            return PRINT;
        }

scan   {    scan_count++;
            yylval.string = strdup(yytext);
            return SCAN;
        }

return  {   yylval.string = strdup(yytext);
            return RETURN;
        }

\+      {   yylval.caracter = yytext[0]; 
            return MAIS;
        }

\-      {   cap_menos++;
            if (cap_menos == 1) {
                yylval.caracter = yytext[0];
                return MENOS;
            }
        }

\*      {   yylval.caracter = yytext[0]; 
            return MULT;
        }

\/      {   yylval.caracter = yytext[0]; 
            return DIV;
        }

\<      {   yylval.string = strdup(yytext);
            return MENOR;
        }

\>      {   yylval.string = strdup(yytext);
            return MAIOR;
        }

\<\=    {   yylval.string = strdup(yytext);
            return MENOR_IGUAL;
        }

\>\=    {   yylval.string = strdup(yytext);
            return MAIOR_IGUAL;
        }

\=\=    {   yylval.string = strdup(yytext);
            return IGUAL;
        }

\!\=    {   yylval.string = strdup(yytext);
            return DIF;
        }

\=      {   yylval.caracter = yytext[0];
            return ATRIB;
        }

\;      {   yylval.caracter = yytext[0];
            return PTV;
        }

\(      {   yylval.caracter = yytext[0];
            cap_parentese = 1;
            return P1;
        }

\)      {   yylval.caracter = yytext[0]; 
            return P2;
        }

\{      {   yylval.caracter = yytext[0]; 
            return CHV1;
        }

\}      {   yylval.caracter = yytext[0]; 
            return CHV2;
        }

\[      {   yylval.caracter = yytext[0]; 
            return PAR1;
        }

\]      {   yylval.caracter = yytext[0]; 
            return PAR2;
        }

\,      {   yylval.caracter = yytext[0]; 
            return VG;
        }

{NUMERO} {  num_count++;
            if(cap_menos == 2 && cap_parentese == 1) {
                yylval.inteiro = atoi(yytext) * (-1);
                cap_menos = 0;
                cap_parentese = 0;
                
                return NUMERO;
            } else if ( (cap_menos == 2 && cap_parentese == 0) || cap_menos > 2) {
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

{COMENTARIO} { }

{ID}    {   cap_menos = 0;
            cap_parentese = 0;
            yylval.string = strdup(yytext);
            return ID;
        }

[ \t\n] {   if (yytext[0] == '\n') {
                linha_count++;
            }
            
        }

.       {   printf("\nErro Lexico: Nao corresponde a gramatica do C-Minus: %s %d\n\n", yytext, yyleng); }

%%
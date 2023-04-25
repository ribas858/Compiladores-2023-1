%{
    /* definições */

%}

%%

[a-z] {printf("Minusculas\n");}

. {printf("erro\n");}

\n {return 0;}

%%

int yywrap() {}


int main() {
    printf("Enter a string");
    yylex();

    return 0;
}
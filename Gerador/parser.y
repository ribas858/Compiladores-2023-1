%{
    #include "funcs.h"
    FILE* yyin;

    struct tbs *tabela_simbolos = NULL;

%}

%define parse.error verbose

%union {
    int inteiro;
    char *string;
    char caracter;
    void *generico;
}

// Tipos
%token<string> INT
%token<string> VOID

// Comandos
%token IF
%token ELSE
%token WHILE
%token PRINT
%token RETURN


// Operadores
%token<caracter> MAIS
%token<caracter> MENOS
%token<caracter> MULT
%token<caracter> DIV
%token<caracter> MENOR
%token<caracter> MAIOR
%token<caracter> MENOR_IGUAL
%token<caracter> MAIOR_IGUAL
%token<caracter> IGUAL
%token<caracter> DIF
%token<caracter> ATRIB


// Delimitadores
%token<caracter> PTV
%token<caracter> P1
%token<caracter> P2
%token<caracter> CHV1
%token<caracter> CHV2
%token<caracter> PAR1
%token<caracter> PAR2
%token<caracter> VG


// Outros
%token<inteiro> NUMERO
%token<string>  ID

// Tipos para regras
%type<inteiro> expr
%type<inteiro> expr_interna


%start programa

%%

programa: 
    |   programa instr
    |   error
    ;

instr:  var PTV
    |   print PTV
    |   if
    |   while
    |   func
    |   RETURN valores PTV
    ;

if:     IF P1 compara P2 CHV1 programa CHV2
    |   if ELSE CHV1 programa CHV2
    ;

while:
        WHILE P1 compara P2 CHV1 programa CHV2
    ;

func:   VOID ID P1 parametros P2 CHV1 programa CHV2
    |   INT ID P1 parametros P2 CHV1 programa CHV2
    ;


parametros:
        valores VG parametros
    |   valores
    ;

compara:
        valores operadores valores
    |   error                       { }
    ;

valores:
        expr
    |   ID  {                                   printf("Ocorrencia\n");
                                                generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = "";
                                                insere_simbolo(&tabela_simbolos, $1, &expr);
                                                }
    ;

operadores:
        MAIOR
    |   MENOR
    |   MAIOR_IGUAL
    |   MENOR_IGUAL
    |   IGUAL
    |   DIF
    ;



expr:   expr MENOS expr_interna     {           $$ = $1 - $3;
                                                expr_nasm($2, $1, $3);
                                                //printf("menos: %d - %d\n", $1, $3);
                                                }

    |   expr MAIS expr_interna      {           $$ = $1 + $3;
                                                expr_nasm($2, $1, $3);
                                                //printf("mais: %d + %d\n", $1, $3);
                                                }

    |   expr_interna                {           $$ = $1;
                                                }
    ;

expr_interna:
        expr_interna MULT NUMERO    {           $$ = $1 * $3;
                                                expr_nasm($2, $1, $3);
                                                //printf("mult: %d * %d\n", $1, $3);
                                                }

    |   expr_interna DIV NUMERO     {           $$ = $1 / $3;
                                                expr_nasm($2, $1, $3);
                                                //printf("div: %d / %d\n", $1, $3);
                                                }

    |   NUMERO                      {           $$ = $1;
                                                }

    |   P1 expr P2                  {           $$ = $2;
                                                }

    |   error                       {           }
    ;


var:    INT ID      {                           printf("Declara\n");
                                                generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = $1;
                                                insere_simbolo(&tabela_simbolos, $2, &expr);
                                                }
                                    
    |   INT ID PAR1 NUMERO PAR2     {           printf("Caso vetor\n");
                                                generico expr; expr.dado = &$4; expr.dado_tipo = $1; expr.tipo_regra = $1;
                                                insere_simbolo(&tabela_simbolos, $2, &expr);
                                                }

    |   INT ID ATRIB expr   {                   printf("Declara com atribuicao\n");
                                                generico expr; expr.dado = &$4; expr.dado_tipo = $1; expr.tipo_regra = $1;
                                                insere_simbolo(&tabela_simbolos, $2, &expr);
                                                //printList(tabela_simbolos);
                                                }
                            

    |   ID ATRIB expr       {                   printf("Atribuicao\n");
                                                generico expr; expr.dado = &$3; expr.dado_tipo = "int"; expr.tipo_regra = "";
                                                insere_simbolo(&tabela_simbolos, $1, &expr);
                                                //printList(tabela_simbolos);
                                                }
    
    |   INT ID ATRIB ID         {               printf("Declara com Atribuicao VAR\n");
                                                generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = $1;
                                                
                                                if(insere_simbolo(&tabela_simbolos, $2, &expr)) {
                                                    atrib_var(&tabela_simbolos, $2, $4);
                                                }
                                                //generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = "";
                                                }

    |   ID ATRIB ID         {                   printf("Atribuicao VAR\n");
                                                //printf("Atribuicao com variavel: %s\n", retorna_tipo(&tabela_simbolos, $1));
                                                atrib_var(&tabela_simbolos, $1, $3);
                                                //generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = "";
                                                }
    ;

print:  PRINT P1 valores P2
    ;

%%


int main(int argc, char *argv[]) {
    erro_count = 0;
    lista_regs = NULL;
    insere_reg(&lista_regs, "eax");
    insere_reg(&lista_regs, "ebx");
    insere_reg(&lista_regs, "ecx");
    insere_reg(&lista_regs, "edx");
    insere_reg(&lista_regs, "esi");
    insere_reg(&lista_regs, "edi");


    yyin = fopen(argv[1], "r");

    if (!yyin) {
        printf("\nArquivo invalido ou inexistente, verifique seu diretório.\nPasse como argumento 'seu_diretorio/seu_arquivo_codigo.cm'\n\n");
        return 1;
    }

    temp = fopen("temp", "w");
    if(!temp) {
        printf("\nErro ao criar arquivo temporario..\n\n");
    }
    

    yyparse();

    /* fseek(yyin, 0, SEEK_SET);

    yyparse(); */

    fclose(yyin);
    fclose(temp);

    //printList(tabela_simbolos);

    if(erro_count == 0) {
        char *nome = retorna_nome(retorna_nome(argv[1], 2), 1);    
        char ext[] = { ".asm" };
        FILE* nasm_saida = fopen(strcat(nome, ext), "w");
        temp = fopen("temp", "r");

        char c;
        while (( c = fgetc(temp)) != EOF) {
            fputc(c, nasm_saida);
        }
        fclose(nasm_saida);
        fclose(temp);

    } else {
        negrito(1); cor_terminal(1, 1);
        printf("\nERRO::COMPILAÇÃO: %d Erros foram encontrados ....\n\n", erro_count);
        negrito(0); cor_terminal(1, 0);
    }
    
    if(remove("temp") < 0) {
        printf("\nErro ao DELETAR temporario..\n\n");
    }

    printRegs(lista_regs);

    return 0;
}

void yyerror(char const* s) {
    print_erros((char*) s, 3);
}




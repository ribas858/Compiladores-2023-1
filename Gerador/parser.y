%{
    #include "funcs.h"
    FILE* yyin;

    struct tbs *tabela_simbolos = NULL;

    int passagem = 1;
    int tem_else = 0;

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
%token<string> MENOR
%token<string> MAIOR
%token<string> MENOR_IGUAL
%token<string> MAIOR_IGUAL
%token<string> IGUAL
%token<string> DIF
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
%type<string> operadores


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

if:     IF P1 expr operadores expr P2 {                         printf("CASO IF expr expr..%d\n", num_count);
                                                                num_count = 0;
                                                                if(passagem == 2) {
                                                                    if_part1($3, "", $4, $5, "", 1, 0);
                                                                }
                                                                
                                    } CHV1 programa CHV2 {      if(passagem == 2) {
                                                                    if_part2(0);
                                                                }

                                    }

    |   IF P1 ID operadores expr P2 {                           printf("CASO IF ID expr..\n");
                                                                num_count = 0;
                                                                if(passagem == 2) {
                                                                    if_part1(0, $3, $4, $5, "", 2, 0);
                                                                }
                                                                
                                    } CHV1 programa CHV2 {      if(passagem == 2) {
                                                                    if_part2(0);
                                                                }

                                    }

    |   IF P1 expr operadores ID P2 {                           printf("CASO IF expr ID..\n");
                                                                num_count = 0;
                                                                if(passagem == 2) {
                                                                    if_part1($3, "", $4, 0, $5, 3, 0);
                                                                }
                                                                
                                    } CHV1 programa CHV2 {      if(passagem == 2) {
                                                                    if_part2(0);
                                                                }

                                    }

    |   IF P1 ID operadores ID P2 {                             printf("CASO IF ID ID..\n");
                                                                if(passagem == 2) {
                                                                    if_part1(0, $3, $4, 0, $5, 4, 0);
                                                                }

                                } CHV1 programa CHV2 {          if(passagem == 2) {
                                                                    if_part2(0);
                                                                }
                                }


    | if ELSE CHV1 programa CHV2
    ;

while:
        WHILE P1 expr operadores expr P2 {                      printf("CASO WHILE expr expr..%d\n", num_count);
                                                                num_count = 0;
                                                                if(passagem == 2) {
                                                                    if_part1($3, "", $4, $5, "", 1, 1);
                                                                }
                                                                
                                    } CHV1 programa CHV2 {      if(passagem == 2) {
                                                                    if_part2(1);
                                                                }

                                    }

    |   WHILE P1 ID operadores expr P2 {                        printf("CASO WHILE ID expr..\n");
                                                                num_count = 0;
                                                                if(passagem == 2) {
                                                                    if_part1(0, $3, $4, $5, "", 2, 1);
                                                                }
                                                                
                                    } CHV1 programa CHV2 {      if(passagem == 2) {
                                                                    if_part2(1);
                                                                }

                                    }

    |   WHILE P1 expr operadores ID P2 {                        printf("CASO WHILE expr ID..\n");
                                                                num_count = 0;
                                                                if(passagem == 2) {
                                                                    if_part1($3, "", $4, 0, $5, 3, 1);
                                                                }
                                                                
                                    } CHV1 programa CHV2 {      if(passagem == 2) {
                                                                    if_part2(1);
                                                                }

                                    }

    |   WHILE P1 ID operadores ID P2 {                          if(passagem == 2) {
                                                                    if_part1(0, $3, $4, 0, $5, 4, 1);
                                                                }
                                                                } CHV1 programa CHV2 {
                                                                    if (passagem == 2) {
                                                                        if_part2(1);
                                                                    }
                                                                }
    ;

func:   VOID ID P1 parametros P2 CHV1 programa CHV2
    |   INT ID P1 parametros P2 CHV1 programa CHV2
    ;


parametros:
        valores VG parametros
    |   valores
    ;


valores:
        expr {                                  if (passagem == 2) {
                                                    // printf("numeros contador.... %d\n", num_count);
                                                    
                                                    num_count = 0;
                                                }
        }

    |   ID  {                                   if (passagem == 1) {
                                                    printf("Ocorrencia\n");
                                                    generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = "";
                                                    insere_simbolo(&tabela_simbolos, $1, &expr, -1);
                                                }
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
                                                if (passagem == 2) {
                                                    char *reg = expr_nasm($2, $1, $3);
                                                    struct registrador *r = retorna_reg(&lista_regs, reg, 0, 1);
                                                    if(r) {
                                                        r->usado = 1;
                                                    } else {
                                                        printf("Registrador nao retornado\n");
                                                    }
                                                }
                                                //printf("menos: %d - %d\n", $1, $3);
                                                }

    |   expr MAIS expr_interna      {           $$ = $1 + $3;
                                                if (passagem == 2) {
                                                    char *reg = expr_nasm($2, $1, $3);
                                                    struct registrador *r = retorna_reg(&lista_regs, reg, 0, 1);
                                                    if(r) {
                                                        r->usado = 1;
                                                    } else {
                                                        printf("Registrador nao retornado\n");
                                                    }
                                                    printf("regist >>>>>>>>> %s\n", reg);
                                                }
                                                //printf("mais: %d + %d\n", $1, $3);
                                                }

    |   expr_interna                {           $$ = $1;
                                                }
    ;

expr_interna:
        expr_interna MULT NUMERO    {           $$ = $1 * $3;
                                                if (passagem == 2) {
                                                    char *reg = expr_nasm($2, $1, $3);
                                                    struct registrador *r = retorna_reg(&lista_regs, reg, 0, 1);
                                                    if(r) {
                                                        r->usado = 1;
                                                    } else {
                                                        printf("Registrador nao retornado\n");
                                                    }
                                                }
                                                //printf("mult: %d * %d\n", $1, $3);
                                                }

    |   expr_interna DIV NUMERO     {           $$ = $1 / $3;
                                                if (passagem == 2) {
                                                    char *reg = expr_nasm($2, $1, $3);
                                                    struct registrador *r = retorna_reg(&lista_regs, reg, 0, 1);
                                                    if(r) {
                                                        r->usado = 1;
                                                    } else {
                                                        printf("Registrador nao retornado\n");
                                                    }
                                                }
                                                //printf("div: %d / %d\n", $1, $3);
                                                }

    |   NUMERO                      {           $$ = $1;
                                                //char *reg = expr_nasm(' ', 0, $1);
                                                }

    |   P1 expr P2                  {           $$ = $2;
                                                }

    |   error                       {           }
    ;


var:    INT ID      {                           if (passagem == 1) {
                                                    printf("Declara\n");
                                                    generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = $1;
                                                    insere_simbolo(&tabela_simbolos, $2, &expr, -1);
                                                    num_count = 0;
                                                }
                                                }
                                    
    |   INT ID PAR1 NUMERO PAR2     {           if (passagem == 1) {
                                                    printf("Caso vetor\n");
                                                    generico expr; expr.dado = &$4; expr.dado_tipo = $1; expr.tipo_regra = $1;
                                                    insere_simbolo(&tabela_simbolos, $2, &expr, -1);
                                                    num_count = 0;
                                                }
                                                else if (passagem == 2) {
                                                    printf("numeros contador.... %d\n", num_count);
                                                    // if(num_count > 1) {
                                                    //     struct tbs *simbolo = retorna_simbolo(&tabela_simbolos, $2);
                                                    //     printf("Simb : %s\n", simbolo->simbolo);
                                                    //     registrador *reg = retorna_valor_regs(&lista_regs, $4);
                                                    //     if(reg) {
                                                    //         fprintf(temp, "%s", "mov [");
                                                    //         fprintf(temp, "%s], ", $2);
                                                    //         fprintf(temp, "%s\n", reg->reg);
                                                    //     } else {
                                                    //         printf("Regs nao encontrado\n");
                                                    //     }
                                                    // }
                                                    num_count = 0;
                                                }
                                                }

    |   INT ID ATRIB expr   {                   if (passagem == 1) {
                                                    printf("Declara com atribuicao, %d %s\n", num_count, $2);
                                                    printRegs(lista_regs, 2);
                                                    generico expr; expr.dado = &$4; expr.dado_tipo = $1; expr.tipo_regra = $1;
                                                    insere_simbolo(&tabela_simbolos, $2, &expr, num_count);
                                                    num_count = 0;
                                                    //printList(tabela_simbolos);
                                                }
                                                else if (passagem == 2) {
                                                    printf("numeros contador.... %d\n", num_count);
                                                    if(num_count > 1) {
                                                        struct tbs *simbolo = retorna_simbolo(&tabela_simbolos, $2);
                                                        printf("Simb : %s\n", simbolo->simbolo);
                                                        registrador *reg = retorna_valor_regs(&lista_regs, $4);
                                                        if(reg) {
                                                            fprintf(temp, "%s", "mov [");
                                                            fprintf(temp, "%s], ", $2);
                                                            fprintf(temp, "%s\n", reg->reg);
                                                        } else {
                                                            printf("Regs nao encontrado\n");
                                                        }
                                                    }
                                                    num_count = 0;
                                                    struct registrador *lista = lista_regs;
                                                    while(lista) {
                                                        if(lista->usado == 1) {
                                                            lista->usado = 0;
                                                            lista->ocup = 0;
                                                        }
                                                        lista = lista->prox;
                                                    }
                                                    printRegs(lista_regs, 2);
                                                }
                                                }
                            

    |   ID ATRIB expr       {                   if (passagem == 1) {
                                                    printf("Atribuicao\n");
                                                    generico expr; expr.dado = &$3; expr.dado_tipo = "int"; expr.tipo_regra = "";
                                                    insere_simbolo(&tabela_simbolos, $1, &expr, num_count);
                                                    num_count = 0;
                                                    //printList(tabela_simbolos);
                                                }
                                                else if (passagem == 2) {
                                                    printf("numeros contador.... %d\n", num_count);
                                                    
                                                    struct tbs *simbolo = retorna_simbolo(&tabela_simbolos, $1);
                                                    if(num_count > 1) {
                                                        registrador *reg = retorna_valor_regs(&lista_regs, $3);
                                                        if(reg) {
                                                            fprintf(temp, "%s", "mov [");
                                                            fprintf(temp, "%s], ", $1);
                                                            fprintf(temp, "%s\n", reg->reg);
                                                        } else {
                                                            printf("Regs nao encontrado\n");
                                                        }
                                                    } else if(igual(simbolo->tipo, "int")) {
                                                        fprintf(temp, "%s", "mov dword [");
                                                        fprintf(temp, "%s], ", $1);
                                                        fprintf(temp, "%d\n\t\t", $3);
                                                    }
                                            
                                                    num_count = 0;

                                                    struct registrador *lista = lista_regs;
                                                    while(lista) {
                                                        if(lista->usado == 1) {
                                                            lista->usado = 0;
                                                            lista->ocup = 0;
                                                        }
                                                        lista = lista->prox;
                                                    }
                                                    printRegs(lista_regs, 2);
                                                }
                                                }
    
    |   INT ID ATRIB ID         {               if (passagem == 1) {
                                                    printf("Declara com Atribuicao VAR\n");
                                                    generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = $1;
                                                    
                                                    if(insere_simbolo(&tabela_simbolos, $2, &expr, -1)) {
                                                        atrib_var(&tabela_simbolos, $2, $4);
                                                    }
                                                    //generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = "";
                                                }
                                                else if (passagem == 2) {
                                                    registrador *reg = retorna_reg(&lista_regs, "", 0, 3);
                                                    struct tbs *simbolo1 = retorna_simbolo(&tabela_simbolos, $2);
                                                    if(simbolo1->opr_expr <= 1) {
                                                        if(reg) {
                                                            fprintf(temp, "%s", "mov ");
                                                            fprintf(temp, "%s, ", reg->reg);
                                                            fprintf(temp, "[%s]\n", $4);
                                                        
                                                            fprintf(temp, "%s", "mov [");
                                                            fprintf(temp, "%s], ", $2);
                                                            fprintf(temp, "%s\n\t\t", reg->reg);
                                                        }
                                                    } 
                                                }
                                                }

    |   ID ATRIB ID         {                   if (passagem == 1) {
                                                    printf("Atribuicao VAR\n");
                                                    atrib_var(&tabela_simbolos, $1, $3);
                                                }
                                                else if (passagem == 2) {
                                                    registrador *reg = retorna_reg(&lista_regs, "", 0, 3);
                                                    
                                                    if(reg) {
                                                        fprintf(temp, "%s", "mov ");
                                                        fprintf(temp, "%s, ", reg->reg);
                                                        fprintf(temp, "[%s]\n", $3);
                                                    
                                                        fprintf(temp, "%s", "mov [");
                                                        fprintf(temp, "%s], ", $1);
                                                        fprintf(temp, "%s\n\t\t", reg->reg);
                                                    }
                                                }
                                                }
    ;

print:  PRINT P1 valores P2
    ;

%%


int main(int argc, char *argv[]) {
    rotulo = malloc(sizeof(struct labels));
    ifs = 0;
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
    
    printf("===================== PASSAGEM 1\n");
    yyparse();

    vars_nasm(tabela_simbolos);

    passagem = 2;
    fseek(yyin, 0, SEEK_SET);

    printf("===================== PASSAGEM 2\n");
    yyparse();


    fprintf(temp, "%s\n", "\nmov eax, 1\nmov ebx, 0\nint 80h");
    fclose(yyin);
    fclose(temp);

    printList(tabela_simbolos);

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

    printRegs(lista_regs, 2);

    return 0;
}

void yyerror(char const* s) {
    print_erros((char*) s, 3);
}




%{
    #include "funcs.h"
    FILE* yyin;

    struct tbs *tabela_simbolos = NULL;
    int passagem = 1;
    int id_count = 0;
%}

%define parse.error verbose

%union {
    int inteiro;
    char *string;
    char caracter;
    struct expressao *expr_;
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
%token SCAN

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
%type<expr_> expr
%type<expr_> expr_interna
%type<expr_> fator
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
    |   scan PTV
    ;

scan:   SCAN P1 ID P2   {                                       if (passagem == 1) {
                                                                    generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = "";
                                                                    insere_simbolo(&tabela_simbolos, $3, &expr, -1);
                                                                }
                                                                
else if(passagem == 2) {
    fprintf(temp, "\n\t%s\n", "pusha\n\
    mov eax, 3\n\
    mov ebx, 0\n\
    mov ecx, scan\n\
    mov edx, 255\n\
    int 80h\n\
    mov byte [scan+eax-1], 0\n\
    push eax\n\
    push scan\n\
    call str_to_int");
    fprintf(temp, "\t%s", "mov ");
    fprintf(temp, "[%s], ", $3);
    fprintf(temp, "%s", "eax\n\t");
    fprintf(temp, "%s", "popa\n\n");
    } 

    }
    ;


print:  PRINT P1 ID P2 {                                       if (passagem == 1) {
                                                                    generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = "";
                                                                    insere_simbolo(&tabela_simbolos, $3, &expr, -1);
                                                                }
                                                                
else if(passagem == 2) {
        fprintf(temp, "\n%s\n", "pusha");
        fprintf(temp, "%s", "push dword");
        fprintf(temp, "[%s]\n", $3);
        fprintf(temp, "%s\n", "push numero_buffer");
        fprintf(temp, "%s\n", "call meu_print_int");
        fprintf(temp, "%s\n", "call barra_n");
        fprintf(temp, "%s\n", "popa");
    }
    
    }
    ;


if:     IF P1 expr operadores expr P2 {                         num_count = 0;
                                                                id_count = 0;
                                                                if(passagem == 2) {
                                                                    rotulo = while_if_nasm($4, $3, $5, "if");
                                                                    strcpy(rot1, rotulo->lab1);
                                                                    strcpy(rot2, rotulo->lab2);
                                                                    strcpy(ret_reg_usado, rotulo->reg_usado);
                                                                    origin_oc = rotulo->origin_ocup;
                                                                }
                                                                
                                    } CHV1 programa CHV2 {      if(passagem == 2) {
                                                                    while_if_nasm_2("if");
                                                                }

                                    }

    | if ELSE CHV1 programa CHV2
    ;

while:
        WHILE P1 expr operadores expr P2 {                      num_count = 0;
                                                                id_count = 0;
                                                                if(passagem == 2) {
                                                                    rotulo = while_if_nasm($4, $3, $5, "while");
                                                                    strcpy(rot1, rotulo->lab1);
                                                                    strcpy(rot2, rotulo->lab2);
                                                                    strcpy(ret_reg_usado, rotulo->reg_usado);
                                                                    origin_oc = rotulo->origin_ocup;
                                                                }
                                                                
                                    } CHV1 programa CHV2 {      if(passagem == 2) {
                                                                    while_if_nasm_2("while");
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


expr:   expr MENOS expr_interna     {           if (passagem == 2) {
                                                    char *reg = expr_nasm($2, $1, $3);
                                                }
                                                $$->numero = $1->numero - $3->numero;
                                                }

    |   expr MAIS expr_interna      {           if (passagem == 2) {
                                                    char *reg = expr_nasm($2, $1, $3);
                                                }
                                                $$->numero = $1->numero + $3->numero;

                                                }

    |   expr_interna                {           $$ = $1;
                                                }
    ;

expr_interna:
        expr_interna MULT fator    {            if (passagem == 2) {
                                                    char *reg = expr_nasm($2, $1, $3);
                                                }
                                                $$->numero = $1->numero * $3->numero;
                                                }

    |   expr_interna DIV fator     {            if (passagem == 2) {
                                                    char *reg = expr_nasm($2, $1, $3);
                                                }
                                                $$->numero = $1->numero / $3->numero;
                                                }
                                                
    |   fator                       {           $$ = $1;    }

    |   error                       {                       }
    ;

fator:
        NUMERO                      {           $$ = malloc(sizeof(struct expressao));
                                                $$->numero = $1;
                                                $$->id = "";
                                                }
    
    |   ID                          {           struct tbs *tab = retorna_simbolo(&tabela_simbolos, $1);
                                                if(tab) {
                                                    $$ = malloc(sizeof(struct expressao));
                                                    $$->numero = tab->valor;
                                                    $$->id = $1;
                                                } else {
                                                    print_erros($1, 2);
                                                    $$ = malloc(sizeof(struct expressao));
                                                    $$->numero = 0;
                                                    $$->id = $1;
                                                }
                                                id_count++;
                                                
                                                }

    |   P1 expr P2                  {           $$ = $2;    }
    
    ;


var:    INT ID      {                           if (passagem == 1) {
                                                    generico expr; expr.dado = NULL; expr.dado_tipo = ""; expr.tipo_regra = $1;
                                                    insere_simbolo(&tabela_simbolos, $2, &expr, -1);
                                                    num_count = 0;
                                                }
                                                }
                                    
    |   INT ID PAR1 NUMERO PAR2     {           
                                                }

    |   INT ID ATRIB {id_expr = $2;} expr   {   if (passagem == 1) {
                                                    generico expr; expr.dado = &$5->numero; expr.dado_tipo = $1; expr.tipo_regra = $1;
                                                    insere_simbolo(&tabela_simbolos, $2, &expr, num_count + id_count);
                                                }
                                                else if (passagem == 2) {
                                                    struct tbs *simbolo = retorna_simbolo(&tabela_simbolos, $2);
                                                    if(simbolo->opr_expr > 1) {  
                                                        registrador *reg = retorna_var_regs(&lista_regs, $2, $5->numero, 2);
                                                        if(reg) {
                                                            fprintf(temp, "%s", "mov [");
                                                            fprintf(temp, "%s], ", $2);
                                                            fprintf(temp, "%s\n", reg->reg);
                                                            libera_regs(lista_regs, simbolo->simbolo);
                                                        } else {
                                                            printf("Regs nao encontrado\n");
                                                        }
                                                    }
                                                    // printRegs(lista_regs, 2);
                                                }
                                                num_count = 0;
                                                id_count = 0;
                                                }
                            

    |   ID ATRIB {id_expr = $1;} expr       {   if (passagem == 1) {
                                                    generico expr; expr.dado = &$4->numero; expr.dado_tipo = "int"; expr.tipo_regra = "";
                                                    insere_simbolo(&tabela_simbolos, $1, &expr, num_count + id_count);
                                                }
                                                else if (passagem == 2) {
                                                    struct tbs *simbolo = retorna_simbolo(&tabela_simbolos, $1);
                                                    if(simbolo->opr_expr > 1) {
                                                        registrador *reg = retorna_var_regs(&lista_regs, $1, $4->numero, 2);
                                                        if(reg) {
                                                            fprintf(temp, "%s", "mov [");
                                                            fprintf(temp, "%s], ", $1);
                                                            fprintf(temp, "%s\n", reg->reg);
                                                            libera_regs(lista_regs, simbolo->simbolo);
                                                        } else {
                                                            printf("Regs nao encontrado\n");
                                                        }
                                                    } else {
                                                        registrador *reg = retorna_var_regs(&lista_regs, $1, $4->numero, 2);
                                                        if(reg) {
                                                            fprintf(temp, "%s", "mov [");
                                                            fprintf(temp, "%s], ", $1);
                                                            fprintf(temp, "%s\n", reg->reg);
                                                            libera_regs(lista_regs, simbolo->simbolo);
                                                        } else {
                                                            fprintf(temp, "%s", "mov dword [");
                                                            fprintf(temp, "%s], ", $1);
                                                            fprintf(temp, "%d\n", $4->numero);
                                                        }
                                                    }
                                                    // printRegs(lista_regs, 2);
                                                    num_count = 0;
                                                    id_count = 0;
                                                }

                                                }
    ;

%%


int main(int argc, char *argv[]) {
    id_expr = "";
    rotulo = malloc(sizeof(struct labels));
    condicionais_count = 0;
    
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

    // printList(tabela_simbolos);

    if(erro_count == 0) {
        vars_nasm(tabela_simbolos);

        passagem = 2;
        fseek(yyin, 0, SEEK_SET);

        yyparse();

        fprintf(temp, "%s\n", "\nmov eax, 1\nmov ebx, 0\nint 80h");
        fclose(yyin);
        fclose(temp);

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

        // printRegs(lista_regs, 2);

    } else {
        negrito(1); cor_terminal(1, 1);
        printf("\nERRO::COMPILAÇÃO: %d Erros foram encontrados ....\n\n", erro_count);
        negrito(0); cor_terminal(1, 0);
    }
    
    if(remove("temp") < 0) {
        printf("\nErro ao DELETAR temporario..\n\n");
    }

    return 0;
}

void yyerror(char const* s) {
    print_erros((char*) s, 3);
}




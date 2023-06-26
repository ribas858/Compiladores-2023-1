%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    
    typedef struct tbs {
        char *simbolo;
        char *tipo;
        int valor;

        struct tbs *prox;
    } tbs;

    int yylex(void);
    void yyerror(char const* s);
    void insere_simbolo(struct tbs **lista, char *simbolo, void* valor, void *tipo);
    void printList(struct tbs *lista);
    void negrito(int status);
    void italico(int status);
    void cor_terminal(int colorCode, int status);
    void print_erros(char *simbolo, int erro);

    

    struct tbs *tabela_simbolos = NULL;
    
    

    size_t tamanho_int = sizeof(int);
    size_t tamanho_char = sizeof(char);
    int linha_count = 0;

%}

%union {
    int inteiro;
    char *string;
    char caracter;
}

%token<caracter> PTV
%token<caracter> P1
%token<caracter> P2
%token<string> INT

%token<string> ID

%token<caracter> MAIS
%token<caracter> MENOS
%token<caracter> MULT
%token<caracter> DIV

%token<caracter> BARRAN


%token<string> OP_COMP
%token<caracter> ATRIB

%token OUTROS

%token PRINT
%token IF

%start linha

%token<inteiro> NUMERO
%type<inteiro> expr
%type<inteiro> expr_interna



%%

linha:  instr PTV
    |   linha instr PTV
    |   instr PTV BARRAN            { linha_count++; }
    |   linha instr PTV BARRAN      { linha_count++; }
    |   BARRAN                      { linha_count++; }
    ;

instr:
    |   print
    |   if
    |   var
    ;

expr:   expr MENOS expr_interna { $$ = $1 - $3;  printf("menos: %d - %d\n", $1, $3); }
    |   expr MAIS expr_interna { $$ = $1 + $3;  printf("mais: %d + %d\n", $1, $3); }
    |   expr_interna { $$ = $1; }
    ;

expr_interna:
        expr_interna MULT NUMERO  { $$ = $1 * $3;  printf("mult: %d * %d\n", $1, $3); }
    |   expr_interna DIV NUMERO  { $$ = $1 / $3;  printf("div: %d / %d\n", $1, $3); }
    |   NUMERO { $$ = $1; }
    |   P1 expr P2 { $$ = $2; }
    ;


print:  PRINT P1 expr P2
    ;

if: IF P1 expr OP_COMP expr P2
    ;

var:    INT ID      {printf("id: %s\n", $2);    
                                                // tabela_simbolos = malloc(sizeof(tbs));
                                                // if(tabela_simbolos) {
                                                //     printf("SUCESSO\n");
                                                // }
                                                // tabela_simbolos->valor = 1;
                                                // tabela_simbolos->simbolo = "var";
                                                // tabela_simbolos->tipo = "int";
                                                // tabela_simbolos->prox = NULL;
                                                // int a = 2;
                                                insere_simbolo(&tabela_simbolos, $2, NULL, $1);
                                                }
    |   INT atrib   { } 
    |   atrib       { }
    ;

atrib:  ID ATRIB expr {                         //printf("%s == %d\n", $1, $3); 
                                                //printf("tab valor antes: %d\n", tabela_simbolos->valor);
                                                insere_simbolo(&tabela_simbolos, $1, &$3, NULL);
                                                //printf("tab valor dps: %d\n\n", tabela_simbolos->valor);
                                                //printList(tabela_simbolos);
                                                }
    ;

%%

extern FILE* yyin;

int main(int argc, char *argv[]) {

    yyin = fopen(argv[1], "r");

    if (!yyin) {
        printf("Arquivo invalido ou inexistente.\n");
        return 1;
    }

    yyparse();

    fclose(yyin);

    return 0;
}

void yyerror(char const* s) {
    printf("\n================>>>>>>ERRO: %s\n", s);
}

void insere_simbolo(struct tbs **lista, char *simbolo, void* valor, void *tipo) {  
    //printf("ENtrou insere\n");
    if(*lista) {
        struct tbs *aux = malloc(sizeof(tbs));
        aux = *lista;
        while(aux) {
            if (strcmp(aux->simbolo, simbolo) == 0) {
                if(!tipo) {
                    printf("tam: %zu\n", sizeof(*(int *) valor));
                    if(sizeof(*(int *) valor) == tamanho_int) {
                        aux->valor = *(int *) valor;
                        return;
                    }
                } else {
                    //printf("Erro semantico!!!! Declaracao dupla\n");
                    print_erros(aux->simbolo, 1);
                    return;
                }
            }
            aux = aux-> prox;
        }
    }

    if(!tipo) {
        //printf("Erro semantico!!!! Variavel não declarada\n");
        print_erros(simbolo, 2);
        return;
    }


    struct tbs *novoSimbolo = malloc(sizeof(tbs));
    // printf("Passou maloc\n");
    if(novoSimbolo) {
        novoSimbolo->simbolo = simbolo;
        //printf("Passou maloc 1\n");

        if(valor) {
            novoSimbolo->valor = *(int *) valor;
        }
        //printf("Passou maloc 2\n");
        novoSimbolo->tipo =  tipo;
        
        
        novoSimbolo->prox = NULL;

        if (*lista == NULL){
            *lista = novoSimbolo;
        } else {
            struct tbs *aux = malloc(sizeof(tbs));
            aux = *lista;
            while (aux->prox) {
                aux = aux->prox;
            }
            aux->prox = novoSimbolo;
        }

    } else {
        printf("Erro ao inserir simbolo\n");
    }
}

void printList(struct tbs *lista) {
    while(lista) {
        printf("Simbolo: %s\n", lista->simbolo);
        printf("Tipo: %s\n", lista->tipo);
        printf("Valor: %d\n\n", lista->valor);
        
        lista = lista->prox;
    }
}

void negrito(int status) {
    static const char *s[] = {"\x1b[0m", "\x1b[1m"};
    printf("%s", s[!!status]);
}

void italico(int status) {
    static const char *s[] = {"\x1b[0m", "\x1b[3m"};
    printf("%s", s[!!status]);
}

void cor_terminal(int colorCode, int status) {
    static const char *s[] = {"\x1b[0m", "\x1b[3%dm"};
    printf(s[!!status], colorCode);
}

void print_erros(char *simbolo, int erro) {
    if(erro == 1) {
        cor_terminal(6, 1);
        printf("\n(linha:%d) : ", linha_count+1 ); cor_terminal(2, 0);

        negrito(1); cor_terminal(1, 1);
        printf("Erro semantico >>>>>>>>>>> "); cor_terminal(1, 0); negrito(0);

        cor_terminal(2, 1);
        printf(" Dupla declaracao "); cor_terminal(2, 0);

        negrito(1); cor_terminal(5, 1);
        printf(" '%s'\n", simbolo); cor_terminal(5, 0); negrito(0);
    }
    else if(erro == 2) {
        cor_terminal(6, 1);
        printf("\n(linha:%d) : ", linha_count+1 ); cor_terminal(2, 0);

        negrito(1); cor_terminal(1, 1);
        printf("Erro semantico >>>>>>>>>>> "); cor_terminal(1, 0); negrito(0);
        
        cor_terminal(2, 1);
        printf(" Variavel"); cor_terminal(2, 0);

        negrito(1); cor_terminal(5, 1);
        printf(" '%s' ", simbolo); cor_terminal(5, 0); negrito(0);

        cor_terminal(2, 1);
        printf("não declarada!!\n\n"); cor_terminal(2, 0);
    }
}
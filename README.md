##### Instalar FLEX Linux/Ubuntu:
```
    $ sudo apt-get update
    
    $ sudo apt-get install flex

```

##### Instalar BISON Linux/Ubuntu:
```
    $ sudo apt-get update
    
    $ sudo apt-get install bison

```

##### Executar Léxico:
```
    $ flex analisador_lex_Cminus_base.l
    
    $ gcc lex.yy.c -o saida
    
    $ ./saida
```

##### Executar Sintático:
```
    $ flex analisador_lex_Cminus_base.l
    
    $ bison -d -t parser.y
    
    $ gcc lex.yy.c parser.tab.c
    
    $  ./a.out
```

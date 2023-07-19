PROJETO COMPILADORES 2023-1 - UnB

C-MINUS PARA IA-32

GRUPO 3
LUCAS JUNIOR RIBAS - 160052289
MATHEUS ARAUJO FERREIRA -
RAFAEL SILVA DE ALENCAR -

PARA EXECUTAR O PROJETO É PRECISO DAS SEGUINTES DEPENDÊNCIAS:

INSTRUÇÕES PARA O UBUNTU/UNIX:

Instalar FLEX Linux/Ubuntu:
    $ sudo apt-get update
    
    $ sudo apt-get install flex

Instalar BISON Linux/Ubuntu:
    $ sudo apt-get update
    
    $ sudo apt-get install bison

Instalar Montador NASM:
    $ sudo apt-get update

    $ sudo apt-get install nasm

Execução:
    ////// COMANDOS 

    $ flex scanner.l

    $ bison -d -t parser.y

    $ gcc funcs.c parser.tab.c lex.yy.c

    ////// ou APENAS

    $ make


Saída: 'a.out'

NA PASTA TESTES TEMOS VÁRIOS EXEMPLOS DE CÓDIGO C-MINUS PARA TESTAR SE PREFERIR.

Compilar no nosso compilador:

    $ ./a.out seu_diretorio/seu_arquivo.cm

    OBS: A extensão '.cm' não é obrigatória para compilar o seu código C-minus. Os arquivos
        de testes estão assim apenas para ficar mais intuitivo.

Saída: 'seu_arquivo.asm'

    Foi gerado um arquivo Assembly IA-32 de mesmo nome. Para testa-lo basta passar para o Montador NASM e ligar.

    $ nasm -f elf -o seu_arquivo.o seu_arquivo.asm
    $ ld -m elf_i386 -o seu_arquivo seu_arquivo.o
    $ ./seu_arquivo



INSTRUÇÕES WINDOWS:
    ////////////////////////////

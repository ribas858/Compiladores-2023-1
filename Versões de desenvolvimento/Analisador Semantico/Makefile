all: lexico sintatico_semantico
	gcc lex.yy.c parser.tab.c funcs.c
	rm *.yy.c
	rm *.tab.c
	rm *.tab.h


lexico: scanner.l
	flex scanner.l

sintatico_semantico: parser.y
	bison -d -t parser.y
    


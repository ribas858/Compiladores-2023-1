all: lexico sintatico_semantico
	gcc funcs.c parser.tab.c lex.yy.c

lexico: scanner.l
	flex scanner.l

sintatico_semantico: parser.y
	bison -d -t parser.y
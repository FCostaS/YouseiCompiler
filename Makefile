all: CFL

CFL:
    bison -d CFL.y && flex ER.l &&  gcc -o CFL CFL.tab.c lex.yy.c main.c Scan.c

clean:
	rm CFL CFL.tab.c lex.yy.c CFL.tab.h

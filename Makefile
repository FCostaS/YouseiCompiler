all: CFL

CFL:
    bison -d CFL.y
    flex ER.l
    gcc *.c -o Compile
    ./Compile

clean:
	rm CFL CFL.tab.c lex.yy.c CFL.tab.h

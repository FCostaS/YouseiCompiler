all: tokenizer parser build compile

tokenizer:
	flex ER.l

parser: 
	bison -d CFL.y

build:
	gcc *.c -o Compile

compile:
	./Compile

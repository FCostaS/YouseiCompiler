bison -d CFL.y
flex ER.l
gcc *.c -o Yousei
./Yousei Algoritmos/Contador.txt > Codigos/ContadorCrescente.txt &
./Yousei Algoritmos/Fibonacci.txt > Codigos/Fibonacci.txt &
./Yousei Algoritmos/Eratostenes.txt > Codigos/Eratostenes.txt &
./Yousei Algoritmos/Combinatoria.txt > Codigos/Combinatoria.txt &
./Yousei Algoritmos/GCD.txt > Codigos/GCD.txt &
./Yousei Algoritmos/Maximo.txt > Codigos/Maximo.txt &
./Yousei Algoritmos/Minimo.txt > Codigos/Minimo.txt &
./Yousei Algoritmos/Exponencial.txt > Codigos/Exponencial.txt &
./Yousei Algoritmos/Reverse.txt > Codigos/Reverse.txt &
./Yousei Algoritmos/Mersenne.txt > Codigos/Mersenne.txt &
./Yousei YouseiOS/YouseiOS.c > YouseiOS/YouseiOS.asm &

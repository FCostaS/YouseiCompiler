# Yousei Compiler
Compilador para linguagem C- implementado na disciplina: Laboratório de Sistemas Computacionais - Compiladores.

- bison -d CFL.y
- flex ER.l
- gcc *.c -o Compile && ./Compile


> Próximos Passos:
- Substituir MemAloc pela posição na memória da tabela de símbolos
- Criar um atribuidor de registradores
    - Criar lista de registradores com os campos (Ocupado {Sim, Não}, TipoArgs {Sim, Não}, CalckUse {Sim, Não}, VariableName,         ArrayType {Sim, Não}, 
    - Reservar t0 para acessar posições na memória
    - Devolver (Limpar) registradores usados pelo Calck (Ver como fazer isso)
    - Criar lista de labels (LabelName, LabelLine,IsFunction {Sim, Não})
    - Limpar todos os registradores após a função utilizar (Carry Junk)

> Registradores Reservados
 - $z0       > 0    < Registrador inalterável (zero sempre)
 - $r0       > 1    < Ponteiro da chamada de função
 - $a0-$a5   > 2-7  < Registradores para salvar argumentos de função
 - $t0-$t11  > 8-19 < Registradores Temporários
 - $s0-$s9   > 20-29< Registradores de acesso à memória
 - $v0       > 30   < Registrador de retorno de função
 - $sp       > 31   < Ponteiro da Pilha de Recursão

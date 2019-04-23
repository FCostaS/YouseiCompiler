# YouseiCompiler
My compiler for discipline Compiladores.

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
 - $z0       - Registrador inutilizável (Permanentemente zero - Acessar posições na memória, Operações com zero, ...)
 - $r0       - Registrador de JR        (Retorna o último endereço da pilha de chamada de função)
 - $v0       - Guarda o valor de retorno
 - $a0-$a5   - Guarda os valores dos argumentos
 - $t0...    - Registradores de propósito geral

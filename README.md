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
 - $z0       - Registrador inalterável (Zero sempre)
 - $r0       - Ponteiro da chamada de função
 - $v0       - Registrador de retorno de valor de função
 - $a0-$a5   - Registradores para salvar argumentos de função
 - $t0-$t10  - Registradores Temporários
 - $s0-$s10  - Registradores Gerais
 - $sp       - Ponteiro da Pilha de Recursão

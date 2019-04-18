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

> Registradores Reservados
$0       - Acessar posições na memória
$1-$10   - Temporários/Gerais

static void typeError(TreeNode * t, char * message)
{
    fprintf(listing,"Type error at line %d: %s\t",t->lineno,message);
    Error = TRUE;
}

void ErrorType(TreeNode * t,int TypeTest,char *function, int lineno)
{
      switch(TypeTest)
      {
        /* Variável não declarada */
        case 1:
            fprintf(listing,"- In function '%s': line %d\n",function,lineno);
            typeError(t,"\tVariable not declared.");
            fprintf(listing,"\n\t\t\tDeclare the variable: %s\n",t->attr.name);
        break;

        /* Atribuição inválida: a é do tipo int e exemplo() não retorna int */
        case 2:
            fprintf(listing,"- In function '%s': line %d\n",function,lineno);
            typeError(t,"\tVariable returns nothing.");
            fprintf(listing,"\n\t\t\tFunction is of type void.: %s\n",t->attr.name);
        break;

        /* Declaração inválida de variável, void só pode ser usado para declaração de função. */
        case 3:
            fprintf(listing,"- In function '%s': line %d\n",function,lineno);
            typeError(t, "\tVariable can not be of type void.");
            fprintf(listing,"\n\t\t\tSet the variable '%s' to integer.\n",t->attr.name);
        break;

        /* Declaração inválida, a já foi declarada previamente */
        case 4:
          fprintf(listing,"- In function '%s': line %d\n",function,lineno);
          typeError(t, "\tVariable has already been defined previously.");
          fprintf(listing,"\n\t\t\tRemove one of the variables '%s'\n",t->attr.name);
        break;

        /* Chamada de função não declarada */
        case 5:
          fprintf(listing,"- In function '%s': line %d\n",function,lineno);
          typeError(t, "\tUndeclared function.");
          fprintf(listing,"\n\t\t\tDeclare the function '%s'\n",t->attr.name);
        break;

        /* Função main() não declarada */
        case 6:
          fprintf(listing,"- Program ended without a main function:\n");
          fprintf(listing,"\t\t\tDeclare the 'main' function\n");
        break;

        /* Declaração inválida, xyz já foi declarado como nome de função */
        case 7:
          fprintf(listing,"- In function '%s': line %d\n",function,lineno);
          typeError(t, "\tAmbiguity in the statement.");
          fprintf(listing,"\n\t\t\tFunction and variable with same name '%s'\n",function);
        break;
      }
}

/******************** IDEIAS *********************/
/*
Erro tipo 1: Quando a variável não for uma declaração e eu desejo usa-la,
ou seja, ela está sendo detectada em ExpK, eu busco primeiro na tabela hash do
escopo atual, se ela não estiver lá eu procuro na hash global, que é o primeiro
nó da lista encadeada, se não estiver em ambos, solto a mensagem de erro.

Erro tipo 2: Como o problema aqui é uma função void ser atribuida à algo,
eu apenas devo verificar se o token anterior era uma atribuição, para isso
uso uma flag que quando detectar a atribuição muda seu valor e quando houver uma chamada de função,
eu verifico se a anterior foi uma atribuição, se sim eu emito o erro.
-- Tenho que ter cuidado para garantir que irei setar 1 quando detecto atribuição e zerar
a flag em todos os demais para garantir que, quando houver chamada de função,
a flag anterior diz respeito ao token anterior à chamada de função.

Erro tipo 3: Verifico o tipo da variavel quando ela for declarada.

Erro tipo 4: Busco na Hash se já existe uma variável com esse nome.

Erro tipo 5: Busco nos escopos, comparando os nomes com as funções que estou pretendendo chamar.

Erro tipo 6: Uso uma flag para indicar se uma função chamada main foi declarada e ao final da montagem,
vejo se a flag foi setada.

Erro tipo 7: O erro tipo 7 achei melhor dividir em dois tipos
          7.1 - Tentando declarar uma variável que já recebeu nome de função
          Busco o nome dessa variável na lista de funções, percorrendo elas e comparando.

          7.2 - Tentando declarar uma função que tem nome de variável global
          Busco o nome da função na tabela hash global (Primeiro no da lista encadeada)

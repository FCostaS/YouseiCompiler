static void typeError(TreeNode * t, char * message)
{
    fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
    Error = TRUE;
}

void ErrorType(TreeNode * t,int TypeTest)
{
      switch(TypeTest)
      {
        /* Variável não declarada */
        case 1:
        break;

        /* Atribuição inválida: a é do tipo int e exemplo() não retorna int */
        case 2:
        break;

        /* Declaração inválida de variável, void só pode ser usado para declaração de função. */
        case 3:
        break;

        /* Declaração inválida, a já foi declarada previamente */
        case 4:
        break;

        /* Chamada de função não declarada */
        case 5:
        break;

        /* Função main() não declarada */
        case 6:
        break;

        /* Declaração inválida, xyz já foi declarado como nome de função */
        case 7:
        break;
      }
}

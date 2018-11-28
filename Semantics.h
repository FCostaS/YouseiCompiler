static void typeError(TreeNode * t, char * message)
{
    fprintf(listing,"Type error at line %d: %s\t",t->lineno,message);
    Error = TRUE;
}

void ErrorType(TreeNode * t,int TypeTest,char *function)
{
      fprintf(listing,"In function '%s': line %d\n",function,t->lineno);
      switch(TypeTest)
      {
        /* Variável não declarada */
        case 1:
            typeError(t,"variável não declarada");
            fprintf(listing,"Declare a variável: %s\n",t->attr.name);
        break;

        /* Atribuição inválida: a é do tipo int e exemplo() não retorna int */
        case 2:
        break;

        /* Declaração inválida de variável, void só pode ser usado para declaração de função. */
        case 3:
            typeError(t, "Variável não pode ser do tipo void");
            fprintf(listing,"Redefina a variável: %s\n",t->attr.name);
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

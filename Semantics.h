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
            typeError(t,"\tVariável não declarada");
            fprintf(listing,"\n\t\t\tDeclare the variable: %s\n",t->attr.name);
        break;

        /* Atribuição inválida: a é do tipo int e exemplo() não retorna int */
        case 2:
            fprintf(listing,"- In function '%s': line %d\n",function,lineno);
            typeError(t,"\tVariável não retorna nada");
            fprintf(listing,"\n\t\t\tFunction is of type void.: %s\n",t->attr.name);
        break;

        /* Declaração inválida de variável, void só pode ser usado para declaração de função. */
        case 3:
            fprintf(listing,"- In function '%s': line %d\n",function,lineno);
            typeError(t, "\tVariável não pode ser do tipo void");
            fprintf(listing,"\n\t\t\tSet the variable '%s' to integer.\n",t->attr.name);
        break;

        /* Declaração inválida, a já foi declarada previamente */
        case 4:
          fprintf(listing,"- In function '%s': line %d\n",function,lineno);
          typeError(t, "\tVariável já foi definida previamente");
          fprintf(listing,"\n\t\t\tRemove one of the variables '%s'\n",t->attr.name);
        break;

        /* Chamada de função não declarada */
        case 5:
          fprintf(listing,"- In function '%s': line %d\n",function,lineno);
          typeError(t, "\tFunção não declarada");
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
          typeError(t, "\tAmbiguidade na declaração");
          fprintf(listing,"\n\t\t\tFunction and variable with same name '%s'\n",function);
        break;
      }
}

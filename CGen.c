#include <stdlib.h>
#include <string.h>
#include "Globals.h"
#include "SymbolTab.h"
#include "CGen.h"
#include "CodeGeass.h"

int i;
Stack MyStack;

static void cGen( TreeNode * tree);
static void genExpK( TreeNode * t);
char *StringOperation(TreeNode *tree);
void CalcGen(TreeNode *t);

/*char *ObterID(TreeNode *t)
{
    char *buffer;
    switch (t->kind.exp)
    {


      case CalcK:
          buffer = (char*)malloc(5*sizeof(char));
          int Reg = MyStack.Reg[MyStack.top];
          sprintf( buffer, "%d", Reg);
          genExpK(t);
          return buffer;
      break;
    }
}*/

char *StringOperation(TreeNode *tree)
{
    char *buffer;
    buffer = (char*)malloc(5*sizeof(char));
    switch (tree->kind.exp)
    {
        case OpK:
          switch (tree->attr.op)
          {
              case PLUS : sprintf( buffer, "+"); return buffer;
              case MINUS: sprintf( buffer, "-"); return buffer;
              case STAR : sprintf( buffer, "*"); return buffer;
              case BAR  : sprintf( buffer, "/"); return buffer;
              case SLT  : sprintf( buffer, "<"); return buffer;
              case SLTE : sprintf( buffer, "<="); return buffer;
              case SBT  : sprintf( buffer, ">"); return buffer;
              case SBTE : sprintf( buffer, ">="); return buffer;
              case EQUAL: sprintf( buffer, "=="); return buffer;
              case DIFF : sprintf( buffer, "!="); return buffer;
        }
       break;
       case IdK:
           return tree->attr.name;
       break;

       case ConstK:
           buffer = (char*)malloc(5*sizeof(char));
           sprintf( buffer, "%d", tree->attr.val );
           return buffer;
       break;

       case CalcK:
           CalcGen(tree);
           buffer = (char*)malloc(5*sizeof(char));
           sprintf( buffer, "%d",Reg);
           Reg--;
           return buffer;
       break;

       default:
        printf("Default StringOperation\n");
       return buffer;
    }
}

void CalcGen(TreeNode *t)
{
      TreeNode *p1,*p2,*p3;
      char *A,*B,*C;
      p1 = t->child[0];
      p2 = t->child[1];
      p3 = t->child[2];

      A = StringOperation(p1);
      B = StringOperation(p2);
      C = StringOperation(p3);

      printf("%d <- %s %s %s\n",Reg,A,B,C);
}

static void genExpK( TreeNode * t)
{
    int loc,args;
    char *A,*B,*C;
    TreeNode *p1, *p2;
    switch (t->kind.exp)
    {
        case OpK:
            //printf("Não é para vir em OpK\n");
        break;

        case ConstK:
            //printf("Não é para vir em ConstK\n");
        break;

        case IdK:
            //printf("Não é para vir em ConstK\n");
        break;

        case TypeK:   break;
        case ArrIdK:  break;
        case CallK:   // Chamada de Função (Análise de Argumentos da Função)
                if( strcmp(t->attr.name,"output") == 0)
                {
                    printf("Output Data\n");
                }
                args = 0;
                for(p1=t->child[0];p1!=NULL;p1 = p1->sibling)
                { genExpK(p1); args++; }
        break;
        case CalcK:
          //
          CalcGen(t);
          Reg--;
          // Devolver todos os últimos registradores anterior ao último usado
        break;
        default: printf("Você me esqueceu parça exp!\n"); break;
    }

}

static void genStmtK( TreeNode * tree)
{
      int loc,i;
      TreeNode *p1,*p2,*p3;
      int savedLoc1,savedLoc2,currentLoc;

      switch (tree->kind.stmt)
      {
          case IfK:

          p1 = tree->child[0] ;
          p2 = tree->child[1] ;
          p3 = tree->child[2] ;

          //IF
          cGen(p1);
          printf("IF\n");

          // ELSE
          cGen(p2);
          printf("ELSE\n");
          cGen(p3);

          break;
          case WhileK:



          break;
          case AssignK:

          /*if( strcmp(tree->child[1]->attr.name,"input") == 0)
          {
              printf("Input Data\n");
          }*/

          break;
          case CompoundK:
                  for (i=0; i < MAXCHILDREN; i++)
                  {
                      cGen(tree->child[i]);
                  }
          break;
          case ReturnK:
                  cGen(tree->child[0]); // Invoca Jump Register
                  printf("Return: Move returned to Register $1!\n");
          break;
          default: printf("Você me esqueceu parça stmt!\n"); break;
      }
}

static void genDeclK( TreeNode * t)
{
      switch (t->kind.decl)
      {
            case ArrParamK:  break; // Util na atribuição de Registradores Gerais
            case ParamK:

            if(t->attr.type!=Void)
            {
              printf("Definindo Registrador Temporário para '%s'\n",t->attr.name );
            }

            break; // Util na atribuição de Registradores Gerais
            case ArrVarK:    break; // Util na atribuição de Registradores Temporários
            case VarK:

            printf("Definindo Registrador Temporário para '%s'\n",t->attr.name );

            break; // Util na atribuição de Registradores Temporários
            case FunK:              // Percorre as funções presentes no programa
                printf("%s:\n",t->attr.name);
                for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); }
            break;
            default: printf("Você me esqueceu parça decl!\n"); break;
      }
}

static void cGen( TreeNode * tree)
{
  if (tree != NULL)
  {
    switch (tree->nodekind) {
      case StmtK:
        genStmtK(tree);
        break;
      case ExpK:
        genExpK(tree);
        break;
      case DeclK:
        genDeclK(tree);
        break;
    }
    cGen(tree->sibling);
  }
}

void codeGen(TreeNode * syntaxTree, char * codefile)
{
    printf("Assembly Generator...\n");
    StartStack(&MyStack);
    cGen(syntaxTree);
}

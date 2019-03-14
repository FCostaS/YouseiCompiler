#include <stdlib.h>
#include <string.h>
#include "Globals.h"
#include "SymbolTab.h"
#include "CGen.h"
#include "CodeGeass.h"

int i;
static int tmpOffset = 0;
Stack MyStack;

static void cGen( TreeNode * tree);
static void genExpK( TreeNode * t);

char *ObterID(TreeNode *t)
{
    char *buffer;
    switch (t->kind.exp)
    {
      case IdK:
          return t->attr.name;
      break;

      case ConstK:
          buffer = (char*)malloc(5*sizeof(char));
          sprintf( buffer, "%d", t->attr.val );
          return buffer;
      break;

      case CalcK:
          buffer = (char*)malloc(5*sizeof(char));
          int Reg = MyStack.Reg[MyStack.top];
          sprintf( buffer, "%d", Reg);
          genExpK(t);
          return buffer;
      break;
    }
}

char *ObterOP(TreeNode *tree)
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
        default: printf("other OpK\n");
        return buffer;
    }
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
        case CallK:
                args = 0;
                for(p1=t->child[0];p1!=NULL;p1 = p1->sibling){ cGen(p1); args++; }
        break;
        case CalcK:

        lockStack(&MyStack);
        A = ObterID(t->child[0]);
        B = ObterOP(t->child[1]);
        C = ObterID(t->child[2]);
        UnlockStack(&MyStack);

        if(A!=NULL && B!=NULL && C!=NULL)
        {
            printf("%d <- %s %s %s\n",MyStack.Reg[MyStack.top],A,B,C);
        }
        //free(A); free(B); free(C);
        Pop(&MyStack);

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

         // ELSE
         cGen(p2);
         cGen(p3);

          break;
          case WhileK:      break;
          case AssignK:     for (i=0; i < MAXCHILDREN; i++){ cGen(tree->child[i]); } break;
          case CompoundK:   for (i=0; i < MAXCHILDREN; i++){ cGen(tree->child[i]); } break;
          case ReturnK:
                  cGen(tree->child[0]); // Invoca Jump Register
          break;
          default: printf("Você me esqueceu parça stmt!\n"); break;
      }
}

static void genDeclK( TreeNode * t)
{
      switch (t->kind.decl)
      {
            case ArrParamK:  break; // Util na atribuição de Registradores Gerais
            case ParamK:     break; // Util na atribuição de Registradores Gerais
            case ArrVarK:    break; // Util na atribuição de Registradores Temporários
            case VarK:       break; // Util na atribuição de Registradores Temporários
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

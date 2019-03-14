#include "Globals.h"
#include "SymbolTab.h"
#include "cgen.h"


int i;
static int tmpOffset = 0;
static void cGen( TreeNode * tree);

static void genExpK( TreeNode * t)
{
    int loc,args;
    TreeNode *p1, *p2;
    switch (t->kind.exp)
    {
        case OpK:

         p1 = t->child[0];
         p2 = t->child[1];
         cGen(p1);

         cGen(p2);

         switch (t->attr.op)
         {
           case PLUS : printf("+\n"); break;
           case MINUS: printf("-\n"); break;
           case STAR : printf("*\n"); break;
           case BAR  : printf("/\n"); break;
           case SLT  : printf("SLT\n"); break;
           case SLTE : printf("SLTE\n"); break;
           case SBT  : printf("SBT\n"); break;
           case SBTE : printf("SBTE\n"); break;
           case EQUAL: printf("==\n"); break;
           case DIFF : printf("!=\n"); break;
         }

        break;

        case ConstK:

        break;

        case IdK:

        break;
        case TypeK:   break;
        case ArrIdK:  break;
        case CallK:

                args = 0;
                for(p1=t->child[0];p1!=NULL;p1 = p1->sibling)
                {
                    cGen(p1);
                    args++;
                }

        break;
        case CalcK: for (i=0; i < MAXCHILDREN; i++){ genExpK(t->child[i]); } break;
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

         cGen(p1);
         cGen(p2);
         cGen(p3);


          break;
          case WhileK:      for (i=0; i < MAXCHILDREN; i++){ cGen(tree->child[i]); } break;
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
    cGen(syntaxTree);
}

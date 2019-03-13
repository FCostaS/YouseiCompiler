#include "Globals.h"
#include "SymbolTab.h"
#include "cgen.h"

int i;
static int MemoryIndex = 0;
static void cGen( TreeNode * tree , TreeNode * pai);

void emitComment( char * c )
{
    if (Intermediary){ fprintf(listing,"* %s\n",c); }
}

static void genExpK( TreeNode * t, TreeNode * tpai)
{
    int loc,args;
    TreeNode * p1, * p2;
    switch (t->kind.exp)
    {
        case OpK:

         p1 = t->child[0];
         p2 = t->child[1];
         cGen(p1,t);
         cGen(p2,t);

         switch (t->attr.op)
         {
           case PLUS : printf("* ADD"); break;
           case MINUS: printf("* SUB"); break;
           case STAR : printf("* MUL"); break;
           case BAR  : printf("* DIV"); break;
           case SLT  : printf("* SLT"); break;
           case SLTE : printf("* SLTE"); break;
           case SBT  : printf("* SBT"); break;
           case SBTE : printf("* SBTE"); break;
           case EQUAL: printf("* EQUAL"); break;
           case DIFF : printf("* DIFF"); break;
         }

        break;

        case ConstK:
        break;

        case IdK:

        //loc = st_lookup(t->attr.name);
        //printf("%s\n",t->attr.name);

        break;
        case TypeK:   break;
        case ArrIdK:  break;
        case CallK:

                args = 0;
                for(p1=t->child[0];p1!=NULL;p1 = p1->sibling)
                {
                    genExpK(p1,t);
                    args++;
                }
                printf("Call %s(%d)\n",t->attr.name,args);

        break;
        case CalcK: break;
        default: printf("Você me esqueceu parça!\n"); break;
    }

}

static void genStmtK( TreeNode * t, TreeNode * tpai)
{
      int loc,i;
      TreeNode *p1,*p2,*p3;
      int savedLoc1,savedLoc2,currentLoc;

      switch (t->kind.stmt)
      {
          case IfK:         break;
          case WhileK:      break;
          case AssignK:

          p1 = t->child[0];
          p2 = t->child[1];

          genExpK(p1,t);

          cGen(p2,t);

          break;
          case CompoundK:   for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i],t); } break;
          case ReturnK:     break; // Invoca Jump Register
          default: printf("Você me esqueceu parça!\n"); break;
      }
}

static void genDeclK( TreeNode * t, TreeNode * tpai)
{
      switch (t->kind.decl)
      {
            case ArrParamK:  break; // Util na atribuição de Registradores Gerais
            case ParamK:     break; // Util na atribuição de Registradores Gerais
            case ArrVarK:    break; // Util na atribuição de Registradores Temporários
            case VarK:       break; // Util na atribuição de Registradores Temporários
            case FunK:              // Lista as funções presentes no programa
                printf("%s:\n",t->attr.name);
                for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i],t); }
            break;
            default: printf("Você me esqueceu parça!\n"); break;
      }
}

static void cGen( TreeNode * tree , TreeNode * pai)
{
  if (tree != NULL)
  {
    switch (tree->nodekind) {
      case StmtK:
        genStmtK(tree,pai);
        break;
      case ExpK:
        genExpK(tree,pai);
        break;
      case DeclK:
        genDeclK(tree,pai);
        break;
    }
    cGen(tree->sibling,tree);
  }
}

void codeGen(TreeNode * syntaxTree, char * codefile){ cGen(syntaxTree,syntaxTree); }

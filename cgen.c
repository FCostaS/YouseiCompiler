#include "Globals.h"
#include "SymbolTab.h"
#include "cgen.h"

static void cGen( TreeNode * tree);

void emitComment( char * c )
{
    if (Intermediary)
    {
      fprintf(listing,"* %s\n",c);
    }
}

void emitCommentFunk( char * c )
{
    if (Intermediary)
    {
      fprintf(listing,"%s:\n",c);
    }
}


static void genExpK( TreeNode * t)
{
    int loc,i;
    TreeNode * p1, * p2;
    switch (t->kind.exp)
    {
        case OpK:

         emitComment("-> Op");
         p1 = t->child[0];
         p2 = t->child[1];
         cGen(p1);
         cGen(p2);

         switch (t->attr.op)
         {
           case PLUS : emitComment("ADD"); break;
           case MINUS: emitComment("SUB"); break;
           case STAR : emitComment("MUL"); break;
           case BAR  : emitComment("DIV"); break;
         }

        break;

        case ConstK:
             emitComment("<- ConstK") ;
        break;

        case IdK:

        loc = st_lookup(t->attr.name);
        emitComment(t->attr.name) ;

        break;
        case TypeK:   emitComment("<- TypeK"); for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); } break;
        case ArrIdK:  emitComment("<- ArrIdK"); for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]);} break;
        case CallK:   for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); } printf("* Call %s\n",t->attr.name);  break;
        case CalcK:   for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); } break;
        default: printf("Você me esqueceu parça!\n"); break;
    }

}

static void genStmtK( TreeNode * t)
{
      int loc,i;
      TreeNode *p1,*p2,*p3;
      int savedLoc1,savedLoc2,currentLoc;

      switch (t->kind.stmt)
      {
          case IfK:
              p1 = t->child[0] ;
              p2 = t->child[1] ;
              p3 = t->child[2] ;

              cGen(p1);

              //savedLoc1 = emitSkip(1) ;
              emitComment("IF");

              /* recurse on then part */

              cGen(p2);
              //savedLoc2 = emitSkip(1) ;
              emitComment("ELSE");
              //currentLoc = emitSkip(0) ;
              //emitBackup(savedLoc1) ;
              //emitRM_Abs("JEQ",ac,currentLoc,"if: jmp to else");
              //emitRestore() ;

              cGen(p3);
              //currentLoc = emitSkip(0) ;
              //emitBackup(savedLoc2) ;
              //emitRM_Abs("LDA",pc,currentLoc,"jmp to end") ;
              //emitRestore() ;
          break;
          case WhileK:    break;
          case AssignK:

          p1 = t->child[0]; p2 = t->child[1];
          cGen(p1); cGen(p2);
          printf("* %s = %s\n",p1->attr.name,p2->attr.name);

          break;
          case CompoundK:
                for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); }
          break;
          case ReturnK:

            for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); }
            emitComment("JR");

          break; // Invoca Jump Register
          default: printf("Você me esqueceu parça!\n"); break;
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
            case FunK:              // Lista as funções presentes no programa
                emitCommentFunk(t->attr.name);
                int i;
                for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); }
            break;
            default: printf("Você me esqueceu parça!\n"); break;
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
    cGen(syntaxTree);
}

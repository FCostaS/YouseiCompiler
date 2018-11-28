#include "Globals.h"

void StartFunctionList(FunctionsProgram *F){ F->iterator = 0; }

void InsertFunctionList(FunctionsProgram *F,char *name)
{
    strcpy(F->ListFunctions[F->iterator],name);
    F->iterator++;
}

void printToken( TokenType token, const char* tokenString )
{
  switch (token)
  {
    case ELSE:
    case IF:
    case INT:
    case RETURN:
    case VOID:
    case WHILE:       fprintf(listing, "reserved word: %s\n",tokenString); break;
    case PLUS:        fprintf(listing, "+\n");  break;
    case MINUS:       fprintf(listing, "-\n");  break;
    case STAR:        fprintf(listing, "*\n");  break;
    case BAR:         fprintf(listing, "/\n");  break;
    case SLT:         fprintf(listing, "<\n");  break;
    case SLTE:        fprintf(listing, "<=\n"); break;
    case SBT:         fprintf(listing, ">\n");  break;
    case SBTE:        fprintf(listing, ">=\n"); break;
    case EQUAL:       fprintf(listing, "==\n"); break;
    case DIFF:        fprintf(listing, "!=\n"); break;
    case ATRIB:       fprintf(listing, "=\n");  break;
    case POINTVIG:    fprintf(listing, ";\n");  break;
    case VIG:         fprintf(listing, ",\n");  break;
    case OPPAR:       fprintf(listing, "(\n");  break;
    case CLPAR:       fprintf(listing, ")\n");  break;
    case OPARR:       fprintf(listing, "[\n");  break;
    case CLARR:       fprintf(listing, "]\n");  break;
    case OPCHA:       fprintf(listing, "{\n");  break;
    case CLCHA:       fprintf(listing, "}\n");  break;
    case NUM:         fprintf(listing, "NUM, val = %s\n",tokenString); break;
    case ID:          fprintf(listing, "ID, name = %s\n",tokenString); break;
    case ERRO:        fprintf(listing, "ERROR: %s\n",tokenString); break;
    default:          fprintf(listing,"Unknown token: %d\n",token);
  }
}

static int indentno = 0;
/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void) {
  int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* printType print types of funcions and variables */
void printTypes(TreeNode* tree)
{
  if (tree->child[0] != NULL) {
    switch (tree->child[0]->type)
    {
      case Integer:
        fprintf(listing,"int");
        break;
      case Void:
        fprintf(listing,"void");
        break;
      case IntegerArray:
        fprintf(listing,"int array");
        break;
      default: return;
    }
  }
  else
  {
    switch (tree->type) {
      case Integer:
        fprintf(listing,"int");
        break;
      case Void:
        fprintf(listing,"void");
        break;
      case IntegerArray:
        fprintf(listing,"int array");
        break;
      default: return;
    }
  }
}

/* procedure printTree prints a syntax tree to the listing file using indentation to indicate subtrees */
void printTree( TreeNode * tree )
{
  int i;
  INDENT;
  while (tree != NULL) {

    // IfK, WhileK, CompoundK, ReturnK
    if (tree->nodekind==StmtK) {
      printSpaces();
      switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case WhileK:
          fprintf(listing, "While\n");
          break;
        case CompoundK:
          fprintf(listing, "Compound statement\n");
          break;
        case ReturnK:
          fprintf(listing, "Return\n");
          break;
        case AssignK:
          fprintf(listing, "Assign\n");
          break;
        default:
          fprintf(listing, "Unknown stmtNode kind\n");
          break;
        }
    }
    // OpK, ConstK, AssignK, IdK, TypeK, ArrIdK, CallK, CalK
    else if (tree->nodekind==ExpK) {
      if (tree->kind.exp != TypeK) printSpaces();
      switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op, "\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        case TypeK: break;
        case ArrIdK:
          fprintf(listing,"ArrId \n");
          break;
        case CallK:
          fprintf(listing, "Call Function : %s\n", tree->attr.name);
          break;
        case CalcK:
          fprintf(listing, "Operator : ");
          printToken(tree->child[1]->attr.op, "\0");
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    // VarK, FunK, ArrVarK, ArrParamK, ParamK
    else if (tree->nodekind==DeclK) {
      printSpaces();
      switch (tree->kind.decl) {
        case FunK :
          fprintf(listing, "Function Declaration:  ");
          printTypes(tree);
          fprintf(listing, " %s()\n", tree->attr.name);
          break;
        case VarK :
          fprintf(listing, "Variable Declaration:  ");
          printTypes(tree);
          fprintf(listing, " %s;\n", tree->attr.name);
          break;
        case ArrVarK :
          fprintf(listing, "Array Variable Declaration:  ");
          printTypes(tree);
          fprintf(listing," %s[%d];\n", tree->attr.arr.name, tree->attr.arr.size);
          break;
        case ArrParamK :
          fprintf(listing, "Array Parameter: %s\n", tree->attr.name);
          break;
        case ParamK :
          fprintf(listing, "Parameter: ");
          printTypes(tree);
          if (tree->attr.name != NULL) {
            fprintf(listing, " %s\n", tree->attr.name);
          } else {
            fprintf(listing, " void\n");
          }
          break;
        default:
          fprintf(listing, "Unknown Declaration\n");
          break;
        }
      }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++) if (tree->child[i] != NULL) printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}

char *copyString(char * s)
{
  int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

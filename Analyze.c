#include "Globals.h"
#include "SymbolTab.h"
#include "Analyze.h"
#include "Semantics.h"

/* counter for variable memory locations */
static int location = 0;

Escopo NovoEscopo(Escopo atual,char name[], int type,int lineno)
{
    Escopo new = (Escopo)malloc(sizeof(Escopo));
    hashTable = (BucketList*)malloc(SIZE*sizeof(BucketList));
    int i;
    for(i=0;i<SIZE;i++){ hashTable[i] = NULL; }
    if(atual!=NULL){ atual->next = new; }
      else{ new->next = NULL; }
    new->hashTable = hashTable;
    new->nameEscopo = copyString(name);
    new->typeEscopo = type;
    new->lineno = lineno;
    new->next = NULL;
    return new;
}

void Init_EscopoGlobal()
{
      Programa = NovoEscopo(NULL,"Global",-1,0);
      EscopoAtual = Programa;
}

static void traverse( TreeNode * t, void (* preProc) (TreeNode *), void (* postProc) (TreeNode *) )
{
  if (t != NULL)
  {
    preProc(t);
    {
      int i;
      for (i=0; i < MAXCHILDREN; i++){ traverse(t->child[i],preProc,postProc); }
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

static void insertNode( TreeNode * t)
{
  switch (t->nodekind) // Escolho o tipo da variavel para analisar
  {
      case StmtK:
      {
          switch (t->kind.stmt)
          {
              case IfK:       break; // Nada a fazer
              case WhileK:    break; // Nada a fazer
              case AssignK:   break; // Nada a fazer
              case CompoundK: break; // Nada a fazer
              case ReturnK:   break; // Nada a fazer
          }
      }
      break;

      case ExpK:
      {
          switch (t->kind.exp)
          {
              case OpK:     break; // Nada a fazer
              case ConstK:  break; // Nada a fazer
              case IdK:     st_insert(t->attr.name, t->lineno, location++); break; // Variável não declarada
              case TypeK:   break; // Nada a fazer
              case ArrIdK:  break; // Nada a fazer
              case CallK:   st_insert(t->attr.name, t->lineno, location++); break; // Função foi invocada por a, mas retorn void
              case CalcK:   break; // Nada a fazer
          }
      }
      break;

      case DeclK:
      {
          switch (t->kind.decl)
          {
              case VarK: // Tipo da declarada é void | Variável já foi declarada
                    st_insert(t->attr.name, t->lineno, location++);
              break;

              case FunK:
                  EscopoAtual = NovoEscopo(EscopoAtual,t->attr.name,t->child[0]->type,t->lineno);
              break;

              case ArrVarK: // Tipo da declarada é void | Variável já foi declarada
                    st_insert(t->attr.arr.name, t->lineno, location++);
              break;

              case ArrParamK:
                    //if (t->attr.name != NULL) printf("%s %s\n",t->attr.name);
                    st_insert(t->attr.name, t->lineno,location++);
              break;

              case ParamK:
                    if (t->attr.name != NULL){ st_insert(t->attr.name, t->lineno, location++); }
              break;
          }
      }
      break;
  }
}

static void nullProc(TreeNode * t)
{
  if (t==NULL) return;
  else return;
}

void buildSymtab(TreeNode * syntaxTree)
{
    Init_EscopoGlobal();
    traverse(syntaxTree,insertNode,nullProc);
    printSymTab(listing);
}

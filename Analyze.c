#include "Globals.h"
#include "SymbolTab.h"
#include "Analyze.h"
#include "Semantics.h"

/* counter for variable memory locations */
static int location = 0;
int Attrib = 0;
Escopo Interator;
BucketList *temp;
int TemMain = 0; // Considera que não existe main
int iteradorINT;

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
    location = 0;
    return new;
}

void Init_EscopoGlobal()
{
      Programa = NovoEscopo(NULL,"Global",Global,0);
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
              case IfK:       Attrib = 0; break; // Nada a fazer
              case WhileK:    Attrib = 0; break; // Nada a fazer
              case AssignK:   Attrib = 1; break; // Nada a fazer
              case CompoundK: Attrib = 0; break; // Nada a fazer
              case ReturnK:   Attrib = 0; break; // Nada a fazer
          }
      }
      break;

      case ExpK:
      {
          switch (t->kind.exp)
          {
              case OpK:     Attrib = 0; break; // Nada a fazer
              case ConstK:  Attrib = 0; break; // Nada a fazer
              case IdK:                        // Tratando Erro Tipo 1

              if( st_lookup(t->attr.name) == -1 )     // Procuro a variável na tabela
              {
                  temp = hashTable;       // Guardo a hash atual
                  hashTable = Programa->hashTable;    // Pego a hash Global
                  // Procuro a variável na hash global
                  if( st_lookup(t->attr.name) == -1 ){ ErrorType(t,1,EscopoAtual->nameEscopo,EscopoAtual->lineno); }
                  hashTable = temp; // Atualizo para hash atual
              }
                // Considera que a variável já foi declarada, não precisa inserir tipo na tabela (Default)
                else{ st_insert(t->attr.name, t->lineno, location++, Default); }

              break;
              case TypeK:   Attrib = 0; break; // Nada a fazer
              case ArrIdK:

              if( st_lookup(t->attr.name) == -1 )     // Procuro a variável na tabela
              {
                  temp = hashTable;       // Guardo a hash atual
                  hashTable = Programa->hashTable;    // Pego a hash Global
                  // Procuro a variável na hash global
                  if( st_lookup(t->attr.name) == -1 ){ ErrorType(t,1,EscopoAtual->nameEscopo,EscopoAtual->lineno); }
                  hashTable = temp; // Atualizo para hash atual
              }
                // Considera que a variável já foi declarada, não precisa inserir tipo na tabela (Default)
                else{ st_insert(t->attr.name, t->lineno, location++, Default); }

              break; // Nada a fazer
              case CallK:  // Tratando Erro tipo 2 e tipo 5

              // Chamada de função não declarada
              Interator = Programa;
              ExpType type;
              int IO = 0;
              if( (strcmp(t->attr.name,"input")==0) || (strcmp(t->attr.name,"output")==0))
              {
                  IO = 1;
              }
              while(Interator != NULL)
              {
                  if(strcmp(t->attr.name,Interator->nameEscopo)==0) // Procurando a função invocada na lista de funções
                  {
                      type = Interator->typeEscopo;
                      break; // Encontrou a função
                  }
                    else{ Interator = Interator->next; }
              }
              if( (Interator == NULL) && (IO == 0) )
              {
                  ErrorType(t,5,EscopoAtual->nameEscopo,EscopoAtual->lineno);
              }
                else
                {
                    // Função foi invocada por a, mas retorn void
                    if( (Attrib == 1) && (type == Void) )
                    {
                        ErrorType(t,2,EscopoAtual->nameEscopo,EscopoAtual->lineno);
                    }
                    st_insert(t->attr.name, t->lineno, location++, Funct);
                }

              break;

              case CalcK:  break; // Nada a fazer
          }
      }
      break;

      case DeclK:
      {
          Attrib = 0;
          switch (t->kind.decl)
          {
              case ArrParamK:
                    AnalyzeErrosDecl(t);
                    st_insert(t->attr.name, t->lineno,location++, IntArray);
              break;

              case ParamK:
                    if (t->attr.name != NULL)
                    {
                        AnalyzeErrosDecl(t);
                        st_insert(t->attr.name, t->lineno, location++, Int);
                    }
              break;

              case ArrVarK: // Tipo da declarada é void | Variável já foi declarada
                    AnalyzeErrosDeclArray(t);
                    st_insert(t->attr.arr.name, t->lineno, location++, IntArray);
              break;

              case VarK: // Tratando erro tipo 3 e 4 e 7.1
                    AnalyzeErrosDecl(t);
                    st_insert(t->attr.name, t->lineno, location++, Int);
              break;

              case FunK: // Tratando erro tipo 3 e 4 e 7.2

                  // Tentando declarar uma função que tem nome de variável global
                  temp = hashTable;       // Guardo a hash atual
                  hashTable = Programa->hashTable;    // Pego a hash Global
                  if( st_lookup(t->attr.name) != -1 ) // Procuro a variável na hash global
                  { ErrorType(t,7,t->attr.name,t->lineno); }
                  hashTable = temp;                   // Atualizo para hash atual

                  EscopoAtual = NovoEscopo(EscopoAtual,t->attr.name,t->child[0]->type,t->lineno);
                  if( strcmp(t->attr.name,"main") == 0){ TemMain = 1; }
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
    if(TemMain==0){ ErrorType(syntaxTree,6,EscopoAtual->nameEscopo,location); }
    if(TSymbol == TRUE){ printSymTab(listing); }
}

void AnalyzeErrosDecl(TreeNode * t)
{
      // Tentando declarar variável como void
      if (t->child[0]->type == Void)
      { ErrorType(t,3,EscopoAtual->nameEscopo,EscopoAtual->lineno); }

      // Tentando redeclarar uma variável
      if( st_lookup(t->attr.name) != -1 )
      {
          ErrorType(t,4,EscopoAtual->nameEscopo,location);
          //break;
      }

      // Tentando declarar uma variável que já recebeu nome de função
      for(iteradorINT=0;iteradorINT < FList.iterator;iteradorINT++)
      {
          if(strcmp(t->attr.name,FList.ListFunctions[iteradorINT])==0)
          {
              ErrorType(t,7,t->attr.name,t->lineno);
          }
      }
}

void AnalyzeErrosDeclArray(TreeNode * t)
{
      // Tentando declarar variável como void
      if (t->child[0]->type == Void)
      { ErrorType(t,3,EscopoAtual->nameEscopo,EscopoAtual->lineno); }

      // Tentando redeclarar uma variável
      if( st_lookup(t->attr.arr.name) != -1 ) // Se eu encontra  a variável na tabela
      {
          ErrorType(t,4,EscopoAtual->nameEscopo,location);
          //break;
      }

      // Tentando declarar uma variável que já recebeu nome de função
      for(iteradorINT=0;iteradorINT < FList.iterator;iteradorINT++)
      {
          if(strcmp(t->attr.arr.name,FList.ListFunctions[iteradorINT])==0)
          {
              ErrorType(t,7,t->attr.arr.name,t->lineno);
          }
      }
}
//if (t->attr.name != NULL) printf("%s %s\n",t->attr.name);

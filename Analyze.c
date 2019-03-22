#include "Globals.h"
#include "SymbolTab.h"
#include "Analyze.h"
#include "Semantics.h" /* Biblioteca com as mensagens dos tipos de erros */

/* counter for variable memory locations */
static int location = 0;
static int Parameters = 0;

/* Attrib = Flag para detectar atribuição, usarei para saber se o token anterior a
chamada de função foi uma atribuição, se sim e a função for void eu devo emitir
o erro tipo 2 */
int Attrib = 0;
Escopo Interator;
BucketList *temp;
/* TemMain = Flag para detectar o main no programa | Considera que não existe main
Se durante a montagem da tabela a função main for encontrada, ela será
setada para 1, nesse caso, ao fim da monstagem da tabela, se ela continuar 0
é porque não encontrou (...) */
int TemMain = 0;
int iteradorINT;

/* NovoEscopo cria um novo nó para lista encadeada de funções */
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
    new->param = 0;
    new->next = NULL;
    //location = 0;
    return new;
}

void Init_EscopoGlobal()
{
      /* Inicio o programa considerando que toda variável antes de uma
      declaração de função será global, assim que detectar uma nova função,
      a lista encadeada receberá no novo nó e então as próximas funções irá
      para tabela específica da tabela dessa função */
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
              case CompoundK: Attrib = 0; EscopoAtual->param = Parameters; break; // Nada a fazer
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
                else{ st_insert(t->attr.name, t->lineno, location, Default); }

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
                else{ st_insert(t->attr.name, t->lineno, location, Default); }

              break; // Nada a fazer
              case CallK:  // Tratando Erro tipo 2 e tipo 5

              // Chamada de função não declarada
              Interator = Programa;
              ExpType type;
              int IO = 0;
              if( (strcmp(t->attr.name,"input")==0) )
              {
                  IO = 1;
              }
              else if( strcmp(t->attr.name,"output")==0 )
              {
                  IO = 2;
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

                    VarType k;
                    switch(type)
                    {
                        case Void:
                            k = FunctVoid;
                        break;
                        case Integer:
                            k = FunctInt;
                        break;
                        case IntegerArray:
                            k = FunctIntArray;
                        break;
                    }

                    if(IO == 1){ k = FunctInt; }
                    else if( IO == 2){ k = FunctVoid; }
                    if( st_lookup(t->attr.name) == -1 )
                    { st_insert(t->attr.name, t->lineno, location++, k); }
                      else { st_insert(t->attr.name, t->lineno, location, k);}

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
                    Parameters++;
                    AnalyzeErrosDecl(t,0);
                    st_insert(t->attr.name, t->lineno,location, IntArray);
                    location++;
              break;

              case ParamK:
                    if (t->attr.name != NULL) /* Impede verificação de no void */
                    {
                        Parameters++;
                        AnalyzeErrosDecl(t,0);
                        st_insert(t->attr.name, t->lineno, location, Int);
                        location++;
                    }
              break;

              case ArrVarK: // Tipo da declarada é void | Variável já foi declarada
                    AnalyzeErrosDecl(t,1);
                    st_insert(t->attr.arr.name, t->lineno, location, IntArray);
                    location+=t->attr.arr.size;
              break;

              case VarK: // Tratando erro tipo 3 e 4 e 7.1
                    AnalyzeErrosDecl(t,0);
                    st_insert(t->attr.name, t->lineno, location, Int);
                    location++;
              break;

              case FunK: // Tratando erro tipo 3 e 4 e 7.2
                  Parameters = 0;

                  // Tentando declarar uma função que tem nome de variável global
                  temp = hashTable;       // Guardo a hash atual
                  hashTable = Programa->hashTable;    // Pego a hash Global
                  if( st_lookup(t->attr.name) != -1 ) // Procuro a variável na hash global
                  { ErrorType(t,7,t->attr.name,t->lineno); }
                  hashTable = temp;                   // Atualizo para hash atual

                  if( strcmp(t->attr.name,"input") == 0 || strcmp(t->attr.name,"output") == 0 )
                  {
                      ErrorType(t,8,t->attr.name,t->lineno);
                  }

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
    Init_EscopoGlobal(); /* Inicio a lista encadeada já definindo o primeiro nó como global */
    traverse(syntaxTree,insertNode,nullProc); /* Peguei do Tiny sem alterar */
    if(TemMain==0){ ErrorType(syntaxTree,6,EscopoAtual->nameEscopo,location); }
    if(TSymbol == TRUE){ printSymTab(listing); }
}

/* As funções AnalyzeErrosDecl e  AnalyzeErrosDeclArray são as mesmas, com exceção
da forma que o nome do token é acessado, uma diz respeito ao tratamento do problema
quando a variável é normal e a outra quando é Array. */

void AnalyzeErrosDecl(TreeNode * t,int TYPE)
{
      // Tentando declarar variável como void
      if (t->child[0]->type == Void)
      { ErrorType(t,3,EscopoAtual->nameEscopo,EscopoAtual->lineno); }

      // Tentando redeclarar uma variável
      if( TYPE == 1 ) // É tipo array
      {
          if( st_lookup(t->attr.arr.name) != -1 ) // Se eu encontra  a variável na tabela
          {
              ErrorType(t,4,EscopoAtual->nameEscopo,location);
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
      else
      {
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
}

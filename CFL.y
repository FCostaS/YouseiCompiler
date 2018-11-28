/* DECLARACOES */
%{
    #define YYPARSER           /* Distingue as variaveis de saida do Yacc das dos demais codigos */
    #include "Globals.h"
    #include <string.h>
    #define YYSTYPE TreeNode*   /* Definindo tipo do valor semantico como no da arvore sintatica */

    static char *savedName;     /* for use in assignments */
    static TreeNode *savedTree; /* Arvore Sintatica Final */

    extern char tokenString[MAXTOKENLEN+1];
    static int yylex(void);     /* Definindo um novo yylex para nao conflitar com o gerado pelo flex */
    int yyerror(char * message);
%}

%start programa
%token ELSE IF INT RETURN VOID WHILE PLUS MINUS
%token STAR BAR SLT SLTE SBT SBTE EQUAL DIFF ATRIB
%token POINTVIG VIG OPPAR CLPAR OPARR CLARR OPCHA CLCHA
%token COMMIT NUM ID ENDLINE SPACE MULTI DIVI
%token ERRO

/* Ignora um conflito de ambiguidade, provocado pela regra do IF ELSE (selecao_decl) */
%expect 1

/* REGRAS DE PRODUCAO */
%%
programa:	          declaracao_lista
                    {
                        savedTree = $1;
                    }
                    ;

declaracao_lista:   declaracao_lista declaracao
                    {
                         YYSTYPE t = $1;
                         if (t != NULL) // NÃO É O PRIMEIRO IRMÃO A INSERIR
                         {
                           while (t->sibling != NULL)
                           {
                              t = t->sibling;
                           }
                           t->sibling = $2;
                           $$ = $1;
                         }
                             else // PRIMEIRO IRMÃO A INSERIR
                             {
                               $$ = $2;
                             }
                    }
                    | declaracao
                      {
                          $$ = $1;
                      }
                    ;

declaracao:         var_declaracao
                    {
                        $$ = $1;
                    }
                    | fun_declaracao
                      {
                        $$ = $1;
                      }
                    ;

id :                ID
                    {
                          savedName = copyString(tokenString);
                    }
                    ;

num :
                    NUM
                    {
                          savedNumber = atoi(tokenString);
                    }

var_declaracao:     tipo_espicificador id POINTVIG
                    {
                        $$ = newNode(VarK,1); /* O no é uma declaracao */
                        $$->child[0] = $1;
                        $$->lineno = lineno;
                        $$->attr.name = savedName;
                    }
                    | tipo_espicificador id OPARR num CLARR POINTVIG
                    {
                        $$ = newNode(ArrVarK,1); /* O no é uma declaracao */
                        $$->child[0] = $1;
                        $$->lineno = lineno;
                        $$->type = IntegerArray;
                        $$->attr.arr.name = savedName;
                        $$->attr.arr.size = savedNumber;
                    }
                    ;

tipo_espicificador: INT
                    {
                        $$ = newNode(TypeK,0); /* O no é uma expressão */
                        $$->type = Integer;
                    }
                    | VOID
                    {
                        $$ = newNode(TypeK,0); /* O no é uma expressão */
                        $$->type = Void;
                     }
                    ;

fun_declaracao:     tipo_espicificador id
                    {
                       $$ = newNode(FunK,1);
                       $$->lineno = lineno;
                       $$->attr.name = savedName;
                       InsertFunctionList(&FList,savedName);
                    }
                    OPPAR params CLPAR composto_decl
                    {
                        $$ = $3;
                        $$->child[0] = $1;
                        $$->child[1] = $5;
                        $$->child[2] = $7;
                    }
                    ;

params:             param_lista
                    {
                        $$ = $1;
                    }
                    | VOID
                    {
                        $$ = newNode(ParamK,1); /* O no é uma declaracao */
                        $$->type = Void;
                     }
                    ;

param_lista:        param_lista VIG param
                    {
                       YYSTYPE t = $1;
                       if (t != NULL) {
                         while (t->sibling != NULL) { t = t->sibling; }
                         t->sibling = $3;
                         $$ = $1;
                       } else {
                         $$ = $2;
                       }
                    }
                    | param
                    {
                        $$ = $1;
                    }
                    ;

param:              tipo_espicificador id
                    {
                        $$ = newNode(ParamK,1);
                        $$->child[0] = $1;
                        $$->attr.name = savedName;
                    }
                    | tipo_espicificador id OPARR CLARR
                    {
                        $$ = newNode(ArrParamK,1);
                        $$->child[0] = $1;
                        $$->attr.name = copyString(savedName);
                    }
                    ;

composto_decl:      OPCHA local_declaracoes statement_lista CLCHA
                    {
                        $$ = newNode(CompoundK,2);
                        $$->child[0] = $2;
                        $$->child[1] = $3;
                    }
                    ;

local_declaracoes:  local_declaracoes var_declaracao
                    {
                        YYSTYPE t = $1;
                        if (t != NULL)
                        {
                            while (t->sibling != NULL) { t = t->sibling; }
                            t->sibling = $2;
                            $$ = $1;
                        }
                          else
                          {
                            $$ = $2;
                          }
                     }
                    | {
                          $$ = NULL;
                      }
                    ;

statement_lista:    statement_lista statement
                    {
                        YYSTYPE t = $1;
                        if (t != NULL)
                        {
                            while (t->sibling != NULL)
                            {
                                t = t->sibling;
                            }
                            t->sibling = $2;
                            $$ = $1;
                        }
                          else
                          {
                            $$ = $2;
                          }
                    }
                    | {
                          $$ = NULL;
                      }
                    ;

statement:          expressao_decl
                    {
                        $$ = $1;
                    }
                    | composto_decl
                    {
                        $$ = $1;
                    }
                    | selecao_decl
                    {
                        $$ = $1;
                    }
                    | iteracao_decl
                    {
                        $$ = $1;
                    }
                    | retorno_decl
                    {
                        $$ = $1;
                    }
                    ;

expressao_decl:     expressao POINTVIG
                    {
                        $$ = $1;
                    }
                    | POINTVIG
                    {
                        $$ = NULL;
                    }
                    ;

selecao_decl:       IF OPPAR expressao CLPAR statement
                    {
                        $$ = newNode(IfK,2);
                        $$->child[0] = $3;
                        $$->child[1] = $5;
                    }
                    | IF OPPAR expressao CLPAR statement ELSE statement
                    {
                        $$ = newNode(IfK,2);
                        $$->child[0] = $3;
                        $$->child[1] = $5;
                        $$->child[2] = $7;
                    }
                    ;

iteracao_decl:      WHILE OPPAR expressao CLPAR statement
                    {
                        $$ = newNode(WhileK,2);
                        $$->child[0] = $3;
                        $$->child[1] = $5;
                    }
                    ;

retorno_decl:       RETURN POINTVIG
                    {
                        $$ = newNode(ReturnK,2);
                        $$->type = Void;
                    }
                    | RETURN expressao POINTVIG
                    {
                        $$ = newNode(ReturnK,2);
                        $$->child[0] = $2;
                    }
                    ;

expressao:          var ATRIB expressao
                    {
                        $$ = newNode(AssignK,2);
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                    }
                    | simples_expressao
                    {
                        $$ = $1;
                    }
                    ;

var:                id
                    {
                        $$ = newNode(IdK,0);
                        $$->attr.name = savedName;
                    }
                    | id
                    {
                        $$ = newNode(ArrIdK,0);
                        $$->attr.name = savedName;
                    }
                    OPARR expressao CLARR
                    {
                        $$ = $2;
                        $$->child[0] = $4;
                    }
                    ;

simples_expressao:  soma_expressao relacional soma_expressao
                    {
                        $$ = newNode(CalcK,0);
                        $$->child[0] = $1;
                        $$->child[1] = $2;
                        $$->child[2] = $3;
                    }
                    | soma_expressao
                    {
                        $$ = $1;
                    }
                    ;

relacional:         SLT
                    {
                      $$ = newNode(OpK,0); /* O no é uma expressão */
                      $$->attr.op = SLT;
                    }
                    | SLTE
                    {
                      $$ = newNode(OpK,0); /* O no é uma expressão */
                      $$->attr.op = SLTE;
                    }
                    | SBT
                    {
                      $$ = newNode(OpK,0); /* O no é uma expressão */
                      $$->attr.op = SBT;
                    }
                    | SBTE
                    {
                      $$ = newNode(OpK,0); /* O no é uma expressão */
                      $$->attr.op = SBTE;
                    }
                    | EQUAL
                    {
                      $$ = newNode(OpK,0); /* O no é uma expressão */
                      $$->attr.op = EQUAL;
                    }
                    | DIFF
                    {
                      $$ = newNode(OpK,0); /* O no é uma expressão */
                      $$->attr.op = DIFF;
                    }
                    ;

soma_expressao:     soma_expressao soma termo
                    {
                        $$ = newNode(CalcK,0);
                        $$->child[0] = $1;
                        $$->child[1] = $2;
                        $$->child[2] = $3;
                    }
                    | termo
                    {
                        $$ = $1;
                    }
                    ;

soma:               PLUS
                    {
                        $$ = newNode(OpK,0); /* O no é uma expressão */
                        $$->attr.op = PLUS;
                    }
                    | MINUS
                    {
                        $$ = newNode(OpK,0); /* O no é uma expressão */
                        $$->attr.op = MINUS;
                    }
                    ;

termo:              termo mult fator
                    {
                        $$ = newNode(CalcK,0);
                        $$->child[0] = $1;
                        $$->child[1] = $2;
                        $$->child[2] = $3;
                     }
                    | fator
                    {
                        $$ = $1;
                    }
                    ;

mult:               STAR
                    {
                        $$ = newNode(OpK,0);
                        $$->attr.op = STAR;
                    }
                    | BAR
                    {
                        $$ = newNode(OpK,0);
                        $$->attr.op = BAR;
                    }
                    ;

fator:              OPPAR expressao CLPAR
                    {
                        $$ = $2;
                    }
                    | var
                    {
                        $$ = $1;
                    }
                    | ativacao
                    {
                        $$ = $1;
                    }
                    | NUM
                    {
                        $$ = newNode(ConstK,0);
                        $$->type = Integer;
                        $$->attr.val = atoi(tokenString);
                    }
                    ;

ativacao:           id
                    {
                        $$ = newNode(CallK,0);
                        $$->attr.name = savedName;
                    }
                    OPPAR args CLPAR
                    {
                       $$ = $2;
                       $$->child[0] = $4;
                    }
                   ;

args:               arg_lista
                    {
                        $$ = $1;
                    }
                    |
                    {
                        $$ = NULL;
                    }
                    ;

arg_lista:          arg_lista VIG expressao
                    {
                        YYSTYPE t = $1;
                        if (t != NULL)
                        {
                          while (t->sibling != NULL)
                          {
                              t = t->sibling;
                          }
                          t->sibling = $3;
                          $$ = $1;
                        }
                          else
                          {
                             $$ = $3;
                          }
                   }
                    | expressao
                    {
                        $$ = $1;
                    }
                    ;
%%

int yyerror(char * message)
{
  fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: "); printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* Invoca o yylex criado pelo Bison e não pelo flex */
static int yylex(void){ return getToken(); }
TreeNode *parse(void){ yyparse(); return savedTree; }

/* Cria no para inserir na arvore */
TreeNode *newNode(DeclKind kind,int type)
{
    TreeNode *t = (TreeNode*) malloc(sizeof(TreeNode));
    int i;
    if (t==NULL){ fprintf(listing,"Out of memory error at line %d\n",lineno); }
    else
    {
        for (i=0;i<MAXCHILDREN;i++){ t->child[i] = NULL; }
        t->sibling = NULL;
        if( type == 1 ){ t->nodekind = DeclK; t->kind.decl = kind; }         /* O nó é uma declaracao */
          else if( type == 0 ){ t->nodekind = ExpK; t->kind.exp = kind; }    /* O nó é uma expressao */
            else{ t->nodekind = StmtK; t->kind.stmt = kind; }                /* O nó é uma statement */
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

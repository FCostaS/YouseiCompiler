#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER
#include "CFL.tab.h"
#define ENDFILE 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXTOKENLEN 40
typedef int TokenType;

// Variaveis da biblioteca CodigoFonte.h
extern FILE* source;  // Program que iremos compilar
extern FILE* listing; /* listing output text file */
extern FILE* code;    /* code text file for TM simulator */
extern int lineno;    /* Indica o número da linha daquele código */
extern int Error;     /* Flag que indica se houve algum erro */
extern int Lexical;

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum { StmtK, ExpK, DeclK } NodeKind;
typedef enum { IfK, WhileK, AssignK, CompoundK, ReturnK } StmtKind;
typedef enum { OpK, ConstK, IdK, TypeK, ArrIdK, CallK, CalcK } ExpKind;
typedef enum { VarK, FunK, ArrVarK, ArrParamK, ParamK } DeclKind; /* DeclKind chega o tipo de declaracao */
typedef enum { Void, Integer, IntegerArray, Global, GlobalArray } ExpType; /* ExpType chega o tipo de expressão da variavel */

#define MAXCHILDREN 3

typedef struct Array{
  TokenType type;
  char * name;
  int size;
} ArrAttr;

typedef struct treeNode {
  struct treeNode * child[MAXCHILDREN];
  struct treeNode * sibling;
  int lineno;
  NodeKind nodekind; // Tipo de variavel/token

  union {
   StmtKind stmt;
   ExpKind exp;
   DeclKind decl;
  } kind;

  union {
   TokenType op;
   int val;
   char *name;

   TokenType type;
   ArrAttr arr;
 } attr; // Atributos do no

  ExpType type;
} TreeNode;

//////////////////////////////////////////////
// PARSE FUNÇÕES
TreeNode *parse(void);
TreeNode *newNode(DeclKind kind,int type); /* Cria no da arvore */
void printTree(TreeNode * tree );
void printTypes(TreeNode* tree);
void printToken(TokenType token, const char* tokenString );
/////////////////////////////////////////////

// SCAN FUNÇÕES
TokenType getToken(void);
char *copyString(char * s);

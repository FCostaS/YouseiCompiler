#include <stdio.h>
#include "Globals.h"
#include "Analyze.h"

// Alocando variáveis globais
int lineno = 0;
int Error = FALSE;
FILE *source;
FILE *listing;
FILE *code;
FunctionsProgram FList;

// Flags
int Arvore  = TRUE;
int TSymbol = FALSE;
int Lexical = FALSE;

// Funcoes
void RunCompiler(char* codename);
void OpenCode(char *filename);
void CloseCode();

// Iniciando compilador
void RunCompiler(char* codename)
{
      OpenCode(codename);
      listing = stdout;
      StartFunctionList(&FList);
      TreeNode *syntax = parse();
      if(Error == FALSE) // Consegui montar a árvore
      {
        if(Arvore) // Árvore Sintática
        {
            printTree(syntax);
        }
        buildSymtab(syntax);
      }
      CloseCode();
}

void OpenCode(char *filename){ source = fopen(filename,"r"); } // Ler o codigo fonte que sera compilado
void CloseCode(){ fclose(source); } // Fecha o arquivo do codigo fonte

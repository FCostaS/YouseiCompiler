#include <stdio.h>
#include "Globals.h"
#include "Analyze.h"

// Alocando variáveis globais
int lineno = 0;
int Error = FALSE;
FILE *source;
FILE *listing;
FILE *code;

// FLAGS
int Arvore  = FALSE;
int TSymbol = TRUE;
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
      TreeNode *syntax = parse();
      if(Error == FALSE)
      {
        if(Arvore)
        { printf("Árvore Sintática\n"); printTree(syntax);} // Árvore Sintática
        buildSymtab(syntax);
        if(TSymbol)
        {}                // Tabela de Símbolos
      }

      CloseCode();
}

void OpenCode(char *filename){ source = fopen(filename,"r"); } // Ler o codigo fonte que sera compilado
void CloseCode(){ fclose(source); } // Fecha o arquivo do codigo fonte

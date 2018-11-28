#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Globals.h"
#include "SymbolTab.h"

// Função Hashing
int hash ( char *key )
{
    int temp =0;
    int i = 0;
    while( key[i] != '\0' )
    {
        temp = ( (temp << SHIFT) + key[i] )% SIZE;
        i++;
    }
    return temp;
}

char *TypeEscopo(ExpType Type)
{
    switch(Type)
    {
        case Void: return "Void";
        case Integer: return "Integer";
        case IntegerArray: return "Integer Array";
        default: return "Global";
    }
}

char *TypeVariable( VarType TYPE)
{
    switch(TYPE)
    {
        case Int:           return "Integer";
        case IntArray:      return "Integer Array";
        case Funct:         return "Function";
        case Default:       printf("Você esqueceu de definir mais tipos\n");
    }
}

/* Função st_lookup retorna o local da memória de uma variável ou -1 se não for encontrado */
int st_lookup ( char * name )
{
  int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return -1;
  else return l->memloc;
}

/* Insere numeros de linhas e localização da memória na tabela de símbolos */
void st_insert( char * name, int lineno, int loc ,VarType var)
{
  int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  {
    l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->DataType = var;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = hashTable[h];
    hashTable[h] = l;
  }
    else /* found in table, so just add line number */
    {
      LineList t = l->lines;
      while (t->next != NULL) t = t->next;
      t->next = (LineList) malloc(sizeof(struct LineListRec));
      t->next->lineno = lineno;
      t->next->next = NULL;
    }
} /* st_insert */

/* printSymTab */
void printSymTab(FILE * listing)
{
  Escopo index = Programa;
  while(index!=NULL)
  {
      int i;
      fprintf(listing,"Function: %s Type: %s Line: %d\n",index->nameEscopo,TypeEscopo(index->typeEscopo),index->lineno);
      fprintf(listing,"Variable Name  Type Variable  Location   Line Numbers\n");
      fprintf(listing,"-------------  -------------  --------   ------------\n");
      for (i=0;i<SIZE;++i)
      {
        if (index->hashTable[i] != NULL)
        {
          BucketList l = index->hashTable[i];
          while (l != NULL)
          {
            LineList t = l->lines;
            fprintf(listing,"%-14s ",l->name);
            fprintf(listing,"%-14s ",TypeVariable(l->DataType));
            fprintf(listing,"%-8d  ",l->memloc);
            while (t != NULL)
            { fprintf(listing,"%4d ",t->lineno);
              t = t->next;
            }
            fprintf(listing,"\n");
            l = l->next;
          }
        }
      }
      fprintf(listing,"\n");
      index = index->next;
  }
}

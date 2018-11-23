#include "SymbolTab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Scope newScope(char *scopeName)
{
  Scope newScope = (Scope) malloc(sizeof(struct ScopeListRec));
  newScope->name = scopeName;
  return newScope;
}

void insertScopeToList(Scope scope)
{
  scopeList[sizeOfList++] = scope;
}

Scope currScope() {
  return scopeStack[topScope-1];
}

void st_insert( char * scopeName, char * name, ExpType type, TreeNode * treeNode, int loc )
{
  int h = hash(name);
  Scope scope = currScope();

  BucketList l = scope->hashTable[h];
  BucketList lb = scopeList[sizeOfList-1]->hashTable[h];
  // fprintf(listing, "%s %s st_insert\n", scope->name, name);
  /** try to find bucket */
  while ((l != NULL) && (strcmp(name, l->name) != 0)) l = l->next;

  /* variable not yet in BucketList */
  if (l == NULL) {
    l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->treeNode = treeNode;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = treeNode->lineno;
    l->type = type;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = scope->hashTable[h];
    scope->hashTable[h] = l;

    // TODO Improve
    lb = (BucketList) malloc(sizeof(struct BucketListRec));
    lb->name = name;
    lb->treeNode = treeNode;
    lb->lines = (LineList) malloc(sizeof(struct LineListRec));
    lb->lines->lineno = treeNode->lineno;
    lb->type = type;
    lb->memloc = loc;
    lb->lines->next = NULL;
    lb->next = scopeList[sizeOfList-1]->hashTable[h];

  }
      else
      {
        /* already exist in the BucketList */
        LineList t = l->lines;
        while (t->next != NULL) t = t->next;
        t->next = (LineList) malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
      }
} /* st_insert */

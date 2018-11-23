#include "Analyze.h"
#include "Globals.h"
#include "SymbolTab.h"

void buildSymtab(TreeNode * syntaxTree)
{
  globalScope = newScope("global");
  // push global scope
  /*pushScope(globalScope);

  insertInputFunc();
  insertOutputFunc();

  traverse(syntaxTree, insertNode, popAfterInsertProc);
  popScope();
  if (TraceAnalyze) {
    printSymTab(listing);
  }*/
}

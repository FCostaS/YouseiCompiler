#define SIZE 211
#define SHIFT 4

typedef struct LineListRec{
  int lineno;
  struct LineListRec * next;
}*LineList;

typedef struct BucketListRec{
  char * name;
  TreeNode * treeNode;
  LineList lines;
  int memloc; /* memory location for variable */
  ExpType type;
  struct BucketListRec * next;
}*BucketList;

typedef struct ScopeListRec{
  char * name; // function name
  int nestedLevel;
  struct ScopeListRec *parent;
  BucketList hashTable[SIZE]; /* the hash table */
}*Scope;

Scope globalScope;

static Scope scopeList[SIZE];
static int sizeOfList = 0;

static Scope scopeStack[SIZE];
static int topScope = 0;

// Funções
int hash ( char *key );
Scope newScope(char * scopeName);
void insertScopeToList(Scope scope);
Scope currScope();
void st_insert( char * scopeName, char * name, ExpType type, TreeNode * treeNode, int loc );
/*
void popScope(void);
void pushScope(Scope scope);
void insertScopeToList(Scope scope);
*/

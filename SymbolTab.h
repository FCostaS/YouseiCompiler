#define SIZE 211
#define SHIFT 4
/*Conteúdo usual da tabela de símbolos
Nome do identificador
Tipo do identificador (variável, função...)
Escopo da variável
Tipo de dados do identificador (int, float, void...)
Número da linha em que o identificador aparece no programa fonte*/

typedef struct LineListRec /*A lista de números de linha do código-fonte no qual uma variável é referência*/
{
  int lineno;
  struct LineListRec * next;
} *LineList;

/* O registro nas listas de intervalos para cada variável, incluindo nome,
local de memória atribuído e a lista de números de linha em que aparece no código-fonte */
typedef struct BucketListRec{
    char * name;
     LineList lines;
     int memloc ; /* memory location for variable */
     struct BucketListRec *next;
} * BucketList;

typedef struct EscopoListRec{
    char *nameEscopo;
    int typeEscopo,lineno;
    struct EscopoListRec *next;
    BucketList *hashTable; /* Tabela Hash */
}*Escopo;

BucketList *hashTable;
Escopo Programa, EscopoAtual;

// Funções
int hash ( char *key );
void printSymTab(FILE * listing);
int st_lookup ( char * name );
void st_insert( char * name, int lineno, int loc );

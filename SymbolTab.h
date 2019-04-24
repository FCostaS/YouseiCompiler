#define SIZE 211
#define SHIFT 4
/*Conteúdo usual da tabela de símbolos
Nome do identificador
Tipo do identificador (variável, função...)
Escopo da variável
Tipo de dados do identificador (int, float, void...)
Número da linha em que o identificador aparece no programa fonte
*/

typedef struct LineListRec /*A lista de números de linha do código-fonte no qual uma variável é referência*/
{
  int lineno;
  struct LineListRec * next;
} *LineList;

/* O registro nas listas de intervalos para cada variável, incluindo nome,
local de memória atribuído e a lista de números de linha em que aparece no código-fonte */
typedef struct BucketListRec{
    char *name;
     LineList lines;
     VarType DataType;
     int memloc; /* memory location for variable */
     struct BucketListRec *next;
}*BucketList;

/* Aqui (EscopoListRec) eu guardo as informações de cada função e a tabela com as variáveis
que estão nessa função, deixar isso bem separado (funções e variáveis) facilitou na hora
de tratar os erros semânticos */
typedef struct EscopoListRec{
    char *nameEscopo;
    int typeEscopo,lineno,param;
    struct EscopoListRec *next;
    BucketList *hashTable; /* Tabela Hash */
}*Escopo;

/* Resolvi convertar hashTable para ponteiro para não precisar modificar as funções
de inserção e busca do Tiny, dessa forma, quando um novo nó (escopo) da lista encadeada
for alocado, hashTable vai começar a apontar para tabela dessa função. Na verdade, montei
uma tabela para cada função, o que implicou em inserir um while na função de impressão da tabela
que peguei do Tiny */
BucketList *hashTable;

/* Os ponteiros Programa e EscopoAtual são da lista encadeada que monstarei no decorrer da montagem
da tabela, Programa aponta para o início da lista e EscopoAtual para o final da lista */
Escopo Programa, EscopoAtual;

// Funções
int hash ( char *key ); /* Peguei do Tiny sem modificar */
void printSymTab(FILE * listing); /* Coloquei um while para imprimir uma tabela para cada escopo */
int st_lookup ( char * name ); /* Peguei do Tiny sem modificar */
void st_insert( char * name, int lineno, int loc ,VarType var); /* Peguei do Tiny sem modificar,
                                                                  apenas adicionei informações acerca da função */
VarType st_lookup_Type( char * name );
BucketList st_lookup_Full( char * name , char *funct );

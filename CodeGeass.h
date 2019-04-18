typedef enum {Empty,Constant, Variable} TypeOP;

typedef enum {ADD, SUB, MULT, DIV, INC, DEC, AND, OR,
              NOT, XOR, ADDI, MOVE, SLt,slte,sbt,sbte,equal,diff,
              JUMP, LOAD, STORE, IN, OUT, BEQ, BNE, NOP, SLL, SRL,
              SGT, SET, MOD, JR, JAL, LI, PUSH, POP,
              CALL} Instructions;

typedef struct {
  TypeOP kind;
      union {
        int val;
        char *Variable;
        BucketList LocalVar;
  } Contents;
} *Operand;

typedef struct Quadr{
      Operand Op1,Op2,Op3;
      Instructions Inst;
      int IndexLine;
      struct Quadr *next;
}Quadruple;

int LineCode = 0;
int Reg = 0,Label = 0;
char * InstructionsNames[] = {"add","sub","mult","div","inc","dec",
                              "and","or","not","xor","addi","move",
                              "slt","slte","sbt","sbte","equal","diff",
                              "jump","load","store","in","out",
                              "beq","bne","nop","sll","srl","sgt",
                              "set","mod","jr","jal","li","push",
                              "pop","call"};


char *TypeInstruction(Instructions i)
{
  return InstructionsNames[i];
}

void PrintQuadruple(Instructions I,char *Op1,char *Op2, char *Op3)
{
   printf("%d: (%s,%s,%s,%s)\n",LineCode++,TypeInstruction(I),Op1,Op2,Op3);
}

char *GiveMeTemporary()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"t%d",Reg++);
    return buffer;
}

char *GiveMeLabel()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"L%d",Label++);
    return buffer;
}

char *ShowMeTemporary()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"t%d",Reg);
    return buffer;
}

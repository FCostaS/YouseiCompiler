typedef enum {Empty,Constant, Variable, Mark, Call_Value, ArrVariable,ArrEmpty} TypeOP;

typedef enum {ADD, SUB, MULT, DIV, INC, DEC, AND, OR,
              NOT, XOR, ADDI, MOVE, SLt,slte,sbt,sbte,equal,diff,
              JUMP, LOAD, STORE, IN, OUT, BEQ, BNE, NOP, SLL, SRL,
              SGT, SET, MOD, JR, JAL, LI, PUSH, POP,CALL,HALT,RETURNi} Instructions;

typedef enum{z0,r0,a0,a1,a2,a3,a4,a5,t0 ,t1,
             t2,t3,t4,t5,t6,t7,t8,t9,t10,s0,
             s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,sp} RegBank;

typedef struct {
  TypeOP kind;
  int val;
  char *Variable;
  BucketList Local;
} *Operand;

typedef struct Quadr{
      Operand Op1,Op2,Op3;
      Instructions Inst;
      int IndexLine;
      struct Quadr *next;
}Quadruple;

int LineCode = 0;
int Label = 0,Reg = 8,ARGS = 2,GeralReg = 20;
int Assign_Type = 1;
char * InstructionsNames[] = {"add","sub","mult","div","inc","dec",
                              "and","or","not","xor","addi","move",
                              "slt","slte","sbt","sbte","equal","diff",
                              "jump","load","store","in","out",
                              "beq","bne","nop","sll","srl","sgt",
                              "set","mod","jr","jal","li","push",
                              "pop","call","halt","return"};

char *RegistersBank[] =   {"$z0","$r0","$a0","$a1","$a2","$a3","$a4","$a5",
                          "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
                          "$t8","$t9","t10","t11","$s0","$s1","$s2","$s3",
                          "$s4","$s5","$s6","$s7","$s8","$s9","$s10","$sp"};


char *TypeInstruction(Instructions i){ return InstructionsNames[i]; }

char *TypeRegister(RegBank Re){ return RegistersBank[Re]; }

void PrintQuadruple(Instructions I,Operand Op1,Operand Op2, Operand Op3, char *Message)
{ printf("%d: (%s,%s,%s,%s)\t\t%s\n",LineCode++,TypeInstruction(I),Op1->Variable,Op2->Variable,Op3->Variable,Message); }

/* Função de atribuição temporária, sem gerenciamento */
char *GiveMeTemporary()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"%s",TypeRegister(Reg++));
    return buffer;
}

char *GiveMeLabel()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"L%d",Label++);
    return buffer;
}

char *GiveMeArgs()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"%s",TypeRegister(ARGS++));
    return buffer;
}

char *GiveMeGeral()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"%s",TypeRegister(GeralReg++));
    return buffer;
}

char *ShowMeTemporary()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"%s",TypeRegister(Reg));
    return buffer;
}

char *ShowMeArgs()
{
    char* buffer = (char*)malloc(5*sizeof(char));
    sprintf(buffer,"%s",TypeRegister(ARGS));
    return buffer;
}

void ResetArg(){ ARGS = 2; }
void ResetTemp(){ Reg = 8; }
/**************************************************/

char *Int2String(int i)
{
    char* buffer = (char*)malloc(5*sizeof(char)); sprintf(buffer,"%d",i);
    return buffer;
}

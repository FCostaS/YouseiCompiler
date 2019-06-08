typedef enum {Empty,Constant, Variable, Mark, Call_Value, ArrVariable,ArrEmpty,ArrParam} TypeOP;

typedef enum {ADD, SUB, MULT, DIV, INC, DEC, AND, OR,
              NOT, XOR, ADDI, MOVE, SLt,slte,sbt,sbte,equal,diff,
              JUMP, LOAD, STORE, IN, OUT, BEQ, BNE, NOP, SLL, SRL,
              SGT, SET, MOD, JR, JAL, LI, PUSH, POP,CALL,HALT,RETURNi} Instructions;

typedef enum{z0,r0,a0,a1,a2,a3,a4,a5,t0 ,t1,
             t2,t3,t4,t5,t6,t7,t8,t9,t10,s0,
             s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,sp} RegBank;

typedef enum{R,I,J} FormaInstruction;

typedef struct {
    int Address;
    char *Name;
} *AssemblyOp;

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
      char *BinaryMode;
}Quadruple;

typedef struct Assembly{
      char *BinaryMode;
      char *AssemblyMode;
      AssemblyOp Op1,Op2,Op3;
      int IndexLine;
      FormaInstruction Type;
      Instructions I;
      struct Assembly *next;
}AssemblyInst;

Quadruple *Intermediary = NULL,*IntermediaryFirst;
AssemblyInst *AssemblyList = NULL, *AssemblyFirst;

typedef struct Marks{
    char *MarkName;
    int LineMark;
    struct Marks *next;
}Marker;

Marker *Markers = NULL, *MarkFirst;

int LineCode = 0;
int Label = 0,Reg = 8,ARGS = 2,GeralReg = 20;
int Assign_Type = 1;
char * InstructionsNames[] = {"add","sub","mult","div","inc","dec",
                              "and","or","not","xor","addi","move",
                              "slt","slte","sbt","sbte","equal","diff",
                              "jump","lw","sw","in","out",
                              "beq","bne","nop","sll","srl","sgt",
                              "set","mod","jr","jal","li","push",
                              "pop","call","halt","return"};

char *RegistersBank[] =   {"$zero","$ra","$a0","$a1","$a2","$a3","$a4","$a5",
                          "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
                          "$t8","$t9","t10","t11","$s0","$s1","$s2","$s3",
                          "$s4","$s5","$s6","$s7","$s8","$s9","$v0","$sp"};


char *TypeInstruction(Instructions i){ return InstructionsNames[i]; }

char *TypeRegister(RegBank Re){ return RegistersBank[Re]; }

static Operand AllocOp()
{
    Operand O = (Operand)malloc(sizeof(Operand));
    O->Local = NULL;
    return O;
}

void InsertMark(char *NameMark, int MarkNumber)
{
      Marker *new = (Marker*)malloc(sizeof(Marker));
      if(Markers == NULL)
      {
          MarkFirst = new;
          Markers   = new;
      }
      Markers->next = new;
      new->MarkName = NameMark;
      new->LineMark = MarkNumber;
      new->next = NULL;
      Markers = new;
}

int SearchMark(char *name)
{
      Marker *i;
      for(i = MarkFirst; i!=NULL; i=i->next)
      {
          if(strcmp(name,i->MarkName)==0)
          {
              return i->LineMark;
          }
      }
      return -1;
}

void PrintQuadruple(Instructions I,Operand Op1,Operand Op2, Operand Op3, char *Message)
{
    //printf("%d: (%s,%s,%s,%s)\t\t%s\n",LineCode++,TypeInstruction(I),Op1->Variable,Op2->Variable,Op3->Variable,Message);
    Quadruple *Q  = (Quadruple*)malloc(sizeof(Quadruple));
    if(Intermediary == NULL)
    {
        Intermediary      = Q;
        IntermediaryFirst = Q;
    }
    Intermediary->next = Q;
    Q->Op1             = Op1;
    Q->Op2             = Op2;
    Q->Op3             = Op3;
    Q->Inst            = I;
    Q->IndexLine       = LineCode++;
    Q->next            = NULL;
    Intermediary       = Q;
}

void ShowMeQuadruplas()
{
      Quadruple *it = IntermediaryFirst;
      while(it!=NULL)
      {
          printf("%d: (%s,%s,%s,%s)\n",it->IndexLine
                                    ,TypeInstruction(it->Inst)
                                    ,it->Op1->Variable
                                    ,it->Op2->Variable
                                    ,it->Op3->Variable);
          it = it->next;
      }
}

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

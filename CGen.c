#include <stdlib.h>
#include <string.h>
#include "Globals.h"
#include "SymbolTab.h"
#include "CodeGeass.h"
#include "CGen.h"
#include "Binary.h"
#include "Archive.h"

/*static void cGen( TreeNode * tree);
static void genExpK( TreeNode * t);
static void genStmtK( TreeNode * tree);
static void genDeclK( TreeNode * t);*/

int i,ArrControl;
int CurrentInst;
char *CurrentFunction;
Operand CurrentOpK,OperadorVazio,OperadorZero,OperadorFixo;

/**********************************************************************/

static Operand InsertOperand(TypeOP expo,char *nameVariable,int constant)
{
    Operand Op = AllocOp();
    char Buffer[5];
    Op->Local = NULL;
    Op->val = constant;

    switch (expo)
    {
      case Variable:
          Op->Variable = nameVariable;
          Op->Local = st_lookup_Full(nameVariable,CurrentFunction);
          if(Op->Local==NULL)
          { Op->Local = st_lookup_Full(nameVariable,"Global"); }
      break;

      case Constant:
          Op->Variable = Int2String(constant);
      break;

      case Mark:
          Op->Variable = nameVariable;
      break;

      case Param:
          Op->Variable = nameVariable;
          Op->Local = st_lookup_Full(nameVariable,CurrentFunction);
          if(Op->Local==NULL)
          { Op->Local = st_lookup_Full(nameVariable,"Global"); }
      break;

      case Call_Value:
          Op->Variable = nameVariable;
      break;

      case Empty:
          Op->Variable = nameVariable;
      break;

      case ArrVariable:
          Op->Variable = nameVariable;
          Op->Local = st_lookup_Full(nameVariable,CurrentFunction);
          if(Op->Local==NULL)
          { Op->Local = st_lookup_Full(nameVariable,"Global"); }
      break;

      case ArrEmpty:
          Op->Variable = nameVariable;
      break;

      case ArrParam:
          Op->Variable = nameVariable;
          Op->Local = st_lookup_Full(nameVariable,CurrentFunction);
          if(Op->Local==NULL)
          { Op->Local = st_lookup_Full(nameVariable,"Global"); }
      break;

    }
    Op->kind = expo;
    return Op;
}

static void genExpK( TreeNode * t)
{
    int loc,args;
    TreeNode *p1, *p2, *p3;
    Operand Op1,Op2,Op3;
    VarType temp;
    char Buffer[5],*CurrentReg;
    BucketList l;
    switch (t->kind.exp)
    {
        case OpK:
          switch (t->attr.op)
          {
                case PLUS : CurrentInst = ADD;   break;
                case MINUS: CurrentInst = SUB;   break;
                case STAR : CurrentInst = MULT;  break;
                case BAR  : CurrentInst = DIV;   break;
                case SLT  : CurrentInst = SLt;   break;
                case SLTE : CurrentInst = slte;  break;
                case SBT  : CurrentInst = sbt;   break;
                case SBTE : CurrentInst = sbte;  break;
                case EQUAL: CurrentInst = equal; break;
                case DIFF : CurrentInst = diff;  break;
                default: printf("Insert new OpK\n"); break;
          }
        break;

        case ConstK:
            Op1 = InsertOperand(Empty,GiveMeTemporary(),Reg-1);
            Op2 = InsertOperand(Constant,Int2String(t->attr.val),t->attr.val);
            PrintQuadruple(LI,Op1,Op2,OperadorVazio,"");
            CurrentOpK = Op1;
        break;

        case IdK:
            CurrentOpK = InsertOperand(Variable,t->attr.name,-1);
        break;

        case TypeK:   break;

        case ArrIdK:
            genExpK(t->child[0]);
            Op1 = CurrentOpK;
            Op2 = InsertOperand(ArrVariable,t->attr.name,-1);
            CurrentOpK = InsertOperand(ArrEmpty,GiveMeTemporary(),Reg-1);
            if(Assign_Type){ PrintQuadruple(LOAD,CurrentOpK,Op2,Op1,"-- Fazendo leitura da memória p/ registrador"); }
            else{ PrintQuadruple(ADD,CurrentOpK,Op2,Op1,"-- Calculando endereço da memória"); }
        break;

        case CallK:   // Chamada de Função (Análise de Argumentos da Função
              args = 0;
              ARGS = 2;
              if(strcmp(t->attr.name,"input")==0)
              {
                  Op1 = InsertOperand(Empty,GiveMeArgs(),2);
                  PrintQuadruple(IN,Op1,OperadorVazio,OperadorVazio,"");
                  CurrentOpK = Op1;
                  args++;
                  return;
              }

              for(p1=t->child[0]; p1!=NULL;p1 = p1->sibling)
              {
                  genExpK(p1);
                  Op1 = InsertOperand(Empty,GiveMeArgs(),ARGS);

                  if(CurrentOpK->kind == Variable && CurrentOpK->Local->DataType==IntArray)
                  {
                        PrintQuadruple(LI,Op1,CurrentOpK,OperadorZero,"");
                  }
                    else
                    {
                        PrintQuadruple(MOVE,Op1,CurrentOpK,OperadorVazio,"");
                    }
                  CurrentOpK = Op1;
                  args++;
              }

              p1 = t;

              if( strcmp(t->attr.name,"output") != 0 )
              {
                  Op1 = InsertOperand(Call_Value,p1->attr.name,-1);
                  PrintQuadruple(CALL,Op1,OperadorVazio,OperadorVazio,"");
                  CurrentOpK = Op1;
              } // Tratando caso em que a chamada é output
                else
                {
                    PrintQuadruple(OUT,CurrentOpK,OperadorVazio,OperadorVazio,"");
                }
        break;
        case CalcK:

            p1 = t->child[0];
            p2 = t->child[1];
            p3 = t->child[2];

            /*genExpK(p1);
            if(p1->kind.exp == CalcK)
            { Op1 = InsertOperand(Empty,GiveMeTemporary(),Reg-1); }
            else if (p1->kind.exp == CallK)
            { Op1 = InsertOperand(Mark,p1->attr.name,-1); }
            else { Op1 = CurrentOpK; }

            genExpK(p3);
            if(p3->kind.exp == CalcK)
            { Op2 = InsertOperand(Empty,GiveMeTemporary(),Reg-1); }
            else if (p3->kind.exp == CallK)
            { Op2 = InsertOperand(Mark,p3->attr.name,-1); }
            else{ Op2 = CurrentOpK; }

            genExpK(p2);
            Op3 = InsertOperand(Empty,GiveMeTemporary(),Reg-1);
            CurrentOpK = Op3;*/

            genExpK(p1);
            Op1 = CurrentOpK;

            genExpK(p3);
            Op2 = CurrentOpK;

            genExpK(p2);

            Op3 = InsertOperand(Empty,GiveMeTemporary(),Reg-1);
            CurrentOpK = Op3;

            PrintQuadruple(CurrentInst,Op3,Op1,Op2,"");

        break;
        default: printf("Você me esqueceu parça exp!\n"); break;
    }

}

static void genStmtK( TreeNode * tree)
{
      int loc,i;
      VarType temp;
      TreeNode *p1,*p2,*p3;
      Operand op1,op2,op3;
      BucketList l;
      char *CurrentReg,*CurrentLabel1,*CurrentLabel2;

      switch (tree->kind.stmt)
      {
          case IfK:

          p1 = tree->child[0];
          p2 = tree->child[1];
          p3 = tree->child[2];

          //TESTE
          CurrentReg = ShowMeTemporary();
          op3 = InsertOperand(Empty,CurrentReg,Reg);
          CurrentLabel1 = GiveMeLabel();
          CurrentLabel2 = GiveMeLabel();
          cGen(p1); /* teste lógico */
          op2 = InsertOperand(Mark,CurrentLabel1,-1);
          PrintQuadruple(BEQ,op3,InsertOperand(Constant,copyString("0"),0),op2,"-- Início do If");

          // IF
          cGen(p2);
          op1 = InsertOperand(Mark,CurrentLabel2,-1);
          PrintQuadruple(JUMP,op1,OperadorVazio,OperadorVazio,"-- Fim do If");
          op2->val = LineCode; // Indico onde o label está localizado
          PrintQuadruple(NOP,op2,OperadorVazio,OperadorVazio,"\t-- Início do Else");

          // ELSE
          cGen(p3);
          op1->val = LineCode; // Indico onde o label está localizado
          PrintQuadruple(NOP,op1,OperadorVazio,OperadorVazio,"-- Fim do Else");
          // Registrador de retorno da pilha

          break;

          case WhileK:

          p1 = tree->child[0];
          p2 = tree->child[1];
          CurrentLabel1 = GiveMeLabel();
          CurrentLabel2 = GiveMeLabel();

          op1 = InsertOperand(Constant,TypeRegister(0),0);
          op2 = InsertOperand(Mark,CurrentLabel1,LineCode);
          op3 = InsertOperand(Mark,CurrentLabel2,-1);
          PrintQuadruple(NOP,op2,OperadorVazio,OperadorVazio,"-- Início do While");
          cGen(p1);
          PrintQuadruple(BEQ,CurrentOpK,op1,op3,"");

          cGen(p2);
          op3->val = LineCode;
          PrintQuadruple(JUMP,op2,OperadorVazio,OperadorVazio,"");
          PrintQuadruple(NOP,op3,OperadorVazio,OperadorVazio,"-- Fim do While");

          break;

          case AssignK:

          p1 = tree->child[0];
          p2 = tree->child[1];

          Assign_Type = 1; // Tipo LOAD
          cGen(p2);
          op2 = CurrentOpK;

          Assign_Type = 0; // Tipo STORE
          cGen(p1);
          op1 = CurrentOpK;

          ArrControl = 0;
          ResetTemp();

          /*if(p2->kind.exp == ArrIdK)
          {
              PrintQuadruple(LOAD,op1,op2,OperadorZero,"");
              ArrControl++;
          }*/

          if(p1->kind.exp == ArrIdK)
          {
              PrintQuadruple(STORE,op2,OperadorFixo,op1,"");
              ArrControl++;
          }
          else
          { PrintQuadruple(STORE,op2,op1,OperadorZero,""); }

          /*// Se manipulei vetores, então não percorre
          if(ArrControl == 0){ PrintQuadruple(STORE,op1,op2,OperadorVazio,""); }*/

          Assign_Type = 1; // Renova atribuição
          break;

          case CompoundK:
              ResetArg(); // Quando iniciar declaração de variável, eu zero o registrador de argumentos
              for (i=0; i < MAXCHILDREN; i++) { cGen(tree->child[i]); }
          break;

          case ReturnK:
                  ResetTemp();
                  cGen(tree->child[0]); // Invoca Jump Register
                  op1 = InsertOperand(Empty,RegistersBank[30],30);
                  PrintQuadruple(MOVE,op1,CurrentOpK,OperadorVazio,"");
                  CurrentOpK = op1;
          break;
          default: printf("Você me esqueceu parça stmt!\n"); break;
      }
}

void StackArguments(TreeNode * t)
{
    if(t!=NULL)
    {
          StackArguments(t->sibling);
          genDeclK(t);
    }
}

static void genDeclK( TreeNode * t)
{
      BucketList l;
      Operand Op1,Op2,Op3;
      int Curegister;
      TreeNode *arguments;

      switch (t->kind.decl)
      {
            case ArrParamK:

              Op1 = InsertOperand(ArrParam,t->attr.name,-1);
              Op2 = InsertOperand(Empty,GiveMeArgs(),ARGS);
              PrintQuadruple(STORE,Op2,Op1,OperadorZero,"-- Informando endereço de memória do vetor");

            break; // Util na atribuição de Registradores Gerais
            case ParamK:

              if(t->attr.type==Void) return;

              //Curegister = Reg;
              //l = st_lookup_Full(t->attr.name,CurrentFunction);
              Op1 = InsertOperand(Param,t->attr.name,-1);
              Op2 = InsertOperand(Empty,GiveMeArgs(),ARGS);
              PrintQuadruple(STORE,Op2,Op1,OperadorZero,"-- Atribuindo argumentos da função");

            break; // Util na atribuição de Registradores Gerais
            case ArrVarK:

              /*l = st_lookup_Full(t->attr.arr.name,CurrentFunction);
              Op2 = InsertOperand(Empty,GiveMeGeral(),GeralReg-1);             // Registrador Zero
              Op1 = InsertOperand(ArrVariable,t->attr.arr.name,l->memloc); // Nome da variável
              PrintQuadruple(MOVE,Op2,Op1,OperadorVazio,"-- Reserva registrador geral para vetor");
              */

            break; // Util na atribuição de Registradores Temporários
            case VarK:

              /*
              l = st_lookup_Full(t->attr.name,CurrentFunction);
              Op2 = InsertOperand(Empty,GiveMeGeral(),GeralReg-1);             // Registrador Zero
              Op1 = InsertOperand(Variable,t->attr.name,l->memloc); // Nome da variável
              PrintQuadruple(MOVE,Op2,Op1,OperadorVazio,"-- Reserva registrador geral");
              */

            break; // Util na atribuição de Registradores Temporários
            case FunK:              // Percorre as funções presentes no programa
              ResetArg(); // Quando iniciar a função, eu zero o registrador de argumentos
              CurrentFunction = t->attr.name;
              Op1 = InsertOperand(Mark,t->attr.name,LineCode); // Nome de função
              PrintQuadruple(NOP,Op1,OperadorVazio,OperadorVazio,"-- Início da Função | Reservando rôtulo");

              cGen(t->child[0]);
              cGen(t->child[1]);
              //StackArguments(t->child[1]);
              cGen(t->child[2]); // Corpo da função

              if( strcmp("main",CurrentFunction) != 0)
              {PrintQuadruple(JR,InsertOperand(Empty,TypeRegister(1),1),OperadorVazio,OperadorVazio,"-- Fim de função");}
            break;
            default: printf("Você me esqueceu parça decl!\n"); break;
      }
}


static void cGen( TreeNode * tree)
{
  if (tree != NULL)
  {
    switch (tree->nodekind)
    {
      case StmtK:
        genStmtK(tree);
        break;
      case ExpK:
        genExpK(tree);
        break;
      case DeclK:
        genDeclK(tree);
        break;
    }
    cGen(tree->sibling);
  }
}

void codeGen(TreeNode * syntaxTree, char * codefile)
{
    //printf("\033[01;33m"); printf("[Intermediary Code]\n"); printf("\033[0m");
    CurrentFunction = "Global";
    OperadorVazio = InsertOperand(Empty,"_",0);
    OperadorZero  = InsertOperand(Empty,TypeRegister(0),0);
    OperadorFixo  = InsertOperand(Empty,Int2String(0),0);
    cGen(syntaxTree);
    PrintQuadruple(HALT,OperadorVazio,OperadorVazio,OperadorVazio,"-- Fim do programa");
    StartArchives();
    ShowMeQuadruplas();
}
/* ASSEMBLY *****************************************************************************************************/
AssemblyOp AssemblyZero, AssemblyNulo;
int IndexAssembly = 0;

AssemblyOp InsertOperandAssembly(int Address,char *NameOp)
{
    AssemblyOp new = (AssemblyOp)malloc(sizeof(AssemblyOp));
    new->Address   = Address;
    new->Name      = NameOp;
    return new;
}

void InsertAssembly(int Type, Instructions I,AssemblyOp Op1,AssemblyOp Op2,AssemblyOp Op3,char *BinChar)
{
    char *Inst = TypeInstruction(I);
    AssemblyInst *new = (AssemblyInst*)malloc(sizeof(AssemblyInst));
    char *MyinstructionBuffer = (char*)malloc(40*sizeof(char));

    if(I == STORE || I == LOAD)
    {
        sprintf(MyinstructionBuffer,"%s\t%s %s(%s)",Inst,Op1->Name,Op2->Name,Op3->Name);
    }
      else if (I == NOP )
      {
        sprintf(MyinstructionBuffer,"%s\t%s",Inst,Op1->Name);
      }
        else if (I == JUMP )
        {
          sprintf(MyinstructionBuffer,"%s\t%s",Inst,Op3->Name);
        }
          else
          {
              sprintf(MyinstructionBuffer,"%s\t%s %s %s",Inst,Op1->Name,Op2->Name,Op3->Name);
          }

    if(AssemblyList == NULL)
    {
        AssemblyFirst  = new;
        AssemblyList   = new;
    }

    AssemblyList->next = new;
    new->AssemblyMode  = MyinstructionBuffer;
    new->BinaryMode    = BinChar;
    new->IndexLine     = IndexAssembly++;
    new->Type          = Type;

    new->Op1           = Op1;
    new->Op2           = Op2;
    new->Op3           = Op3;

    new->I             = I;

    /* Type Flag
    0: Tipo R
    1: Tipo I
    2: Tipp J
    */

    new->next          = NULL;
    AssemblyList       = new;
}


AssemblyOp GiveMeANumber(Operand Op,int TypeValueAssembly)
{
    AssemblyOp A, B, C;
    char *BinInstruction;
    int HaveMark;

    switch (Op->kind)
    {
      case Variable:

            if(Op->Local->DataType == IntArray)
            {
                A = InsertOperandAssembly(Op->Local->memloc,Int2String(Op->Local->memloc)); // Caso 1: Quero um endereço
                return A;
            }

            if(TypeValueAssembly < 2) // Caso < 2: Quero um valor
            {
                A = InsertOperandAssembly(TypeValueAssembly+20,TypeRegister(TypeValueAssembly+20));
                B = InsertOperandAssembly(Op->Local->memloc,Int2String(Op->Local->memloc));
                C = AssemblyZero;
                BinInstruction= TypeI(LOAD,C->Address,A->Address,B->Address);
                InsertAssembly(1,LOAD,A,B,C,BinInstruction);
            }
              else
              {
                  A = InsertOperandAssembly(Op->Local->memloc,Int2String(Op->Local->memloc)); // Caso 1: Quero um endereço
              }
           return A;
      break;

      case Constant:

          A = InsertOperandAssembly(Op->val,Int2String(Op->val));
          return A;

      break;

      case Mark:
          HaveMark = SearchMark(Op->Variable);
          A = InsertOperandAssembly(HaveMark,Op->Variable);
          return A;
      break;

      case Param:
            if(TypeValueAssembly < 2) // Caso < 2: Quero um valor
            {
                A = InsertOperandAssembly(TypeValueAssembly+20,TypeRegister(TypeValueAssembly+20));
                B = InsertOperandAssembly(Op->Local->memloc,Int2String(Op->Local->memloc));
                C = AssemblyZero;
                BinInstruction= TypeI(LOAD,C->Address,A->Address,B->Address);
                InsertAssembly(1,LOAD,A,B,C,BinInstruction);
            }
              else
              {
                  A = InsertOperandAssembly(Op->Local->memloc,Int2String(Op->Local->memloc)); // Caso 1: Quero um endereço
              }
           return A;
      break;

      case ArrParam:

            A = InsertOperandAssembly(Op->Local->memloc,Int2String(Op->Local->memloc));

      return A;

      case ArrVariable:

            if(Op->Local->Pointer)
            {
                A = InsertOperandAssembly(TypeValueAssembly+20,TypeRegister(TypeValueAssembly+20));
                B = InsertOperandAssembly(Op->Local->memloc,Int2String(Op->Local->memloc));
                C = AssemblyZero;
                BinInstruction= TypeI(LOAD,C->Address,A->Address,B->Address);
                InsertAssembly(1,LOAD,A,B,C,BinInstruction);
                return A;
            }
              else
              {
                  A = InsertOperandAssembly(Op->Local->memloc,Int2String(Op->Local->memloc)); // Caso 1: Quero um endereço
              }
      return A;

      case Call_Value:
            A = InsertOperandAssembly(30,TypeRegister(30));
      break;

      default:
          A = InsertOperandAssembly(Op->val,Op->Variable);
          return A;
      break;
    }
}


void AssemblyGenerator(Quadruple *Q)
{
      if(Q==NULL) return;

      AssemblyOp Op1,Op2,Op3,Op4,Op5;
      char *BinInstruction = NULL;
      int NewIndex;

      char *Instruct = TypeInstruction(Q->Inst);
      switch (Q->Inst)
      {
          case NOP:
              InsertMark(Q->Op1->Variable,IndexAssembly);
              BinInstruction = TypeJ(NOP,0);
              Op1 = GiveMeANumber(Q->Op1,0);
              InsertAssembly(J,NOP,Op1,AssemblyZero,AssemblyZero,BinInstruction);
          break;

          case STORE: // OK
              // Nem sempre será 0
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,2);
              Op3 = GiveMeANumber(Q->Op3,1);
              if(Q->Op2->kind != Empty && Q->Op2->kind != ArrParam  && Q->Op2->Local->Pointer==1)
              {
                    Op4 = InsertOperandAssembly(20,TypeRegister(20));
                    BinInstruction = TypeI(LOAD,AssemblyZero->Address,Op2->Address,Op4->Address);
                    InsertAssembly(I,LOAD,Op4,Op2,AssemblyZero,BinInstruction);

                    BinInstruction = TypeR(ADD,Op3->Address,Op4->Address,Op4->Address,0);
                    InsertAssembly(I,ADD,Op4,Op4,Op3,BinInstruction);

                    BinInstruction = TypeI(STORE,Op4->Address,Op1->Address,Op2->Address);
                    InsertAssembly(I,STORE,Op1,Op2,Op4,BinInstruction);
              }
                else
                {
                    BinInstruction = TypeI(STORE,Op3->Address,Op1->Address,Op2->Address);
                    InsertAssembly(I,STORE,Op1,Op2,Op3,BinInstruction);
                }
          break;

          case LOAD: // OK
              // Nem sempre será 0
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,2);
              Op3 = GiveMeANumber(Q->Op3,1);
              // Se for um ponteiro, então o endereço está na variável
              if(Q->Op2->kind != ArrParam && Q->Op2->Local->Pointer==2)
              {
                    Op4 = InsertOperandAssembly(20,TypeRegister(20));
                    Op5 = InsertOperandAssembly(0,Int2String(0));
                    BinInstruction = TypeI(LOAD,AssemblyZero->Address,Op4->Address,Op2->Address);
                    InsertAssembly(I,LOAD,Op4,Op2,AssemblyZero,BinInstruction);

                    BinInstruction = TypeR(ADD,Op3->Address,Op4->Address,Op4->Address,0);
                    InsertAssembly(I,ADD,Op4,Op4,Op3,BinInstruction);

                    BinInstruction = TypeI(LOAD,Op4->Address,Op1->Address,Op5->Address);
                    InsertAssembly(I,LOAD,Op1,Op5,Op4,BinInstruction);
              }
                else
                {
                    BinInstruction = TypeI(LOAD,Op3->Address,Op1->Address,Op2->Address);
                    InsertAssembly(I,LOAD,Op1,Op2,Op3,BinInstruction);
                }
          break;

          case MOVE: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,1);
              BinInstruction = TypeR(MOVE,Op1->Address,Op2->Address,0,0);
              InsertAssembly(R,MOVE,Op1,Op2,AssemblyNulo,BinInstruction);
          break;

          case JUMP: // OK
              //Op1 = GiveMeANumber(Q->Op1,0);
              Op1 = InsertOperandAssembly(-1,Q->Op1->Variable);
              BinInstruction = TypeJ(JUMP,Op1->Address);
              InsertAssembly(J,JUMP,AssemblyNulo,AssemblyNulo,Op1,BinInstruction);
          break;

          case OUT: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              BinInstruction = TypeI(OUT,Op1->Address,0,0);
              InsertAssembly(I,OUT,Op1,AssemblyNulo,AssemblyNulo,BinInstruction);
          break;

          case IN:
              Op1 = GiveMeANumber(Q->Op1,0);
              BinInstruction = TypeI(IN,Op1->Address,0,0);
              InsertAssembly(I,IN,Op1,AssemblyNulo,AssemblyNulo,BinInstruction);
          break;

          case HALT: // OK
              BinInstruction = TypeJ(JUMP,IndexAssembly);
              InsertAssembly(J,HALT,AssemblyNulo,AssemblyNulo,AssemblyNulo,BinInstruction);
          break;

          case LI: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,1);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeI(ADDI,Op3->Address,Op1->Address,Op2->Address);
              if( strcmp(Op3->Name,"_") == 0 )
              {
                  Op3 = AssemblyZero;
              }
              InsertAssembly(I,ADDI,Op1,Op3,Op2,BinInstruction);
          break;

          case ADD: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(ADD,Op2->Address,Op3->Address,Op1->Address,0);
              InsertAssembly(R,ADD,Op1,Op2,Op3,BinInstruction);
          break;

          case SUB: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(SUB,Op3->Address,Op2->Address,Op1->Address,1);
              InsertAssembly(R,SUB,Op1,Op2,Op3,BinInstruction);
          break;

          case MULT: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(MULT,Op3->Address,Op2->Address,Op1->Address,2);
              InsertAssembly(R,MULT,Op1,Op2,Op3,BinInstruction);
          break;

          case DIV: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(DIV,Op3->Address,Op2->Address,Op1->Address,3);
              InsertAssembly(R,DIV,Op1,Op2,Op3,BinInstruction);
          break;

          case BEQ: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeI(BEQ,Op1->Address,Op2->Address,Op3->Address);
              InsertAssembly(I,BEQ,Op1,Op2,Op3,BinInstruction);
          break;

          case SLt: // OK
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(SLt,Op3->Address,Op2->Address,Op1->Address,0);
              InsertAssembly(R,SLt,Op1,Op2,Op3,BinInstruction);
          break;

          case sbte:
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(sbte,Op3->Address,Op2->Address,Op1->Address,0);
              InsertAssembly(R,sbte,Op1,Op2,Op3,BinInstruction);
          break;

          case equal:
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(equal,Op3->Address,Op2->Address,Op1->Address,0);
              InsertAssembly(R,equal,Op1,Op2,Op3,BinInstruction);
          break;

          case sbt:
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(sbt,Op3->Address,Op2->Address,Op1->Address,0);
              InsertAssembly(R,sbt,Op1,Op2,Op3,BinInstruction);
          break;

          case slte:
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(slte,Op3->Address,Op2->Address,Op1->Address,0);
              InsertAssembly(R,slte,Op1,Op2,Op3,BinInstruction);
          break;

          case POP:
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = InsertOperandAssembly(Q->Op2->Local->memloc,Int2String(Q->Op2->Local->memloc));
              BinInstruction = TypeR(POP,Op2->Address,Op1->Address,0,0);
              InsertAssembly(R,POP,Op1,Op2,AssemblyNulo,BinInstruction);
              if(Q->Op2->kind == Param) // Caso < 2: Quero um valor
              {
                  BinInstruction= TypeI(STORE,AssemblyZero->Address,Op2->Address,Op3->Address);
                  InsertAssembly(1,STORE,Op2,Op3,AssemblyZero,BinInstruction);
              }
          break;

          case PUSH:
              Op1 = GiveMeANumber(Q->Op1,0);
              Op2 = GiveMeANumber(Q->Op2,0);
              Op3 = GiveMeANumber(Q->Op3,1);
              BinInstruction = TypeR(PUSH,Op3->Address,Op2->Address,Op1->Address,0);
              InsertAssembly(R,PUSH,Op1,Op2,AssemblyNulo,BinInstruction);
          break;

          case RETURNi:
              printf("\tjr %s\n",Q->Op1->Variable);
          break;

          case JR:
                Op1 = GiveMeANumber(Q->Op1,0);
                Op2 = InsertOperandAssembly(20,TypeRegister(20));

                BinInstruction = TypeR(POP,Op2->Address,Op1->Address,0,0);
                InsertAssembly(R,POP,Op1,Op2,AssemblyNulo,BinInstruction);

                BinInstruction = TypeI(JR,Op2->Address,AssemblyZero->Address,AssemblyZero->Address);
                InsertAssembly(I,JR,Op2,AssemblyNulo,AssemblyNulo,BinInstruction);
          break;


          case CALL:
                NewIndex = IndexAssembly + 3;
                Op1 = InsertOperandAssembly(-1,Q->Op1->Variable);
                Op2 = InsertOperandAssembly(20,TypeRegister(20));
                Op3 = InsertOperandAssembly(NewIndex,Int2String(NewIndex));
                Op4 = InsertOperandAssembly(1,TypeRegister(1));

                BinInstruction = TypeI(ADDI,0,Op2->Address,NewIndex);
                InsertAssembly(I,ADDI,Op2,AssemblyZero,Op3,BinInstruction);

                BinInstruction = TypeR(PUSH,Op2->Address,Op4->Address,0,0);
                InsertAssembly(R,PUSH,Op4,Op2,AssemblyNulo,BinInstruction);
                BinInstruction = TypeJ(JUMP,Op1->Address);
                InsertAssembly(J,JUMP,AssemblyNulo,AssemblyNulo,Op1,BinInstruction);
          break;

          default:
              printf("\t%s %s,%s,%s\n",Instruct,Q->Op1->Variable,Q->Op2->Variable,Q->Op3->Variable);
          break;
      }
      Q->BinaryMode = BinInstruction;
      AssemblyGenerator(Q->next);
}

void ShowMeQuadruplas()
{
      Quadruple *it = IntermediaryFirst;
      while(it!=NULL)
      {
          fprintf(IntermediaryCode,"%d: (%s,%s,%s,%s)\n",it->IndexLine
                                    ,TypeInstruction(it->Inst)
                                    ,it->Op1->Variable
                                    ,it->Op2->Variable
                                    ,it->Op3->Variable);
          it = it->next;
      }
}

void BinaryPrint()
{
    AssemblyInst *it = AssemblyFirst;
    printf("\033[01;33m"); printf("\n[Binary and Assembly Generator]\n"); printf("\033[0m\n");
    while(it!=NULL)
    {
          printf("Memory[%2d] = 32'B%s; // %s\n",it->IndexLine,it->BinaryMode,it->AssemblyMode);
          fprintf(Full,"Memory[%2d] = 32'B%s; // %s\n",it->IndexLine,it->BinaryMode,it->AssemblyMode);
          fprintf(BinaryCode,"%d : %s;\n",it->IndexLine,it->BinaryMode);
          fprintf(AssemblyCode,"%s\n",it->AssemblyMode);
          it = it->next;
    }
}

void CorrectionMarks()
{
    AssemblyInst *it = AssemblyFirst;
    int im;
    char *im_;
    //printf("\033[01;33m"); printf("\n[Assembly Generator]\n"); printf("\033[0m\n");

    while(it!=NULL)
    {
          im = strlen(it->BinaryMode);
          if( im < 32 )
          {
                int k = SearchMark(it->Op3->Name);
                im_ = BinarizeMe(k,32-im);
                strcat(it->BinaryMode,im_);
          }
          it = it->next;
    }
}

void Processor_Configs()
{
      int PilhaArgumentos = 50;
      int ChamadaFuncao = 75;
      AssemblyOp Op1,Op2;
      char *InstructBin;

      // Ponteiro de pilha de argumentos
      Op1 = InsertOperandAssembly(1,TypeRegister(1));
      Op2 = InsertOperandAssembly(PilhaArgumentos,Int2String(PilhaArgumentos));
      InstructBin = TypeI(ADDI,AssemblyZero->Address,Op1->Address,Op2->Address);
      InsertAssembly(I,ADDI,Op1,AssemblyZero,Op2,InstructBin);

      // Ponteiro de chamada de função
      Op1 = InsertOperandAssembly(31,TypeRegister(31));
      Op2 = InsertOperandAssembly(ChamadaFuncao,Int2String(ChamadaFuncao));
      InstructBin = TypeI(ADDI,AssemblyZero->Address,Op1->Address,Op2->Address);
      InsertAssembly(I,ADDI,Op1,AssemblyZero,Op2,InstructBin);

      // Saltar para main
      AssemblyOp Main = InsertOperandAssembly(-1,"main");
      InstructBin = TypeJ(JUMP,Main->Address);
      InsertAssembly(J,JUMP,AssemblyNulo,AssemblyNulo,Main,InstructBin);

      //
}

void Assembly()
{
    StartArchives();
    AssemblyZero = InsertOperandAssembly(0,TypeRegister(0));
    AssemblyNulo = InsertOperandAssembly(0,"");
    Processor_Configs();
    AssemblyGenerator(IntermediaryFirst);
    CorrectionMarks();
    BinaryPrint();
    CloseArchives();
}

#include <stdlib.h>
#include <string.h>
#include "Globals.h"
#include "SymbolTab.h"
#include "CGen.h"
#include "CodeGeass.h"

/*static void cGen( TreeNode * tree);
static void genExpK( TreeNode * t);
static void genStmtK( TreeNode * tree);
static void genDeclK( TreeNode * t);*/

int i,ArrControl;
int CurrentInst;
char *CurrentFunction;
Operand CurrentOpK,OperadorVazio,OperadorZero;

/**********************************************************************/
static Operand AllocOp()
{
    Operand O = (Operand)malloc(sizeof(Operand));
    O->Local = NULL;
    return O;
}

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
      break;

      case Constant:
          Op->Variable = Int2String(constant);
      break;

      case Mark:
          Op->Variable = nameVariable;
      break;

      case Call_Value:
          Op->Variable = nameVariable;
      break;

      case Empty:
          Op->Variable = nameVariable;
      break;

      case ArrVariable:
          Op->Variable = nameVariable;
      break;

      case ArrEmpty:
          Op->Variable = nameVariable;
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
            CurrentOpK = InsertOperand(Constant,ShowMeTemporary(),t->attr.val);
        break;

        case IdK:
            CurrentOpK = InsertOperand(Variable,t->attr.name,t->attr.val);
        break;

        case TypeK:   break;

        case ArrIdK:
            genExpK(t->child[0]);
            Op1 = CurrentOpK;
            Op2 = InsertOperand(ArrVariable,t->attr.name,t->attr.val);
            CurrentOpK = InsertOperand(ArrEmpty,GiveMeTemporary(),Reg-1);
            if(Assign_Type){ PrintQuadruple(LOAD,CurrentOpK,Op2,Op1,"-- Fazendo leitura da memória p/ registrador"); }
            else{ PrintQuadruple(ADD,CurrentOpK,Op2,Op1,"-- Calculando endereço da memória"); }
        break;

        case CallK:   // Chamada de Função (Análise de Argumentos da Função
              args = 0;

              for(p1=t->child[0]; p1!=NULL;p1 = p1->sibling)
              {
                  genExpK(p1);
                  Op1 = InsertOperand(Empty,GiveMeArgs(),ARGS-1);
                  PrintQuadruple(MOVE,Op1,CurrentOpK,OperadorVazio,"");
                  CurrentOpK = Op1;
                  args++;
              }

              p1 = t;
              ResetArg();
              if( strcmp(t->attr.name,"output") != 0 )
              {
                  Op1 = InsertOperand(Variable,p1->attr.name,-1);
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

            genExpK(p1);
            if(p1->kind.exp == CalcK)
            { Op1 = InsertOperand(Empty,GiveMeTemporary(),-1); }
            else if (p1->kind.exp == CallK)
            { Op1 = InsertOperand(Mark,p1->attr.name,-1); }
            else { Op1 = CurrentOpK; }

            genExpK(p3);
            if(p3->kind.exp == CalcK)
            { Op2 = InsertOperand(Empty,GiveMeTemporary(),-1); }
            else if (p3->kind.exp == CallK)
            { Op2 = InsertOperand(Mark,p3->attr.name,-1); }
            else{ Op2 = CurrentOpK; }

            genExpK(p2);
            Op3 = InsertOperand(Empty,ShowMeTemporary(),-1);
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
              PrintQuadruple(STORE,op2,op1,OperadorZero,"");
              ArrControl++;
          }

          // Se manipulei vetores, então não percorre
          if(ArrControl == 0){ PrintQuadruple(MOVE,op1,op2,OperadorVazio,""); }

          Assign_Type = 1; // Renova atribuição
          break;

          case CompoundK:
              ResetArg(); // Quando iniciar declaração de variável, eu zero o registrador de argumentos
              for (i=0; i < MAXCHILDREN; i++) { cGen(tree->child[i]); }
          break;

          case ReturnK:
                  ResetTemp();
                  cGen(tree->child[0]); // Invoca Jump Register
                  PrintQuadruple(RETURNi,CurrentOpK,OperadorVazio,OperadorVazio,"");
          break;
          default: printf("Você me esqueceu parça stmt!\n"); break;
      }
}

static void genDeclK( TreeNode * t)
{
      BucketList l;
      Operand Op1,Op2,Op3;
      int Curegister;
      switch (t->kind.decl)
      {
            case ArrParamK:

              Op1 = InsertOperand(ArrVariable,t->attr.name,-1);
              Op2 = InsertOperand(Empty,GiveMeArgs(),-1);
              PrintQuadruple(MOVE,Op1,Op2,OperadorVazio,"-- Informando endereço de memória do vetor");

            break; // Util na atribuição de Registradores Gerais
            case ParamK:

              if(t->attr.type==Void) return;

              //Curegister = Reg;
              l = st_lookup_Full(t->attr.name,CurrentFunction);
              Op1 = InsertOperand(Variable,t->attr.name,l->memloc);
              Op2 = InsertOperand(Empty,GiveMeArgs(),-1);
              PrintQuadruple(MOVE,Op1,Op2,OperadorVazio,"-- Atribuindo argumentos da função");

            break; // Util na atribuição de Registradores Gerais
            case ArrVarK:

              l = st_lookup_Full(t->attr.arr.name,CurrentFunction);
              Op2 = InsertOperand(Empty,GiveMeGeral(),GeralReg-1);             // Registrador Zero
              Op1 = InsertOperand(ArrVariable,t->attr.arr.name,l->memloc); // Nome da variável
              PrintQuadruple(MOVE,Op2,Op1,OperadorVazio,"-- Reserva registrador geral para vetor");

            break; // Util na atribuição de Registradores Temporários
            case VarK:

              l = st_lookup_Full(t->attr.name,CurrentFunction);
              Op2 = InsertOperand(Empty,GiveMeGeral(),GeralReg-1);             // Registrador Zero
              Op1 = InsertOperand(Variable,t->attr.name,l->memloc); // Nome da variável
              PrintQuadruple(MOVE,Op2,Op1,OperadorVazio,"-- Reserva registrador geral");

            break; // Util na atribuição de Registradores Temporários
            case FunK:              // Percorre as funções presentes no programa
              ResetArg(); // Quando iniciar a função, eu zero o registrador de argumentos
              CurrentFunction = t->attr.name;
              Op1 = InsertOperand(Mark,t->attr.name,LineCode); // Nome de função
              PrintQuadruple(NOP,Op1,OperadorVazio,OperadorVazio,"-- Início da Função | Reservando rôtulo");
              for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); }
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
    printf("[Intermediary Code]\n");
    CurrentFunction = "Global";
    OperadorVazio = InsertOperand(Empty,"_",-1);
    OperadorZero  = InsertOperand(Empty,"0",0);
    cGen(syntaxTree);
    PrintQuadruple(HALT,OperadorVazio,OperadorVazio,OperadorVazio,"-- Fim do programa");
}

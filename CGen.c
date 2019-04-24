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

int i;
int CurrentInst;
char *CurrentFunction;
Operand CurrentOpK,OperadorVazio;

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

      default:
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
            //PrintQuadruple(LI,InsertOperand(Empty,ShowMeTemporary(),Reg),CurrentOpK,OperadorVazio,"");
            //CurrentInst = LI;
        break;

        case IdK:
            CurrentOpK = InsertOperand(Variable,t->attr.name,t->attr.val);
        break;

        case TypeK:   break;

        case ArrIdK:
            CurrentOpK = InsertOperand(Variable,t->attr.name,t->attr.val);
        break;
        case CallK:   // Chamada de Função (Análise de Argumentos da Função)
              args = 0;
              if(strcmp("input",t->attr.name)==0)
              { return; }

              for(p1=t->child[0]; p1!=NULL;p1 = p1->sibling)
              {
                  genExpK(p1);
                  if ( p1->kind.exp == IdK )
                  {
                      l = st_lookup_Full(p1->attr.name,CurrentFunction);
                      Op1 = InsertOperand(Variable,p1->attr.name,-1);
                      Op2 = InsertOperand(Empty,GiveMeArgs(),ARGS-1);
                      PrintQuadruple(MOVE,Op2,Op1,OperadorVazio,"");
                  }
                  else if( p1->kind.exp == ConstK )
                  {
                        Op1 = InsertOperand(Empty,GiveMeArgs(),ARGS-1);
                        Op2 = InsertOperand(Constant,Int2String(p1->attr.val),p1->attr.val);
                        PrintQuadruple(LI,Op1,Op2,OperadorVazio,"");
                  }
                  else if( p1->kind.exp == CallK )
                  {
                        Op1 = InsertOperand(Empty,GiveMeArgs(),ARGS-1);
                        Op2 = InsertOperand(Empty,p1->attr.name,-1);
                        PrintQuadruple(MOVE,Op1,Op2,OperadorVazio,"");
                  }
                  else
                  {
                        Op1 = InsertOperand(Empty,GiveMeArgs(),ARGS-1);
                        Op2 = InsertOperand(Empty,ShowMeTemporary(),Reg);
                        PrintQuadruple(MOVE,Op1,Op2,OperadorVazio,"");
                  }
                  args++;
              }

              p1 = t;
              if( strcmp(t->attr.name,"output") != 0 )
              {
                if( p1->kind.exp == CallK )
                {
                    Op1 = InsertOperand(Variable,p1->attr.name,-1);
                    Op2 = InsertOperand(Empty,TypeRegister(2),2);
                    PrintQuadruple(JAL,Op1,OperadorVazio,OperadorVazio,"");
                    //PrintQuadruple(MOVE,Op2,Op1,OperadorVazio,"");
                }

              }// Tratando caso em que a chamada é output
              else
              {
                  temp = t->child[0]->kind.exp;
                  switch(temp)
                  {
                    case CallK:
                        ARGS--;
                        Op1 = InsertOperand(Empty,GiveMeArgs(),ARGS-1);
                        PrintQuadruple(OUT,Op1,OperadorVazio,OperadorVazio,"");
                    break;

                    case IdK:
                        ARGS--;
                        Op1 = InsertOperand(Empty,GiveMeArgs(),ARGS-1);
                        PrintQuadruple(OUT,Op1,OperadorVazio,OperadorVazio,"");
                    break;

                    default:
                        printf("Exigimos mais\n");
                    break;
                  }
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
          PrintQuadruple(NOP,op2,OperadorVazio,OperadorVazio,"\t-- Início do Else");

          // ELSE
          cGen(p3);
          PrintQuadruple(NOP,op1,OperadorVazio,OperadorVazio,"-- Fim do Else");
          // Registrador de retorno da pilha

          break;

          case WhileK:

          break;

          case AssignK:

          p1 = tree->child[0];
          p2 = tree->child[1];

          cGen(p2);
          op2 = CurrentOpK;

          cGen(p1);
          op1 = CurrentOpK;

          if( p2->attr.name!=NULL && strcmp(p2->attr.name,"input") == 0 )
          {
              l = st_lookup_Full(p1->attr.name,CurrentFunction);
              op1 = InsertOperand(Variable,p1->attr.name,-1);
              PrintQuadruple(IN,op1,OperadorVazio,OperadorVazio,"");
          }
            else
            { PrintQuadruple(MOVE,op1,op2,OperadorVazio,""); }

          break;
          case CompoundK:
              ResetArg(); // Quando iniciar declaração de variável, eu zero o registrador de argumentos
              for (i=0; i < MAXCHILDREN; i++) { cGen(tree->child[i]); }
          break;
          case ReturnK:
                  cGen(tree->child[0]); // Invoca Jump Register
                  op2 = InsertOperand(Empty,ShowMeTemporary(),Reg);
                  temp = tree->child[0]->kind.exp;
                  if( temp == CallK )
                  {
                        op1 = InsertOperand(Mark,tree->child[0]->attr.name,-1);
                        //PrintQuadruple(JAL,op1,OperadorVazio,OperadorVazio,"");
                  }
                  if( temp == IdK )
                  { op1 = InsertOperand(Variable,tree->child[0]->attr.name,-1); }
                  if( temp == CalcK)
                  { op1 = InsertOperand(Empty,ShowMeTemporary(),-1); }
                  if( temp == Constant)
                  { op1 = InsertOperand(Constant,Int2String(tree->child[0]->attr.val),tree->child[0]->attr.val); }
                  PrintQuadruple(RETURNi,op1,OperadorVazio,OperadorVazio,"");
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
            case ArrParamK:  break; // Util na atribuição de Registradores Gerais
            case ParamK:

              if(t->attr.type!=Void)
              {
                  Curegister = Reg;
                  Op1 = InsertOperand(Variable,t->attr.name,-1);
                  Op2 = InsertOperand(Empty,GiveMeArgs(),-1);
                  PrintQuadruple(MOVE,Op1,Op2,OperadorVazio,"-- Atribuindo argumentos da função");
              }

            break; // Util na atribuição de Registradores Gerais
            case ArrVarK:

            break; // Util na atribuição de Registradores Temporários
            case VarK:

                l = st_lookup_Full(t->attr.name,CurrentFunction);
                Op2 = InsertOperand(Empty,GiveMeTemporary(),Reg-1);             // Registrador Zero
                Op1 = InsertOperand(Variable,t->attr.name,-1); // Nome da variável
                PrintQuadruple(MOVE,Op2,Op1,OperadorVazio,"-- Reserva registrador $s");

            break; // Util na atribuição de Registradores Temporários
            case FunK:              // Percorre as funções presentes no programa
                ResetArg(); // Quando iniciar a função, eu zero o registrador de argumentos
                CurrentFunction = t->attr.name;
                Op1 = InsertOperand(Mark,t->attr.name,-1); // Nome de função
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
    Start(&Stack);
    OperadorVazio = InsertOperand(Empty,"_",-1);
    PrintQuadruple(JUMP,InsertOperand(Mark,"main",-1),OperadorVazio,OperadorVazio,"-- Salta para main");
    cGen(syntaxTree);
    PrintQuadruple(HALT,OperadorVazio,OperadorVazio,OperadorVazio,"-- Fim do programa");
}

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
Operand CurrentOpK;

/**********************************************************************/
static Operand AllocOp()
{
    Operand O = (Operand)malloc(sizeof(Operand));
    O->Contents.LocalVar = NULL;
    return O;
}

static Operand InsertOperand(TypeOP expo,char *nameVariable,int constant)
{
    Operand Op = AllocOp();
    char Buffer[5];

    switch (expo)
    {
      case Variable:
          Op->kind = Variable;
          Op->Contents.Variable = nameVariable;
      break;

      case Constant:
          sprintf(Buffer,"%d",constant);
          Op->kind = Constant;
          Op->Contents.val = constant;
          Op->Contents.Variable = copyString(Buffer);
      break;

      default:
          Op->kind = Empty;
          Op->Contents.Variable = nameVariable;
          //Op->Contents.LocalVar = st_lookup_Type;
      break;
    }

    return Op;
}

static void genExpK( TreeNode * t)
{
    int loc,args;
    TreeNode *p1, *p2, *p3;
    Operand Op1,Op2,Op3;
    VarType temp;
    char Buffer[5],*CurrentReg;
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
            PrintQuadruple(LI,ShowMeTemporary(),CurrentOpK->Contents.Variable,"_");
            CurrentInst = LI;
        break;

        case IdK:

        CurrentOpK = InsertOperand(Variable,t->attr.name,t->attr.val);

        break;
        case TypeK:   break;

        case ArrIdK:  break;
        case CallK:   // Chamada de Função (Análise de Argumentos da Função)
              args = 0;
              for(p1=t->child[0];p1!=NULL;p1 = p1->sibling)
              {
                  genExpK(p1);
                  if( p1->attr.name != NULL)
                  { temp = st_lookup_Type(p1->attr.name); }
                  if( temp == FunctInt || temp == FunctVoid )
                  { PrintQuadruple(JAL,p1->attr.name,"_","_"); }
                    else if ( p1->kind.exp == IdK )
                    { PrintQuadruple(MOVE,"ArgsReg",p1->attr.name,"_"); }
                      else if( p1->kind.exp == ConstK )
                      {
                          sprintf(Buffer,"%d",p1->attr.val);
                          PrintQuadruple(LI,"ArgsReg",Buffer,"_");
                      }
                        else
                        {
                            PrintQuadruple(MOVE,"ArgsReg",ShowMeTemporary(),"_");
                        }
                  args++;
              }
              if( strcmp(t->attr.name,"output") == 0)
              { PrintQuadruple(OUT,t->child[0]->attr.name,"_","_"); }
        break;
        case CalcK:

            p1 = t->child[0];
            p2 = t->child[1];
            p3 = t->child[2];

            genExpK(p1);
            if(p1->kind.exp == CalcK)
            {
                  Op1 = InsertOperand(Empty,GiveMeTemporary(),-1);
            }
              else{ Op1 = CurrentOpK; }

            genExpK(p3);
            if(p3->kind.exp == CalcK)
            {
                  Op2 = InsertOperand(Empty,GiveMeTemporary(),-1);
            }
              else
              { Op2 = CurrentOpK; }

            genExpK(p2);
            Op3 = InsertOperand(Empty,ShowMeTemporary(),-1);

            PrintQuadruple(CurrentInst,Op3->Contents.Variable,Op1->Contents.Variable,Op2->Contents.Variable);

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
      char *CurrentReg,*CurrentLabel1,*CurrentLabel2;

      switch (tree->kind.stmt)
      {
          case IfK:

          p1 = tree->child[0];
          p2 = tree->child[1];
          p3 = tree->child[2];

          //IF
          CurrentReg = ShowMeTemporary();
          CurrentLabel1 = GiveMeLabel();
          CurrentLabel2 = GiveMeLabel();
          cGen(p1); /* teste lógico */
          PrintQuadruple(NOP,"_","_","_)\t\t -- if composition");
          PrintQuadruple(BEQ,CurrentReg,copyString("0"),CurrentLabel1);


          // ELSE
          cGen(p2);
          PrintQuadruple(JUMP,"_","_",CurrentLabel2);
          PrintQuadruple(NOP,CurrentLabel1,"_","_)\t\t -- else composition");

          cGen(p3);
          PrintQuadruple(NOP,CurrentLabel2,"_","_");

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
          op3 = NULL;

          PrintQuadruple(MOVE,p1->attr.name,p2->attr.name,"_");

          break;
          case CompoundK:
              for (i=0; i < MAXCHILDREN; i++) { cGen(tree->child[i]); }
          break;
          case ReturnK:
                  genExpK(tree->child[0]); // Invoca Jump Register
                  if( tree->child[0]->attr.name != NULL)
                  {
                      temp = st_lookup_Type(tree->child[0]->attr.name);
                      if( temp == FunctInt || temp == FunctVoid )
                      {
                        PrintQuadruple(JAL,tree->child[0]->attr.name,"_","_");
                      }
                      PrintQuadruple(JR,"_","_","_");
                  }
                    else{ }
          break;
          default: printf("Você me esqueceu parça stmt!\n"); break;
      }
}

static void genDeclK( TreeNode * t)
{
      switch (t->kind.decl)
      {
            case ArrParamK:  break; // Util na atribuição de Registradores Gerais
            case ParamK:

            if(t->attr.type!=Void)
            {
                PrintQuadruple(MOVE,t->attr.name,"ArgsReg","_");
            }

            break; // Util na atribuição de Registradores Gerais
            case ArrVarK:    break; // Util na atribuição de Registradores Temporários
            case VarK:

            PrintQuadruple(LOAD,t->attr.name,"MemAloc","_");

            break; // Util na atribuição de Registradores Temporários
            case FunK:              // Percorre as funções presentes no programa
                PrintQuadruple(NOP,"_","_",t->attr.name);
                for (i=0; i < MAXCHILDREN; i++){ cGen(t->child[i]); }
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
    cGen(syntaxTree);
}

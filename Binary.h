#include <string.h>

char *Opcodes[] = {"000000","000000","000000","000000","000000","000000",
                  "000001","000001","000001","000001","000010","000011",
                  "000100","slte","sbt","sbte","equal","diff",
                  "000101","000110","000111","001000","001001",
                  "001010","001011","001100","sll","srl","sgt",
                  "set","mod","jr","jal","li","001101",
                  "001110","call","halt","return"};

void Int2Bin(int Value,char *BinaryElement)
{
      if(Value!=0)
      {
            char r[5];
            int b = Value/2;
            sprintf(r,"%d",Value%2);
            Int2Bin(b,BinaryElement);
            strcat(BinaryElement,r);
      }
}

char *BinarizeMe(int Value, int Space)
{
      char *String = (char*)malloc(Space*sizeof(char));
      Int2Bin(Value,String);
      long int n = strlen(String);
      char *Zeros = (char*)malloc(n*sizeof(char));
      while(n<Space)
      {
          strcat(Zeros,"0");
          n++;
      }
      strcat(Zeros,String);
      free(String);
      return Zeros;
}

char *TypeR(int Opcode,int rs, int rt, int rd, int funct)
{
      char *Instruction = (char*)malloc(33*sizeof(char));
      char *funct_ = BinarizeMe(funct,6);
      char *shamt_ = BinarizeMe(0,5);
      char *rd_    = BinarizeMe(rd,5);
      char *rt_    = BinarizeMe(rt,5);
      char *rs_    = BinarizeMe(rs,5);
      strcat(Instruction,funct_); free(funct_);
      strcat(Instruction,shamt_); free(shamt_);
      strcat(Instruction,rd_);    free(rd_);
      strcat(Instruction,rs_);    free(rs_);
      strcat(Instruction,rt_);    free(rt_);
      strcat(Instruction,Opcodes[Opcode]);
      return Instruction;
}

char *TypeI(int Opcode,int rs, int rt, int Imediato)
{
      char *Instruction = (char*)malloc(33*sizeof(char));
      char *Imediato_ = BinarizeMe(Imediato,16);
      char *rt_ = BinarizeMe(rt,5);
      char *rs_ = BinarizeMe(rs,5);
      strcat(Instruction,Imediato_); free(Imediato_);
      strcat(Instruction,rt_);       free(rt_);
      strcat(Instruction,rs_);       free(rs_);
      strcat(Instruction,Opcodes[Opcode]);
      return Instruction;
}

char *TypeJ(int Opcode, int Imediato)
{
      char *Instruction = (char*)malloc(33*sizeof(char));
      char *Imediato_ = BinarizeMe(Imediato,26);
      strcat(Instruction,Imediato_); free(Imediato_);
      strcat(Instruction,Opcodes[Opcode]);
      return Instruction;
}

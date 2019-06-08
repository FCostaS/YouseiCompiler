#include <string.h>

char *Opcodes[] = {"000000_","000000_","000000_","000000_","000000_","000000_",
                  "000001_","000001_","000001_","000001_","000010_","000011_",
                  "000100_","slte","sbt","sbte","equal","diff",
                  "000101_","000110_","000111_","001000_","001001_",
                  "001010_","001011_","001100_","sll","srl","sgt",
                  "set","mod","jr","jal","li","001101_",
                  "001110_","call","halt","return"};

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
      char *Zeros = (char*)malloc(Space*sizeof(char));
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
      char *Instruction = (char*)malloc(34*sizeof(char));
      char *funct_ = BinarizeMe(funct,6);
      char *shamt_ = BinarizeMe(0,5);
      char *rd_    = BinarizeMe(rd,5);
      char *rt_    = BinarizeMe(rt,5);
      char *rs_    = BinarizeMe(rs,5);
      strcat(Instruction,Opcodes[Opcode]);
      strcat(Instruction,rt_);    //free(rt_);
      strcat(Instruction,rs_);    //free(rs_);
      strcat(Instruction,rd_);    //free(rd_);
      strcat(Instruction,shamt_); //free(shamt_ );
      strcat(Instruction,funct_); //free(funct_);

      return Instruction;
}

char *TypeI(int Opcode,int rs, int rt, int Imediato)
{
      char *Instruction = (char*)malloc(34*sizeof(char));
      char *Imediato_ = BinarizeMe(Imediato,16);
      char *rt_ = BinarizeMe(rt,5);
      char *rs_ = BinarizeMe(rs,5);

      strcat(Instruction,Opcodes[Opcode]);
      strcat(Instruction,rs_);       //free(rs_);
      strcat(Instruction,rt_);       //free(rt_);

      if( Imediato != -1 )
      {
          strcat(Instruction,Imediato_); //free(Imediato_);
      }
        else
        {
            if( (Opcode != BEQ) && (Opcode != BNE) )
            {
                strcat(Instruction,Imediato_); //free(Imediato_);
            }
        }
      return Instruction;
}

char *TypeJ(int Opcode, int Imediato)
{
      char *Instruction = (char*)malloc(34*sizeof(char));
      char *Imediato_ = BinarizeMe(Imediato,26);
      strcat(Instruction,Opcodes[Opcode]);
      if( Imediato != -1 )
      {
          strcat(Instruction,Imediato_); //free(Imediato_);
      }
      return Instruction;
}

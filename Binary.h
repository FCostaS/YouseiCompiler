#include <string.h>

char *Opcodes[] = {"000000","000000","000000","000000","000000","000000",
                  "000001","000001","000001","000001","000010","000011",
                  "000100","010010","001111","010001","010000","diff",
                  "000101","000110","000111","001000","001001",
                  "001010","001011","001100","sll","srl","sgt",
                  "set","mod","010011","jal","li","001101",
                  "001110","call","halt","return","010100"};

/*
"000000", ADD  (OK)
"000000", SUB  (OK)
"000000", MULT (OK)
"000000", DIV  (OK)
"000000", INC   XX
"000000", DEC   XX
"000001", AND   XX
"000001", OR    XX
"000001", NOT   XX
"000001", XOR   XX
"000010", ADDI  (OK)
"000011", MOVE  (OK)
"000100", SLt   (OK)
"010010", slte
"001111", sbt
"010001", sbte
"010000", equal
"diff",  diff
"000101",JUMP  (OK)
"000110", LOAD (OK)
"000111", STORE (OK)
"001000", IN    (OK)
"001001", OUT   (OK)
"001010", BEQ   (OK)
"001011", BNE   XX
"001100", NOP   (OK)
"sll",    SLL
"srl",    SRL
"sgt",    SGT
"set",    SET
"mod",    MOD
"010011", JR   (--)
"jal",    JAL
"li",     LI
"001101", PUSH XX
"001110", POP  XX
"call",   CALL
"halt",   HALT
"return", RETURN
"010100", SUBI
};
*/


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
      strcat(Instruction,Opcodes[Opcode]);
      if( Imediato != -1 )
      {
          char *Imediato_ = BinarizeMe(Imediato,26);
          strcat(Instruction,Imediato_); //free(Imediato_);
      }
      return Instruction;
}

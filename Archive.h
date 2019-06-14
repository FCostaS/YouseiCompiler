#include <stdio.h>

FILE *AssemblyCode, *IntermediaryCode,*BinaryCode,*Full;

void Configs_MIFS()
{
      fprintf(BinaryCode, "-- begin_signature\n");
      fprintf(BinaryCode, "-- single_port_rom\n");
      fprintf(BinaryCode, "-- end_signature\n");
      fprintf(BinaryCode, "WIDTH=32;\n");
      fprintf(BinaryCode, "DEPTH=1024;\n");
      fprintf(BinaryCode, "\n");
      fprintf(BinaryCode, "ADDRESS_RADIX=UNS;\n");
      fprintf(BinaryCode, "DATA_RADIX=BIN;\n");
      fprintf(BinaryCode, "\n");
      fprintf(BinaryCode, "CONTENT BEGIN\n");
}

void StartArchives()
{
      BinaryCode       = fopen("Codigos/Object.txt","w");
      IntermediaryCode = fopen("Codigos/Intermediary.txt","w");
      AssemblyCode     = fopen("Codigos/Assembly.txt","w");
      Full             = fopen("Codigos/MemoryTest.txt","w");
      Configs_MIFS();
}

void CloseArchives()
{
    fprintf(BinaryCode, "END;\n");
    fclose(AssemblyCode);
    fclose(IntermediaryCode);
    fclose(BinaryCode);
    fclose(Full);
}

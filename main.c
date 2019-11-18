#include "CodigoFonte.h" // Biblioteca com as variaveis e funcoes do codigo fonte

int main(int argc, char **argv)
{
    if(argc > 1)
    {
      RunCompiler(argv[1]);
    }else{
      printf("Use ./Yousei <codigo.yousei>");
    }
}

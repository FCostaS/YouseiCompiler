#define NReg 16

typedef struct{
    int Reg[NReg];
    int top;
    int block;
}Stack;

void StartStack(Stack *MyStack);
void BlockStack(Stack *MyStack);
void UnblockStack(Stack *MyStack);
int Pop(Stack *MyStack);
void Push(Stack *MyStack);

/* Eu inicializo a pilha com os registradores
O item block é para caso eu impeça a remoção de um registrador
na pilha, isso é apenas para atribuir o mesmo registrador à
operação */
void StartStack(Stack *MyStack){
    int n = NReg;

    while(n >= 0)
    {
        n--;
        MyStack->Reg[n] = n;
    }
    MyStack->block = 0;
    MyStack->top  = NReg-1;
}

void lockStack(Stack *MyStack)
{
    MyStack->block = 1;
}

void UnlockStack(Stack *MyStack)
{
    MyStack->block = 0;
}

/* Como havia dito, se a pilha está bloqueada, eu apenas
envio o valor do registrador atual */
int Pop(Stack *MyStack)
{
    if(MyStack->block == 1)
    {
        return MyStack->Reg[MyStack->top];
    }
      else if( MyStack->top > 0)
      {
          MyStack->top--;
          return MyStack->Reg[MyStack->top+1];
      }
}

void Push(Stack *MyStack)
{
    if(MyStack->top == NReg-1)
    {
        printf("Não há registradores para adicionar\n");
        /* Em tese, isso nunca deve acontecer
        Se acontecer, é sinal que há um bug na minha
        lógica, então devo ficar esperto */
    }
      else
      {
          MyStack->top++;
          MyStack->Reg[MyStack->top] = MyStack->top;
      }
}

/*************************************/

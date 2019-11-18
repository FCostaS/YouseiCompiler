int QtdProcessos;
int PonteiroHD; /* END_HD faz o store do registrador que a BIOS usou nessa posicao que o compilador ira gerar */
int ProcessoCorrente;
int Processos[10];

void Contexto(void) {
	/* Salva contexto do SO */
	MANUAL(); /* $3 = 500; */
	MANUAL(); /* store $1,0($3); */
	MANUAL(); /* store $2,1($3); */
	MANUAL(); /* store $3,2($3); */
	MANUAL(); /* store $4,3($3); */
	MANUAL(); /* store $5,4($3); */
	MANUAL(); /* store $6,5($3); */
	MANUAL(); /* store $7,6($3); */
	MANUAL(); /* store $8,7($3); */
	MANUAL(); /* store $9,8($3); */
	MANUAL(); /* store $10,9($3); */
	MANUAL(); /* store $11,10($3); */
	MANUAL(); /* store $12,11($3); */
	MANUAL(); /* store $13,12($3); */
	MANUAL(); /* store $14,13($3); */
	MANUAL(); /* store $15,14($3); */
	MANUAL(); /* store $16,15($3); */
	MANUAL(); /* store $17,16($3); */
	MANUAL(); /* store $18,17($3); */
	MANUAL(); /* store $19,18($3); */
	MANUAL(); /* store $20,19($3); */
	MANUAL(); /* store $21,20($3); */
	MANUAL(); /* store $22,21($3); */
	MANUAL(); /* store $23,22($3); */
	MANUAL(); /* store $24,23($3); */
	MANUAL(); /* store $25,24($3); */
	MANUAL(); /* store $26,25($3); */
	MANUAL(); /* store $27,26($3); */
	MANUAL(); /* store $28,27($3); */
	MANUAL(); /* store $30,28($3); */
	MANUAL(); /* store $31,29($3); */

	/* Ler contexto do Processo */
	MANUAL(); /* $3 = 30*ProcessoCorrente */
	MANUAL(); /* $3 = 500 + $3; */
	MANUAL(); /* load $1,0($3); */
	MANUAL(); /* load $2,1($3); */
	MANUAL(); /* load $3,2($3); */
	MANUAL(); /* load $4,3($3); */
	MANUAL(); /* load $5,4($3); */
	MANUAL(); /* load $6,5($3); */
	MANUAL(); /* load $7,6($3); */
	MANUAL(); /* load $8,7($3); */
	MANUAL(); /* load $9,8($3); */
	MANUAL(); /* load $10,9($3); */
	MANUAL(); /* load $11,10($3); */
	MANUAL(); /* load $12,11($3); */
	MANUAL(); /* load $13,12($3); */
	MANUAL(); /* load $14,13($3); */
	MANUAL(); /* load $15,14($3); */
	MANUAL(); /* load $16,15($3); */
	MANUAL(); /* load $17,16($3); */
	MANUAL(); /* load $18,17($3); */
	MANUAL(); /* load $19,18($3); */
	MANUAL(); /* load $20,19($3); */
	MANUAL(); /* load $21,20($3); */
	MANUAL(); /* load $22,21($3); */
	MANUAL(); /* load $23,22($3); */
	MANUAL(); /* load $24,23($3); */
	MANUAL(); /* load $25,24($3); */
	MANUAL(); /* load $26,25($3); */
	MANUAL(); /* load $27,26($3); */
	MANUAL(); /* load $28,27($3); */
	MANUAL(); /* load $30,28($3); */
	MANUAL(); /* load $31,29($3); */

	/* Inicia processo que sera executado e inicia temporizador */
	MANUAL(); /* SetPID(ProcessoCorrente); Set_PID é um load na posicao alocada para ProcessoCorrente */
	MANUAL(); /* StartTemporizer(); Temporizer é um NOP que indica ao SO para iniciar o temporizador */

	/* Salva contexto do processo */
	MANUAL(); /* $3 = 30*ProcessoCorrente */
	MANUAL(); /* $3 = 500 + $3; */
	MANUAL(); /* store $1,0($3); */
	MANUAL(); /* store $2,1($3); */
	MANUAL(); /* store $3,2($3); */
	MANUAL(); /* store $4,3($3); */
	MANUAL(); /* store $5,4($3); */
	MANUAL(); /* store $6,5($3); */
	MANUAL(); /* store $7,6($3); */
	MANUAL(); /* store $8,7($3); */
	MANUAL(); /* store $9,8($3); */
	MANUAL(); /* store $10,9($3); */
	MANUAL(); /* store $11,10($3); */
	MANUAL(); /* store $12,11($3); */
	MANUAL(); /* store $13,12($3); */
	MANUAL(); /* store $14,13($3); */
	MANUAL(); /* store $15,14($3); */
	MANUAL(); /* store $16,15($3); */
	MANUAL(); /* store $17,16($3); */
	MANUAL(); /* store $18,17($3); */
	MANUAL(); /* store $19,18($3); */
	MANUAL(); /* store $20,19($3); */
	MANUAL(); /* store $21,20($3); */
	MANUAL(); /* store $22,21($3); */
	MANUAL(); /* store $23,22($3); */
	MANUAL(); /* store $24,23($3); */
	MANUAL(); /* store $25,24($3); */
	MANUAL(); /* store $26,25($3); */
	MANUAL(); /* store $27,26($3); */
	MANUAL(); /* store $28,27($3); */
	MANUAL(); /* store $30,28($3); */
	MANUAL(); /* store $31,29($3); */

	/* Ler contexto do SO */
	MANUAL(); /* $3 = 500; */
	MANUAL(); /* load $1,0($3); */
	MANUAL(); /* load $2,1($3); */
	MANUAL(); /* load $3,2($3); */
	MANUAL(); /* load $4,3($3); */
	MANUAL(); /* load $5,4($3); */
	MANUAL(); /* load $6,5($3); */
	MANUAL(); /* load $7,6($3); */
	MANUAL(); /* load $8,7($3); */
	MANUAL(); /* load $9,8($3); */
	MANUAL(); /* load $10,9($3); */
	MANUAL(); /* load $11,10($3); */
	MANUAL(); /* load $12,11($3); */
	MANUAL(); /* load $13,12($3); */
	MANUAL(); /* load $14,13($3); */
	MANUAL(); /* load $15,14($3); */
	MANUAL(); /* load $16,15($3); */
	MANUAL(); /* load $17,16($3); */
	MANUAL(); /* load $18,17($3); */
	MANUAL(); /* load $19,18($3); */
	MANUAL(); /* load $20,19($3); */
	MANUAL(); /* load $21,20($3); */
	MANUAL(); /* load $22,21($3); */
	MANUAL(); /* load $23,22($3); */
	MANUAL(); /* load $24,23($3); */
	MANUAL(); /* load $25,24($3); */
	MANUAL(); /* load $26,25($3); */
	MANUAL(); /* load $27,26($3); */
	MANUAL(); /* load $28,27($3); */
	MANUAL(); /* load $30,28($3); */
	MANUAL(); /* load $31,29($3); */
}

void InserirProcesso(int i) {
	Processos[ QtdProcessos ] = i + 1; /* Processo 0 é o SO */
	QtdProcessos = QtdProcessos + 1;
}

int mod(int u, int v) {
	return (u-u/v*v);
}

void RoundRobinRun(void) {

	int i;

	while( QtdProcessos > 0 ) {
		i = i + 1;
		ProcessoCorrente = Processos[ mod(i, QtdProcessos) ];
		Contexto();
	}

}


void main(void) {
	int k;

	InserirProcesso(0);
	InserirProcesso(1);
	InserirProcesso(2);
	RoundRobinRun();

}

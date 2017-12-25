/***********************************************
*INF1019 - Sistemas de Computação***************
*                                              *
*Trabalho 02 - Gerenciador de Memoria Worst Fit*
*                                              *
*Professor: Luiz Fernando Seibel               *
*                                              *
*Alunos:	Fernando Homem da Costa - 1211971  *
*			Mateus Ribeiro de Castro - 1213068 *
*                                              *
************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

/*******************Declaração de variável global*******************/
int tempoTotal;
/*******************Fim: Declaração de variável global*******************/

/***********************************************************************
*																	   *
*  $TC Tipo de dados: Process, ListOfProcesses e Memory                *
*                                                                      *
*                                                                      *
*  $ED Descrição do tipo                                               *
*     Descreve a estrutura process, listofprocesses e memory           *
*                                                                      *
***********************************************************************/
struct process{
	int Number; //Number that identifies the process.
	int Memory; //amount of memory we are going to use for each process.
	int qtInfo; //quantity of information 
	int *TimeExec; //pointer: time of execution, its necessary in every process.
	int *TimeIO; //pointer: time of IO, its not necessary in every process.
	int order; //will establish the order, -1 = NULL, 1,2,3,4.. = Processes
	int partition; // ONLY WORKS FOR MEMORY!
};
typedef struct process Process;

struct listofprocesses{
	int quantity; //numbers of processes that will be on the list.
	Process *proc; //pointer for the next process.
};
typedef struct listofprocesses ListOfProcesses;

struct memory{
	int mOrder;
	ListOfProcesses *LMemory;
};
typedef struct memory Memory;
/************************Fim da Declaração de Tipos de Dadosr***********************/

/****************************Inicio de Declaração de Funções****************************/

/***********************************************************************
*                                                                      *
*  $FC Função: FilaReader                                              *
*                                                                      *
*                                                                      *
***********************************************************************/
ListOfProcesses* FileReader(FILE *pEntry, int *TamReady){//This function will read all the information and fill the processes and listofprocesses. -----> OK! FUNCIONANDO!
	ListOfProcesses *Lprocesses = NULL;
	int pNprocesses = 0;
	int i, /*k*/ j;
	int	cExec;
	int cIO;
	char catcher = '\0';

	fscanf(pEntry, "%d\n", &pNprocesses); //saving the number of processes to do malloc.

	Lprocesses = (ListOfProcesses*)malloc(1 * sizeof(ListOfProcesses));

	Lprocesses->proc = (Process*)malloc((pNprocesses)*sizeof(Process));
	for (i = 0; i < pNprocesses; i++){
		Lprocesses->proc[i].Memory = 0;
		Lprocesses->proc[i].Number = 0;
		Lprocesses->proc[i].order = 0;
		Lprocesses->proc[i].partition = 0;
		Lprocesses->proc[i].qtInfo = 0;
	}

	(*TamReady) = pNprocesses;

	if (Lprocesses->proc == NULL){
		printf("Erro Lprocesses->proc:  Malloc\n");
	}

	Lprocesses->quantity = pNprocesses;

	for (i = 0; i < (Lprocesses->quantity); i++){
		cExec = 0;
		cIO = 0;

		fscanf(pEntry, "Processo#%d - %dMb\n", &Lprocesses->proc[i].Number, &Lprocesses->proc[i].Memory);

		fscanf(pEntry, "%d\n", &Lprocesses->proc[i].qtInfo);
		Lprocesses->proc[i].TimeExec = (int*)malloc(((Lprocesses->proc[i].qtInfo / 2) + 1)*sizeof(int));
		if (Lprocesses->proc[i].TimeExec == NULL){
			printf("Erro Lprocesses->proc[i].TimeExec:  Malloc\n");
		}

		if (Lprocesses->proc[i].qtInfo != 1){
			Lprocesses->proc[i].TimeIO = (int*)malloc(((Lprocesses->proc[i].qtInfo / 2))*sizeof(int));
			if (Lprocesses->proc[i].TimeIO == NULL){
				printf("Erro Lprocesses->proc[i].TimeIO:  Malloc\n");
			}
		}

		else{
			Lprocesses->proc[i].TimeIO = (int*)malloc((1)*sizeof(int));
			if (Lprocesses->proc[i].TimeIO == NULL){
				printf("Erro Lprocesses->proc[i].TimeIO:  Malloc\n");
			}
			Lprocesses->proc[i].TimeIO[0] = 0;
		}

		for (j = 0; j < (Lprocesses->proc[i].qtInfo); j++){

			fscanf(pEntry, "%c", &catcher);

			switch (catcher){
			case 'e':
				fscanf(pEntry, "xec ");
				fscanf(pEntry, "%d\n", &Lprocesses->proc[i].TimeExec[cExec]);
				cExec++;
				break;

			case 'i':
				fscanf(pEntry, "o ");
				fscanf(pEntry, "%d\n", &Lprocesses->proc[i].TimeIO[cIO]);
				cIO++;
				break;

			}

		}

	}

	return Lprocesses;
}
/************************Fim função: FileReader***********************/


/***********************************************************************
*                                                                      *
*  $FC Função: PrintProcess                                            *
*                                                                      *
*                                                                      *
***********************************************************************/
void PrintProcess(Process *pProcess, FILE *Output){
	int i;
	int TimeExec = 0;
	int TimeIO = 0;

	printf("\n----------------Imprimindo Informacoes do Processo----------------\n");
	printf("Processo : %d\n", pProcess->Number);
	fprintf(Output, "Processo : %d\n", pProcess->Number);

	for (i = 0; i < (pProcess->qtInfo / 2); i++){
		TimeExec = TimeExec + pProcess->TimeExec[i];
		TimeIO = TimeIO + pProcess->TimeIO[i];
	}

	TimeExec = TimeExec + pProcess->TimeExec[i];

	printf("Left: TimeExec : %d ---------- TimeIO: %d\n", TimeExec, TimeIO);
	fprintf(Output, "Tempo Restante: %d\n", TimeExec + TimeIO);
	fprintf(Output, "exec: %d ----- io: %d\n", TimeExec, TimeIO);

	printf("\n----------------Terminou de Imprimir Informacoes do Processo----------------\n");
}
/***********************Fim função: PrintProcess**********************/


/***********************************************************************
*                                                                      *
*  $FC Função: PrintListOfProcesses                                    *
*                                                                      *
*                                                                      *
***********************************************************************/
void PrintListOfProcesses(ListOfProcesses *LGeneric, FILE *Output){
	int i, j;
	int TimeExec;
	int TimeIO;

	printf("\n----------------Imprimindo Lista de Processos----------------\n");

	for (i = 0; i < LGeneric->quantity; i++){
		TimeExec = 0;
		TimeIO = 0;
		printf("Processo : %d ---------- Memoria: %d\n", LGeneric->proc[i].Number, LGeneric->proc[i].Memory);
		printf("Order: %d ---------- Partition: %d\n", LGeneric->proc[i].order, LGeneric->proc[i].partition);
		printf("qtInfo : %d\n", LGeneric->proc[i].qtInfo);
		for (j = 0; j < ((LGeneric->proc[i].qtInfo / 2) + 1); j++){
			TimeExec = LGeneric->proc[i].TimeExec[j] + TimeExec;
		}
		for (j = 0; j < ((LGeneric->proc[i].qtInfo / 2)); j++){
			TimeIO = LGeneric->proc[i].TimeIO[j] + TimeIO;
		}
		fprintf(Output, "Processo : %d\n", LGeneric->proc[i].Number);
		fprintf(Output, "Tempo Restante: %d\n", (TimeExec + TimeIO));
		fprintf(Output, "exec: %d ----- io: %d\n", TimeExec, TimeIO);
	}
	printf("\n----------------Terminou de Imprimir Lista de Processos----------------\n");
}
/********************Fim função: PrintListOfProcesses******************/


/***********************************************************************
*                                                                      *
*  $FC Função: PrintMemory                                             *
*                                                                      *
*                                                                      *
***********************************************************************/
void PrintMemory(Memory *pMemory, FILE *Output){
	int i, j;
	int TimeExec;
	int TimeIO;

	printf("\n----------------Imprimindo Mapa da Memoria----------------\n");

	for (i = 0; i < 5; i++){
		TimeExec = 0;
		TimeIO = 0;

		if (pMemory->LMemory->proc[i].order != -1){
			printf("Particao: %dMb ---------- Processo: %d\n", pMemory->LMemory->proc[i].partition, pMemory->LMemory->proc[i].Number);
			fprintf(Output, "Memória: %dMb ---------- Processo: %d\n", pMemory->LMemory->proc[i].partition, pMemory->LMemory->proc[i].Number);
			for (j = 0; j < ((pMemory->LMemory->proc[i].qtInfo / 2) + 1); j++){
				TimeExec = pMemory->LMemory->proc[i].TimeExec[j] + TimeExec;
			}
			for (j = 0; j < ((pMemory->LMemory->proc[i].qtInfo / 2)); j++){
				TimeIO = pMemory->LMemory->proc[i].TimeIO[j] + TimeIO;
			}
			fprintf(Output, "Tempo Restante: %d ---------- Exec: %d ---------- IO: %d\n", (TimeExec + TimeIO), TimeExec, TimeIO);
		}

		else{
			printf("Particao: %dMb ---------- Processo: EMPTY/VAZIO\n", pMemory->LMemory->proc[i].partition);
			fprintf(Output, "Memória: %dMb ---------- Processo: NULL\n", pMemory->LMemory->proc[i].partition);
		}
	}

}
/************************Fim função: PrintMemory***********************/


/***********************************************************************
*                                                                      *
*  $FC Função: BuscaMaiorOrdem                                         *
*                                                                      *
*                                                                      *
***********************************************************************/
int BuscaMaiorOrdem(Memory *pMemory){
	int i;
	int order = pMemory->mOrder;
	int index;

	for (i = 0; i < 5; i++){
		if ((order <= pMemory->LMemory->proc[i].order) && (pMemory->LMemory->proc[i].order != -1)){
			order = pMemory->LMemory->proc[i].order;
			index = i;
		}
	}
	return index;
}
/**********************Fim função: BuscaMaiorOrdem********************/


/***********************************************************************
*                                                                      *
*  $FC Função: BuscaFHMR		                                       *
*                                                                      *
*                                                                      *
***********************************************************************/
int BuscaFHMR(Memory *pMemory){
	int i;
	int order = pMemory->mOrder;
	int index, indexAux = 0;
	int counter = 0, counter2 = 0;

	for (i = 0; i < 5; i++){
		if (pMemory->LMemory->proc[i].order != -1){
			if (order >= pMemory->LMemory->proc[i].order){
				order = pMemory->LMemory->proc[i].order;
				index = i;
				counter2++;
			}
			else{
				counter++;
				indexAux = i;
			}
		}
	}

	if (counter > 0 && counter2 == 0)
		index = indexAux;

	return index;
}
/**********************Fim função: BuscaFHMR**************************/


/***********************************************************************
*                                                                      *
*  $FC Função: CheckList		                                       *
*                                                                      *
*                                                                      *
***********************************************************************/
int CheckList(ListOfProcesses *LGeneric){//Returns 1 or -1 

	if (LGeneric->quantity > 0){
		return 1;
	}

	else{//LGeneric->quantity <= 0
		return -1;
	}

}
/***********************Fim função: CheckList*************************/


/***********************************************************************
*                                                                      *
*  $FC Função: CheckMemory		                                       *
*                                                                      *
*                                                                      *
***********************************************************************/
int CheckMemory(Memory *pMemory){//Check on the memory the biggest consecutive space avaiable.
	int BiggestSpace = 0;
	int i;

	for (i = 0; i < 5; i++){//OBS: Lembrar que dividimos a memoria em 5 partições(8,4,2,1,1).
		if (pMemory->LMemory->proc[i].order == -1){//OBS: Convencionamos que o espaço vazio tem ordem -1.

			if (pMemory->LMemory->proc[i].partition > BiggestSpace){

				BiggestSpace = pMemory->LMemory->proc[i].partition;

			}
		}
	}

	return BiggestSpace;

}
/***********************Fim função : CheckMemory************************/


/***********************************************************************
*                                                                      *
*  $FC Função: FinishedCreator                                         *
*                                                                      *
*                                                                      *
***********************************************************************/
ListOfProcesses* FinishedCreator(int TamReady){
	ListOfProcesses *LFinished;

	LFinished = (ListOfProcesses*)malloc(1 * sizeof(ListOfProcesses));
	LFinished->proc = (Process*)malloc(TamReady * sizeof(Process));
	LFinished->quantity = 0;

	return LFinished;
}
/***********************Fim função: FinishedCreator************************/


/***********************************************************************
*                                                                      *
*  $FC Função: BlockCreator                                            *
*                                                                      *
*                                                                      *
***********************************************************************/
ListOfProcesses* BlockCreator(int TamReady){
	ListOfProcesses *LBlock;

	LBlock = (ListOfProcesses*)malloc(1 * sizeof(ListOfProcesses));
	LBlock->proc = (Process*)malloc(TamReady * sizeof(Process));
	LBlock->quantity = 0;

	return LBlock;
}
/***********************Fim função: BlockCreator************************/


/***********************************************************************
*                                                                      *
*  $FC Função: MemoryCreator                                           *
*                                                                      *
*                                                                      *
***********************************************************************/
Memory* MemoryCreator(void){
	Memory *LCreator = NULL;
	int i;

	LCreator = (Memory*)malloc(1 * sizeof(Memory));
	LCreator->LMemory = (ListOfProcesses*)malloc(1 * sizeof(ListOfProcesses));
	LCreator->LMemory->proc = (Process*)malloc(5 * sizeof(Process));
	LCreator->mOrder = 0;

	for (i = 0; i < 5; i++){
		LCreator->LMemory->proc[i].order = -1;
		LCreator->LMemory->proc[i].Memory = 0;
		LCreator->LMemory->proc[i].Number = 0;
		LCreator->LMemory->proc[i].qtInfo = 0;
	}

	LCreator->LMemory->proc[0].partition = 8;
	LCreator->LMemory->proc[1].partition = 4;
	LCreator->LMemory->proc[2].partition = 2;
	LCreator->LMemory->proc[3].partition = 1;
	LCreator->LMemory->proc[4].partition = 1;

	return LCreator;
}
/***********************Fim função: MemoryCreator************************/


/***********************************************************************
*                                                                      *
*  $FC Função: ReadyToMemory -> WORST FIT                              *
*                                                                      *
*                                                                      *
***********************************************************************/
void ReadyToMemory(ListOfProcesses *Lprocesses, Memory *Memory){// WORST FIT
	int partAux;
	int i;

	if ((Lprocesses->proc[0].Memory > 4) && (Lprocesses->proc[0].Memory <= 8)){
		if (Memory->LMemory->proc[0].order == -1){ //proc[0]->8Mb e order == -1->vazio
			partAux = Memory->LMemory->proc[0].partition;
			Memory->LMemory->proc[0] = Lprocesses->proc[0];
			Memory->LMemory->proc[0].partition = partAux; //mantendo a partição fixa da memória
			Memory->mOrder = Memory->mOrder + 1;
			Memory->LMemory->quantity = Memory->LMemory->quantity + 1;
			Lprocesses->quantity = Lprocesses->quantity - 1;
			Lprocesses->proc[0].order = -1;
			Memory->LMemory->proc[0].order = Memory->mOrder;
		}
	}

	else if ((Lprocesses->proc[0].Memory > 2) && (Lprocesses->proc[0].Memory <= 4)){
		if (Memory->LMemory->proc[0].order == -1){
			partAux = Memory->LMemory->proc[0].partition;
			Memory->LMemory->proc[0] = Lprocesses->proc[0];
			Memory->LMemory->proc[0].partition = partAux; //mantendo a partição fixa da memória
			Memory->mOrder = Memory->mOrder + 1;
			Memory->LMemory->quantity = Memory->LMemory->quantity + 1;
			Lprocesses->quantity = Lprocesses->quantity - 1;
			Lprocesses->proc[0].order = -1;
			Memory->LMemory->proc[0].order = Memory->mOrder;
		}
		else if (Memory->LMemory->proc[1].order == -1){ //proc[0]->8Mb e order == -1->vazio
			partAux = Memory->LMemory->proc[1].partition;
			Memory->LMemory->proc[1] = Lprocesses->proc[0];
			Memory->LMemory->proc[1].partition = partAux; //mantendo a partição fixa da memória
			Memory->mOrder = Memory->mOrder + 1;
			Memory->LMemory->quantity = Memory->LMemory->quantity + 1;
			Lprocesses->quantity = Lprocesses->quantity - 1;
			Lprocesses->proc[0].order = -1;
			Memory->LMemory->proc[1].order = Memory->mOrder;
		}
	}

	else if (Lprocesses->proc[0].Memory == 2){
		if (Memory->LMemory->proc[0].order == -1){
			partAux = Memory->LMemory->proc[0].partition;
			Memory->LMemory->proc[0] = Lprocesses->proc[0];
			Memory->LMemory->proc[0].partition = partAux; //mantendo a partição fixa da memória
			Memory->mOrder = Memory->mOrder + 1;
			Memory->LMemory->quantity = Memory->LMemory->quantity + 1;
			Lprocesses->quantity = Lprocesses->quantity - 1;
			Lprocesses->proc[0].order = -1;
			Memory->LMemory->proc[0].order = Memory->mOrder;
		}

		else if (Memory->LMemory->proc[1].order == -1){
			partAux = Memory->LMemory->proc[1].partition;
			Memory->LMemory->proc[1] = Lprocesses->proc[0];
			Memory->LMemory->proc[1].partition = partAux; //mantendo a partição fixa da memória
			Memory->mOrder = Memory->mOrder + 1;
			Memory->LMemory->quantity = Memory->LMemory->quantity + 1;
			Lprocesses->quantity = Lprocesses->quantity - 1;
			Lprocesses->proc[0].order = -1;
			Memory->LMemory->proc[1].order = Memory->mOrder;
		}
		else if (Memory->LMemory->proc[2].order == -1){ //proc[0]->8Mb e order == -1->vazio
			partAux = Memory->LMemory->proc[2].partition;
			Memory->LMemory->proc[2] = Lprocesses->proc[0];
			Memory->LMemory->proc[2].partition = partAux; //mantendo a partição fixa da memória
			Memory->mOrder = Memory->mOrder + 1;
			Memory->LMemory->quantity = Memory->LMemory->quantity + 1;
			Lprocesses->quantity = Lprocesses->quantity - 1;
			Lprocesses->proc[0].order = -1;
			Memory->LMemory->proc[2].order = Memory->mOrder;
		}
	}

	else if (Lprocesses->proc[0].Memory == 1){
		for (i = 0; i < 5; i++){
			if (Memory->LMemory->proc[i].order == -1){
				partAux = Memory->LMemory->proc[i].partition;
				Memory->LMemory->proc[i] = Lprocesses->proc[0];
				Memory->LMemory->proc[i].partition = partAux; //mantendo a partição fixa da memória
				Memory->mOrder = Memory->mOrder + 1;
				Memory->LMemory->quantity = Memory->LMemory->quantity + 1;
				Lprocesses->quantity = Lprocesses->quantity - 1;
				Lprocesses->proc[0].order = -1;
				Memory->LMemory->proc[i].order = Memory->mOrder;
				break;
			}
		}
	}
}
/***********************Fim função: ReadyToMemory************************/


/***********************************************************************
*                                                                      *
*  $FC Função: RunningProcess			                               *
*                                                                      *
*                                                                      *
***********************************************************************/
int RunningProcess(Memory *pMemory, ListOfProcesses *LBlocked, FILE *Output){
	int index;
	int i;
	int TexecAux;

	index = BuscaFHMR(pMemory);

	/******************************Comecço Arquivo de Saída**********************************/
	fprintf(Output, "\n----------------CPU: Imprimindo Informação do Processo----------------\n");
	fprintf(Output, "Processo #%d irá para CPU!\n", pMemory->LMemory->proc[index].Number);
	fprintf(Output, "----------------CPU: Terminou de Imprimir Informação do Processo----------------\n");
	/******************************Terminio Arquivo de Saída**********************************/

	if (pMemory->LMemory->proc[index].TimeExec[0] >= 10){
		sleep(10);
		tempoTotal += 10;
		pMemory->LMemory->proc[index].TimeExec[0] = pMemory->LMemory->proc[index].TimeExec[0] - 10;

		for (i = 0; i < LBlocked->quantity; i++){
			if (LBlocked->proc[i].TimeIO[0] >= 10){

				LBlocked->proc[i].TimeIO[0] = LBlocked->proc[i].TimeIO[0] - 10;

			}
			else{

				LBlocked->proc[i].order = LBlocked->proc[i].TimeIO[0]; //garantindo que processos que terminem primeiro tenham menores ordens na hora de enviar para prontos

				LBlocked->proc[i].TimeIO[0] = 0;
			}
		}
	}

	else if (0 <= pMemory->LMemory->proc[index].TimeExec[0] && pMemory->LMemory->proc[index].TimeExec[0] < 10){
		sleep(pMemory->LMemory->proc[index].TimeExec[0]);

		tempoTotal += pMemory->LMemory->proc[index].TimeExec[0];
		TexecAux = pMemory->LMemory->proc[index].TimeExec[0];
		pMemory->LMemory->proc[index].TimeExec[0] = 0;
		for (i = 0; i < LBlocked->quantity; i++){
			if (LBlocked->proc[i].TimeIO[0] >= TexecAux){

				LBlocked->proc[i].TimeIO[0] = LBlocked->proc[i].TimeIO[0] - TexecAux;

			}
			else{

				LBlocked->proc[i].order = LBlocked->proc[i].TimeIO[0]; //garantindo que processos que terminem primeiro tenham menores ordens na hora de enviar para prontos

				LBlocked->proc[i].TimeIO[0] = 0;
			}
		}
	}

	else{//tempo negativo
		printf("Erro Running Process: Tempo Negativo!\n");
	}

	if (pMemory->LMemory->proc[index].TimeExec[0] == 0){
		for (i = 0; i < ((pMemory->LMemory->proc[index].qtInfo) / 2); i++){
			pMemory->LMemory->proc[index].TimeExec[i] = pMemory->LMemory->proc[index].TimeExec[i + 1];
		}
		pMemory->LMemory->proc[index].TimeExec[(pMemory->LMemory->proc[index].qtInfo) / 2] = 0;
		return 0;
	}
	return 1;
}
/***********************Fim função: RunningProcess************************/


/***********************************************************************
*                                                                      *
*  $FC Função: ProcessToEverywhere		                               *
*                                                                      *
*                                                                      *
***********************************************************************/
void ProcessToEverywhere(Memory *Mmemory, ListOfProcesses *LBlock, ListOfProcesses *LReady, ListOfProcesses *LFinished, int fimExec){
	/*Essa funcao devera ser usada logo depois da RunningProcess, pois esta foi implementada partindo da ideia que RunningProcess atualiza todos tempos de Execucao e IO dos processos em execucao e bloqueados.
	*/
	int indexMenor;
	int indexMaior;

	indexMenor = BuscaFHMR(Mmemory);
	indexMaior = BuscaMaiorOrdem(Mmemory);

	if ((CheckList(LReady) == -1 && CheckList(LBlock) == -1) || (CheckList(LReady) == -1 && CheckList(LBlock) == 1)){// Caso a Lista de Prontos esteja vazia e a Lista de Bloqueados esteja vazia OU pelo menos um processo..

		if (Mmemory->LMemory->proc[indexMenor].TimeExec[0] == 0 && Mmemory->LMemory->proc[indexMenor].TimeIO[0] == 0){//Acabou tempo de Exec e IO -> Vai para Terminados
			LFinished->proc[LFinished->quantity] = Mmemory->LMemory->proc[indexMenor];
			LFinished->quantity = LFinished->quantity + 1;
			Mmemory->LMemory->quantity = Mmemory->LMemory->quantity - 1;
			Mmemory->LMemory->proc[indexMenor].order = -1;
		}

		else if (Mmemory->LMemory->proc[indexMenor].TimeExec[0] > 0 && Mmemory->LMemory->proc[indexMenor].TimeIO[0] > 0){//SAI DA CPU, POREM NAO DA MEMORIA-> LOGO SE TIVER TIMEEXEC > 0  e TIMEIO > 0 , entra em IO..
			if (fimExec == 0){ //se o valor de timeexec[0] for de outra execução vai pra BLOQUEADOS
				Mmemory->LMemory->proc[indexMenor].order = -1;
				Mmemory->LMemory->quantity = Mmemory->LMemory->quantity - 1;
				LBlock->proc[LBlock->quantity] = Mmemory->LMemory->proc[indexMenor];
				LBlock->quantity = LBlock->quantity + 1;
			}
			else if (fimExec == 1){//se o valor de timeexec[0] for da mesma execução FICA NA MEMÓRIA
				Mmemory->LMemory->proc[indexMenor].order = (Mmemory->LMemory->proc[indexMaior].order) + 1;
				Mmemory->mOrder++;
			}
		}

		else{//Não tem mais tempo de IO, mas ainda tem tempo de Execucao continua na memoria, mas sai da execucao
			Mmemory->LMemory->proc[indexMenor].order = (Mmemory->LMemory->proc[indexMaior].order) + 1;
			Mmemory->mOrder++;
		}
	}

	else if ((CheckList(LReady) == 1 && CheckList(LBlock) == -1) || ((CheckList(LReady) == 1 && CheckList(LBlock) == 1))){//Caso a Lista de Prontos tenha pelo menos um elemento e a lista de bloqueado tambem..
		if (Mmemory->LMemory->proc[indexMenor].TimeExec[0] == 0 && Mmemory->LMemory->proc[indexMenor].TimeIO[0] == 0){//SAI DA MEMORIA E DA CPU ->>>>Acabou tempo de Exec e IO -> Vai para Terminados
			LFinished->proc[LFinished->quantity] = Mmemory->LMemory->proc[indexMenor];
			LFinished->quantity = LFinished->quantity + 1;
			Mmemory->LMemory->quantity = Mmemory->LMemory->quantity - 1;
			Mmemory->LMemory->proc[indexMenor].order = -1;
		}

		else if (Mmemory->LMemory->proc[indexMenor].TimeExec[0] > 0 && Mmemory->LMemory->proc[indexMenor].TimeIO[0] > 0){//SAIR DA CPU E DA MEMORIA, TEM PROCESSO PRA ENTRAR!-> LOGO SE TIVER TIMEEXEC > 0  e TIMEIO > 0 , entra em IO..
			if (fimExec == 0){
				LBlock->proc[LBlock->quantity] = Mmemory->LMemory->proc[indexMenor];
				LBlock->quantity = LBlock->quantity + 1;
				Mmemory->LMemory->quantity = Mmemory->LMemory->quantity - 1;
				Mmemory->LMemory->proc[indexMenor].order = -1;
			}
			else if (fimExec == 1){
				LReady->proc[LReady->quantity] = Mmemory->LMemory->proc[indexMenor];
				LReady->quantity = LReady->quantity + 1;
				Mmemory->LMemory->proc[indexMenor].order = -1;
				Mmemory->LMemory->quantity = Mmemory->LMemory->quantity - 1;
			}
		}

		else{//Não tem mais tempo de IO, mas ainda tem tempo de Execucao continua na memoria, mas sai da execucao e da memoria , vai pra prontos!!!
			LReady->proc[LReady->quantity] = Mmemory->LMemory->proc[indexMenor];
			LReady->quantity = LReady->quantity + 1;
			Mmemory->LMemory->quantity = Mmemory->LMemory->quantity - 1;
			Mmemory->LMemory->proc[indexMenor].order = -1;
		}

	}
}
/***********************Fim função: ProcessToEverywhere************************/


/***********************************************************************
*                                                                      *
*  $FC Função: OrganizeList     		                               *
*                                                                      *
*                                                                      *
***********************************************************************/
void OrganizeList(ListOfProcesses *LBlocked){
	int i;

	for (i = 0; i < LBlocked->quantity; i++){
		if (LBlocked->proc[i].order == -1){
			while (i < LBlocked->quantity){
				LBlocked->proc[i] = LBlocked->proc[i + 1];
				i++;
			}
		}
	}

	LBlocked->proc[LBlocked->quantity].order = -1;

}
/***********************Fim função: OrganizeList************************/


/***********************************************************************
*                                                                      *
*  $FC Função: BlockToReady     		                               *
*                                                                      *
*                                                                      *
***********************************************************************/
void BlockToReady(ListOfProcesses *Lready, ListOfProcesses *LBlocked){
	int i, j;

	for (i = 0; i < LBlocked->quantity; i++){
		if (LBlocked->proc[i].TimeIO[0] == 0){//Caso o tempo de IO tenha acabado.
			for (j = 0; j < (((LBlocked->proc[i].qtInfo) / 2) - 1); j++){
				LBlocked->proc[i].TimeIO[j] = LBlocked->proc[i].TimeIO[j + 1];
			}
			LBlocked->proc[i].TimeIO[((LBlocked->proc[i].qtInfo) / 2) - 1] = 0;

			Lready->proc[Lready->quantity] = LBlocked->proc[i];
			LBlocked->proc[i].order = -1; //transforma em um processo NULL.
			LBlocked->quantity = LBlocked->quantity - 1;
			Lready->quantity = Lready->quantity + 1;
			OrganizeList(LBlocked);
		}
	}
}
/***********************Fim função: BlockToReady************************/


/***********************************************************************
*                                                                      *
*  $FC Função: ExecutaBloqueados   		                               *
*                                                                      *
*                                                                      *
***********************************************************************/
void ExecutaBloqueados(ListOfProcesses *LBlocked){ //função chamada sempre que só houver processos na fila de bloqueados e/ou terminados
	int i;
	int j;

	sleep(1);
	tempoTotal++;
	for (i = 0; i < LBlocked->quantity; i++){
		LBlocked->proc[i].TimeIO[0] = LBlocked->proc[i].TimeIO[0] - 1;
		if (LBlocked->proc[i].TimeIO[0] == 0){//Caso o tempo de IO tenha acabado.
			for (j = 0; j < (((LBlocked->proc[i].qtInfo) / 2) - 1); j++){
				LBlocked->proc[i].TimeIO[j] = LBlocked->proc[i].TimeIO[j + 1];
			}
			LBlocked->proc[i].TimeIO[((LBlocked->proc[i].qtInfo) / 2) - 1] = 0;
		}
	}
}
/***********************Fim função: ExecutaBloqueados************************/

/****************************Fim de Declaração de Funções****************************/

int main(void){
	ListOfProcesses *LReady;
	ListOfProcesses *LBlock;
	ListOfProcesses *LFinished;
	Memory *Mmemory;
	FILE *pEntry;
	FILE *pOut;
	int i;
	int counter = 0;
	int retRunning;
	int TamReady = 0;

	tempoTotal = 0;

	pOut = fopen("OutputWORST.txt", "w");
	if (pOut == NULL){
		printf("Erro ao tentar abrir o arquivo!\n");
		exit(1);
	}

	pEntry = fopen("teste.txt", "r");
	if (pEntry == NULL){
		printf("Erro ao tentar abrir o arquivo!\n");
		exit(1);
	}

	LReady = FileReader(pEntry, &TamReady);

	Mmemory = MemoryCreator();
	LBlock = BlockCreator(TamReady);
	LFinished = FinishedCreator(TamReady);
	Mmemory->LMemory->quantity = 0;

	for (i = 0; i < LReady->quantity; i++){
		PrintProcess(&(LReady->proc[i]), pOut);
	}

	while ((Mmemory->LMemory->quantity + LReady->quantity + LBlock->quantity) != 0){
		while ((CheckMemory(Mmemory) >= LReady->proc[0].Memory) && (LReady->proc[0].order != -1)){
			ReadyToMemory(LReady, Mmemory);
			OrganizeList(LReady);
			fprintf(pOut, "\n-----------------------Mapa de Alocação de Memória-----------------------\n");
			PrintMemory(Mmemory, pOut);
			fprintf(pOut, "-----------------------Fim Mapa de Alocação de Memória-----------------------\n");
		}
		counter++;
		retRunning = RunningProcess(Mmemory, LBlock, pOut);
		BlockToReady(LReady, LBlock);
		ProcessToEverywhere(Mmemory, LBlock, LReady, LFinished, retRunning);
		PrintMemory(Mmemory, pOut);
		fprintf(pOut, "\n-----------------------Processos na Fila de Prontos-----------------------\n");
		PrintListOfProcesses(LReady, pOut);
		fprintf(pOut, "\n-----------------------Processos em IO-----------------------\n");
		PrintListOfProcesses(LBlock, pOut);
		fprintf(pOut, "\n-----------------------Processos Já Terminados-----------------------\n");
		PrintListOfProcesses(LFinished, pOut);

		if ((Mmemory->LMemory->quantity == 0) && (LReady->quantity == 0) && (LBlock->quantity != 0)){
			for (i = 0; i < LBlock->quantity; i++){
				while (LBlock->proc[i].TimeIO[0] != 0){
					ExecutaBloqueados(LBlock);
					Mmemory->LMemory->quantity = 1;
				}
				LBlock->proc[i].order = -1;
				LBlock->quantity = LBlock->quantity - 1;
			}

		}

	}

	printf("Tempo Decorrido: %ds\n", tempoTotal);
	fprintf(pOut, "Tempo Total Decorrido: %d\n", tempoTotal);
	printf("Counter: %d\n", counter);
	fprintf(pOut, "Numero de iterações do while: %d\n", counter);

	free(LReady->proc);
	free(LBlock->proc);
	free(LFinished->proc);
	free(LReady);
	free(LBlock);
	free(LFinished);

	free(Mmemory->LMemory->proc);
	free(Mmemory->LMemory);
	free(Mmemory);

	fclose(pEntry);
	fclose(pOut);

	return 0;
}

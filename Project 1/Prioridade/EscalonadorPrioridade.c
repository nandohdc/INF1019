#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/ipc.h>

/*Estrutura de Processos*/
struct processo{
	char nome[300];//Nome do processo
	int prioridade;//nivel de prioridade do processo
	int pid;//PID do processo.
};
typedef struct processo Processo;

/*Interpretador de Comandos*/
int interpretador(char nome[][300], int prioridade[100]){ //Lembrar do teste que fizemos com scanf.

	int i = 0;
	char aux;

	while (scanf("exec %s prioridade= %c [^\n]", nome[i], &aux) == 2){ //para cada string ou inteiro que quisermos "guardar" precisamos adicionar um nova variavel, com %s só lemos uma string por vez.
		prioridade[i] = aux - '0';

		printf("Nome: %s ---- Prioridade: %d\n", nome[i], prioridade[i]); //Só para testar a função, não apagar!
		
		i++;
	}
	return i;//Retorna o número de processo que terão de ser incializados.
}

/*Função para criar fila de criado , Estado(NEW)*/
void PreencheEstadoNewYork(Processo EstadoNewYork[], char nome[100][300], int prioridade[], int NumProcessos){
	int i;

	for(i = 0 ; i < NumProcessos ; i++){
		strcpy(EstadoNewYork[i].nome, nome[i]);
		EstadoNewYork[i].prioridade = prioridade[i];
		printf("NOME: %s ---- Prioridade: %d\n",EstadoNewYork[i].nome, EstadoNewYork[i].prioridade);
	}

}

/*Função de Comparação da Qsort*/
int ComparaQsort(const void *x, const void *y){
	const struct processo *p1 =(const struct processo*)x;
	const struct processo *p2 =(const struct processo*)y;
	
	if (p1->prioridade < p2->prioridade){
		return -1;
	}

	else if (p1->prioridade > p2->prioridade){
		return 1;
	}

	else{
		return 0;
	}
}



int Busca(Processo pProcesso[], int NumProcessos){
	int i, Procurado = -1, indice = -1;

	Procurado = pProcesso[0].prioridade;
	indice = 0;

	for (i = 1; i < NumProcessos; i++){
		if (Procurado > pProcesso[i].prioridade){
			Procurado = pProcesso[i].prioridade;
			indice = i;
		}
	}
	
	return indice;
}

int main (void){

	char nome[100][300];
	int prioridade[100];
	int i = 0, NumProcessos = 0;
	int memo, pid,status;
	int fdin, retorno;
	
	if ((fdin = open("arquivoEntrada.txt",O_RDONLY,0666)) == -1)
	{
		perror("Error open() entrada");
		return -1;
	}
	close(0);

	if ( (retorno = dup(fdin)) == -1 ) //stdin passa a ser (0)
	{
		perror("Error dup() fdin");
		return -2;
	}
	printf("IN: INTERPRETADTOR\n");
	NumProcessos = interpretador(nome, prioridade);
	printf("OUT: INTERPRETADTOR\n");

	Processo *EstadoNewYork = (Processo*)malloc(NumProcessos*sizeof(Processo));//Cria Vetor de Estado(NEW)
	printf("IN: PREENCHE\n");
	PreencheEstadoNewYork(EstadoNewYork, nome, prioridade, NumProcessos);// Atribui valores ao vetor de Estado(NEW)
	printf("OUT: PREENCHE\n");

	/*ALOCAÇÃO DE MEMORIA COMPARTILHADA*/
	memo = shmget(IPC_PRIVATE, (NumProcessos*sizeof(Processo)), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	/*Associa a memoria compartilhada ao processo*/
	Processo *FilaProntos = (Processo*)shmat(memo, 0, 0);//Cria Fila de Prontos
	
	
	for(i = 0 ; i < NumProcessos; i++){//Jogando para fila de prontos.

		strcpy(FilaProntos[i].nome,EstadoNewYork[i].nome);
		FilaProntos[i].prioridade = EstadoNewYork[i].prioridade;
		printf("NOME: %s ---- Prioridade: %d\n",FilaProntos[i].nome, FilaProntos[i].prioridade);
	}

	qsort(FilaProntos, NumProcessos,sizeof(Processo),ComparaQsort);
	
	for(i = 0; i < NumProcessos; i++){
	
	if((pid = fork()) < 0 ){
		printf("Erro ao Forkar!\n");
		exit(1);
	}

	else if(pid == 0 ){/*Son, fiho, hijo*/
		//sleep(2);
		printf("NOME: %s ---- Prioridade: %d\n",FilaProntos[0].nome, FilaProntos[0].prioridade);
		execve(FilaProntos[i].nome, 0 , 0);
	}

	else{/*Dad, Pai, Papa*/
		waitpid(pid ,&status, 0);
		qsort(FilaProntos, NumProcessos,sizeof(Processo),ComparaQsort);
	}

}
	return 0;

}

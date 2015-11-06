#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>

#define TAM_MAX 100;

/*Estrutura de Processos*/
struct processo{
	char nome[300];//Nome do processo
	int pid;//PID do processo.
};
typedef struct processo Processo;

/*Interpretador de Comandos*/
int interpretador(char nome[][300]){ //Lembrar do teste que fizemos com scanf.

	int i = 0;

	while (scanf("exec %s [^\n]", nome[i]) == 1){ //para cada string ou inteiro que quisermos "guardar" precisamos adicionar um nova variavel, com %s só lemos uma string por vez.

		printf("Nome: %s", nome[i]); //Só para testar a função, não apagar!

		i++;
	}
	return i;//Retorna o número de processo que terão de ser incializados.
}


int main(void){

	char nome[100][300];
	int i, k, NumProcessos = 0;
	int memo, pid;
	int fdin,fdout, retorno, retorno2;
	int *vPID;
	Processo *FilaProntos;

	if((fdin=open("arquivoEntrada.txt",O_RDONLY,0666)) ==-1)
	{
		printf("Error open()\n");
		return -1;
	}
	if((fdout=open("arquivoSaida.txt",O_WRONLY|O_CREAT|O_TRUNC,0666)) ==-1)
	{
		printf("Error open()\n");
		return -1;
	}

	if(retorno = dup2(fdin,0) == -1)
	{
		printf("Erro dup2(fdin,0) \n");
		return -2;
	}
	if(retorno2 = dup2(fdout,1) == -1)
	{
		printf("Erro dup2(fdout,1) \n");
		return -3;
	}
	NumProcessos = interpretador(nome);

	FilaProntos = (Processo*)malloc(NumProcessos*sizeof(Processo));

	for (i = 0; i < NumProcessos; i++){
		strcpy(FilaProntos[i].nome, nome[i]);//Jogando no vetor de Fila de Prontos o que foi lido do interpretador.
	}
	
printf("Alocar memoria compartilhada! \n");
	/*ALOCAÇÃO DE MEMORIA COMPARTILHADA*/
	memo = shmget(IPC_PRIVATE, (NumProcessos*sizeof(int)), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	/*Associa a memoria compartilhada ao processo*/
	vPID = (int*)shmat(memo, 0, 0);//Criando um vetor para armazenar PID de processos.

	for (i = 0; i < NumProcessos; i++){
		if ((pid = fork()) < 0){
			printf("Erro ao Forkar!\n");
			exit(1);
		}

		else if (pid == 0){
			printf("Este programa esta execução: %s <-------> i = %d\n", FilaProntos[i].nome, i);
			execve(FilaProntos[i].nome, 0, 0);
			exit(0);
		}

		else{
			printf("<********************************>\n");
			FilaProntos[i].pid = pid;//Recebendo PID do processo e amarzenando na estrutura.
			vPID[i] = pid;//Recebendo PID do processo e guardando no vetor de PID.
			sleep(2);
			printf("Este programa vai parar de executar: %s ---- PID: %d\n", FilaProntos[i].nome, FilaProntos[i].pid);
			kill(FilaProntos[i].pid, SIGSTOP);

		}

	}

	for (i = 0; i < 7; i++){
		printf("-------------------Entrando em loop-----------------\n");
		for (k = 0; k < NumProcessos; k++){
			printf(" \n");
			printf("SIGCONT: Executando o programa: %s ---- i = %d\n", FilaProntos[k].nome, k);
			kill(vPID[k], SIGCONT);//Voltando a executar o programa que foi parado antes, atraves do PID armazenado no vetor de PID
			sleep(2);//Tempo de Round Roubin.
			printf("SIGSTOP: Parando de executar o programa: %s ---- i = %d\n", FilaProntos[k].nome, k);
			kill(vPID[k], SIGSTOP); //Parando de executar o programa que foi parado antes, atraves do PID armazenado no vetor de PID
		}
	}
	for(i=0;i<NumProcessos;i++){
		kill(vPID[i], SIGKILL);
	}

	shmdt(vPID);
	shmctl(memo,IPC_RMID,0);
	free(FilaProntos);
	return 0;

}

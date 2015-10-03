#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

/*Estrutura de Processos*/
struct processo{
	char nome[300];//Nome do processo
	int prioridade;//nivel de prioridade do processo
};

typedef struct processo Processo;

/*Declarações de Protótipos de Função*/
/*************************************/

/*Interpretador de Comandos*/
int interpretador(char nome[100][300], int prio[]){ //Lembrar do teste que fizemos com scanf.

	int i = 0;
	char aux;

	while (scanf("exec %s prioridade= %c", nome[i], &aux) == 2){ //para cada string ou inteiro que quisermos "guardar" precisamos adicionar um nova variavel, com %s só lemos uma string por vez.
		prio[i] = aux - '0';

		printf("Nome: %s ---- Prioridade: %d", nome[i], prio[i]); //Só para testar a função, não apagar!
		
		i++;
	}
	return i;//Retorna o número de processo que terão de ser incializados.
}

/*Função de Comparação da Qsort*/
int ComparaQsort(const void *x, const void *y){
	const struct processo *p1 =(const struct processo*)x;
	const struct processo *p2 =(const struct processo*)y;
	
	if (p1->prioridade > p2->prioridade){
		return -1;
	}

	else if (p1->prioridade < p2->prioridade){
		return 1;
	}

	else{
		return 0;
	}
}

void ordenaVetorProcesso(Processo VetorProcesso[], int elementos){
	
	qsort(VetorProcesso,elementos,sizeof(Processo),ComparaQsort);

}


Processo inicializaProcesso(char nome[300], int prioridade){
	Processo proc;
	
	strcpy(proc.nome, nome);
	proc.prioridade = prioridade;

	return proc;
}

void preencheVetor(Processo vProc[], int elementos, char nome[][300], int prioridade[]){
	int i = 0;

	for(i = 0; i < elementos; i++){

		vProc[i] = inicializaProcesso(nome[i], prioridade[i]);

	}
	ordenaVetorProcesso(vProc,elementos);
}

/*Retira Processo do Vetor de Processos da fila de pronto*/
Processo retiraEorganiza(Processo vProc[], int elementos){//Destaca o primeiro processo da fila de prontos e reorganiza o vetor
	Processo retirado;
	int i;

	strcpy(retirado.nome,vProc[0].nome);
	retirado.prioridade = vProc[0].prioridade;

	for(i = 0 ; i < elementos ; i++){

	vProc[i] = vProc[i+1];

	}

	vProc[elementos - 1].prioridade = NULL;
	vProc[elementos - 1].nome = '\0';
	
	
	return retirado;
}

/*Adiciona Processo no Vetor de Processos*/
void adicionaProcesso(Processo vProc[], int elementos, Processo retirado){
	
	strcpy(vProc[elementos - 1].nome, retirado.nome);
	vProc[elementos - 1].prioridade = retirado.prioridade;
	
	ordenaVetorProcesso(vProc,elementos);
}

/*Verifica Prioridade*/
int verificaPrioridade(Processo vProc[], Processo processoRetirado){

	if (vProc[0].prioridade > processoRetirado.prioridade){ //Prioridade do vetor eh maior do que o processo em execucao.
		return 1;
	}

	else if (vProc[0].prioridade < processoRetirado.prioridade){ //Prioridade do vetor eh menor do que o processo em execucao.
		return -1;
	}

	else{//Prioridade do vetor eh igual ao processo em execucao.
		return 0;
	}
}

/*Funcao que manda o processo para execução*/
void mandaExecutar(Processo vProc[], int elementos){
	/*Lembrete: Partimos da idea de que a fila de pronto está ordenada(qsort),
	então o primeiro processo sempre vai o primeiro a ser executado,
	com isso só precisamos retirar ele da fila de pronto e mandar executa-lo.*/

	int pid;
	Processo pRetirado;

	pRetirado = retiraEorganiza(vProc, elementos);
	/*
	if((pid = fork()) < 0){ //Em caso de Erro ao forkar
		printf("Erro ao forkar!\n");
		exit(1);
	}

	else if(pid == 0){//Son, filho, hijo.
	
	}

	else{//Dad, pai, papa.
		while (verificaPrioridade(&vProc[0], pRetirado) != 1){}//enquanto ele nao acha o elemento com maior prioridade, ele irá ficar verificando.
		kill(pid, SIGSTOP);
		adicionaProcesso(vProc, elementos, pRetirado);
		mandaExecutar(vProc, elementos);
	}*/
}

/*Função de listagem dos processos*/
void listaProcessos(Processo vProc[], int elementos){//Printa na tela todos os procesos presente na Lista de Processos
	int i;
	printf("Lista de Processos: \n\n");

	for(i = 0 ; i < elementos ; i++){
		printf("Processo: %s ---- Prioridade: %d\n", vProc[i].nome, vProc[i].prioridade);
		i++;
	}
	printf("\n");
}

int main (void){

	int fdin, fdout, retorno, retorno2; 
	int nprocessos;
	int prio[100];
	char nome[100][300];
	Processo *vProc;
		
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

	if ((fdout=open("arquivoSaida.txt",O_WRONLY|O_CREAT|O_TRUNC,0666)) == -1)
	{
		perror("Error open()");
		return -3;
	}
	close(1);

	if ( (retorno2 = dup(fdout)) == -1 ) //stdout passa a ser (1)
	{
		perror("Error dup() fdout");
		return -4;
	}

	nprocessos = interpretador(nome, prio);

	printf("\n%d\n", nprocessos);

	vProc = (Processo*)malloc(nprocessos*sizeof(Processo));

	preencheVetor(vProc, nprocessos, nome, prio);

	listaProcessos(vProc, nprocessos);

	mandaExecutar(vProc, nprocessos);

	return 0;	
}


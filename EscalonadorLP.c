/*Escalonador de Processos(Lista de Prioridades)******
**INF1019 2015.2 - PUC-RIO****************************
**Professor: Luiz Fernando Seibel*********************
**Nome: Fernando Homem da Costa - 1211971*************
**      Mateus Ribeiro de Castro - 1213068************
**
******************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*Estrutura de Processos*/

struct processo{
	char nome[300]; //Nome do processo
	int prio; //nivel de prioridade do processo
	struct processo *prox; // ponteiro para uma pr�xima estrutura de processo
};
typedef struct processo Processo;
typedef struct processo *Pprocesso;

/*Declara��es de Prot�tipos de Fun��o*/
/*************************************/

/*Interpretador de Comandos*/
void interpretador(char nome[], int prio){ //Lembrar do teste que fizemos com scanf.

	while (scanf("exec %s prioridade %d", &nome, &prio) != EOF){ //para cada string ou inteiro que quisermos "guardar" precisamos adicionar um nova variavel, com %s s� lemos uma string por vez.

		printf("Nome: %s ---- Prioridade: %d", nome, prio); //S� para testar a fun��o, n�o apagar!
	}
}

/*Fun��o de Compara��o da Qsort*/
int ComparaQsort(const void *x, const void *y){
	const struct processo *comp1 = x;
	const struct processo *comp2 = y;

	if (comp1->prio > comp2->prio){
		return -1;
	}

	else if (comp1->prio < comp2->prio){
		return 1;
	}

	else{
		return 0;
	}
}

/*Fun��o para contar Elementos da Lista de Processos e Ordenh�-los*/
void OrdenaProcessos(Pprocesso *ListaProc, Pprocesso proc){/*ORDENADO ---> OBS: ORDENAR PROCESSOS!!!*/
	int i = 0;
	Pprocesso aux = proc;

	if (aux != NULL){
		while (aux->prox != NULL){
			i++;
		}

		qsort(ListaProc, i, sizeof(Processo), ComparaQsort);
	}
}

/*Inicializa��o de Lista de Processos*/
Pprocesso inicializaProcesso(char *nome, int prioridade){
	Pprocesso proc;
	proc = (Pprocesso)malloc(sizeof(Processo));

	if (proc == NULL) {
		printf("Erro ao alocar mem�ria\n");
		exit(1);
	}

	strcpy(proc->nome, nome);
	proc->prio = prioridade;
	proc->prox = NULL;

	return proc;
}

/*Verifica Prioridade*/

int verificaPrioridade(Pprocesso *ListaProc, Pprocesso proc){
	int i = 0;

	while (ListaProc[i]->prox != NULL){

		if (ListaProc[i]->prio > proc->prio){ //Prioridade da lista eh maior do que o processo em execucao.
			return 1;
		}

		else if (ListaProc[i]->prio < proc->prio){ //Prioridade da lista eh menor do que o processo em execucao.
			return -1;
		}

		else{//Prioridade da lista eh igual ao processo em execucao.
			return 0;
		}
	}
}

/*Adiciona Processo na Lista de Processos*/
void adicionaProcesso(Pprocesso *ListaProc, Pprocesso proc){
	int i = 0;

	if (ListaProc[0] == NULL){// Se o primeiro elemento da Lista de Processo for nulo, ou seja, sem processos.

		ListaProc[0] = proc;// Adiciona o primeiro processo a lista de processos.
	}

	else{// Caso j� tenha elementos inseridos na lista de Processos.
		while (ListaProc[i]->prox != NULL){//Vai at� o final da lista.
			i++;
		}
		ListaProc[i]->prox = proc;//Muda para quem o ultimo elemento da lista ir� apontar.
	}

	OrdenaProcessos(ListaProc, proc); //Ordena depois de ter ordenado.
}

/*Retira Processo da Lista de Processos da fila de pronto*/

Pprocesso retiraProcesso(Pprocesso *ListaProc){//Destaca o primeiro processo da fila de pronto
	/*NAO SEI SE EST� CERTO, PENSAR MELHOR...
	Pprocesso ret;

	ret = ListaProc[0];

	free(ListaProc[0]);

	return ret;
	*/
}

/*Funcao que manda o processo para execu��o*/
void mandaExecutar(Pprocesso *ListaProc){
	/*Lembrete: Partimos da idea de que a fila de pronto est� ordenada(qsort),
	ent�o o primeiro processo sempre vai o primeiro a ser executado,
	com isso s� precisamos retirar ele da fila de pronto e mandar executa-lo.*/

	Pprocesso exe;

	exe = retiraProcesso(&ListaProc); //Destacou o primeiro processo da lista.


}

/*Fun��o de listagem dos processos*/
void listaProcessos(Pprocesso *ListaProc){//Printa na tela todos os procesos presente na Lista de Procesos
	int i = 0;
	printf("Lista de Processos: /n/n");

	while (ListaProc[i] != NULL) {
		printf("Processo: %s ---- Prioridade: %d\n", ListaProc[i]->nome, ListaProc[i]->prio);
		i++;
	}
	printf("\n");
}



/*D�vidas:
interpretador:  
		[RESOLVIDO]Como ler do arquivo texto e armazenar?
		R:Ler do arquivo texto e armazenar num vetor de string(o nome do arquivo a ser executado) e num int(inteiro relacionado a prioridade).
		
		[RESOLVIDO] Como ler linha por linha?
		R: Ainda a pensar, DEVEMOS descartar "exec" e "prioridade".
		S� precisamos guardar string do nome do arquivo e o int.		
		
		Como retirar o processo que j� terminou da fila de execu��o?
		R: Utilizar kill, ou seja, matar o processo. Tamb�m podemos usar o PID do processo.

		Como saber que um processo j� terminou de executar?
		R:Todos os processos ser�o de loop.

		Como tirar um processo de execu��o para executar outro?
		R:SIGSTOP e SIGCONTINUE.

		OBS: Para cada processo criado dever� ter um fork, ou seja, cada processo novo ser� um filho novo. Para exec lido dever� executar um processo novo.
Com o PID do processo, conseguiremos para, continuar ou matar o processo.	
		
*/

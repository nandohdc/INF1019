/*Escalonador de Processos(Lista de Prioridades)******
**INF1019 2015.2 - PUC-RIO****************************
**Professor: Luiz Fernando Seibel*********************
**Nome: Fernando Homem da Costa - 1211971*************
**      Mateus Ribeiro de Castro - 1213068************
**
******************************************************/


#include<stdio.h>
#include<stdlib.h>

/*Estrutura de Processos*/

struct processo{
	int id; //identificador do processo
	int prio; //nivel de prioridade do processo
	struct processo *prox; // ponteiro para uma próxima estrutura de processo
};
typedef struct processo Processo;
typedef struct processo *Pprocesso;

/*Declarações de Protótipos de Função*/
/*************************************/

/*Inicialização de Lista de Processos*/
Pprocesso inicializaProcesso(int identificador, int prioridade){
	Pprocesso proc;
	proc = (Pprocesso)malloc(sizeof(Processo));

	if(proc == NULL) {
		printf("Erro ao alocar memória\n");
		exit(1);
	}

	proc->id = identificador;
	proc->prio = prioridade;
	proc->prox = NULL;

	return proc;
}

/*Adiciona Processo na Lista de Processos*/
void adicionaProcesso(Pprocesso *ListaProc, Pprocesso proc){
	int i = 0;
	
	if (ListaProc[0] == NULL){// Se o primeiro elemento da Lista de Processo for nulo, ou seja, sem processos.

		ListaProc[0] = proc;// Adiciona o primeiro processo a lista de processos.
	}

	else{// Caso já tenha elementos inseridos na lista de Processos.
		while (ListaProc[i]->prox != NULL){//Vai até o final da lista.
			i++;
		}
		ListaProc[i]->prox = proc;//Muda para quem o ultimo elemento da lista irá apontar.
	}

}

/*Função de listagem dos processos*/
void listaProcessos(Pprocesso *ListaProc){//Printa na tela todos os procesos presente na Lista de Procesos
	int i = 0;
	printf("Lista de Processos: /n/n");

	while (ListaProc[i] != NULL) {
		printf("Processo: %d\tPrioridade: %d\n", ListaProc[i]->id, ListaProc[i]->prio);
		i++;
	}
	printf("\n");
}

/*Função de Comparação da Qsort*/
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

/*Função para contar Elementos da Lista de Processos e Ordenhá-los*/
void OrdenaProcessos(Pprocesso proc, Pprocesso *ListaProc){/*ORDENADO ---> OBS: ORDENAR PROCESSOS!!!*/
	int i = 0;
	Pprocesso aux = proc;
	
	if (aux != NULL){
		while (aux->prox != NULL){
			i++;
		}

		qsort(ListaProc, i, sizeof(Processo), ComparaQsort);
	}
}

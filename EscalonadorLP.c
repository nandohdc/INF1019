/*Escalonador de Processos(Lista de Prioridades)
INF1019 2015.2 - PUC-RIO
Professor: Luiz Fernando Seibel
Nome: Fernando Homem da Costa - 1211971
      Mateus Ribeiro de Castro - 1213068

*/


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

/*Função de listagem dos processos*/
void listaProcessos(Pprocesso proc){
	Pprocesso aux;
	printf("Lista de Processos: /n/n");

	while(aux != NULL) {
		printf("Processo: %d\tPrioridade: %d\n", aux->id, aux->prio);
		aux = aux->prox;
	}
	printf("\n");
}

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
void OrdenaProcessos(Pprocesso proc, Pprocesso *ListaProc){
	int i;
	Pprocesso aux = proc;
	
	if (aux != NULL){
		while (aux->prox != NULL){
			i++;
		}

		qsort(ListaProc, i, sizeof(Processo), ComparaQsort);
	}
}


/*OBS: ORDENAR PROCESSOS!!!*/
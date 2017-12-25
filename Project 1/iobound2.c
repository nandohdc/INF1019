#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main (void){
	while(1){
	printf("IO-Bound1: Ola, eu sou IO-Bound1 seu amiguinho!\n");
	printf("IO-Bound1: Vamos brincar de entrar em I/O?\n");
	printf("IO-Bound1: Eba!Estou em I/O..\n");
	sleep(4);
	printf("IO-Bound1: Eh hora de dizer Tchau! Tchauuu!\n");
	printf("IO-Bound1: Terminei\n");
	}

	return 0;
}

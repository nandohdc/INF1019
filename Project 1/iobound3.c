#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main (void){
	int i;

while(1){

	printf("IOBound2: Ola, eu sou IO-Bound2 seu amiguinho!\n");
	printf("IOBound2: Vamos brincar de entrar em I/O?\n");
	printf("IOBound2: Eba!Estou em I/O..\n");
	for(i = 0; i < 10; i++){
	printf("IOBound2: Vamos ser amiguinhos?\n");
	sleep(2);
	}
	printf("IOBound2: Voce nao quer ser meu amiguinho!\n");
	printf("IOBound2: Entao eh hora de dizer Tchau! Tchauuu!\n");
	printf("IOBound2: Terminei\n");

	}
	return 0;
}

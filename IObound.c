#include<stdio.h>
#include<stdlib.h>
int main()
{
	int soma, subtracao, numero1, numero2;

	while(1)
	{
		printf("Digite dois numeros:");
		scanf("%d %d", &numero1, &numero2);
		soma = numero1+numero2;
		subtracao = numero1-numero2;
		printf("\nSoma: %d\nSubtracao: %d\n", soma, subtracao);
	}

	return 0;
}
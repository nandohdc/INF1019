#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define PI 3.14159265358979323846

int main()
{
	printf("CPUBound2: Ola, sou o CPUBound seu amiguinho!\n");
	printf("CPUBound2: Vamos brincar de matematica?\n");
while(1){
	

	double numero = 4534541231454564, resultado = 0, temp = 0;
	int i;

	for(i = 0; i < 10;i++)
	{
		for(i=0;i<1000;i++)
		{
			temp = numero/PI;
		}
		for(i=0;i<1000;i++)
		{
			temp = temp*PI;
		}
		
		resultado = (numero - temp)+i;
		
	}

}
	printf("CPUBound2: Fazendo as contas, amiguinho!\n");
	printf("CPUBound2: Terminei, amiguinho! Tchauuuu!\n");
	return 0;
}

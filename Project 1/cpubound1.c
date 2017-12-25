#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define PI 3.14159265359

int main()
{
	double numero = 4534541231454564, resultado = 0, temp = 0;
	int i;
	printf("CPUBOUND1: OLA!\n");
	for(i = 0;;i++)
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
	printf("CPUBOUND1: TCHAU!\n");
	return 0;
}

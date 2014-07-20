#include<stdio.h>

#define N 10

#define nenhuma 0
#define ambos 1
#define bloqueado 
int main()
{
	int i, aux = 0;
	char registro[10], *nome_arq = "arquivos/ordem-csi.TXT";
	FILE *arquivo; 
	
	arquivo = fopen(nome_arq,"w");

	for(i = 0; i < N; i++)
	{	
		aux = rand()%3;
		printf("%d;%d\n",i,aux);
			
		sprintf(registro, "%d\n",aux);
		
		fputs (registro, arquivo);
	}
}

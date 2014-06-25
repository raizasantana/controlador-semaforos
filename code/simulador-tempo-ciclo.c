#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEMPO_VERMELHO 5
#define TEMPO_AMARELO 3
#define TEMPO_VERDE 8

#define NUM_CICLOS 24


int main()
{

	FILE *arquivo;
	char registro [80] = "\0";
	char *endereco_arquivo = "arquivos/tempo-ciclo-csi.TXT";

	int *tempo_verde, *tempo_amarelo, *tempo_vermelho;

	tempo_verde = (int *) calloc (NUM_CICLOS, sizeof(int));
	tempo_amarelo = (int *) calloc (NUM_CICLOS, sizeof(int));
	tempo_vermelho = (int *) calloc (NUM_CICLOS, sizeof(int));
	
	int i;

	arquivo = fopen (endereco_arquivo, "w");

	if (!arquivo)
	{
		printf ("Arquivo com problema");
		return;
	}
	srand (time(NULL));
	for(i = 0; i < NUM_CICLOS; i++)
	{	

		tempo_verde[i] = rand()% TEMPO_VERDE + 2;
		
		tempo_amarelo[i] = rand()% TEMPO_AMARELO + 2;
		
		tempo_vermelho[i] = rand()% TEMPO_VERMELHO + 2;
		
		printf("Verde%d Amarelo%d Vermelho%d\n",tempo_verde[i], tempo_amarelo[i],tempo_vermelho[i]);
			
		sprintf(registro, "%d  %d  %d %d\n",i + 1,tempo_verde[i],tempo_amarelo[i],tempo_vermelho[i]);
		
		fputs (registro, arquivo);

		
	}
}



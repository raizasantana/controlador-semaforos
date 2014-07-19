#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITERACOES 10
#define QTD_MAX_E 10
#define QTD_MAX_S 10
#define VEL_MAXIMA 100

//Variacao
#define MAIOR 0
#define MENOR 1
#define IGUAL 2

//Trafego

#define PARADO 6
#define LENTO 5
#define NORMAL 4
#define RAPIDO 3

FILE *arq;
char registro [80] = "\0";
char *end_arq_via1 = "arquivos/fluxo-p-via1.txt";
char *end_arq_via2 = "arquivos/fluxo-p-via2.txt";
char *end_arq_via3 = "arquivos/fluxo-s-via1.txt";
char *end_arq_via4 = "arquivos/fluxo-s-via2.txt";

int num_randomico (int i)
{
	
	return rand()% i;
}

int main(int argc, char *argv[])
{
	
	int index;
	int detector_Inicial = 0, detector_Final = 0;
	int vel_entrada = 0, vel_saida = 0, vel_media = 0;
	int qtd_veiculos_ant = 0, qtd_veiculos_atu = 0;
	int variacao_Veiculos = 0;

	int variacao = 0, situacao = 0;

	int i = 0;
	
	for(i = 0; i < 4; i++){

		if (i == 0)	
			arq = fopen (end_arq_via1, "w");
	
		if(i == 1)	
			arq = fopen (end_arq_via2, "w");
		
		if (i == 2)	
			arq = fopen (end_arq_via3, "w");
	
		if(i == 3)	
			arq = fopen (end_arq_via4, "w");
	
		if (!arq)
		{
			printf ("Arquivo com problema");
			return;
		}
	
		
		printf("\n\n");
		for (index = 0; index < ITERACOES; index++)
		{
		
			detector_Inicial = num_randomico (QTD_MAX_E);
			detector_Final = num_randomico (QTD_MAX_S);

			vel_entrada = num_randomico(VEL_MAXIMA);
			vel_saida = num_randomico(VEL_MAXIMA);

			qtd_veiculos_atu = qtd_veiculos_ant + detector_Inicial - detector_Final;

			vel_media = (vel_saida + vel_entrada)/2;

			variacao_Veiculos = qtd_veiculos_atu - qtd_veiculos_ant;
		
			if (variacao_Veiculos == 0)
			{
				variacao = IGUAL;	
				if (detector_Inicial == 1)
				{
					situacao = NORMAL;
				}
				else
				{
					situacao = PARADO;
				}
			}else
				if (variacao_Veiculos > 0)
				{
					variacao = MAIOR;
					situacao = LENTO;
				}
				else
				{
					variacao = MENOR;
					situacao = RAPIDO;
				
				}
				
				printf("I=%04d VENT=%d VSAI=%d VM=%d VAR=%d QATU=%03d SIT=%d\n",index + 1,vel_entrada, vel_saida, vel_media, variacao,qtd_veiculos_atu, situacao);
			
				sprintf(registro, "%04d  %d  %d  %03d  %03d %d %d\n",index + 1,detector_Inicial, detector_Final,qtd_veiculos_ant,qtd_veiculos_atu,variacao, situacao);

				fputs (registro, arq);
			
				qtd_veiculos_ant = qtd_veiculos_atu;
		}
		
		fclose(arq);
		srand (time(NULL));
	}
	return 0;
}


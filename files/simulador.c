#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Variacao
#define MAIOR 0
#define MENOR 1
#define IGUAL 2

//Trafego
#define NORMAL 3
#define LENTO 4
#define PARADO 5
#define RAPIDO 6

FILE *arq;
char registro [80] = "\0";
char *end_arquivo = "simulador-fluxo-veiculo.txt";

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

	char variacao2 [5]="\0";
	char situacao2 [6]="\0";
	
	arq = fopen (end_arquivo, "w");

	if (!arq)
	{
		printf ("Arquivo com problema");
		return;
	}
	
	srand (time(NULL));

	for (index = 0; index < 160; index++)
	{
		
		detector_Inicial = num_randomico (10);
		detector_Final = num_randomico (10);

		vel_entrada = num_randomico(100);
		vel_saida = num_randomico(100);

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

	return 0;
}


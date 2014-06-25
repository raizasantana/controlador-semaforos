/*
* Controlador de Fluxo de Veículo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXIMO 49

FILE *arquivo01;
char registro02 [100] = "\0";
char *end_arquivo03 = "CFV-E.TXT";

FILE *arquivo04;
char registro05 [100] = "\0";
char *end_arquivo06 = "CFV-S.TXT";

typedef struct
{
	int Sequencia;
	int Vi; // velocidade de entrada
	int Vs; // velocidade de saida
	int Vm; // velocidade media
	int Dq; // variacao da quantidade de veiculos na via
	int Q; // quantidade de veiculos na via
	int Risco; // risco
} MASSA;

MASSA *massa;

void abre_arquivo_01()
{
	arquivo01 = fopen (end_arquivo03, "r");
	if (!arquivo01)
	{
		printf ("Com problema - Abertura 01");
		return;
	}
}

void fecha_arquivo_01()
{
	fclose(arquivo01);
}

int carrega_massa ()
{
	int i = 0, j = 0;

	abre_arquivo_01();

	massa = (MASSA *) calloc (MAXIMO, sizeof(MASSA));
	
	while (!feof(arquivo01))
	{
		fgets (registro02, 80, arquivo01);
		
		i = atoi(strtok(registro02," "));

		j = i;	

		massa[j].Sequencia = i;

		i = atoi(strtok(NULL," "));

		massa[j].Vi = i;

		i = atoi(strtok(NULL," "));

		massa[j].Vs = i;

		i = atoi(strtok(NULL," "));

		massa[j].Vm = i;

		i = atoi(strtok(NULL," "));

		massa[j].Dq = i;

		i = atoi(strtok(NULL," "));

		massa[j].Q = i;

		i = atoi(strtok(NULL," "));

		massa[j].Risco = i;
		
	}

	fecha_arquivo_01();
	return(MAXIMO);
}

void iniciar_num_aleatorio ()
{
	srand((unsigned) time(NULL));
}

int num_aleatorio (int i)
{
	return rand() % i;
}

int qual_situacao(int qtd_massa)
{
	int i = 0, r = 0;
	
	i = num_aleatorio(qtd_massa - 1);
	
	printf ("Aleatorio=%d ", i);
	
	r = massa[i].Risco;

	printf ("Risco=%d ", r);

	return (r);
}

int maior (int a, int b)
{
	return ((a > b) ? a : b);
}

void abre_arquivo_02()
{
	arquivo04 = fopen (end_arquivo06, "w");
	if (!arquivo04)
	{
		printf ("Com problema - Abertura 02");
		return;
	}
}

void fecha_arquivo_02()
{
	fclose(arquivo04);
}

void grava(int n)
{

	int i = 0, pista_01 = 0, pista_02 = 0, resultado = 0;

	abre_arquivo_02();

	for (i = 0; i < n; i++)
	{
		printf("Passei aqui %d vezes %d\n",i,n);
		
		pista_01 = qual_situacao(i);
		/*		
		pista_02 = qual_situacao(MAXIMO);
		resultado = maior (pista_01, pista_02);
		printf ("Risco = %d\n", resultado);

		sprintf(registro02, "%d  %d \n", i + 1,resultado);
		fputs (registro02, arquivo04);
*/
	
	}
	
	fecha_arquivo_02();
}


int main(int argc, char *argv[])
{	
	int i = 0, j = 0, l = 0, m;
	/*	
	int pista_1 = 0, pista_2 = 0;
	int L_06_Index = 0, L_07_Index;

	l = carrega_massa();
	iniciar_num_aleatorio ();
	*/
	printf ("Quantas simulacoes ? ");
	scanf ("%d", &i);
		
	grava(i);
	
	printf("Hello World!");	
	return 0;
}

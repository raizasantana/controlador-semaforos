/*
* Controlador de Fluxo de Veiculos 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXIMO 100

//Variacao
#define MAIOR 0
#define MENOR 1
#define IGUAL 2

//Trafego
#define PARADO 6
#define LENTO 5
#define NORMAL 4
#define RAPIDO 3


FILE *arquivo;
char registro02 [100] = "\0";

char *end_arq_via1 = "arquivos/S1-fluxo-p.txt";
char *end_arq_via2 = "arquivos/S2-fluxo-s.txt";

FILE *arq_principal_S;
char registro05 [100] = "\0";
char *end_arq_principal_S = "arquivos/situacao_via.txt";

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

MASSA massa_via1[MAXIMO], massa_via2[MAXIMO];

FILE* abre_arquivo(char *endereco, char *modo)
{
	FILE *arquivo = fopen (endereco, modo);
	
	if (!arquivo)
	{
		printf ("Arquivo %s com problema.\n",endereco);
		return;
	}
	
	return arquivo;
}

void fecha_arquivo(FILE *arquivo)
{
	fclose(arquivo);
}

int carrega_massa (char *endereco_arquivo, MASSA *massa)
{
	int i = 0, j = 0;

	arquivo = abre_arquivo(endereco_arquivo, "r");

	while (fgets (registro02, 80, arquivo) != NULL)
	{	
		//puts(registro02);	

		i = atoi(strtok(registro02," "));		
		j = i-1;	
		
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
	
	fecha_arquivo(arquivo);
	
	return(MAXIMO);
}

void imprime_situacao(int risco) //Situacao da pista analisada
{

	switch(risco)
	{	
		case 4:
			printf ("Trafego Normal.", risco);
		break;

		case 5:
			printf ("Trafego Lento.", risco);
		break;

		case 6:
			printf ("Trafego Parado.", risco);
		break;

		case 3:
			printf ("Trafego Rapido.", risco);
		break;
	}
}


int qual_situacao(int pista, int qtd_massa, MASSA massa[])
{
	int i = 0, r = 0;

	
	i = rand() % qtd_massa;
//	i = num_aleatorio(qtd_massa); //qtd_massa = qtd de instantes disponiveis

	if(pista == 0)
		printf("\nPista Principal: ");
	else
		printf("\nPista Secundaria: ");

	printf ("Seq = %d ", i); //Escolhendo um instante pra analisar
	
	r = massa[i].Risco;
	imprime_situacao(r);
	//printf ("Situacao=%d \n", r); //Situacao da pista analisada

	return (r);
}


int maior (int a, int b)
{
	return ((a > b) ? a : b);
}

void grava(int n)
{

	int i = 0, pista_01 = 0, pista_02 = 0, resultado = 0;

	arq_principal_S = abre_arquivo(end_arq_principal_S,"w");

	printf("\n");

	for (i = 1; i <= n; i++)
	{
		printf("*** Simulacao n%d ***\n",i);
		
		pista_01 = qual_situacao(0, i, massa_via1);
		
		pista_02 = qual_situacao(1,i, massa_via2);
		
		resultado = maior (pista_01, pista_02);
		
		printf ("\nSituacao da Via ");
		imprime_situacao(resultado);
		srand (time(NULL));
		sprintf(registro05, "%d  %d \n", i + 1,resultado);
		
		fputs (registro05, arq_principal_S);
		
		printf("\n-------------------------------------------------------\n\n");
		
	}
	
	fecha_arquivo(arq_principal_S);
}


int main(int argc, char *argv[])
{	
	int i = 0, j = 0, l = 0, m;
		
	int pista_1 = 0, pista_2 = 0;
	
	int L_06_Index = 0, L_07_Index;

	l = carrega_massa(end_arq_via1, massa_via1);

	l = carrega_massa(end_arq_via2, massa_via2);
	
	printf("\n\n\n|-----------------Controlador de Fluxo de Veiculos-----------------|\n\n");
	printf ("Quantas simulacoes? ");
	scanf ("%d", &i);
	
	grava(i);

	return 0;
}

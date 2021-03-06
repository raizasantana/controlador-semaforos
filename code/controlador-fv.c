/*
* Controlador de Fluxo de Veiculos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXIMO 10

//Variacao
#define MAIOR 0
#define MENOR 1
#define IGUAL 2

//Trafego
#define NORMAL 3
#define LENTO 4
#define PARADO 5
#define RAPIDO 6

FILE *arq_principal_E;
char registro02 [100] = "\0";
char *end_arq_principal_E = "arquivos/cfv_principal_E.txt";

FILE *arq_principal_S;
char registro05 [100] = "\0";
char *end_arq_principal_S = "arquivos/cfv_principal_S.txt";

//RN
FILE *arq_rn;
char registro_rn [100] = "\0";
char *end_arq_rn = "arquivos/entrada_rn.txt";
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

MASSA massa[MAXIMO];

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

int carrega_massa ()
{
	int i = 0, j = 0;

	arq_principal_E = abre_arquivo(end_arq_principal_E, "r");

	while (fgets (registro02, 80, arq_principal_E) != NULL)
	{
		//puts(registro02);	

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
	
	fecha_arquivo(arq_principal_E);
	
	return(MAXIMO);
}


int num_aleatorio (int i)
{
	return rand() % i;
}

int qual_situacao(int qtd_massa)
{
	int i = 0, r = 0;
	
	//i = num_aleatorio(qtd_massa - 1);

	i = num_aleatorio(qtd_massa); //qtd_massa = qtd de instantes disponiveis
	
	printf ("Aleatorio=%d ", i); //Escolhendo um instante pra analisar
	
	r = massa[i].Risco;

	printf ("Situacao=%d \n", r); //Situacao da pista analisada

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

	for (i = 1; i <= n; i++)
	{
		
		pista_01 = qual_situacao(i);
		
		pista_02 = qual_situacao(MAXIMO);
		
		resultado = maior (pista_01, pista_02);
		
		printf ("Situacao da pista = %d\n", resultado);

		sprintf(registro05, "%d  %d \n", i + 1,resultado);
		
		fputs (registro05, arq_principal_S);


	}
	
	fecha_arquivo(arq_principal_S);
}


int main(int argc, char *argv[])
{	
	int i = 0, j = 0, l = 0, m;
		
	int pista_1 = 0, pista_2 = 0;
	
	int L_06_Index = 0, L_07_Index;

	l = carrega_massa();
	
	printf("\n[Controlador de Fluxo de Veiculos]\n\n");
	printf ("Quantas simulacoes ? ");
	scanf ("%d", &i);
	
	grava(i);

	return 0;
}

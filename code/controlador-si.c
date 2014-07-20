#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MULTIPLO 5
#define ESCOLHIDO 2

#define INTERMITENTE 0
#define VERDE 1
#define AMARELO 2
#define VERMELHO 3

FILE *G_01_Arquivo;
char G_02_Registro [80] = "\0";
char *G_03_Endereco_Arquivo = "arquivos/tempo-ciclo-csi.TXT";
char *G_04_Endereco_Arquivo = "arquivos/ordem-csi.TXT";
char *G_05_Endereco_Arquivo = "arquivos/CSI-ACAO.TXT";

int *G_06_Tempo_Verde;
int *G_06_Tempo_Amarelo;
int *G_06_Tempo_Vermelho;
int *G_07_Ordem;

//int *G_07_Sinal_2;
//time_t G_08_Inicio;
//time_t G_09_Fim;
//time_t G_10_Inicio;
//time_t G_11_Fim;


void p010_Recupera_Guarda_Tempo()
{
	int P010_01_Aux = 0, P010_02_Index = 0;

	G_06_Tempo_Verde = (int *) calloc (24, sizeof(int));
	G_06_Tempo_Amarelo = (int *) calloc (24, sizeof(int));
	G_06_Tempo_Vermelho = (int *) calloc (24, sizeof(int));

	G_01_Arquivo = fopen (G_03_Endereco_Arquivo, "r");
	
	if (!G_01_Arquivo)
	{
		printf ("\n[!!!] Com problema - Abertura Recupera_Guarda_Tempo()\n\n");
		return;
	}
	
	while (fgets (G_02_Registro, 80, G_01_Arquivo) != NULL)
	{
		
		//puts(G_02_Registro);
		P010_01_Aux = atoi(strtok(G_02_Registro,";"));
		P010_02_Index = P010_01_Aux;
		P010_01_Aux = atoi(strtok(NULL,";"));
		G_06_Tempo_Verde[P010_02_Index] = P010_01_Aux;
		P010_01_Aux = atoi(strtok(NULL,";"));
		G_06_Tempo_Amarelo[P010_02_Index] = P010_01_Aux;
		P010_01_Aux = atoi(strtok(NULL,";"));
		G_06_Tempo_Vermelho[P010_02_Index] = P010_01_Aux;
	}
	
	fclose(G_01_Arquivo);
}


void p020_Inicializar_Numero_Aleatorio ()
{
	srand((unsigned) time(NULL));
}



int p030_Numero_Aleatorio (int P040_Numero)
{
	return rand() % P040_Numero;
}



void p040_Recupera_Ordem(int P040_Ordem)
{
	int P040_01_Aux = 0, P040_02_Index = 0;

	G_07_Ordem = (int *) calloc (P040_Ordem, sizeof(int));
	printf("\nOrdem %d\n",G_07_Ordem[0]);
	G_01_Arquivo = fopen (G_04_Endereco_Arquivo, "r");

	if (!G_01_Arquivo)
	{
		printf ("\n[!!!] Com problema - Abertura Recupera_Ordem()\n\n");
		return;
	}


	while (fgets (G_02_Registro, 80, G_01_Arquivo) != NULL)
	{
		//puts(G_02_Registro);

		P040_01_Aux = atoi(strtok(G_02_Registro,";"));
		P040_02_Index = P040_01_Aux;
		printf("\n 40 01 i %d",P040_01_Aux);
		if (P040_02_Index > P040_Ordem)
		{	printf("40 02 %d\n",P040_02_Index);
			break;
		}

		P040_01_Aux = atoi(strtok(NULL,";"));
		G_07_Ordem[P040_02_Index] = P040_01_Aux;
	}
	fclose(G_01_Arquivo);
}


int p090_Mudar_Sinal (int P090_Ordem_1, int P090_Ordem_2)
{

	/*
	#define INTERMITENTE 0
	#define VERDE 1
	#define AMARELO 2
	#define VERMELHO 3
	*/
	static P090_01_Sinal_1 = 0;
	static P090_02_Sinal_2 = 0;

	static P090_03_Proximo_1 = 0;
	static P090_04_Proximo_2 = 0;

	static P090_05_Antes_1 = 0;
	static P090_06_Antes_2 = 0;

	int P090_07_Qual_Tempo = 0;
	int P090_08_Tempo_Recuperado = 0;

	time_t P090_09_Inicio;
	time_t P090_10_Fim;

	int P090_11_Diferenca = 0;
	int P090_12_Fez_Recursividade = 0;

	// achar qual o proximo ciclo
	if (P090_01_Sinal_1 == INTERMITENTE && P090_01_Sinal_1 == INTERMITENTE) // intermitente
	{
		P090_03_Proximo_1 = VERMELHO;
		P090_04_Proximo_2 = VERMELHO;
	}
	else
	{
		P090_03_Proximo_1 = P090_01_Sinal_1 + 1;
		P090_04_Proximo_2 = P090_02_Sinal_2 + 1;
		
		if (P090_03_Proximo_1 > 3)
		{
			if (P090_03_Proximo_1 > 3)
			{
				if (P090_05_Antes_1 == AMARELO)
					P090_03_Proximo_1 = VERMELHO;
				else
				{
					if (P090_02_Sinal_2 == VERDE || P090_02_Sinal_2 == AMARELO)
						P090_03_Proximo_1 = VERMELHO;
					else
						P090_03_Proximo_1 = VERDE;
				}
			}
		}

		if (P090_04_Proximo_2 > 3)
		{
			if (P090_04_Proximo_2 > 3)
			{
				if (P090_06_Antes_2 == AMARELO)
					P090_04_Proximo_2 = VERMELHO;
				else
				{
					if (P090_01_Sinal_1 == VERDE || P090_01_Sinal_1 == AMARELO)
						P090_04_Proximo_2 = VERMELHO;
					else
					{
						if (P090_05_Antes_1 == INTERMITENTE)	
							P090_04_Proximo_2 = VERMELHO;
						else
							P090_04_Proximo_2 = VERDE;
					}
				}
			}
		}
	}

	if (P090_Ordem_1 == 0 && P090_Ordem_2 == 0) // nao eh uma ordem
	{
		// faltando o tratamento para mudar ao horario intermitente
		P090_Ordem_1 = P090_03_Proximo_1;
		P090_Ordem_2 = P090_04_Proximo_2;
	}

	printf ("Mudar S1=%d S2=%d P1=%d P2=%d\n",P090_01_Sinal_1, P090_02_Sinal_2, P090_03_Proximo_1,P090_04_Proximo_2);
	
	// mudar o valor do sinal
	P090_05_Antes_1 = P090_01_Sinal_1;
	P090_06_Antes_2 = P090_02_Sinal_2;
	P090_01_Sinal_1 = P090_03_Proximo_1;
	P090_02_Sinal_2 = P090_04_Proximo_2;


	switch(P090_01_Sinal_1)
	{
		case 1 : printf("Sinal 1=%d Verde\n", P090_01_Sinal_1);
		break;

		case 2 : printf("Sinal 1=%d Amarelo\n", P090_01_Sinal_1);
		break;

		case 3 : printf("Sinal 1=%d Vermelho\n", P090_01_Sinal_1);
		break;
	
		default: printf("Sinal 1=%d Intermitente\n", P090_01_Sinal_1);
		break;
	}

	switch(P090_02_Sinal_2)
	{
		case 1 : printf("Sinal 2=%d Verde \n", P090_02_Sinal_2);
		break;

		case 2 : printf("Sinal 2=%d Amarelo \n", P090_02_Sinal_2);	
		break;

		case 3 : printf("Sinal 2=%d Vermelho \n", P090_02_Sinal_2);
		break;

		default: printf("Sinal 2=%d Intermitente\n", P090_02_Sinal_2);
		break;
	}


	if (P090_01_Sinal_1 != P090_Ordem_1 || P090_02_Sinal_2 != P090_Ordem_2)
	{
		printf ("\n\n[Recursividade]\n\n");//realiza a mudança de sinal respeitando o tempo até a ordem ser executada
		
		P090_09_Inicio = time(NULL);

		while (P090_11_Diferenca <= 5) // cinco segundos
		{
			P090_10_Fim = time(NULL);
			P090_11_Diferenca = difftime(P090_10_Fim, P090_09_Inicio);
		}

		p090_Mudar_Sinal (P090_Ordem_1, P090_Ordem_2);
		P090_12_Fez_Recursividade = 1;
	}

	if (P090_12_Fez_Recursividade == 0)
	{
		P090_07_Qual_Tempo = p030_Numero_Aleatorio (24);
		P090_07_Qual_Tempo++;
		
		if (P090_01_Sinal_1 == VERDE)
			P090_08_Tempo_Recuperado = G_06_Tempo_Verde[P090_07_Qual_Tempo];
		else
			if (P090_01_Sinal_1 == AMARELO)
				P090_08_Tempo_Recuperado = G_06_Tempo_Amarelo[P090_07_Qual_Tempo];
		else
			P090_08_Tempo_Recuperado = G_06_Tempo_Vermelho[P090_07_Qual_Tempo];
		
		printf ("\nTempo Sinal 1(%d)=%d\n\n", P090_07_Qual_Tempo,P090_08_Tempo_Recuperado);
		
		return(P090_08_Tempo_Recuperado);
	}
	else
		return(0);
}



int p100_Processa(int P100_Qtd_Ordem, int P100_Qtd_Ciclo)
{
	time_t P100_01_Inicio;
	time_t P100_02_Fim;

	int P100_03_Ha_Ordem = 0;
	int P100_04_Resultado = 0;
	int P100_05_Qual_Ordem = 0;
	int P100_06_Ordem_1 = 0;
	int P100_07_Ordem_2 = 0;

	int P100_08_Diferenca = 0;
	int P100_09_Tempo_Ciclo = 0;
	int P100_10_Qtd_Ordem = 0;
	int P100_11_Qtd_Ciclo = 0;

	p020_Inicializar_Numero_Aleatorio();

	while (1==1)
	{
		// recuperar um numero aleatorio para identificar qual a ordem
		P100_05_Qual_Ordem = p030_Numero_Aleatorio (P100_Qtd_Ordem);
	
		if (G_07_Ordem[P100_05_Qual_Ordem] == 0)
		{
			P100_06_Ordem_1 = 0;
			P100_07_Ordem_2 = 0;
		}
		else
		{
			if (G_07_Ordem[P100_05_Qual_Ordem] == 1)
			// sinal1=verde e sinal2=vermelho
			{
				P100_06_Ordem_1 = 1; // verde = 1
				P100_07_Ordem_2 = 3; // vermelho = 3
			}
			else
			{
				if (G_07_Ordem[P100_05_Qual_Ordem] == 2)
				// sinal1=vermelho e sinal2=verde
				{
					P100_06_Ordem_1 = 3; // vermelho = 3
					P100_07_Ordem_2 = 1; // verde = 1
				}
				else
				{
					if (G_07_Ordem[P100_05_Qual_Ordem] == 3)
					// sinal1=vermelho e sinal2=vermelho
					P100_06_Ordem_1 = P100_07_Ordem_2 = 3;
					// vermelho = 3
				}
			}
			
			P100_01_Inicio = time(NULL);
			P100_09_Tempo_Ciclo = -1;
		}

		P100_02_Fim = time(NULL);
		P100_08_Diferenca = difftime(P100_02_Fim, P100_01_Inicio);

		if (P100_08_Diferenca > P100_09_Tempo_Ciclo)
		{
			P100_11_Qtd_Ciclo++;
		
			if (P100_06_Ordem_1 != 0 && P100_07_Ordem_2 != 0)
			{
				P100_10_Qtd_Ordem++;
				printf ("Ordem Nr.=%d Ordem1=%d Ordem2=%d\n",P100_10_Qtd_Ordem, P100_06_Ordem_1, P100_07_Ordem_2);
			}
			else	
				printf ("Ciclo Nr.=%d Ordem1=%d Ordem2=%d\n",P100_11_Qtd_Ciclo, P100_06_Ordem_1, P100_07_Ordem_2);

			P100_09_Tempo_Ciclo = p090_Mudar_Sinal (P100_06_Ordem_1,P100_07_Ordem_2);
			P100_01_Inicio = time(NULL);
		}
		//if (P100_10_Qtd_Ordem >= P100_Qtd_Ordem ||P100_11_Qtd_Ciclo >= P100_Qtd_Ciclo)
		if (P100_11_Qtd_Ciclo >= P100_Qtd_Ciclo)
			break;
	}
}





int main(int argc, char *argv[])
{


	int L_01_Qtd_Ordem = 0;
	int L_02_Qtd_Ciclo = 0;

	p010_Recupera_Guarda_Tempo();
	printf("//********************************************************************\n//                                CSI\n//********************************************************************\n\n");
	printf ("Informe a quantidade de ordens a processar ? ");
	scanf ("%d", &L_01_Qtd_Ordem);
	
	p040_Recupera_Ordem(L_01_Qtd_Ordem);


	L_02_Qtd_Ciclo = L_01_Qtd_Ordem;
	p100_Processa(L_01_Qtd_Ordem, L_02_Qtd_Ciclo);
	
	return 0;
}

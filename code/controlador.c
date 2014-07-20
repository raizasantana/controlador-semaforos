/*
*	Controlador de Fluxo e Semaforo para 1 cruzamento
*/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include<mpi.h>


//CFV
#define MAIOR 0
#define MENOR 1
#define IGUAL 2

//Fluxo
#define NENHUM 0
#define BAIXO 1
#define MEDIO 2
#define ALTO 3

#define MAXIMO 100

//CSI
#define MULTIPLO 5
#define ESCOLHIDO 2

#define INTERMITENTE 0
#define VERDE 1
#define AMARELO 2
#define VERMELHO 3

int rank, np, origem, destino, tag = 0;
int l = 0, pista_1 = 0, pista_2 = 0, ordem = 0, estado_atual;
MPI_Status status;

char *end_arq_via1 = "arquivos/fluxo-p.txt";
char *end_arq_via2 = "arquivos/fluxo-s.txt";
char *end_arq_csi = "arquivos/tempo-ciclo-csi.TXT";

FILE *arquivo;
char registro02 [100] = "\0";

int *tempo_verde;
int *tempo_amarelo;
int *tempo_vermelho;
int *G_07_Ordem;

int sp, ss;
static sem_atual_1 = 0;
static sem_atual_2 = 0;
static int qtd_ciclos = 0;

typedef struct
{
	int Sequencia;
	int Vi; // velocidade de entrada
	int Vs; // velocidade de saida
	int Vm; // velocidade media
	int Dq; // variacao da quantidade de veiculos na via
	int Q; // quantidade de veiculos na via
	int situacao; // situacao
} FLUXO;

typedef struct
{
	int sequencia, fp, fs, sa, op,or;
} ENTRADA;


ENTRADA *entrada;
FLUXO fluxo_via1[MAXIMO], fluxo_via2[MAXIMO];

/*
*	CONTROLE DE FLUXO DE VEICULOS
*/

int carrega_fluxo (char *endereco_arquivo, FLUXO *fluxo)
{
	int i = 0, j = 0;

	arquivo = fopen(endereco_arquivo, "r");

	while (fgets (registro02, 80, arquivo) != NULL)
	{	
		//puts(registro02);	

		i = atoi(strtok(registro02," "));		
		j = i-1;	
		
		fluxo[j].Sequencia = i;

		i = atoi(strtok(NULL," "));	
	
		fluxo[j].Vi = i;
	
		i = atoi(strtok(NULL," "));

		fluxo[j].Vs = i;

		i = atoi(strtok(NULL," "));

		fluxo[j].Vm = i;

		i = atoi(strtok(NULL," "));

		fluxo[j].Dq = i;

		i = atoi(strtok(NULL," "));

		fluxo[j].Q = i;

		i = atoi(strtok(NULL," "));

		fluxo[j].situacao = i;
	
	
	}
	
	fclose(arquivo);
	
	return(MAXIMO);
}


void imprime_situacao(int risco) //Situacao da pista analisada
{

	switch(risco)
	{	
		case 0:
			printf ("Nenhum Fluxo.");
		break;

		case 1:
			printf ("Fluxo Baixo.");
		break;

		case 2:
			printf ("Fluxo Medio.");
		break;

		case 3:
			printf ("Fluxo Alto.");
		break;
	}
}


int get_situacao(int pista, int qtd_massa, FLUXO fluxo[], int r)
{
	int i = 0, sit = 0;
	i = rand() % qtd_massa;

	printf("\nProcesso %d :: ",r);

	if(pista == 0)
		printf("Pista Principal: ");
	else
		printf("Pista Secundaria: ");

	printf ("| Seq = %d | ", i); //Escolhendo um instante pra analisar
	
	sit = fluxo[i].situacao;
	imprime_situacao(sit);

	return (sit);
}

/*
*	CONTROLADOR SEMAFORO DE INTERSECAO
*/

void get_tempo_ciclo()
{
	int aux = 0, indice = 0;

	tempo_verde = (int *) calloc (24, sizeof(int));
	tempo_amarelo = (int *) calloc (24, sizeof(int));
	tempo_vermelho = (int *) calloc (24, sizeof(int));

	arquivo = fopen(end_arq_csi, "r");
	
	if (!arquivo)
	{
		printf ("\n[!!!] Com problema - Abertura Recupera_Guarda_Tempo()\n\n");
		return;
	}
	
	while (fgets (registro02, 80, arquivo) != NULL)
	{
		
		//puts(G_02_Registro);
		aux = atoi(strtok(registro02,";"));
		indice = aux - 1;
		aux = atoi(strtok(NULL,";"));
		tempo_verde[indice] = aux;
		aux = atoi(strtok(NULL,";"));
		tempo_amarelo[indice] = aux;
		aux = atoi(strtok(NULL,";"));
		tempo_vermelho[indice] = aux;
	}

	fclose(arquivo);
}

int mudar_sinal (int ordem_1, int sp, int ordem_2, int ss)
{

	/*
	#define INTERMITENTE 0
	#define VERDE 1
	#define AMARELO 2
	#define VERMELHO 3
	*/

	static proximo_1 = 0;
	static proximo_2 = 0;

	static sinal_1 = 0;
	static sinal_2 = 0;	

	int qual_tempo = 0;
	int tempo_rec = 0;

	time_t inicio;
	time_t fim;

	int diferenca = 0;
	int recursividade = 0;

	// achar qual o proximo ciclo
	if (sinal_1 == INTERMITENTE && sinal_1 == INTERMITENTE) // intermitente
	{
		proximo_1 = VERMELHO;
		proximo_2 = VERMELHO;
	}
	else
	{
		proximo_1 = sinal_1 + 1;
		proximo_2 = sinal_2 + 1;
		
		if (proximo_1 > 3)
		{
			if (proximo_1 > 3)
			{
				if (sem_atual_1 == AMARELO)
					proximo_1 = VERMELHO;
				else
				{
					if (sinal_2 == VERDE || sinal_2 == AMARELO)
						proximo_1 = VERMELHO;
					else
						proximo_1 = VERDE;
				}
			}
		}

		if (proximo_2 > 3)
		{
			if (proximo_2 > 3)
			{
				if (sem_atual_2 == AMARELO)
					proximo_2 = VERMELHO;
				else
				{
					if (sinal_1 == VERDE || sinal_1 == AMARELO)
						proximo_2 = VERMELHO;
					else
					{
						if (sem_atual_1 == INTERMITENTE)	
							proximo_2 = VERMELHO;
						else
							proximo_2 = VERDE;
					}
				}
			}
		}
	}

	if (ordem_1 == 0 && ordem_2 == 0) // nao eh uma ordem
	{
		// faltando o tratamento para mudar ao horario intermitente
		ordem_1 = proximo_1;
		ordem_2 = proximo_2;

	}

	printf ("Mudar S%d=%d S%d=%d P%d=%d P%d=%d\n",sp, sinal_1, ss, sinal_2, sp, proximo_1, ss, proximo_2);
	
	// mudar o valor do sinal
	sem_atual_1 = sinal_1;
	sem_atual_2 = sinal_2;
	sinal_1 = proximo_1;
	sinal_2 = proximo_2;


	switch(sinal_1)
	{
		case 1 : printf("S1=%d Verde\n", sinal_1);
		break;

		case 2 : printf("S1=%d Amarelo\n", sinal_1);
		break;

		case 3 : printf("S1=%d Vermelho\n", sinal_1);
		break;	
		default: printf("S1=%d Intermitente\n", sinal_1);
		break;
	}

	switch(sinal_2)
	{
		case 1 : printf("S2=%d Verde \n", sinal_2);
		break;

		case 2 : printf("S2=%d Amarelo \n", sinal_2);	
		break;

		case 3 : printf("S2=%d Vermelho \n", sinal_2);
		break;

		default: printf("S2=%d Intermitente\n", sinal_2);
		break;
	}


	if (sinal_1 != ordem_1 || sinal_2 != ordem_2)
	{
		printf ("\n\n[Recursividade]\n\n");//realiza a mudança de sinal respeitando o tempo até a ordem ser executada
		
		inicio = time(NULL);

		while (diferenca <= 5) // cinco segundos
		{
			fim = time(NULL);
			diferenca = difftime(fim, inicio);
		}

		mudar_sinal (ordem_1, sp, ordem_2, ss);
		recursividade = 1;
	}

	if (recursividade == 0)
	{
		qual_tempo = rand() % 24;

		qual_tempo++;		
		if (sinal_1 == VERDE)
			tempo_rec = tempo_verde[qual_tempo];
		else
			if (sinal_1 == AMARELO)
				tempo_rec = tempo_amarelo[qual_tempo];
		else	
			tempo_rec = tempo_vermelho[qual_tempo];
		
		printf ("\nRegulagem completa. Tempo de sinal %d.\n\n",tempo_rec);		
		
		return(tempo_rec);
	}
	else
		return(0);
}

int processa_ordem(int ordem, int rank)
{
	time_t P100_01_Inicio;
	time_t P100_02_Fim;

	int P100_03_Ha_Ordem = 0;
	int P100_04_Resultado = 0;
	int P100_05_get_ordem = 0;
	int P100_06_Ordem_1 = 0;
	int P100_07_Ordem_2 = 0;

	int P100_08_Diferenca = 0;
	int P100_09_Tempo_Ciclo = 0;
	int P100_10_Qtd_Ordem = 0;

	
	
	if (ordem == 0)
	{
		P100_06_Ordem_1 = 0;
		P100_07_Ordem_2 = 0;
	}
	else
	{
		if (ordem == 1)
		// sinal1=verde e sinal2=vermelho
		{
			P100_06_Ordem_1 = 1; // verde = 1
			P100_07_Ordem_2 = 3; // vermelho = 3
		}
		else
		{
			if (ordem == 2)
			// sinal1=vermelho e sinal2=verde
			{
				P100_06_Ordem_1 = 3; // vermelho = 3
				P100_07_Ordem_2 = 1; // verde = 1
			}
			else
			{
				if (ordem == 3)
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

		qtd_ciclos++;
	
		if (P100_06_Ordem_1 != 0 && P100_07_Ordem_2 != 0)
		{
			P100_10_Qtd_Ordem++;
			printf ("\n\nProcesso %d :: Ordem Nr.=%d Ordem1=%d Ordem2=%d\n\n",rank,P100_10_Qtd_Ordem, P100_06_Ordem_1, P100_07_Ordem_2);
		}
		else
		{	
			printf("\n\nCiclo normal do semáforo.\n");
			printf ("Processo %d :: Ciclo Nr.=%d Ordem1 = %d Ordem2=%d\n\n",rank, qtd_ciclos, P100_06_Ordem_1, P100_07_Ordem_2);
		}

		P100_09_Tempo_Ciclo = mudar_sinal (P100_06_Ordem_1, sp, P100_07_Ordem_2, ss);
		P100_01_Inicio = time(NULL);
	
}

/*
*	RN
*/


int le_entrada()
{
	FILE * arquivo;
	char registro[80]="\0";
	char *arquivo_entrada_rn = "arquivos/entrada_rn.txt";
	int index_01 = 0;
	int aux = 0;
	int index_02 = 0;
	
	arquivo = fopen (arquivo_entrada_rn, "r");
	
	if (!arquivo)
	{
		printf ("\n[!!!] Erro na Abertura do Arquivo de Entrada da RN\n\n");		
		return(9);
	}

	entrada = (ENTRADA *) calloc (49,sizeof(ENTRADA));

	while (fgets (registro, 80, arquivo) != NULL)
	{

		//puts(registro);
		aux = atoi(strtok(registro, ";"));
		index_01 = aux-1;
		
		entrada[index_01].sequencia = aux;

		aux = atoi(strtok(NULL, ";"));
		entrada[index_01].fp = aux;	
		
		aux = atoi(strtok(NULL, ";"));
		entrada[index_01].fs = aux;

		aux = atoi(strtok(NULL, ";"));
		entrada[index_01].sa = aux;

		aux = atoi(strtok(NULL, ";"));
		entrada[index_01].op = aux;

		aux = atoi(strtok(NULL, ";"));
		entrada[index_01].or = aux;
	}

	fclose(arquivo);


	return(0);
}


int get_ordem(int fluxo_p, int fluxo_s, int s_atual)
{

	int i;
	for(i = 0; i < 50; i++)
		if(entrada[i].fp == fluxo_p && entrada[i].fs == fluxo_s && entrada[i].sa == s_atual)
			return entrada[i].or;
		
}	

int get_estado_atual (int s1, int s2)
{
	if(s1 == s2 == 0)
		return 0; //INTERMITENTE
	if(s1 == 1 && s2 == 3)
		return 1; //LIBERADO
	if(s1 == 3 && s2 == 1)
		return 2; //BLOQUEADO
	if(s1 == s2 == 3)
		return 3;//AMBOS
}

void regula_semaforo(int r)
{
	
	l = carrega_fluxo(end_arq_via1, fluxo_via1);
	l = carrega_fluxo(end_arq_via2, fluxo_via2);
	
	pista_1 = get_situacao(0,100,fluxo_via1,r);
	srand((unsigned) time(NULL));
	pista_2 = get_situacao(1,100,fluxo_via2,r);
	
	l = le_entrada();
	get_tempo_ciclo();
	estado_atual = get_estado_atual(sem_atual_1, sem_atual_2);
	
	ordem = get_ordem(pista_1, pista_2, estado_atual);
	processa_ordem(ordem,r);
}
/*
*	PROGRAMA PRINCIPAL
*/
int main(int argc, char *argv[])
{	
	
	//Iniciando o MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);	
	MPI_Comm_size(MPI_COMM_WORLD,&np);

	char *msg;
	msg = malloc(20);
	int status_pista;

	if(rank == 0) //Primeiro cruzamento
	{	sp = 1; ss = 2;

		printf("\n*****************************************************\n*** Controlador de Fluxo de Veículos ***\n*****************************************************\n");
		
		printf("\n\tProcesso 0\n");
		regula_semaforo(rank);
		
		//Wait
		destino = 1;
		MPI_Send(&pista_1,1,MPI_INT,destino,tag,MPI_COMM_WORLD);

		//Recebe status da outra pista 
		origem = 1; tag = 0;
		MPI_Recv(&status_pista,1,MPI_INT,origem,tag, MPI_COMM_WORLD,&status);
		printf("\n\nP0::::Situacao da pista principal 2 %d",status_pista);
	}
	if (rank == 1)//Segundo cruzamento
	{	
		//GO
		origem = 0; tag = 0;
		MPI_Recv(&status_pista,1,MPI_INT,origem,tag, MPI_COMM_WORLD,&status);

		sp = 5; ss = 6;
		printf("\n\tProcesso 1\n");	
		regula_semaforo(rank);
		
		destino = 0; tag = 0;	
		MPI_Send(&pista_1,1,MPI_INT,destino,tag,MPI_COMM_WORLD);
		
	}			

	MPI_Finalize();
	return 0;
	/*if (rank != 0) //Processo receptor
	{
		sprintf(msg,"Processo %d está vivo!",rank);
		destino = 0;
		MPI_Send(msg,strlen(msg)+1,MPI_CHAR,destino,tag,MPI_COMM_WORLD);
	} else
	{	printf("Esperando msgs...\n");
		//for(origem = 1; origem < np; origem++)	
		//{
			MPI_Recv(msg,1,MPI_CHAR,origem,tag,MPI_COMM_WORLD,&status);
			printf("%d MSG %s\n",origem,msg);
		//}
	}*/

}























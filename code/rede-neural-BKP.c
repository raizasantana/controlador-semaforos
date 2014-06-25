#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define raizquadrada(x) ((x) * (x))

int * G01_Qtd_Neuronio;
double * G01_Entrada;
double ** G01_Dado;
double *** G01_Peso_Recuperado;
double *** G01_Peso_Salvo;
double *** G01_Peso_Variacao;
double * G01_Saida_Esperada;
double * G01_Saida_Calculada;
double G01_Erro_Padrao;
double * G01_Delta;

// area para guardar os dados do arquivo
typedef struct
{
	int G02_Seq;
	int * G02_Parametro;
	int G02_Ordem;
} ENTRADA;

ENTRADA * G02_Entrada;

int G03_Qtd_Registro;
int G04_Qtd_Camadas;
int * G05_Qtd_Neuronios;
double G06_Momento;
double G07_Taxa_Aprendizado;
double G08_Erro_Alvo;
int G09_Epoca;
int G10_Funcao_Ativacao;
int G11_Saida_Errada = 0;
FILE * G99_01_Arquivo;
char G99_02_Nome_Arquivo[20]="\0";
char G99_03_Registro[80]="\0";
int G99_04_Tem_Log = 0;
time_t G99_05_Tempo;
struct tm *G99_06_Tempo;
int G99_07_Ano,
G99_08_Mes,
G99_09_Dia,
G99_10_Hora,
G99_11_Minuto,
G99_12_Segundo;
char G99_13_Buffer[256];


int p010Le_Entrada() // OK
{
	FILE * p010_01_Arquivo;
	char p010_02_Registro[80]="\0";
	char p010_03_Nome_Arquivo[12]="\0";
	int p010_04_Indx = 0.;
	int p010_05_Aux = 0;
	int p010_06_Indx = 0;
	
	// recebendo o nome do arquivo de entrada
	fflush(stdin);
	printf ("Qual o arquivo de entrada:");
	gets(p010_03_Nome_Arquivo);
	
	// abrindo o arquivo de entrada
	p010_01_Arquivo = fopen (p010_03_Nome_Arquivo, "r");
	
	if (!p010_01_Arquivo)
	{
		printf ("Erro na Abertura do Arquivo %s\n\n",p010_03_Nome_Arquivo);

		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "Erro na Abertura do Arquivo %s\n\n",p010_03_Nome_Arquivo);
		}
		
		return(9);
	}

	for (p010_04_Indx = 0; p010_04_Indx < 12; p010_04_Indx++)
	{
		p010_03_Nome_Arquivo[p010_04_Indx] = toupper(p010_03_Nome_Arquivo[p010_04_Indx]);
	}
	
	if (G99_04_Tem_Log == 1)
	{
		fprintf (G99_01_Arquivo, "Arquivo=%s\n\n",p010_03_Nome_Arquivo);
	}

	// lendo o 1o. registro do arquivo de entrada
	fgets(p010_02_Registro, 80, p010_01_Arquivo);
	
	if (!feof(p010_01_Arquivo))
	{ // recuperando os parametros para montagem da rede
		G03_Qtd_Registro = atoi(strtok(p010_02_Registro,";"));

		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "Registros=%d ", G03_Qtd_Registro);
		}

		G04_Qtd_Camadas = atoi(strtok(NULL,";"));

		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "Camadas=%d ", G04_Qtd_Camadas);
		}
	
		G05_Qtd_Neuronios = (int *) calloc (G04_Qtd_Camadas,sizeof(int));

		for (p010_04_Indx = 0; p010_04_Indx < G04_Qtd_Camadas;p010_04_Indx++)
		{
			G05_Qtd_Neuronios[p010_04_Indx] = atoi(strtok(NULL,";"));

			if (G99_04_Tem_Log == 1)
			{
				fprintf (G99_01_Arquivo, "Neuronios[%d]=%d",p010_04_Indx+1, G05_Qtd_Neuronios[p010_04_Indx]);
			}
		}

		G06_Momento = atof (strtok(NULL,";"));
	
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "\n\n");
			fprintf (G99_01_Arquivo, "Momento=%f ", G06_Momento);
		}

		G07_Taxa_Aprendizado = atof (strtok(NULL,";"));

		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "Tx.Aprendizado=%f ",G07_Taxa_Aprendizado);
		}

		G08_Erro_Alvo = atof (strtok(NULL, ";"));

		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "Erro=%f ", G08_Erro_Alvo);
		}

		G09_Epoca = atoi (strtok(NULL, ";"));
		
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "Epoca=%d ", G09_Epoca);
		}
	
		G10_Funcao_Ativacao = atoi (strtok(NULL, ";"));
	
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "Funcao Ativacao=%d\n\n",G10_Funcao_Ativacao);
		}
	}
	else
	{
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo, "Erro na 1a. Leitura do Arquivo%s\n\n",p010_03_Nome_Arquivo);
		}

		printf("Problema na 1a. Leitura do %s\n\n",p010_03_Nome_Arquivo);
		return(8);
	}
		
	// alocando o espaco necessario para conter os registros de entrada
	G02_Entrada = (ENTRADA *) calloc (G03_Qtd_Registro,sizeof(ENTRADA));

	for (p010_04_Indx = 0; p010_04_Indx < G03_Qtd_Registro;p010_04_Indx++)
		G02_Entrada[p010_04_Indx].G02_Parametro = (int *) calloc (G05_Qtd_Neuronios[p010_04_Indx], sizeof(int));

	// Carregando os registros de entrada
	while (!feof(p010_01_Arquivo))
	{
		fgets(p010_02_Registro, 80, p010_01_Arquivo);
		p010_05_Aux = atoi(strtok(p010_02_Registro, ";"));
		p010_04_Indx = p010_05_Aux-1;
		G02_Entrada[p010_04_Indx].G02_Seq = p010_05_Aux;
		p010_05_Aux = atoi(strtok(NULL, ";"));
		G02_Entrada[p010_04_Indx].G02_Parametro[0] = p010_05_Aux;	

		if (G05_Qtd_Neuronios[0] > 1)
		{
			p010_05_Aux = atoi(strtok(NULL, ";"));
			G02_Entrada[p010_04_Indx].G02_Parametro [1] = p010_05_Aux;
		}

		if (G05_Qtd_Neuronios[0] > 2)
		{
			p010_05_Aux = atoi(strtok(NULL, ";"));
			G02_Entrada[p010_04_Indx].G02_Parametro [2] = p010_05_Aux;
		}

		if (G05_Qtd_Neuronios[0] > 3)
		{
			p010_05_Aux = atoi(strtok(NULL, ";"));
			G02_Entrada[p010_04_Indx].G02_Parametro[3] = p010_05_Aux;
		}

		p010_05_Aux = atoi(strtok(NULL, ";"));
		G02_Entrada[p010_04_Indx].G02_Ordem = p010_05_Aux;
	}

	fclose(p010_01_Arquivo);
	return(0);
}



void p020_Inicializa_Randomizacao() // OK
{
	srand((unsigned) time(NULL));
}


double p030_Encontra_Real(float p030_Menor_Valor, float 030_Maior_Valor)
{ // OK
	double p030_Aux = 0;
	p030_Aux = ((((double)(rand()%10))/10) - p030_Maior_Valor);
	return (p030_Aux);
}


void p040_Cria_Rede()
{
	int p040_01_Indx = 0,
	p040_02_Indx = 0,
	p040_03_Indx = 0,
	p040_04_Aux = 0;

	// define a rede de acordo com a quantidade de camadas
	G01_Qtd_Neuronio = (int *) calloc (G04_Qtd_Camadas, sizeof(int));
	
	for (p040_01_Indx = 0; p040_01_Indx < G04_Qtd_Camadas;p040_01_Indx++)
	{
		G01_Qtd_Neuronio [p040_01_Indx] = G05_Qtd_Neuronios[p040_01_Indx];
	}


/*
G01_Entrada = (double *) calloc (G05_Qtd_Neuronios[0]+1,
sizeof(double));
for (p040_01_Indx = 0; p040_01_Indx <= G05_Qtd_Neuronios [0];
p040_01_Indx++)
{
if (p040_01_Indx == 0) // bias
G01_Entrada [p040_01_Indx] = 1.0;
else
G01_Entrada [p040_01_Indx] = 0.0;
}
*/

	G01_Dado = (double **) calloc (G04_Qtd_Camadas, sizeof(double *));
	
	for (p040_01_Indx = 0; p040_01_Indx < G04_Qtd_Camadas;p040_01_Indx++)
	{
		p040_04_Aux = 1;
		G01_Dado[p040_01_Indx] = (double *)calloc (G05_Qtd_Neuronios[p040_01_Indx] + p040_04_Aux,sizeof(double));
	}

	for (p040_01_Indx = 0; p040_01_Indx < G04_Qtd_Camadas;p040_01_Indx++)
	{
		for (p040_02_Indx = 0; p040_02_Indx <= G05_Qtd_Neuronios[p040_01_Indx];p040_02_Indx++)
		{
			if (p040_02_Indx == 0)
				G01_Dado[p040_01_Indx][p040_02_Indx] = 1.0;
			else
				G01_Dado[p040_01_Indx][p040_02_Indx] = 0.0;
		}
	}

	G01_Saida_Esperada = (double *)calloc (G05_Qtd_Neuronios [G04_Qtd_Camadas - 1],sizeof(double));
	
	G01_Saida_Calculada = (double *) calloc (G05_Qtd_Neuronios [G04_Qtd_Camadas - 1],sizeof(double));

	for (p040_01_Indx = 0; p040_01_Indx < G05_Qtd_Neuronios[G04_Qtd_Camadas - 1];p040_01_Indx++)
	{
		G01_Saida_Esperada [p040_01_Indx] = 0.;
		G01_Saida_Calculada [p040_01_Indx] = 0.;
	}

	G01_Peso_Recuperado = (double ***) calloc (G04_Qtd_Camadas - 1,sizeof(double **));
	
	G01_Peso_Salvo = (double ***) calloc (G04_Qtd_Camadas - 1,sizeof(double **));
	
	G01_Peso_Variacao = (double ***) calloc (G04_Qtd_Camadas - 1,sizeof(double **));

	for (p040_01_Indx = 0; p040_01_Indx < G04_Qtd_Camadas - 1;p040_01_Indx++)
	{
		p040_04_Aux = 1;

		G01_Peso_Recuperado[p040_01_Indx]= (double **)

		calloc (G05_Qtd_Neuronios[p040_01_Indx] + p040_04_Aux,sizeof(double *));

		G01_Peso_Salvo[p040_01_Indx] = (double **)calloc (G05_Qtd_Neuronios[p040_01_Indx] + p040_04_Aux,sizeof(double *));

		G01_Peso_Variacao[p040_01_Indx] = (double **)calloc (G05_Qtd_Neuronios[p040_01_Indx] + p040_04_Aux,sizeof(double *));

		for (p040_02_Indx = 0;p040_02_Indx <= G05_Qtd_Neuronios[p040_01_Indx];p040_02_Indx++)
		{
			G01_Peso_Recuperado[p040_01_Indx][p040_02_Indx] = (double *) calloc (G05_Qtd_Neuronios[p040_01_Indx + 1],sizeof(double));

			G01_Peso_Salvo[p040_01_Indx][p040_02_Indx] = (double *) calloc (G05_Qtd_Neuronios[p040_01_Indx + 1],sizeof(double));
	
			G01_Peso_Variacao[p040_01_Indx][p040_02_Indx] = (double *) calloc (G05_Qtd_Neuronios[p040_01_Indx + 1], sizeof(double));
		}
	}

	G01_Delta = (double *) calloc (G04_Qtd_Camadas, sizeof (double));

	for (p040_01_Indx = 0; p040_01_Indx < G04_Qtd_Camadas;p040_01_Indx++)
	{
		G01_Delta[p040_01_Indx] = 0.;
	}

/*
for (p040_01_Indx = 1; p040_01_Indx < G04_Qtd_Camadas;
p040_01_Indx++)
{
for (p040_02_Indx = 0; p040_02_Indx <=
G05_Qtd_Neuronios[p040_01_Indx]; p040_02_Indx++)
{
if (p040_01_Indx == (G04_Qtd_Camadas - 1) &&
p040_02_Indx == G05_Qtd_Neuronios[p040_01_Indx])
;
else
{
for (p040_03_Indx = 0; p040_03_Indx <=
G05_Qtd_Neuronios[p040_01_Indx -1];
p040_03_Indx++)
{
printf ("G01_Peso_Recuperado[%d][%d][%d]=%f\n",
p040_01_Indx, p040_02_Indx, p040_03_Indx,
G01_Peso_Recuperado[p040_01_Indx][p040_02_Indx][p040_03_Indx]);
}
}
}
}
*/
}


void p050_Recupera_Peso(char *p050_Nome_Arquivo)
{
	int p050_01_Indx, p050_02_Indx, p050_03_Indx;
	FILE * p050_04_Arquivo;
	char p050_05_Registro[80]="\0";

	p050_04_Arquivo = fopen (p050_Nome_Arquivo, "r");

	if (!p050_04_Arquivo)
	{
		p050_04_Arquivo = fopen (p050_Nome_Arquivo, "w");
	}

	// Soh deve existir ateh a camada oculta antes da camada de saida
	for (p050_01_Indx = 0; p050_01_Indx < G04_Qtd_Camadas - 1;p050_01_Indx++)
	{	
		for (p050_02_Indx = 0; p050_02_Indx <= G01_Qtd_Neuronio[p050_01_Indx]; p050_02_Indx++)
		{
			for (p050_03_Indx = 0;p050_03_Indx <= G01_Qtd_Neuronio[p050_01_Indx + 1];p050_03_Indx++)
			{	
				G01_Peso_Recuperado[p050_01_Indx][p050_02_Indx][p050_03_Indx] = p030_Encontra_Real(-0.5, 0.5);

				G01_Peso_Salvo[p050_01_Indx][p050_02_Indx][p050_03_Indx] = 0.;

				G01_Peso_Variacao[p050_01_Indx][p050_02_Indx][p050_03_Indx] = 0.;

				fprintf(p050_04_Arquivo, "Peso[%d][%d][%d]=%f\n",p050_01_Indx, p050_02_Indx, p050_03_Indx,G01_Peso_Recuperad
o[p050_01_Indx][p050_02_Indx][p050_03_Indx]);
			}
		}
	}
	
	fclose(p050_04_Arquivo);
}



int p060_Recupera_Inteiro(int p060_Menor_Valor, int p060_Maior_Valor)
{
	int p060_01_Aux =0;
	
	p060_01_Aux = rand() % ((p060_Maior_Valor - p060_Menor_Valor)+ 1);
	p060_01_Aux += p060_Menor_Valor;
	return (p060_01_Aux);
}



void p070_Seleciona_Massa(char *p070_Nome_Arquivo, double p070_Percentual)
{
	FILE * p070_01_Arquivo;
	char p070_02_Registro[80]="\0";
	int p070_04_Indx = 0;
	float p070_05_Perc = 0.;
	int * p070_06_Tab;
	int p070_07_Indx = 0;

	// criando a tabela para ter os registros selecionados;
	p070_06_Tab = (int *) calloc (G03_Qtd_Registro, sizeof(int));
	
	for (p070_04_Indx = 0; p070_04_Indx < G03_Qtd_Registro;p070_04_Indx++)
		p070_06_Tab[p070_04_Indx] = 0;

	// abrindo o arquivo de treino
	p070_01_Arquivo = fopen (p070_Nome_Arquivo, "r");
	
	if (!p070_01_Arquivo)
	{
		p070_01_Arquivo = fopen (p070_Nome_Arquivo, "w");
		p070_05_Perc = G03_Qtd_Registro * p070_Percentual;
		p070_04_Indx = p070_05_Perc;

		while (1==1)
		{
			p070_07_Indx = p060_Recupera_Inteiro(1, G03_Qtd_Registro);
			
			if (p070_06_Tab[p070_07_Indx - 1] == 0)
			{
				p070_04_Indx--;
	
				if (p070_04_Indx <= 0)
				break;
	
				p070_06_Tab[p070_07_Indx - 1] = 1;
			}
		}

		for (p070_04_Indx = 0; p070_04_Indx < G03_Qtd_Registro;p070_04_Indx++)
		{

			if (p070_06_Tab[p070_04_Indx] == 1)
				fprintf(p070_01_Arquivo, "%03d%\n", p070_04_Indx);
		}
		
		fclose(p070_01_Arquivo);
	}	
}



float p080_Funcao_Ativacao (float p080_Dado, int p080_Funcao)
{
	if (p080_Funcao == 1) // sigmoide
		return (1.0 / (1.0 + exp(-p080_Dado)));
	else // tangente hiperbolica
		return (0);
}




float p090_Derivada_Funcao_Ativacao (float p090_Dado, int p090_Funcao)
{
	float p090_Resultado = 0;
	
	// faz calculo da sigmoide
	p090_Resultado = p080_Funcao_Ativacao (p090_Dado, p090_Funcao);

	if (p090_Funcao == 1) // sigmoide
		return (p090_Resultado * (1.0 - p090_Resultado));
	else // tangente hiperbolica
		return (0);
}



void p100_Limpa_Dados_Entrada ()
{
	int p100_01_Indx =0,
	p100_02_Indx = 0;

	// limpa os dados de entrada
	for (p100_01_Indx = 0; p100_01_Indx < G04_Qtd_Camadas;p100_01_Indx++)
	{
		for (p100_02_Indx = 0; p100_02_Indx <= G05_Qtd_Neuronios[p100_01_Indx];p100_02_Indx++)
		{	
			if (p100_02_Indx == 0)
				G01_Dado[p100_01_Indx][p100_02_Indx] = 1.0;
			else
				G01_Dado[p100_01_Indx][p100_02_Indx] = 0.0;
		}
	}
}



void p110_Recupera_Dados_Entrada(FILE * p110_Arquivo, int p110_Indx)
{
	int p110_01_Indx = 0,
	p110_02_Indx = 0;
	p110_02_Indx = 0;
	
	for (p110_01_Indx = 1; p110_01_Indx <= G05_Qtd_Neuronios[0];p110_01_Indx++)
	{
		if (p110_01_Indx == 1)
		{
			G01_Dado[p110_02_Indx][p110_01_Indx - 1] = 1.0;
		//fprintf(p110_Arquivo, "B%d=%f ", p110_01_Indx - 1,
		//G01_Dado[p110_02_Indx][p110_01_Indx - 1]);
		}
		
		G01_Dado[p110_02_Indx][p110_01_Indx] = ((double)
		
		G02_Entrada[p110_Indx - 1].G02_Parametro[p110_01_Indx - 1]);
		//fprintf(p110_Arquivo, "I%d=%f ", p110_01_Indx,
		//G01_Dado[p110_02_Indx][p110_01_Indx]);
	}
}




void p120_Recupera_Dados_Saida (FILE * p120_Arquivo, int p120_Indx)
{
	int p120_01_Indx = 0, p120_02_Indx = 0;
	p120_02_Indx = G04_Qtd_Camadas - 1;
	
	for (p120_01_Indx = 1; p120_01_Indx <= G05_Qtd_Neuronios[p120_02_Indx]; p120_01_Indx++)
	{
		G01_Saida_Esperada [p120_01_Indx] = ((double)
		G02_Entrada[p120_Indx].G02_Ordem / 3) ;
		//fprintf(p120_Arquivo, "O%d=%f %d\n", p120_01_Indx,
		//G01_Saida_Esperada[p120_01_Indx],
		//G02_Entrada[p120_Indx].G02_Ordem);
	}
}



void p130_Backpropagation_Forward (FILE * p130_Arquivo)
{
	int p130_01_Indx = 0, p130_02_Indx = 0, p130_03_Indx = 0;
	double p130_04_Aux = 0.;
	
	for (p130_01_Indx = 1; p130_01_Indx < G04_Qtd_Camadas;p130_01_Indx++)
	{	
		for (p130_02_Indx = 1; p130_02_Indx <= G01_Qtd_Neuronio[p130_01_Indx];p130_02_Indx++)
		{
			for(p130_03_Indx = 0; p130_03_Indx <= G01_Qtd_Neuronio[p130_01_Indx - 1];p130_03_Indx++)
			{
				if (G99_04_Tem_Log == 1)
				{	
					fprintf (G99_01_Arquivo,"01=%d 02=%d 03=%d\n", p130_01_Indx, p130_02_Indx,p130_03_Indx);
				}
		// calcula o somatorio do produto do neuronio pelo peso
		p130_04_Aux = G01_Dado[p130_01_Indx][p130_02_Indx];
		//fprintf (p130_Arquivo, " Ant(%d, %d)=%f ",
		//p130_01_Indx, p130_02_Indx, p130_04_Aux);

		G01_Dado[p130_01_Indx][p130_02_Indx] = p130_04_Aux + ((double) G01_Dado[p130_01_Indx - 1][p130_03_Indx] * G01_Peso_Recuperado[p130_01_Indx - 1][p130_03_Indx][p130_02_Indx]);

//fprintf (p130_Arquivo, "Dado(%d,%d)=%f * Peso(%d,%d,%d)=%f Atual(%d,%d)=%f",
//p130_01_Indx - 1, p130_03_Indx,
//G01_Dado[p130_01_Indx - 1][p130_03_Indx],
//p130_01_Indx - 1, p130_03_Indx, p130_02_Indx,
//G01_Peso_Recuperado[p130_01_Indx -1][p130_03_Indx][p130_02_Indx],
//p130_01_Indx, p130_02_Indx,
//G01_Dado[p130_01_Indx][p130_02_Indx]);
}
//fprintf(p130_Arquivo, "\n");
//fprintf (p130_Arquivo, "Somatorio(%d,%d)=%f ",
//p130_01_Indx, p130_02_Indx,
//G01_Dado[p130_01_Indx][p130_02_Indx]);
		G01_Dado[p130_01_Indx][p130_02_Indx] = p080_Funcao_Ativacao(G01_Dado[p130_01_Indx][p130_02_Indx],G10_Funcao_Ativacao);
//fprintf (p130_Arquivo, "Resultado(%d,%d)=%f\n",
//p130_01_Indx, p130_02_Indx,
//G01_Dado[p130_01_Indx][p130_02_Indx]);
		}
	}
}


void p140_Backpropagation_Backward (FILE * p140_Arquivo)
{
	int p140_01_Indx = 0,p140_02_Indx = 0;
	double p140_03_Aux =0.;
	int p140_04_Saida_Errada = 0,p140_05_Indx = 0;
	
	G11_Saida_Errada++;
	p140_01_Indx = G04_Qtd_Camadas - 1;

	for (p140_02_Indx = 1;p140_02_Indx <= G01_Qtd_Neuronio[p140_01_Indx]; p140_02_Indx++)
	{
		G01_Saida_Calculada [p140_02_Indx - 1] = G01_Dado [p140_01_Indx][p140_02_Indx];
//fprintf (p140_Arquivo, "Saida(%d)=%f\n",
//p140_02_Indx, G01_Dado[p140_01_Indx][p140_02_Indx]);

		if (G01_Saida_Calculada [p140_02_Indx - 1] != G01_Saida_Esperada [p140_02_Indx])
		{
			G11_Saida_Errada++;
			p140_04_Saida_Errada = 1;
			G01_Erro_Padrao += 0.5 * raizquadrada(G01_Saida_Esperada[p140_02_Indx] - G01_Saida_Calculada [p140_02_Indx - 1]);
			
			fprintf (p140_Arquivo,"%f;%f\n",G01_Saida_Esperada[p140_02_Indx],G01_Saida_Calculada [p140_02_Indx - 1]);
//"Saida Esperada[%d]=%f Calculada(%d)=%f\n",
//p140_02_Indx, G01_Saida_Esperada[p140_02_Indx],
//p140_02_Indx, G01_Saida_Calculada [p140_02_Indx - 1]);


// achar o delta erro da camada de saida
			G01_Delta[p140_01_Indx] = (G01_Saida_Esperada[p140_02_Indx - 1] - G01_Saida_Calculada [p140_02_Indx - 1])* G01_Saida_Calculada [p140_02_Indx - 1] * (1 - G01_Saida_Calculada [p140_02_Indx - 1]);
		}
		else
		{
			p140_04_Saida_Errada = 0;
		}
		// achar o delta das camadas intermediarias
		p140_03_Aux = 0;
	
		for (p140_01_Indx = G04_Qtd_Camadas - 2; p140_01_Indx > 0;p140_01_Indx--);
		{
			for(p140_02_Indx = 0; p140_02_Indx <= G05_Qtd_Neuronios[p140_01_Indx];p140_02_Indx++)
			{
				for (p140_05_Indx = 0; p140_05_Indx < G05_Qtd_Neuronios[p140_01_Indx + 1]; p140_05_Indx++)
				{
					p140_03_Aux += 1;
					G01_Peso_Recuperado[p140_01_Indx][p140_02_Indx][p140_05_Indx] * G01_Delta [p140_01_Indx + 1];
				}
			}	
		
			G01_Delta[p140_01_Indx] = p140_03_Aux * G01_Dado [p140_01_Indx][p140_02_Indx] * (1 - G01_Dado [p140_01_Indx][p140_02_Indx]);
		}
	}
	// atualiza os pesos da camada de entrada para a camada	intermediaria

	for (p140_01_Indx = 0; p140_01_Indx < G04_Qtd_Camadas - 1;p140_01_Indx++)
	{
		for (p140_02_Indx = 0; p140_02_Indx < G05_Qtd_Neuronios[p140_01_Indx]; p140_02_Indx++)
		{
			for (p140_05_Indx = 1; p140_05_Indx <= G05_Qtd_Neuronios[p140_01_Indx + 1]; p140_05_Indx++)
			{
				G01_Peso_Variacao[p140_01_Indx][p140_02_Indx][p140_05_Indx] = G07_Taxa_Aprendizado * G01_Delta [p140_01_Indx + 1] +
G06_Momento * G01_Peso_Recuperado[p140_01_Indx][p140_02_Indx][p140_05_Indx];

				G01_Peso_Recuperado[p140_01_Indx][p140_02_Indx][p140_05_Indx] = G01_Peso_Variacao[p140_01_Indx][p140_02_Indx][p140_05_Indx];
			}
		}
	}
//fprintf (p140_Arquivo, "\n");
}



void p190_Recupera_Data_Hora()
{
	G99_05_Tempo = time(NULL);
	G99_06_Tempo = localtime(&G99_05_Tempo);
	strftime (G99_13_Buffer, 256, "%Y", G99_06_Tempo);
	G99_07_Ano = atoi(G99_13_Buffer);
	strftime (G99_13_Buffer, 256, "%m", G99_06_Tempo);
	G99_08_Mes = atoi(G99_13_Buffer);
	strftime (G99_13_Buffer, 256, "%d", G99_06_Tempo);
	G99_09_Dia = atoi(G99_13_Buffer);
	strftime (G99_13_Buffer, 256, "%H", G99_06_Tempo);
	G99_10_Hora = atoi(G99_13_Buffer);
	strftime (G99_13_Buffer, 256, "%M", G99_06_Tempo);
	G99_11_Minuto = atoi(G99_13_Buffer);
	strftime (G99_13_Buffer, 256, "%S", G99_06_Tempo);
	G99_12_Segundo = atoi(G99_13_Buffer);
}


void p200_Treina_Rede(char *p200_Nome_Entrada, char *p200_Nome_Saida)
{
	int p200_01_Indx = 0, p200_02_Indx = 0, p200_03_Indx = 0;
	FILE * p200_04_Arquivo;
	char p200_05_Registro[80]="\0";
	int p200_06_Indx = 0;
	FILE * p200_07_Arquivo;
	int p200_07_Indx = 0,p200_08_Epoca = 0;
	char p200_09_Inicio[20];
	char p200_10_Final[20];

	// abre os arquivos
	p200_04_Arquivo = fopen (p200_Nome_Entrada, "r");
	p200_07_Arquivo = fopen (p200_Nome_Saida, "w");

	// inicia a leitura do arquivo de entrada
	p200_08_Epoca = 1;
//
	p190_Recupera_Data_Hora();
	sprintf (p200_09_Inicio, "%04d/%02d/%02d=%02d:%02d:%02d", G99_07_Ano, G99_08_Mes, G99_09_Dia, G99_10_Hora, G99_11_Minuto, G99_12_Segundo);
//
	while (p200_08_Epoca <= G09_Epoca)
	{
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo,"Epoca=%04d\n\n", p200_08_Epoca);
		}
		rewind (p200_04_Arquivo);

		// le arquivo de entrada
		fgets(p200_05_Registro, 80, p200_04_Arquivo);
		
		while (!feof(p200_04_Arquivo))
		{
			p100_Limpa_Dados_Entrada();
			p200_06_Indx = atoi(strtok(p200_05_Registro, ";"));
		//fprintf (p200_07_Arquivo, "Reg.=%03d ", p200_06_Indx);

		// recupera os dados de entrada da rede
		p110_Recupera_Dados_Entrada(p200_07_Arquivo, p200_06_Indx);
		
		// recupera os dados esperados de saida
		p120_Recupera_Dados_Saida (p200_07_Arquivo, p200_06_Indx);
		
		// faz a parte forward do backpropagation
		p130_Backpropagation_Forward (p200_07_Arquivo);
	
		// faz a parte backward do backpropagation
		p140_Backpropagation_Backward (p200_07_Arquivo);
	
		// le arquivo de entrada
		fgets(p200_05_Registro, 80, p200_04_Arquivo);
	}


	/*
		if (G01_Erro_Padrao < G08_Erro_Alvo)
	{
		// tem que ter um local para guardar os pesos
		G08_Erro_Alvo = G01_Erro_Padrao;
	}
	else
	{
		if (G01_Erro_Padrao > (G08_Erro_Alvo * 1.2))
	{
	if (G99_04_Tem_Log == 1)
	{
		fprintf (G99_01_Arquivo,"Erro Padrao=%f Erro Esperado=%f\n\n",G01_Erro_Padrao, G08_Erro_Alvo);
	}
	printf ("\n\nProcessamento interrompido por causa do erro\n\n");
	p200_08_Epoca = G09_Epoca + 5;
	break;
	}
}
*/
	p200_08_Epoca++;
	}

	p190_Recupera_Data_Hora();
	sprintf (p200_10_Final, "%04d/%02d/%02d=%02d:%02d:%02d",G99_07_Ano, G99_08_Mes, G99_09_Dia, G99_10_Hora, G99_11_Minuto,G99_12_Segundo);

	if (G99_04_Tem_Log == 1)
	{
		if (p200_08_Epoca > G09_Epoca + 5)
			fprintf (G99_01_Arquivo,"Processamento Interrompido - Inicio=%s Final=%s\n\n",p200_09_Inicio, p200_10_Final);
		else
			fprintf (G99_01_Arquivo,"Epoca=%04d Inicio=%s Final=%s\n\n",p200_08_Epoca - 1, p200_09_Inicio, p200_10_Final);
	}
	
	//fclose (p200_04_Arquivo);
	fclose (p200_07_Arquivo);
}



int p210_Avalia_Rede()
{
}



int main(int argc, char *argv[])
{
	float L01_Entrada = 2;
	float L02_Saida = 0.;
	char L03_Nome_Arquivo [32];
	char L04_Nome_Arquivo [32];
	char L05_Nome_Arquivo_Aux [32] = "\0";
	
	printf ("Gravar Log do Processamento (0=Nao / 1=Sim) ?");
	scanf ("%d", &G99_04_Tem_Log);
	printf ("\n\n");
	p190_Recupera_Data_Hora();
	sprintf (L05_Nome_Arquivo_Aux, "%04d%02d%02d_%02d%02d%02d_",G99_07_Ano, G99_08_Mes, G99_09_Dia, G99_10_Hora, G99_11_Minuto,G99_12_Segundo);

	if (G99_04_Tem_Log == 1)
	{
		sprintf (G99_02_Nome_Arquivo,"%04d%02d%02d_%02d%02d%02d_LOG.TXT",G99_07_Ano, G99_08_Mes, G99_09_Dia, G99_10_Hora,G99_11_Minuto,
G99_12_Segundo);
		G99_01_Arquivo = fopen (G99_02_Nome_Arquivo, "w");
	}
	
	if (p010Le_Entrada()==0)
	{
		p020_Inicializa_Randomizacao();
		p040_Cria_Rede();
	
		//strcpy (L03_Nome_Arquivo, L05_Nome_Arquivo_Aux);
		strcat (L03_Nome_Arquivo, "PR.TXT");
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo,"Peso Recuperado=%s\n\n", L03_Nome_Arquivo);
		}

		p050_Recupera_Peso(L03_Nome_Arquivo);

		//strcpy (L03_Nome_Arquivo, L05_Nome_Arquivo_Aux);

		strcat (L03_Nome_Arquivo, "DADO_TRE.TXT");
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo,"Dado Teino=%s\n\n", L03_Nome_Arquivo);
		}
		
		p070_Seleciona_Massa(L03_Nome_Arquivo, 0.4);
		//strcpy (L03_Nome_Arquivo, L05_Nome_Arquivo_Aux);
		
		strcat (L03_Nome_Arquivo, "DADO_TES.TXT");
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo,"Dado Teste=%s\n\n", L03_Nome_Arquivo);
		}	
		p070_Seleciona_Massa(L03_Nome_Arquivo, 0.3);
		
		//strcpy (L03_Nome_Arquivo, L05_Nome_Arquivo_Aux);
		strcat (L03_Nome_Arquivo, "DADO_TRE.TXT");
		strcpy (L04_Nome_Arquivo, L05_Nome_Arquivo_Aux);
		strcat (L04_Nome_Arquivo, "SAIDA_TRE.TXT");
	
		if (G99_04_Tem_Log == 1)
		{
			fprintf (G99_01_Arquivo,"Saida Treino=%s\n\n", L04_Nome_Arquivo);
		}	
		p200_Treina_Rede(L03_Nome_Arquivo, L04_Nome_Arquivo);
	
		//p210_Avalia_Rede();
		//p200_Treina_Rede("RNL-TS.TXT", "RNL-SS.TXT");
		//printf ("%f ", p080_Funcao_Ativacao(L01_Entrada,G10_Funcao_Ativacao));
		//printf ("%f\n",
		//p090_Derivada_Funcao_Ativacao(L01_Entrada,G10_Funcao_Ativacao));
	}
	fclose(G99_01_Arquivo);
	system("PAUSE");
	return;
}

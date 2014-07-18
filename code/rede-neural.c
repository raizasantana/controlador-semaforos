#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <float.h>

#define raizquadrada(x) ((x) * (x))



int * qtd_neuronio;
double * entrada_01;
double ** dado;
double *** peso_recuperado;
double *** peso_salvo;
double *** peso_variacao;
double * saida_esperada;
double * saida_calculada;
double erro_padrao;
double * delta;

// area para guardar os dados do arquivo
typedef struct
{
	int sequencia;
	int * parametro;
	int ordem;
} ENTRADA;

ENTRADA *entrada;

int qtd_registro;
int qtd_camadas;
int * qtd_neuronios;
double momento;
double taxa_aprendizado;
double erro_alvo;
int epoca_global;
int funcao_ativacao_var;
int saida_errada = 0;
FILE * arquivo_01;
char nome_arquivo_02[80]="\0";
char registro[80]="\0";
int tem_log = 0;

time_t tempo;
struct tm *tempo_struct;

int ano, mes, dia, hora, minuto, segundo;
char buffer[256];


int le_entrada() // OK
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

		if (tem_log == 1)
		{
			fprintf (arquivo_01, "\n[!!!] Erro na Abertura do Arquivo de Entrada da RN\n\n");
		}
		
		return(9);
	}

	
	if (tem_log == 1)
	{
		fprintf (arquivo_01, "Arquivo=%s\n\n",arquivo_entrada_rn);
	}

	
	fgets(registro, 80, arquivo);
	
	if (!feof(arquivo)) // lendo os parametros para montagem da rede
	{
		qtd_registro = atoi(strtok(registro,";"));

		if (tem_log == 1)
		{
			fprintf (arquivo_01, "Registros=%d ", qtd_registro);
		}

		qtd_camadas = atoi(strtok(NULL,";"));

		if (tem_log == 1)
		{
			fprintf (arquivo_01, "Camadas=%d ", qtd_camadas);
		}
	
		qtd_neuronios = (int *) calloc (qtd_camadas,sizeof(int));

		for (index_01 = 0; index_01 < qtd_camadas;index_01++)
		{
			qtd_neuronios[index_01] = atoi(strtok(NULL,";"));

			if (tem_log == 1)
			{
				fprintf (arquivo_01, "Neuronios[%d]=%d",index_01+1, qtd_neuronios[index_01]);
			}
		}

		momento = atof (strtok(NULL,";"));
	
		if (tem_log == 1)
		{
			fprintf (arquivo_01, "\n\n");
			fprintf (arquivo_01, "Momento=%f ", momento);
		}

		taxa_aprendizado = atof (strtok(NULL,";"));

		if (tem_log == 1)
		{
			fprintf (arquivo_01, "Tx.Aprendizado=%f ",taxa_aprendizado);
		}

		erro_alvo = atof (strtok(NULL, ";"));

		if (tem_log == 1)
		{
			fprintf (arquivo_01, "Erro=%f ", erro_alvo);
		}

		epoca_global = atoi (strtok(NULL, ";"));
		
		if (tem_log == 1)
		{
			fprintf (arquivo_01, "Epoca=%d ",epoca_global);
		}
	
		funcao_ativacao_var = atoi (strtok(NULL, ";"));
	
		if (tem_log == 1)
		{
			fprintf (arquivo_01, "Funcao Ativacao=%d\n\n",funcao_ativacao_var);
		}
	}
	else
	{
		if (tem_log == 1)
		{
			fprintf (arquivo_01, "\n[!!!] Erro na 1a. Leitura do Arquivo de entrada da RN\n\n");
		}

		printf("\n[!!!] Problema na 1a. Leitura do Arquivo de Entrada da RN\n\n");
		return(8);
	}
		
	// alocando o espaco necessario para conter os registros de entrada
	entrada = (ENTRADA *) calloc (qtd_registro,sizeof(ENTRADA));

	for (index_01 = 0; index_01 < qtd_registro;index_01++)
		entrada[index_01].parametro = (int *) calloc (qtd_neuronios[0], sizeof(int)); //[0] é a qtd de neuronios na camada de entrada
	

	// Carregando os registros de entrada
	while (fgets (registro, 80, arquivo) != NULL)
	{
	

		//puts(registro);
		aux = atoi(strtok(registro, ";"));

		index_01 = aux-1;
		
		entrada[index_01].sequencia = aux;

		aux = atoi(strtok(NULL, ";"));

		entrada[index_01].parametro[0] = aux;	

		if (qtd_neuronios[0] > 1)
		{
			aux = atoi(strtok(NULL, ";"));
			entrada[index_01].parametro [1] = aux;
		}

		if (qtd_neuronios[0] > 2)
		{
			aux = atoi(strtok(NULL, ";"));
			entrada[index_01].parametro [2] = aux;
		}

		if (qtd_neuronios[0] > 3)
		{
			aux = atoi(strtok(NULL, ";"));
			entrada[index_01].parametro[3] = aux;
		}

		aux = atoi(strtok(NULL, ";"));

		entrada[index_01].ordem = aux;
	}

	fclose(arquivo);


	return(0);
}



void inicializa_randomizacao()
{
	srand((unsigned) time(NULL));
}


double encontra_real(float menor_valor, float maior_valor)
{ 
	double aux = 0;
	aux = ((((double)(rand()%10))/10) - maior_valor);
	return (aux);
}


void cria_rede()
{
	int index_01 = 0, index_02 = 0, index_03 = 0, index_04 = 0;

	// define a rede de acordo com a quantidade de camadas
	qtd_neuronio = (int *) calloc (qtd_camadas, sizeof(int));
	
	for (index_01 = 0; index_01 < qtd_camadas;index_01++)
	{
		qtd_neuronio [index_01] = qtd_neuronios[index_01];
	}

	dado = (double **) calloc (qtd_camadas, sizeof(double *));
	
	for (index_01 = 0; index_01 < qtd_camadas;index_01++)
	{
		index_04 = 1;
		dado[index_01] = (double *)calloc (qtd_neuronios[index_01] + index_04,sizeof(double));
	}

	for (index_01 = 0; index_01 < qtd_camadas;index_01++)
	{
		for (index_02 = 0; index_02 <= qtd_neuronios[index_01];index_02++)
		{
			if (index_02 == 0)
				dado[index_01][index_02] = 1.0;
			else
				dado[index_01][index_02] = 0.0;
		}
	}

	saida_esperada = (double *)calloc (qtd_neuronios [qtd_camadas - 1],sizeof(double));
	
	saida_calculada = (double *) calloc (qtd_neuronios [qtd_camadas - 1],sizeof(double));

	for (index_01 = 0; index_01 < qtd_neuronios[qtd_camadas - 1];index_01++)
	{
		saida_esperada [index_01] = 0.;
		saida_calculada [index_01] = 0.;
	}

	peso_recuperado = (double ***) calloc (qtd_camadas - 1,sizeof(double **));
	
	peso_salvo = (double ***) calloc (qtd_camadas - 1,sizeof(double **));
	
	peso_variacao = (double ***) calloc (qtd_camadas - 1,sizeof(double **));

	for (index_01 = 0; index_01 < qtd_camadas - 1;index_01++)
	{
		index_04 = 1;

		peso_recuperado[index_01]= (double **)

		calloc (qtd_neuronios[index_01] + index_04,sizeof(double *));

		peso_salvo[index_01] = (double **)calloc (qtd_neuronios[index_01] + index_04,sizeof(double *));

		peso_variacao[index_01] = (double **)calloc (qtd_neuronios[index_01] + index_04,sizeof(double *));

		for (index_02 = 0;index_02 <= qtd_neuronios[index_01];index_02++)
		{
			peso_recuperado[index_01][index_02] = (double *) calloc (qtd_neuronios[index_01 + 1],sizeof(double));

			peso_salvo[index_01][index_02] = (double *) calloc (qtd_neuronios[index_01 + 1],sizeof(double));
	
			peso_variacao[index_01][index_02] = (double *) calloc (qtd_neuronios[index_01 + 1], sizeof(double));
		}
	}

	delta = (double *) calloc (qtd_camadas, sizeof (double));

	for (index_01 = 0; index_01 < qtd_camadas;index_01++)
	{
		delta[index_01] = 0.;
	}

}


void recupera_peso(char *nome_arquivo)
{
	int index_01, index_02, index_03;
	FILE * arquivo;
	char registro[80]="\0";
	
	
	arquivo = fopen (nome_arquivo, "r");

	if (!arquivo)
	{
		arquivo = fopen (nome_arquivo, "w");
	}

	// Soh deve existir ateh a camada oculta antes da camada de saida
	for (index_01 = 0; index_01 < qtd_camadas - 1;index_01++)
	{	
		for (index_02 = 0; index_02 <= qtd_neuronio[index_01]; index_02++)
		{
			for (index_03 = 0;index_03 <= qtd_neuronio[index_01 + 1];index_03++)
			{	
				peso_recuperado[index_01][index_02][index_03] = encontra_real(-0.5, 0.5);

				peso_salvo[index_01][index_02][index_03] = 0.;

				peso_variacao[index_01][index_02][index_03] = 0.;

				fprintf(arquivo, "Peso[%d][%d][%d]=%f\n",index_01, index_02, index_03,peso_recuperado[index_01][index_02][index_03]);
			}
		}
	}
	
	fclose(arquivo);
}



int recupera_inteiro(int menor_valor, int maior_valor)
{
	int aux = 0;
	
	aux = rand() % ((maior_valor - menor_valor)+ 1);
	aux += menor_valor;
	return (aux);
}



void seleciona_massa(char *nome_arquivo, double percentual)
{
	FILE * arquivo;
	char registro[80]="\0";
	int index_01 = 0;
	float perc = 0.;
	int * tabela;
	int index_02 = 0;

	// criando a tabela para ter os registros selecionados;
	tabela = (int *) calloc (qtd_registro, sizeof(int));
	
	for (index_01 = 0; index_01 < qtd_registro;index_01++)
		tabela[index_01] = 0;

	// abrindo o arquivo de treino
	arquivo = fopen (nome_arquivo, "r");
	
	if (!arquivo)
	{
		arquivo = fopen (nome_arquivo, "w");
		perc = qtd_registro * percentual;
		index_01 = perc;

		while (1==1)
		{
			index_02 = recupera_inteiro(1, qtd_registro);
			
			if (tabela[index_02 - 1] == 0)
			{
				index_01--;
	
				if (index_01 <= 0)
				break;
	
				tabela[index_02 - 1] = 1;
			}
		}

		for (index_01 = 0; index_01 < qtd_registro;index_01++)
		{

			if (tabela[index_01] == 1)
				fprintf(arquivo, "%d\n", index_01);
		}
		
		fclose(arquivo);
	}	
}



float funcao_ativacao (double dado, int funcao)
{	

	if (funcao == 1) 
		return (1.0 / (1.0 +exp(-dado) ) );
	else 
		return (0);
}


float derivada_funcao_ativacao (float dado, int funcao)
{
	float resultado = 0;
	
	// faz calculo da sigmoide
	resultado = funcao_ativacao (dado, funcao);

	if (funcao == 1) // sigmoide
		return (resultado * (1.0 - resultado));
	else // tangente hiperbolica
		return (0);
}



void limpa_dados_entrada ()
{
	int index_01 =0, index_02 = 0;

	// limpa os dados de entrada
	for (index_01 = 0; index_01 < qtd_camadas;index_01++)
	{
		for (index_02 = 0; index_02 <= qtd_neuronios[index_01];index_02++)
		{	
			if (index_02 == 0)
				dado[index_01][index_02] = 1.0;
			else
				dado[index_01][index_02] = 0.0;
		}
	}
}



void recupera_dados_entrada(FILE * arquivo, int index)
{
	int index_01 = 0, index_02 = 0;

	
	for (index_01 = 1; index_01 <= qtd_neuronios[0];index_01++)
	{
		if (index_01 == 1)
		{
			dado[index_02][index_01 - 1] = 1.0;
		//fprintf(arquivo, "B%d=%f ", index_01 - 1,
		//dado[index_02][index_01 - 1]);
		}
		
		dado[index_02][index_01] = ((double)
		
		entrada[index - 1].parametro[index_01 - 1]);
		//fprintf(arquivo, "I%d=%f ", index_01,
		//dado[index_02][index_01]);
	}
}




void recupera_dados_saida (FILE * arquivo, int index)
{
	int index_01 = 0, index_02 = 0;

	index_02 = qtd_camadas - 1;
	
	for (index_01 = 1; index_01 <= qtd_neuronios[index_02]; index_01++)
	{
		saida_esperada [index_01] = ((double)entrada[index].ordem / 3) ;
		//fprintf(arquivo, "O%d=%f %d\n", index_01,
		//saida_esperada[index_01],
		//entrada[index].ordem);
	}
}



void backpropagation_forward (FILE * arquivo)
{
	int index_01 = 0, index_02 = 0, index_03 = 0;
	double aux = 0.;
	
	for (index_01 = 1; index_01 < qtd_camadas;index_01++)
	{	
		for (index_02 = 1; index_02 <= qtd_neuronio[index_01];index_02++)
		{
			for(index_03 = 0; index_03 <= qtd_neuronio[index_01 - 1];index_03++)
			{
				if (tem_log == 1)
				{	
					fprintf (arquivo_01,"01=%d 02=%d 03=%d\n", index_01, index_02,index_03);
				}
		// calcula o somatorio do produto do neuronio pelo peso
		aux = dado[index_01][index_02];
		//fprintf (arquivo, " Ant(%d, %d)=%f ",
		//index_01, index_02, aux);

		dado[index_01][index_02] = aux + ((double) dado[index_01 - 1][index_03] * peso_recuperado[index_01 - 1][index_03][index_02]);

//fprintf (arquivo, "Dado(%d,%d)=%f * Peso(%d,%d,%d)=%f Atual(%d,%d)=%f",
//index_01 - 1, index_03,
//dado[index_01 - 1][index_03],
//index_01 - 1, index_03, index_02,
//peso_recuperado[index_01 -1][index_03][index_02],
//index_01, index_02,
//dado[index_01][index_02]);
}
//fprintf(arquivo, "\n");
//fprintf (arquivo, "Somatorio(%d,%d)=%f ",
//index_01, index_02,
//dado[index_01][index_02]);
		dado[index_01][index_02] = funcao_ativacao(dado[index_01][index_02],funcao_ativacao_var);
//fprintf (arquivo, "Resultado(%d,%d)=%f\n",
//index_01, index_02,
//dado[index_01][index_02]);
		}
	}
}


void backpropagation_backward (FILE * arquivo)
{
	int index_01 = 0, index_02 = 0;
	double aux =0.;
	int saida_errada = 0, index_03 = 0;
	
	saida_errada++;
	index_01 = qtd_camadas - 1;

	for (index_02 = 1;index_02 <= qtd_neuronio[index_01]; index_02++)
	{
		saida_calculada [index_02 - 1] = dado [index_01][index_02];
//fprintf (arquivo, "Saida(%d)=%f\n",
//index_02, dado[index_01][index_02]);

		if (saida_calculada [index_02 - 1] != saida_esperada [index_02])
		{
			saida_errada++;
			saida_errada = 1;
			erro_padrao += 0.5 * raizquadrada(saida_esperada[index_02] - saida_calculada [index_02 - 1]);
			
			fprintf (arquivo,"%f;%f\n",saida_esperada[index_02],saida_calculada [index_02 - 1]);
//"Saida Esperada[%d]=%f Calculada(%d)=%f\n",
//index_02, saida_esperada[index_02],
//index_02, saida_calculada [index_02 - 1]);


// achar o delta erro da camada de saida
			delta[index_01] = (saida_esperada[index_02 - 1] - saida_calculada [index_02 - 1])* saida_calculada [index_02 - 1] * (1 - saida_calculada [index_02 - 1]);
		}
		else
		{
			saida_errada = 0;
		}
		// achar o delta das camadas intermediarias
		aux = 0;
	
		for (index_01 = qtd_camadas - 2; index_01 > 0;index_01--);
		{
			for(index_02 = 0; index_02 <= qtd_neuronios[index_01];index_02++)
			{
				for (index_03 = 0; index_03 < qtd_neuronios[index_01 + 1]; index_03++)
				{
					aux += 1;
					peso_recuperado[index_01][index_02][index_03] * delta [index_01 + 1];
				}
			}	
		
			delta[index_01] = aux * dado [index_01][index_02] * (1 - dado [index_01][index_02]);
		}
	}
	// atualiza os pesos da camada de entrada para a camada	intermediaria

	for (index_01 = 0; index_01 < qtd_camadas - 1;index_01++)
	{
		for (index_02 = 0; index_02 < qtd_neuronios[index_01]; index_02++)
		{
			for (index_03 = 1; index_03 <= qtd_neuronios[index_01 + 1]; index_03++)
			{
				peso_variacao[index_01][index_02][index_03] = taxa_aprendizado * delta [index_01 + 1] +
momento * peso_recuperado[index_01][index_02][index_03];

				peso_recuperado[index_01][index_02][index_03] = peso_variacao[index_01][index_02][index_03];
			}
		}
	}
//fprintf (arquivo, "\n");
}



void recupera_data_hora()
{

	tempo = time(NULL);

	tempo_struct = localtime(&tempo);

	strftime (buffer, 256, "%Y", tempo_struct);
	ano = atoi(buffer);
	strftime (buffer, 256, "%m", tempo_struct);
	mes = atoi(buffer);
	strftime (buffer, 256, "%d", tempo_struct);
	dia = atoi(buffer);
	strftime (buffer, 256, "%H", tempo_struct);
	hora = atoi(buffer);
	strftime (buffer, 256, "%M", tempo_struct);
	minuto = atoi(buffer);
	strftime (buffer, 256, "%S", tempo_struct);
	segundo = atoi(buffer);
}


void treina_rede(char *nome_entrada, char *nome_saida)
{
	int index_01 = 0, index_02 = 0, index_03 = 0;
	FILE * arquivo;
	char registro[80]="\0";
	int index_04 = 0;
	FILE * arquivo_02;
	int index_05 = 0,epoca_local = 0;
	char inicio[20];
	char final[20];

	// abre os arquivos
	arquivo = fopen (nome_entrada, "r");	
	if (!arquivo)
	{
		perror("Erro na abertura do arquivo - TREINA REDE.");
		exit (EXIT_FAILURE);	
	}

	arquivo_02 = fopen (nome_saida, "w");
	if (!arquivo_02)
	{
		perror("Erro na abertura do arquivo - TREINA REDE.");
		exit (EXIT_FAILURE);	
	}

	// inicia a leitura do arquivo de entrada
	epoca_local = 1;

	recupera_data_hora();
	sprintf (inicio, "%04d/%02d/%02d=%02d:%02d:%02d", ano, mes, dia, hora, minuto, segundo);
	puts(inicio);
	printf("%d\n",epoca_global);


	while (epoca_local <= epoca_global)
	{
		if (tem_log == 1)
		{
			fprintf (arquivo_01,"Epoca=%04d\n\n",epoca_local);
		}
		
		
		while(fgets (registro, 80, arquivo) != NULL)
		{
			limpa_dados_entrada();
			index_04 = atoi(strtok(registro, ";"));
		}
		rewind (arquivo);
	
	
				
		//fprintf (arquivo_02, "Reg.=%03d ", index_04);

		// recupera os dados de entrada da rede
		recupera_dados_entrada(arquivo_02, index_04);
		
		// recupera os dados esperados de saida
		recupera_dados_saida (arquivo_02, index_04);
		
		// faz a parte forward do backpropagation
		backpropagation_forward (arquivo_02);
	
		// faz a parte backward do backpropagation
		backpropagation_backward (arquivo_02);

		// le arquivo de entrada
		fgets(registro, 80, arquivo);
		

	/*
		if (erro_padrao < erro_alvo)
	{
		// tem que ter um local para guardar os pesos
		erro_alvo = erro_padrao;
	}
	else
	{
		if (erro_padrao > (erro_alvo * 1.2))
	{
	if (tem_log == 1)
	{
		fprintf (arquivo_01,"Erro Padrao=%f Erro Esperado=%f\n\n",erro_padrao, erro_alvo);
	}
	printf ("\n\nProcessamento interrompido por causa do erro\n\n");
	epoca_local =epoca_global + 5;
	break;
	}
}*/

	epoca_local++;
}

	recupera_data_hora();
	sprintf (final, "%04d/%02d/%02d=%02d:%02d:%02d",ano, mes, dia, hora, minuto,segundo);

	if (tem_log == 1)
	{
		if (epoca_local >epoca_global + 5)
			fprintf (arquivo_01,"Processamento Interrompido - Inicio=%s Final=%s\n\n",inicio, final);
		else
			fprintf (arquivo_01,"Epoca=%04d Inicio=%s Final=%s\n\n",epoca_local - 1, inicio, final);
	}
	
	fclose (arquivo);
	fclose (arquivo_02);
}



int p210_Avalia_Rede()
{
}
	

int main(int argc, char *argv[])
{
	float entrada = 2;
	float saida = 0.;
	char nome_arquivo_01[80] = "arquivos/entrada_rn.txt";
	char nome_arquivo_PR [80] = "arquivos/";
	char nome_arquivo_D_TRE [80] = "arquivos/";
	char nome_arquivo_D_TES [80] = "arquivos/";
	char nome_arquivo_S_TRE [80] = "arquivos/";
	char nome_arquivo_aux [80] = "arquivos/";
	
		printf("//********************************************************************\n//                                Rede Neural\n//********************************************************************\n\n");
	printf("\n\n\nGravar Log do Processamento? (0=Nao / 1=Sim) ");
	scanf("%d", &tem_log);

	recupera_data_hora();
	sprintf (nome_arquivo_aux, "arquivos/%04d%02d%02d_%02d%02d%02d_",ano, mes, dia, hora, minuto,segundo);

	if (tem_log == 1)
	{
		sprintf (nome_arquivo_02,"log/%04d%02d%02d_%02d%02d%02d_LOG.TXT",ano, mes, dia, hora,minuto,
segundo);
		arquivo_01 = fopen (nome_arquivo_02, "w");
	}
	
	if (le_entrada()==0)
	{
		inicializa_randomizacao();
		cria_rede();

		strcat (nome_arquivo_PR, "PR.TXT");
		puts(nome_arquivo_PR);
		if (tem_log == 1)
		{
			fprintf (arquivo_01,"Peso Recuperado=%s\n\n", nome_arquivo_PR);
		}
		
		recupera_peso(nome_arquivo_01);
		
		strcat (nome_arquivo_D_TRE, "DADO_TRE.TXT");
		if (tem_log == 1)
		{
			fprintf (arquivo_01,"Dado Teino=%s\n\n", nome_arquivo_D_TRE);
		}
		
		seleciona_massa(nome_arquivo_01, 0.4);
		
		
		strcat (nome_arquivo_D_TES, "DADO_TES.TXT");
		if (tem_log == 1)
		{
			fprintf (arquivo_01,"Dado Teste=%s\n\n",nome_arquivo_D_TES);
		}	
		
		seleciona_massa(nome_arquivo_01, 0.3);
			
		//strcpy (nome_arquivo_01, nome_arquivo_aux);
		strcat (nome_arquivo_D_TRE, "DADO_TRE.TXT");
		puts(nome_arquivo_D_TRE);
		strcpy (nome_arquivo_02, nome_arquivo_aux);

		strcat (nome_arquivo_S_TRE, "SAIDA_TRE.TXT");
	
		if (tem_log == 1)
		{
			fprintf (arquivo_01,"Saida Treino=%s\n\n", nome_arquivo_02);
		}	
	
		puts(nome_arquivo_01);
		treina_rede(nome_arquivo_01, nome_arquivo_02);
	
		//p210_Avalia_Rede();
		//treina_rede("RNL-TS.TXT", "RNL-SS.TXT");
		//printf ("%f ", funcao_ativacao(entrada,funcao_ativacao));
		//printf ("%f\n",
		//derivada_funcao_ativacao(entrada,funcao_ativacao));*/

	}
	fclose(arquivo_01);
	
	return 0;
}

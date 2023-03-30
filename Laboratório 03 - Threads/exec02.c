/**
 * Código de emultiemplo para paralelização de dados com pthreads.
 * Problema: calcularar média aritimetica das linhas, e média geometrica das colunas.

 * o codigo consiste em escrever uma matriz em um arquivo .in, fazer a leitura e realizar
  calculos de media aritimetica e geometria. As respotas são escritas no arquivo de saida
  respostas.txt
 */

/*
    Autores:
        Diogo Rodrigues - 2380252
        Carolina Yumi Fujii - 2335468
        Marcos Vinicius de Quadros - 2380560

    Data de criação: 30/03/2023
*/

#include <pthread.h> // pThreads_create(), pThreadJoin()
#include <stdlib.h>  //
#include <ctype.h>   // pThread_t
#include <stdio.h>   // printf()
#include <stdbool.h> // bool
#include <math.h>    // pow()
#include "matriz.h"  // print_matrix(), create_matrix(), generate_elements()
#include <unistd.h>

float aritmethicAvarage[200];
double geometricAvarage[200];
int aritimethicSize = 0;
int geomtricSize = 0;

/* estrutura para passar os parâmetros para as threads */
typedef struct matrizDate
{
    int r; // linhas da matriz
    int c; // colunas da matriz
    int **matriz;
} matrizDate;

/* estrutura que guarda os dados para utilizar no calculo da media aritimetica */
typedef struct aritmethicDates
{
    int finishLine;  // ultima linha que a thread fará o calculo
    int initialLine; // primeira linha que a thread fará o calculo
    int *array;      // array com as posições finais que a thread irá calcular

    int initialColumn; // primeiro coluna lida pela threada
    int finishColumn;  // ultima linha lida pela thread
    int *arrayColuna;  // array com as posições finais de cada coluna

    matrizDate *dates; // estrutura com os dados da matriz que será lida
} aritmethicDates;

/* escreve as respostas no arquivo de saida */
void writeWrongsAritmethic();

/* escreve as reposta no arquivo de saida */
void writeWrongsGeometric();

/* cria uma matriz com valores aleatorios */
int **
newMatriz(int, int);

/* cria uma estrutura de matrizDate */
matrizDate *newMatrizDate(int, int);

/* escreve os dados da matriz no arquivo de saida */
void writeMatrizInOutput(int **, int, int);

/* copia os dados de uma entrada para um determinada matriz */
void copyDatesForMatriz(matrizDate *);

/* realiza o calculo da media aritimetica da linhas */
void *aritmethicCalculate(void *);

/* calcula a media geometrica da colunas */
void *geometricCalculate(void *);

/* valida a quantidade de entradas */
bool validateInput(int);

/* define as posiçoes que cada thread vai calcular */
void definePositionLines(aritmethicDates *, int, int);

/* define as posições das colunas que cada thread vai calcular */
void definePositionColumns(aritmethicDates *, int, int);

/* chamada na função threads_create(), realiza as duas operações pedidas */
void *
execThread(void *dates);

int main(int argc, char **argv)
{
    if (validateInput(argc))
        return 0;

    int rows = atoi(argv[1]);         // numero de linhas da matriz
    int columns = atoi(argv[2]);      // numero de colunas da matriz
    int threadsAmout = atoi(argv[3]); // quantidade de threads que serão criadas
    int status = 0;                   // variavel que recebe o retorno da função thread_create()

    /* inicializando a matriz */
    int **matriz = newMatriz(rows, columns);    // iniciando uma matriz vazia
    writeMatrizInOutput(matriz, rows, columns); // preenchendo a matriz criada com os valores do arquivo de entrada

    pthread_t threadsRows[threadsAmout];    // threads para calcular média aritimetica
    pthread_t threadsColumns[threadsAmout]; // threads para calcular média geometrica

    /* escrevendo a matriz criada num arquivo de saída */
    matrizDate *dateMatriz = newMatrizDate(rows, columns); // iniciando uma estrutura para informações da matriz
    copyDatesForMatriz(dateMatriz);                        // copiando os dados para a nova estrutura

    /* imprime a matriz gerada */
    printf("\n Matriz %ix%i\n", rows, columns);
    print_matrix(dateMatriz->matriz, dateMatriz->r, dateMatriz->c); // printando a matriz para conferência

    /* criando as threads para calcular media aritimetica */
    for (int i = 0; i < threadsAmout; i++)
    {
        /* criand e preenche a estrutura que sera enviada como parametro */
        aritmethicDates *datesRecent = malloc(sizeof(aritmethicDates));
        datesRecent->array = malloc(threadsAmout * sizeof(int));
        definePositionLines(datesRecent, rows, threadsAmout);

        datesRecent->arrayColuna = malloc(threadsAmout * sizeof(int));
        definePositionColumns(datesRecent, columns, threadsAmout);

        datesRecent->dates = dateMatriz;
        datesRecent->initialLine = datesRecent->array[i - 1];
        datesRecent->finishLine = datesRecent->array[i];

        datesRecent->initialColumn = datesRecent->arrayColuna[i - 1];
        datesRecent->finishColumn = datesRecent->arrayColuna[i];

        /* criação das threads */
        status = pthread_create(&threadsRows[i], NULL, execThread, datesRecent);
    }

    /* aguarda a finalização das threads */
    for (int i = 0; i < threadsAmout; i++)
        pthread_join(threadsRows[i], NULL);

    writeWrongsAritmethic();
    writeWrongsGeometric();

    // imprime os resultados do arquivos respostas.txt no teminal
    printf("\n RESPOSTAS\n");
    execl("/bin/cat", "cat", "respostas.txt", (char *)NULL);
    exit(0);
}

void writeWrongsGeometric()
{

    FILE *saida = fopen("respostas.txt", "a");

    fprintf(saida, "%s", "\n\n * media geometrica das colunas: \n   ");
    for (int i = 0; i < geomtricSize; i++)
    {
        fprintf(saida, "%f ", geometricAvarage[i]);
    }
    fprintf(saida, "%s", "\n");

    fclose(saida);
}

void definePositionColumns(aritmethicDates *dates, int columns, int threadsAmout)
{

    int sizePartition = columns / threadsAmout;

    for (int i = 0; i < threadsAmout; i++)
    {
        dates->arrayColuna[i] = sizePartition * (i + 1);
    }

    dates->arrayColuna[threadsAmout - 1] += columns % threadsAmout;
}

void definePositionLines(aritmethicDates *dates, int rows, int size)
{

    int sizePartition = rows / size;

    for (int i = 0; i < size; i++)
    {
        dates->array[i] = sizePartition * (i + 1);
    }

    dates->array[size - 1] += rows % size;
}

void writeWrongsAritmethic()
{
    FILE *saida = fopen("respostas.txt", "w");

    fprintf(saida, "%s", " * media aritimetica das linhas: \n   ");
    for (int i = 0; i < aritimethicSize; i++)
    {
        fprintf(saida, "%f  ", aritmethicAvarage[i]);
    }

    fclose(saida);
}

int **newMatriz(int rows, int columns)
{
    int **matriz = create_matrix(rows, columns);
    generate_elements(matriz, rows, columns, 99);

    return matriz;
}

matrizDate *newMatrizDate(int rows, int columns)
{
    matrizDate *newMatriz = malloc(sizeof(matrizDate)); // aloca a matriz dinamicamente
    newMatriz->r = rows;
    newMatriz->c = columns;
    newMatriz->matriz = malloc(rows * sizeof(int *)); // aloca as linhas da matriz da estrutura

    /* alocando as colunas da matriz da estrutura */
    for (int i = 0; i < rows; i++)
    {
        newMatriz->matriz[i] = malloc(columns * sizeof(int));
    }

    return newMatriz;
}

void copyDatesForMatriz(matrizDate *dates)
{
    FILE *arqOrigem = fopen("matrizTexto.in", "r"); // abrindo arquivo para leitura

    int i = 0;
    int j = 0;
    int number = 0; // variavel qu guarda o valor lido no arquivo

    while (i < dates->r)
    {
        /* lendo o arquivo */
        fscanf(arqOrigem, "%i", &number);

        /* escreve o valor lido, na linha indexada pelo indice i */
        if (j < dates->c)
        {
            dates->matriz[i][j++] = number;
        }

        /* troca o indexador i, quando o numero maximo de colunas é atingido */
        if (j == dates->c)
        {
            j = 0;
            i++;
        }
    }

    fclose(arqOrigem); // fecha o arquivo leitura
}

void writeMatrizInOutput(int **matriz, int rows, int columns)
{
    /* abrindo arquivo para leitura */
    FILE *arqMatriz = fopen("matrizTexto.in", "w");

    /* copiando os dados da matriz para o arquivo */
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            fprintf(arqMatriz, "%i ", matriz[i][j]);

        fprintf(arqMatriz, "\n");
    }

    /* fechando arquivo */
    fclose(arqMatriz);
}

void *execThread(void *dates)
{
    aritmethicDates *date = dates;
    aritmethicCalculate(date);
    geometricCalculate(date);
}

void *geometricCalculate(void *dates)
{
    aritmethicDates *localdates = dates;
    matrizDate *matriz = localdates->dates;

    double soma = 1;
    double media = 0;

    for (int j = localdates->initialColumn; j < localdates->finishColumn; j++)
    {
        for (int i = 0; i < matriz->r; i++)
        {
            soma *= matriz->matriz[i][j];
        }

        media = pow(soma, (1.0 / matriz->r));
        geometricAvarage[j] = media;
        geomtricSize++;
        soma = 1;
    }

    return NULL;
}

void *aritmethicCalculate(void *dates)
{

    aritmethicDates *localdates = dates;
    matrizDate *matriz = localdates->dates;

    float soma = 0;
    float medeiLinear = 0;

    for (int i = localdates->initialLine; i < localdates->finishLine; i++)
    {
        for (int j = 0; j < matriz->c; j++)
        {
            soma += matriz->matriz[i][j];
        }

        medeiLinear = soma / matriz->c;
        aritmethicAvarage[i] = medeiLinear;
        aritimethicSize++;
        soma = 0;
    }

    return NULL;
}

bool validateInput(int number)
{
    if (number < 4)
    {
        printf("É necessário informar o numero de linhas e colunas da matriz, e o numero de threads!\n");
        return true;
    }
    return false;
}
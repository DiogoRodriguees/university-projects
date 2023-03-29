/**
 * Código de emultiemplo para paralelização de dados com pthreads.
 * Problema: Contabilizar o número de consoantes e vogais em um arquivo temultito.
 * Solução paralela: Atribuir a contabilização para threads. Cada thread processa
 * uma parte do temultito. Ao final, junta-se as respostas.
 */

#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "matriz.h"

/* estrutura para passar os parâmetros para as threads */
typedef struct matrizDate
{
    int r;
    int c;
    int **matriz;
} matrizDate;

/* estrutura que guarda os dados para utilizar no calculo da media aritimetica */
typedef struct aritmethicDates
{
    int line;
    matrizDate *dates;
} aritmethicDates;

float vetor_respostas[99];

/* cria uma matriz com valores aleatorios */
int **newMatriz(int, int); 

/* cria uma estrutura de matrizDate */
matrizDate *newMatrizDate(int, int); 

/* escreve os dados da matriz no arquivo de saida */
void writeMatrizInFile(int **, int, int); 

/* copia os dados de uma entrada para um determinada matriz */
void copyDatesForMatriz(matrizDate *);    

/* realiza o calculo da media aritimetica da linhas */
void *aritmethicCalculate(void *);        

bool validateInput(int);

int main(int argc, char **argv)
{
    if (validateInput(argc))
        return 0;

    int rows = atoi(argv[1]);         // numero de linhas da matriz
    int columns = atoi(argv[2]);      // numero de colunas da matriz
    int threadsAmout = atoi(argv[3]); // quantidade de threads que serão criadas

    int **matriz = newMatriz(rows, columns);  // iniciando uma matriz vazia
    writeMatrizInFile(matriz, rows, columns); // preenchendo a matriz criada com os valores do arquivo de entrada

    pthread_t threadsRows[threadsAmout];    // threads para calcular média aritimetica
    pthread_t threadsColumns[threadsAmout]; // threads para calcular média geometrica

    matrizDate *dateMatriz = newMatrizDate(rows, columns); // iniciando uma estrutura para guardar os dados
    copyDatesForMatriz(dateMatriz);                        // copiando os dados para a nova estrutura

    print_matrix(dateMatriz->matriz, rows, columns); // printando a matriz para conferência

    /* criando as threads para calcular media aritimetica */
    for (int i = 0; i < threadsAmout; i++)
    {
        /* criando estrutra e preenchendo os dados */
        aritmethicDates *dates = malloc(sizeof(aritmethicDates));
        dates->line = i;
        dates->dates = dateMatriz;

        /* criação das threads */
        pthread_create(&threadsRows[i], NULL, aritmethicCalculate, dates);
    }

    return 0;
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

void writeMatrizInFile(int **matriz, int rows, int columns)
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

void *aritmethicCalculate(void *dates)
{
    aritmethicDates *localdates = dates;

    float media, x, soma;
    media = x = soma = 0;

    for (int i = 0; i <= localdates->dates->r; i++)
    {
        for (int j = 0; j < localdates->dates->c; j++)
        {
            if (i == localdates->line)
            {
                soma += localdates->dates->matriz[i][j];
            }
        }
    }

    media = soma / localdates->dates->c;

    printf("%f", soma);
    return NULL;
}

bool validateInput(int number)
{
    if (number < 4)
    {
        printf("É necessário informar o numero de linhas e colunas da matriz, e o numero de threads!\n");
        return false;
    }
    return true;
}
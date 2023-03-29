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
#include <math.h>
#include "matriz.h"

/* estrutura para passar os parâmetros para as threads */
typedef struct matrizDate
{
    int r;
    int c;
    int **matriz;
} matrizDate;

float vetor_respostas[99];

void writeMatrizInFile(int **, int, int);
int **newMatriz(int, int);
matrizDate *newMatrizDate(int, int);
void createThreads(int size);
void copyDatesForMatriz(matrizDate*);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("É necessário informar o numero de linhas e colunas da matriz, e o numero de threads!\n");
        return 0;
    }

    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    int threadsAmout = atoi(argv[3]);

    int **matriz = newMatriz(rows, columns);
    writeMatrizInFile(matriz, rows, columns);

    pthread_t threadsRows[threadsAmout]; // threads para calcular média aritimetica
    pthread_t threadsColumns[threadsAmout]; // threads para calcular média geometrica

    matrizDate *dateMatriz = newMatrizDate(rows, columns); // iniciando uma estrutura para guardar os dados
    copyDatesForMatriz(dateMatriz); // copiando os dados para a nova estrutura

    

    print_matrix(dateMatriz->matriz, rows, columns);

    for (int i = 0; i < threadsAmout; i++)
    {
        // pthread_create(&threadsRows[i], NULL, aritmethicCalculate, 1);
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
    matrizDate *newMatriz = malloc(sizeof(matrizDate));
    newMatriz->r = rows;
    newMatriz->c = columns;
    newMatriz->matriz = malloc(rows * sizeof(int *));

    for (int i = 0; i < rows; i++)
    {
        newMatriz->matriz[i] = malloc(columns * sizeof(int));
    }

    return newMatriz;
}

void copyDatesForMatriz(matrizDate * dates){
    FILE *arqOrigem = fopen("matrizTexto.in", "r");
    int i = 0;
    int j = 0;

    int number = 0;
    while (i < dates->r)
    {
        fscanf(arqOrigem, "%i", &number);
        if (j < dates->c)
        {
            dates->matriz[i][j++] = number;
        }
        if (j == dates->c)
        {
            j = 0;
            i++;
        }
    }
    fclose(arqOrigem);
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

// void *aritmethicCalculate(int rows, int columns)
// {

//     float media, x, soma;
//     media = x = soma = 0;

//     int **matriz = fopen(arqLeitura, "r");

//     for (int i = 0; i <= rows; i++)
//     {
//         for (int j = 0; j < columns; j++)
//         {
//             if (i == linha)
//             {
//                 soma += matriz[i][j];
//             }
//         }
//     }

//     media = soma / columns;

//     printf("%i", soma);
//     return NULL;
// }
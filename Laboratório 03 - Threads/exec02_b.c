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

/* especifica o número de threads e partes */
#define M 7
#define N 8

/* estrutura para passar os parâmetros para as threads */
typedef struct data_chunk
{
    int num_seq;
    int pos_inicio;
    int *vetor;
} data_chunk;
typedef struct elementosDaMAtriz
{
    int vetor[(M * N) + 1];
    int sizeArray;
}  elementosDaMAtriz;

/* variavel global para armazenar as respostas para N threads */
/* coluna 0 = total de vogais e coluna 1 = total consoantes */
double vetor_respostas[N];

/* Thread para contabilizar a média de cada
   linha da matriz informado como param */
void *thread_cont(void *param)
{
    data_chunk *dados = param;

    double media_geometrica, multi;
    media_geometrica = 0;
    multi = 1;

    // printf("Thread id: %ld\n", pthread_self());

    int posicao = dados->pos_inicio;
    for (int j = 0; j < M; j++)
    {
        multi *= dados->vetor[posicao];
        posicao += (N);
    }

    double aux = 1.0/M;
    media_geometrica = pow(multi, aux);

    printf("* Media geométrica da coluna %i: %f\n", dados->pos_inicio,media_geometrica);

    vetor_respostas[dados->num_seq] = media_geometrica;
    return NULL;
}

void showMenssages(int opcao){
    switch (opcao)
    {
    case 1:
        printf("----------------------------------------------\n");
        printf("* Exemplo - Paralelismo de dados com threads *\n");
        printf("----------------------------------------------\n");
        break;
    
    default:
        break;
    }
}

int main(int argc, char **argv)
{

    showMenssages(1);

    // abrindo o arquivo e fazendo leitura
    FILE *file = fopen("matriz_6por8.in", "r");
    fseek(file, 0, SEEK_END);

    // exibi os tamanho do arquivo lido
    int tamanho = ftell(file);
    printf("\nTamanho do arquivo (bytes): %d\n", tamanho);

    int vetor_total[tamanho + 1]; // conteudo do arquivo

    int i = 0;
    int elementoAtual = 0; // recebe o valor que está sendo lido na posição atual da matriz
    rewind(file);

    while (fscanf(file, "%d", &elementoAtual) != EOF)
    {
        vetor_total[i++] = elementoAtual;
    }

    int tamanho_vetor = i;

    fclose(file); // fechando o arquivo

    /* prepara os dados para as threads - divide em partes */
    struct data_chunk dados[N];
    int tam_parte = tamanho_vetor / N;

    printf("TAM_vetor: %d, TAM-Parte: %d\n\n", tamanho_vetor, tam_parte);
    int ini_parte = 0;
    int fim_parte = tam_parte;
    for (int p = 0; p < N; p++)
    {
        dados[p].num_seq = p;
        dados[p].pos_inicio = ini_parte;
        dados[p].vetor = vetor_total;

        ini_parte++;
        fim_parte += tam_parte;
    }

    /* declara, cria e invoca as threads */
    pthread_t t[N];

    for (int p = 0; p < N; p++)
        pthread_create(&t[p], NULL, thread_cont, &dados[p]);

    /* aguarda a finalização das threads */
    for (int p = 0; p < N; p++)
        pthread_join(t[p], NULL);

    /* junta os resultados das threads */
    int total = 0;

    FILE *arquivo_respostas;
    arquivo_respostas = fopen("respostas.txt", "w"); // abre o arquivo em modo de escrita binária

    printf("\nVetor de médias: ");

    fprintf(arquivo_respostas, "vetor_médias: "); // escreve os dados no arquivo

    // escrevendo as reposta no arquivo de saída 
    for (int i = 0; i < N; i++)
    {
        fprintf(arquivo_respostas, "%f ", vetor_respostas[i]); // escreve os dados no arquivo
        printf("%f ", vetor_respostas[i]);
    }

    printf("\n");
    fclose(arquivo_respostas); // fechando arquivo

    return 0;
}

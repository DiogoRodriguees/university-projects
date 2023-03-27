/**
 * Código de emultiemplo para paralelização de dados com pthreads.
 * Problema: Contabilizar o número de consoantes e vogais em um arquivo temultito.
 * Solução paralela: Atribuir a contabilização para threads. Cada thread processa
 * uma parte do temultito. Ao final, junta-se as respostas.
 */

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <math.h>

/* especifica o número de threads e partes */
#define M 6
#define N 8

/* estrutura para passar os parâmetros para as threads */
struct data_chunk
{
    int num_seq;
    int pos_inicio;
    int *vetor;
};

/* variavel global para armazenar as respostas para N threads */
/* coluna 0 = total de vogais e coluna 1 = total consoantes */
double vetor_respostas[N];

/* Thread para contabilizar a média de cada
   linha da matriz informado como param */
void *thread_cont(void *param)
{
    struct data_chunk *dados = param;

    double media_geometrica, multi;
    media_geometrica = 0;
    multi = 1;

    printf("Thread id: %ld\n", pthread_self());

    int posicao = dados->pos_inicio;
    for (int j = 0; j < M; j++)
    {
        printf("%d  ", dados->vetor[posicao]);
        multi *= dados->vetor[posicao];
        posicao += (N);
    }
    printf("\n\n");
    // media_geometrica = pow(multi, 1.0/M);
    // printf("Media geométric apor coluna: %.2f\n\n", media_geometrica);

    vetor_respostas[dados->num_seq] = media_geometrica;
    return NULL;
}

int main()
{
    printf("Emultiemplo - Paralelismo de dados com threads\n\n");

    /* lê o arquivo temultito */
    FILE *file = fopen("matriz_6por8.in", "r");
    fseek(file, 0, SEEK_END);
    int tamanho = ftell(file);
    printf("Tamanho do arquivo (bytes): %d\n\n", tamanho);

    int vetor_total[tamanho + 1]; // conteudo do arquivo

    int num = 0;
    int i = 0;
    rewind(file);
    int tamanho_vetor;
    while (fscanf(file, "%d", &num) != EOF)
    {
        printf("%d\n", num);
        vetor_total[i++] = num;
        tamanho_vetor = i;
    }
    // vetor_total[i] = 0;

    // int coluna[N]; // N é o número de linhas da matriz

    // for (int linha = 0; linha < N; linha++)
    // {
    //     for (i = 0; i < M; i++) // M é o número de colunas da matriz
    //     {
    //         fscanf(file, "%d", &num);
    //         printf("%d ", num);
    //         vetor_total[i++] = num;
    //         tamanho_vetor = i;
    //     }
    //     printf("\n");
    // }

    fclose(file);

    /* prepara os dados para as threads - divide em partes */
    struct data_chunk dados[N];
    int tam_parte = tamanho_vetor / N;

    printf("TAM_vetor: %d, TAM-Parte: %d\n", tamanho_vetor, tam_parte);
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
    arquivo_respostas = fopen("respostas.tmultit", "w"); // abre o arquivo em modo de escrita binária

    printf("Vetor de médias: ");
    fprintf(arquivo_respostas, "vetor_médias: "); // escreve os dados no arquivo
    for (int i = 0; i < N; i++)
    {
        fprintf(arquivo_respostas, "%.1f ", vetor_respostas[i]); // escreve os dados no arquivo
        printf("%.1f ", vetor_respostas[i]);
    }
    printf("\n");

    return 0;
}

/**
 * Código de exemplo para paralelização de dados com pthreads.
 * Problema: Contabilizar o número de consoantes e vogais em um arquivo texto.
 * Solução paralela: Atribuir a contabilização para threads. Cada thread processa
 * uma parte do texto. Ao final, junta-se as respostas.
 */

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

/* especifica o número de threads e partes */
#define N 7

/* estrutura para passar os parâmetros para as threads */
struct data_chunk
{
    int num_seq;
    int pos_inicio, pos_final;
    int *vetor_linha;
};

/* variavel global para armazenar as respostas para N threads */
/* coluna 0 = total de vogais e coluna 1 = total consoantes */
float vetor_respostas[N];

/* Thread para contabilizar a média de cada
   linha da matriz informado como param */
void *aritmethicCalculate(void *param)
{
    struct data_chunk *dados = param;

    float media_linha, x, soma;
    media_linha = x = soma = 0;

    printf("Thread id: %ld, Posicao inicio: %d, Posicao final: %d\n",
           pthread_self(), dados->pos_inicio, dados->pos_final);

    for (int j = dados->pos_inicio; j <= dados->pos_final; j++)
    {
        soma += dados->vetor_linha[j];
        x++;
    }

    media_linha = soma / x;
    printf("Valor por linha: %.2f\t", soma);
    printf("Media por linha: %.2f\n\n", media_linha);

    vetor_respostas[dados->num_seq] = media_linha;

    return NULL;
}

int main()
{
    printf("Exemplo - Paralelismo de dados com threads\n\n");

    /* lê o arquivo texto */
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
        vetor_total[i++] = num;
        tamanho_vetor = i;
    }
    vetor_total[i] = 0;
    fclose(file);

    /* prepara os dados para as threads - divide em partes */
    struct data_chunk dados[N];
    int tam_parte = tamanho_vetor / N;

    // printf("TAM_vetor: %d, TAM-Parte: %d\n", tamanho_vetor, tam_parte);
    int ini_parte = 0;
    int fim_parte = tam_parte;
    for (int p = 0; p < N; p++)
    {
        dados[p].num_seq = p;
        dados[p].pos_inicio = ini_parte;
        dados[p].pos_final = fim_parte - 1;
        dados[p].vetor_linha = vetor_total;

        ini_parte = fim_parte;
        fim_parte += tam_parte;
    }
    dados[N - 1].pos_final = tamanho_vetor - 1;

    /* declara, cria e invoca as threads */
    pthread_t t[N];

    for (int p = 0; p < N; p++)
        pthread_create(&t[p], NULL, aritmethicCalculate, &dados[p]);

    /* aguarda a finalização das threads */
    for (int p = 0; p < N; p++)
        pthread_join(t[p], NULL);

    /* junta os resultados das threads */
    int total = 0;

    FILE *arquivo_respostas;
    arquivo_respostas = fopen("respostas.txt", "w"); // abre o arquivo em modo de escrita binária

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

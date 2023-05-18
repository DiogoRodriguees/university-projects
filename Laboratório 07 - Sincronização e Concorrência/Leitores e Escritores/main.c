/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242
        Marcos Vinicius de Quadros - 2380560

    Descrição:
        Esse programa tem como objetivo, demostrar
        o algoritmo de leitor/escritor que é um
        problema clássico de concorrência.

    Data:
        18 de Maior de 2023
*/

#include <pthread.h>   // pthread
#include <semaphore.h> // sem_t
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t condicao_leitura = PTHREAD_COND_INITIALIZER;
pthread_cond_t condicao_escrita = PTHREAD_COND_INITIALIZER;

int total_escritores = 0;
int total_leitores = 0;

#define QUANTIDADE_DE_ESCRITORES 2
#define QUANTIDADE_DE_LEITORES 4

int buf;

/*
    1° - Realiza a leitura caso o escritor não esteja escrevendo
    2° - Informa que está saindo da operação
*/
void *leitor(void *);

/*
    1° - Escreve algo
    2° - Informa que terminou de fazer a escrita
    3° - Libera para os leitores lerem
*/
void *escritor();

/*
    1° - Bloquear o mutuex
    2° - Verificar se há um esritor escrevendo
    3° - Incrementar numeros de leitores
    4° - Desbloquear o mutex
*/
void ativarLeitor();
void ativarEscrita();

/*
    1° - Bloquear o mutex
    2° - Decrementar o numero de leitores
    3° -
    4° - Desbloquear o mutex
*/
void terminarLeitura();
void terminarEscrita();

int main(int argc, char **argv)
{
    pthread_t array_leitores[QUANTIDADE_DE_LEITORES];
    pthread_t array_escritores[QUANTIDADE_DE_ESCRITORES];

    /* Criar monitor */
    pthread_mutex_init(&mutex, NULL);

    /* Criar threads escritoras */
    for (int i = 0; i < QUANTIDADE_DE_ESCRITORES; i++)
    {
        pthread_create(&array_escritores[i], NULL, escritor, (void *)i);
        sleep(2);
    }

    /* Criar threads leitores */
    for (int i = 0; i < QUANTIDADE_DE_LEITORES; i++)
    {
        pthread_create(&array_leitores[i], NULL, leitor, (void *)i);
        sleep(2);
    }

    /* Esperar as threas terminarem */
    for (int i = 0; i < QUANTIDADE_DE_ESCRITORES; i++)
    {
        pthread_join(array_escritores[i], NULL);
    }
    for (int i = 0; i < QUANTIDADE_DE_LEITORES; i++)
    {
        pthread_join(array_leitores[i], NULL);
    }

    /* Destruir monitor */
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *leitor(void *id)
{
    int id_int = (int *)id;
    for (int i = 0; i < 5; i++)
    {
        ativarLeitor();
        printf("LEITOR %i LENDO %i\n", id_int, buf);
        sleep(1);
        terminarLeitura();
        sleep(2);
    }
}

void *escritor(void *id)
{
    int id_int = (int *)id;

    for (int i = 0; i < 5; i++)
    {
        ativarEscrita();

        printf("ESCRITOR %i ESCREVENDO %i\n", id_int, i);
        buf = i;
        sleep(3);
        terminarEscrita();
        sleep(2);
    }
}

void ativarLeitor()
{
    /* Bloquear o mutex */
    pthread_mutex_lock(&mutex);

    /* Verificar se existe algum escritor escrevendo */
    while (total_escritores > 0)
    {
        /* Bloqueia a thread até o escritor sair */
        pthread_cond_wait(&condicao_leitura, &mutex);
        sleep(1);
    }

    /* Incrementar o numero de leitores */
    total_leitores++;

    /* Desbloquear o mutex */
    pthread_mutex_unlock(&mutex);
}

void terminarLeitura()
{
    /* Bloqueando o mutex */
    pthread_mutex_lock(&mutex);

    /* Decrementa para avisar que o leitor ta saindo */
    total_leitores--;

    /* Caso a thread seja a ultima a escrever */
    if (total_leitores == 0)
    {
        pthread_cond_signal(&condicao_escrita);
    }

    /* Desbloqueando o mutex */
    pthread_mutex_unlock(&mutex);
}

void ativarEscrita()
{
    /* Desbloquenado mutex */
    pthread_mutex_lock(&mutex);

    /* Verifica se tem um escritor escrevendo ou leitor lendo */
    while ((total_leitores > 0) || (total_escritores > 0))
    {
        pthread_cond_wait(&condicao_escrita, &mutex);
        sleep(1);
    }

    /* Incrementa para avisar que tem um leitor escrevendo */
    total_escritores++;

    /* Desbloquenado mutex */
    pthread_mutex_unlock(&mutex);
}

void terminarEscrita()
{
    /* Bloqueando o mutex */
    pthread_mutex_lock(&mutex);

    /* Descrementa para avisar que o escritor parou de escrever */
    total_escritores--;

    /* Envia sinal para a fila de escritores e leitores */
    pthread_cond_signal(&condicao_escrita);
    pthread_cond_broadcast(&condicao_leitura);

    /* Desbloqueandoo o Mutex */
    pthread_mutex_unlock(&mutex);
}
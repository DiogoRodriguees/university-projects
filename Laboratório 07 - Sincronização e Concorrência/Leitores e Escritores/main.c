/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242

    Descrição:
        Esse programa tem como objetivo, demostrar
        o algoritmo de leitor/escritor que é um
        problema clássico de concorrência.

    Data:
        18 de Maior de 2023
*/

#include "resource_monitor.h" // initMonitor(), destroyMonitor()
#include <pthread.h>          // pthread
#include <semaphore.h>        // sem_t
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t condicao_leitura = PTHREAD_COND_INITIALIZER;
pthread_cond_t condicao_escrita = PTHREAD_COND_INITIALIZER;
int totalDeEscritores = 0;
int totalDeLeitores = 0;

/*
    1° - Realiza a leitura caso o escritor não esteja escrevendo
    2° - Informa que está saindo da operação
*/
void *leitor();

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
    pthread_t leitores, escritores;

    /* Criar monitor */
    initMonitor();

    /* Criar threads leitores */
    pthread_create(&leitores, NULL, leitor, NULL);

    /* Criar threads escritoras */
    pthread_create(&escritores, NULL, escritor, NULL);

    /* Esperar as threas terminarem */
    pthread_join(escritores, NULL);
    pthread_join(leitores, NULL);

    /* Destruir monitor */
    destroyMonitor();

    return 0;
}

void *leitor()
{
    ativarLeitor();
    printf("LENDO\n");
    terminarLeitura();

}

void *escritor()
{
    ativarEscrita();
    printf("ESCREVENDO\n");
    terminarEscrita();

}

void ativarLeitor()
{
    /* Bloquear o mutex */
    pthread_mutex_lock(&mutex);

    /* Verificar se existe algum escritor escrevendo */
    while (totalDeEscritores > 0)
    {
        /* Bloqueia a thread até o escritor sair */
        pthread_cond_wait(&condicao_leitura, &mutex);
        sleep(1);
    }

    /* Incrementar o numero de leitores */
    totalDeLeitores++;

    /* Desbloquear o mutex */
    pthread_mutex_unlock(&mutex);
}

void terminarLeitura()
{
    pthread_mutex_lock(&mutex);
    totalDeLeitores--;

    /* Caso a thread seja a ultima a escrever */
    if (totalDeLeitores == 0)
    {
        pthread_cond_signal(&condicao_escrita);
    }
    pthread_mutex_unlock(&mutex);
}

void ativarEscrita()
{
    pthread_mutex_lock(&mutex);
    while ((totalDeLeitores > 0) || (totalDeEscritores > 0))
    {
        pthread_cond_wait(&condicao_escrita, &mutex);
        sleep(1);
    }
    totalDeEscritores++;

    pthread_mutex_unlock(&mutex);
}

void terminarEscrita()
{
    pthread_mutex_lock(&mutex);
    totalDeEscritores--;
    pthread_cond_signal(&condicao_escrita);
    pthread_cond_broadcast(&condicao_leitura);
    pthread_mutex_unlock(&mutex);
}
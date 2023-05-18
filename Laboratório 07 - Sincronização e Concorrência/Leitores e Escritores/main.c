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

#include <pthread.h>          // pthread
#include <semaphore.h>        // sem_t
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t condicao_leitura = PTHREAD_COND_INITIALIZER;
pthread_cond_t condicao_escrita = PTHREAD_COND_INITIALIZER;
int totalDeEscritores = 0;
int totalDeLeitores = 0;

int buf;

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
    pthread_t leitores, leitor2, escritores;

    /* Criar monitor */
    pthread_mutex_init(&mutex, NULL);

    /* Criar threads escritoras */
    pthread_create(&escritores, NULL, escritor, NULL);

    /* Criar threads leitores */
    pthread_create(&leitores, NULL, leitor, NULL);
    pthread_create(&leitor2, NULL, leitor, NULL);

    /* Esperar as threas terminarem */
    pthread_join(leitores, NULL);
    pthread_join(leitor2, NULL);
    pthread_join(escritores, NULL);

    /* Destruir monitor */
    pthread_mutex_destroy(&mutex); /* Free up the_mutex */

    return 0;
}

void *leitor()
{
    for (int i = 0; i < 5; i++)
    {
        ativarLeitor();
        printf("LENDO %i\n", buf);
        sleep(2);
        terminarLeitura();
        sleep(2);
    }
}

void *escritor()
{
    for (int i = 0; i < 5; i++)
    {
        ativarEscrita();

        printf("ESCREVENDO %i\n", i);
        buf = i;
        sleep(4);
        terminarEscrita();
        sleep(1);
    }
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
    /* Bloqueando o mutex */
    pthread_mutex_lock(&mutex);

    /* Decrementa para avisar que o leitor ta saindo */
    totalDeLeitores--;

    /* Caso a thread seja a ultima a escrever */
    if (totalDeLeitores == 0)
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
    while ((totalDeLeitores > 0) || (totalDeEscritores > 0))
    {
        pthread_cond_wait(&condicao_escrita, &mutex);
        sleep(1);
    }

    /* Incrementa para avisar que tem um leitor escrevendo */
    totalDeEscritores++;

    /* Desbloquenado mutex */
    pthread_mutex_unlock(&mutex);
}

void terminarEscrita()
{
    /* Bloqueando o mutex */
    pthread_mutex_lock(&mutex);

    /* Descrementa para avisar que o escritor parou de escrever */
    totalDeEscritores--;

    /* Envia sinal para a fila de escritores e leitores */
    pthread_cond_signal(&condicao_escrita);
    pthread_cond_broadcast(&condicao_leitura);

    /* Desbloqueandoo o Mutex */
    pthread_mutex_unlock(&mutex);
}
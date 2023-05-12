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

#include <resource_monitor.h> // initMonitor(), destroyMonitor()
#include <pthread.h>          // pthread
#include <semaphore.h>

sem_t mutex;

int condicao_leitura = 1;
int totalDeEscritores = 0;
int totalDeLeitores = 0;
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
void *escritor(void *);

/* 
    1° - Bloquear o mutuex
    2° - Verificar se há um esritor escrevendo
    3° - Incrementar numeros de leitores
    4° - Desbloquear o mutex
*/
void ativarLeitor();

int main(int argc, char **argv)
{
    pthread_t leitores, escritores;

    /* Criar monitor */
    initMonitor();

    /* Criar threads leitores */
    pthread_create(leitores, NULL, leitor, NULL);

    /* Criar threads escritoras */
    pthread_create(escritores, NULL, escritor, NULL);

    /* Esperar as threas terminarem */
    pthread(escritores, NULL);
    pthread(leitores, NULL);

    /* Destruir monitor */
    destroyMonitor();

    return 0;
}

void ativarLeitor(){
    /* Bloquear o mutex */
    pthread_mutex_lock(&mutex);
    
    /* Verificar se existe algum escritor escrevendo */
    while(totalDeEscritores > 0){
        /* Bloqueia a thread até o escritor sair */
        pthread_cond_wait(&condicao_leitura, &mutex);
        sleep(1);
    }
    
    /* Incrementar o numero de leitores */
    totalDeLeitores++;
    
    /* Desbloquear o mutex */
    pthread_mutex_unlock(&mutex);
}

void *leitor(void *param) {
    ativarLeitor();

    terminarLeitura();
}

void *escritor(void *param) {}
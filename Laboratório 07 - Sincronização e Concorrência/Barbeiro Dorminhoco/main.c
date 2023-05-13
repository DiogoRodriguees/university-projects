/*
    Autores:
        Diogo Rodrigues dos Santos - 2380232
        Marcos Vinicius de Quadros - 2380560

    Descrição:
        O programa implementa o algoritmo do barbeiro dorminhoco, 
        que consiste em simular uma barbearia com uma cadeira para atendimento e N cadeiras para espera.
        Quando não houver nenhum cliente na cadeira para atendimento, o barbeiro está dormindo. 
        Quando um cliente chega, ele é acordado e atende o cliente. 
        O barbeiro continua atendendo os clientes até que não haja mais clientes na espera. 
        Quando não houver mais clientes na espera, o barbeiro volta a dormir até que um novo cliente chegue. 
        O programa utiliza semáforos para controlar o acesso aos recursos compartilhados e para sincronizar as threads.

    Data:
        13 de Maio 2023
*/

#include <unistd.h>    /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <errno.h>     /* Errors */
#include <stdio.h>     /* Input/Output */
#include <pthread.h>   /* POSIX Threads */
#include <semaphore.h> /* Semaphore */

#define NUMTHREAD 6
#define NUMCHAIR 2

/* semaforo declarado como global */
sem_t sBarbeiro;   // atua como o barbeiro
sem_t cadCorte;    // cadeira do corte
sem_t cortePronto; // informa se o corte está pronto

int num_clientes = 0;
int id_cliente = 0;

void barbeiro(void *ptr);
void cliente(void *ptr);

int main()
{
    int id[NUMTHREAD];
    pthread_t p_barbeiro;
    pthread_t clientes[NUMTHREAD];

    // Identificadores para as Threads.
    for (int i = 0; i < NUMTHREAD; i++)
    {
        id[i] = i;
    }

    sem_init(&cadCorte, 0, 1);    /* inicializa cadeiraCorte com 1 */
    sem_init(&sBarbeiro, 0, 0);   /* inicializa barbeiro com 0 */
    sem_init(&cortePronto, 0, 0); /* inicializa corte com 0 */

    pthread_create(&p_barbeiro, NULL, (void *)&barbeiro, (void *)0);

    /* criar as threads */
    for (int i = 0; i < NUMTHREAD; i++)
    {
        pthread_create(&clientes[i], NULL, (void *)&cliente, (void *)&id[i]);
    }

    for (int i = 0; i < NUMTHREAD; i++)
    {
        pthread_join(clientes[i], NULL);
    }

    pthread_join(p_barbeiro, NULL);
    
    /* destroe os semaforos */
    sem_destroy(&cadCorte);    
    sem_destroy(&cortePronto); 
    sem_destroy(&sBarbeiro);   

    return 0;
}

void barbeiro(void *ptr)
{
    printf("Barbeiro está dormindo\n");
    sem_wait(&sBarbeiro);       // Espera a sinalização de que tem cliente
    printf("Barbeiro acordou\n");

    while (1)
    {
        sleep(1);
        printf("Barbeiro cortou o cabelo do cliente %d\n", id_cliente);
        sem_post(&cortePronto);    // Sinaliza que o corte foi feito
        sem_post(&cadCorte);       // Libera a cadeira para o próximo cliente 
        num_clientes--;            
        if (num_clientes == 0)
        {
            break;
        }
        sem_wait(&sBarbeiro);       // Espera a sinalização de que tem cliente
    }

    printf("Sem clientes na barbearia\n");
    printf("Barbeiro está dormindo\n");
    pthread_exit(0);
}

void cliente(void *cli)
{
    int id = *((int *)cli);

    if (num_clientes <= NUMCHAIR)
    {
        if (num_clientes == 0)
        {
            num_clientes++;
            sem_wait(&cadCorte);    // Bloqueia a cadeira do barbeiro  
            printf("Cliente %d sentou na cadeira do barbeiro\n", id);
            
            sem_post(&sBarbeiro);   // Sinaliza para o barbeiro que tem cliente 
            id_cliente = id;
            
            sem_wait(&cortePronto); // Espera a sinalização do barbeiro de que o corte foi feito
            printf("Cliente %d foi atendido e saiu da barbearia\n", id);
        }
        else
        {
            num_clientes++;
            printf("Cliente %d sentou em uma cadeira de espera\n", id);

            sem_wait(&cadCorte);    // Bloqueia a cadeira do barbeiro  
            sleep(1);
            printf("Cliente %d sentou na cadeira do barbeiro\n", id);
            
            sem_post(&sBarbeiro);   // Sinaliza para o barbeiro que tem cliente
            id_cliente = id;
            
            sem_wait(&cortePronto); // Espera a sinalização do barbeiro de que o corte foi feito
            printf("Cliente %d foi atendido e saiu da barbearia\n", id);
        }
    }
    else
    {
        printf("Não há cadeiras de espera para o cliente %d\n", id);
        printf("Cliente %d foi embora\n", id);
    }
    pthread_exit(0);
}
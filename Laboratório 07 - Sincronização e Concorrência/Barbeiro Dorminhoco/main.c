
/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242
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

#define NUMTHREAD 4    // Quantidade de clientes
#define NUMCHAIR 2      // Quantidade de cadeiras de espera

/* Semaforos declarados como global */
sem_t s_barbeiro;   // Atua como o barbeiro
sem_t cad_corte;    // Cadeira do corte
sem_t corte_pronto; // Informa se o corte está pronto

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

    sem_init(&cad_corte, 0, 1);    /* inicializa cadeiraCorte com 1 */
    sem_init(&s_barbeiro, 0, 0);   /* inicializa barbeiro com 0 */
    sem_init(&corte_pronto, 0, 0); /* inicializa corte com 0 */


    /* criar as threads */
    for (int i = 0; i < NUMTHREAD; i++)
    {
        pthread_create(&clientes[i], NULL, (void *)&cliente, (void *)&id[i]);
    }

    sleep(NUMTHREAD/2);
    pthread_create(&p_barbeiro, NULL, (void *)&barbeiro, (void *)0);

    for (int i = 0; i < NUMTHREAD; i++)
    {
        pthread_join(clientes[i], NULL);
    }

    pthread_join(p_barbeiro, NULL);
    
    /* destroe os semaforos */
    sem_destroy(&cad_corte);    
    sem_destroy(&corte_pronto); 
    sem_destroy(&s_barbeiro);   

    return 0;
}

void barbeiro(void *ptr)
{
    printf("Barbeiro está dormindo\n");
    sem_wait(&s_barbeiro);       // Espera a sinalização de que tem cliente
    printf("Barbeiro acordou\n");

    while (1)
    {
        printf("Barbeiro cortou o cabelo do cliente %d\n", id_cliente);
        sleep(2);
        sem_post(&corte_pronto);    // Sinaliza que o corte foi feito
        sem_post(&cad_corte);       // Libera a cadeira para o próximo cliente 
        num_clientes--;            
        if (num_clientes == 0)
        {
            sleep(1);
            break;
        }
        sem_wait(&s_barbeiro);       // Espera a sinalização de que tem cliente
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
            sem_wait(&cad_corte);    // Bloqueia a cadeira do barbeiro  
            printf("Cliente %d sentou na cadeira do barbeiro\n", id);
            
            sem_post(&s_barbeiro);   // Sinaliza para o barbeiro que tem cliente 
            id_cliente = id;
            
            sem_wait(&corte_pronto); // Espera a sinalização do barbeiro de que o corte foi feito
            printf("Cliente %d foi atendido e saiu da barbearia\n", id);
        }
        else
        {
            num_clientes++;
            printf("Cliente %d sentou em uma cadeira de espera\n", id);

            sleep(1);
            sem_wait(&cad_corte);    // Bloqueia a cadeira do barbeiro  
            printf("Cliente %d sentou na cadeira do barbeiro\n", id);
            
            sem_post(&s_barbeiro);   // Sinaliza para o barbeiro que tem cliente
            id_cliente = id;
            
            sem_wait(&corte_pronto); // Espera a sinalização do barbeiro de que o corte foi feito
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

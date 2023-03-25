
#include <pthread.h> //pthread_t, pthread_create, pthread_detach, pthread_exit
#include <stdio.h>   //fflush
#include <stdlib.h>  //exit
#include <unistd.h>  //sleep

int vetor[] = {0, 1, 7, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14};           // Vetor que será dividido entre as threads
int num_pthreads = 3;                                   // Quantidade de threads que desejo criar
int size_vector = sizeof(vetor) / sizeof(vetor[0]);     // Tamanho do vetor
int begin_position = 0;                                 
int end_position = 0;
int search_space;                                       // Tamanho do espaço que cada thread irá buscar
int thread = 1;                                         // Indica qual thread está buscando o vetor
int value = 7;                                          // Valor buscado

void *search_value(void *arg)
{
    
    search_space = size_vector / num_pthreads;
    int i;
    end_position = *((int *)arg);

    for (i = begin_position; i < end_position; i++)
    {
        if (vetor[i] == value)
        {
            printf("\nThread %d encontrou o valor!\n", thread);
        }
        printf("%d", vetor[i]);
        fflush(stdout);
        sleep(1);
    }

    printf("\n");
    begin_position += search_space;
    thread++;
    return (NULL);
}

int main()
{
    pthread_t thread;
    int i;
    search_space = size_vector / num_pthreads;

    // Laço responsável por criar as threads 
    // e calcular a posição final para cada thread
    for (i = 0; i < num_pthreads; i++)
    {
        if (i == num_pthreads - 1)
        {
            end_position = size_vector;
        }
        else
        {
            end_position += search_space;
        }

        if (pthread_create(&thread, NULL, search_value, (void *)&end_position))
        {
            fprintf(stderr, "error creating a new thread \n");
            exit(1);
        }

        // pthread_detach(thread); // desanexa a thread
        pthread_join(thread, NULL); // aguarda a thread terminar
    }

    printf("main thread finished\n");
    pthread_exit(NULL); // finaliza a thread (depois de liberar os recursos)
}

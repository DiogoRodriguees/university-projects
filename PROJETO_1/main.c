/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242
        Marcos Vinicios de Quadros - 2380560

    Descrição:


    Data:
        08 de Junho de 2023
*/
/*
    Ações dos PROFESSORES
        * abrirSala
        * fecharSala
        * avisarAlunos
        * avisarMonitores

    Ações dos ALUNOS
        * entraSala
        * sairSala
        * estudar

    Ações dos MONITORES
        * entrarSala
        * sairSala
        * supervisionarAlunos

    OBS:
        Exibir uma mensagem para cada ação executada.
*/

#include <stdbool.h>   // bool
#include <pthread.h>   // pthread_t
#include <semaphore.h> // sem_t
#include <stdio.h>
#include <unistd.h>

/* Variaveis para teste com diferentes numeros de ALUNOS, MONITORES e PROFESSORES */
#define MAX_ALUNOS_GRUPO 5
#define MAX_TURMA_ALUNOS 5
#define MAX_MONITORES_GRUPO 1

/* Variaveis que serão inicializadas pelo monitor */
int total_alunos = 0;

sem_t s_alunos;    // semaforo para controle do grupo
sem_t s_monitores; // semaforo para controle do grupo
int monitores_disponiveis = 0;

void *executarAlunos(void *id)
{
    int a_id = *((int *)id);
    sem_wait(&s_monitores);
    printf("ALUNO %i ENTROU NA SALA\n", a_id);
}

void *executarMonitores(void *id)
{
    int m_id = *((int *)id);
    sem_post(&s_monitores);
    printf("MONITOR %i ENTROU NA SALA\n", m_id);
}

int main(int argc, char **argv)
{
    /* Criar threads para: Alunos, Monitores, Professores */
    pthread_t alunos[MAX_TURMA_ALUNOS], monitores[MAX_MONITORES_GRUPO], professor;

    /* Criação do semaforo para os ALUNOS */
    sem_init(&s_alunos, 0, MAX_TURMA_ALUNOS);

    /* Semaforos dos MONITORES*/
    sem_init(&s_monitores, 0, MAX_MONITORES_GRUPO);

    /* Criando alunos */
    for (int i = 0; i < MAX_TURMA_ALUNOS; i++)
    {
        pthread_create(&alunos[i], NULL, executarAlunos, &i);
        sleep(1);
    }

    /* Criando monitores */
    for (int i = 0; i < MAX_MONITORES_GRUPO; i++)
    {
        pthread_create(&monitores[i], NULL, executarMonitores, &i);
        sleep(1);
    }

    /* Aguardando alunos */
    for (int i = 0; i < MAX_TURMA_ALUNOS; i++)
        pthread_join(alunos[i], NULL);

    /* Aguardando monitores */
    for (int i = 0; i < MAX_MONITORES_GRUPO; i++)
        pthread_join(monitores[i], NULL);

    /* Semaforos dos MONITORES*/
    sem_destroy(&s_monitores);
    
    /* Criação do semaforo para os ALUNOS */
    sem_destroy(&s_alunos);

    return 0;
}
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
#define MAX_TURMA_ALUNOS 10
#define MAX_MONITORES_GRUPO 2

/* Variaveis que serão inicializadas pelo monitor */

sem_t s_alunos;    // semaforo para controle do grupo
sem_t s_monitores; // semaforo para controle do grupo
sem_t s_sala;      // semaforo para controle do grupo

int total_alunos = 0;
int monitores_disponiveis = 0;

void *executarProfessor(void*id){
    
    /* Abrir sala */
    sem_init(&s_sala, 0, MAX_TURMA_ALUNOS);
    
    /* Avisar os alunos */
    
    /* Avisar alunos Monitores */
    
    /* Fechar a sala */
}

void *executarAlunos(void *id)
{
    int a_id = *((int *)id);

    sem_wait(&s_sala);
    printf("ALUNO %i ENTROU NA SALA\n", a_id);

    sem_wait(&s_alunos);
    printf("ALUNO %i PEGOU TOKEN\n", a_id);
    sleep(15);

    /* Aluno devolve um token */
    sem_post(&s_alunos);
}

void *executarMonitores(void *id)
{
    int m_id = *((int *)id);

    /* Monitor entrou na sala */
    sem_wait(&s_sala);
    monitores_disponiveis++;
    printf("MONITOR %i ENTROU NA SALA\n", m_id);
    
    /* Monitore   */
    sem_wait(&s_monitores);

    /* Liberando tokens para o semaforo do ALUNOS */
    for (int i = 0; i < MAX_ALUNOS_GRUPO; i++)
    {
        sem_post(&s_alunos);
    }

    sem_post(&s_monitores);
    monitores_disponiveis--;
    
    /* Monitore devolve um token - FUNCAÇÂO SAIR_DA_SALA*/
    while ((total_alunos / monitores_disponiveis) > MAX_ALUNOS_GRUPO)
    {
        sleep(1);
    }
}

int main(int argc, char **argv)
{
    /* Criar threads para: Alunos, Monitores, Professores */
    pthread_t alunos[MAX_TURMA_ALUNOS], monitores[MAX_MONITORES_GRUPO], professor;
    

    /* Criação do semaforo para os ALUNOS */
    sem_init(&s_alunos, 0, 0);

    /* Semaforos dos MONITORES*/
    sem_init(&s_monitores, 0, 0);

    pthread_create(&professor, NULL, executarProfessor, NULL);
    
    /* Criando alunos */
    for (int i = 0; i < MAX_TURMA_ALUNOS; i++)
    {
        pthread_create(&alunos[i], NULL, executarAlunos, &i);
    }

    /* Criando monitores */
    for (int i = 0; i < MAX_MONITORES_GRUPO; i++)
    {
        pthread_create(&monitores[i], NULL, executarMonitores, &i);
        sleep(10);
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
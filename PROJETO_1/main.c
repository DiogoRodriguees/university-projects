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
#define MAX_ALUNOS_GRUPO 3
#define MAX_TURMA_ALUNOS 6
#define MAX_MONITORES_GRUPO 2

/* Semaforos */
sem_t s_alunos;    // Controle dos alunos estudando
sem_t s_monitores; // Controle do limite de monitores
sem_t s_sala;      // Controle dos estudantes na sala (Alunos e Monitores)

/* Controle para saida de monitores */
int total_alunos = 0;
int monitores_disponiveis = 0;

/************************************************************************ 
 *                                ALUNOS                                *
 ************************************************************************/
void abrirSala()
{
    printf("PROFESSOR ABRIU A SALA\n");
    sem_init(&s_sala, 0, MAX_TURMA_ALUNOS);
}

void avisarEstudantesMonitores()
{
    printf("PROFESSOR AVISOU OS MONITORES\n");
    for (int i = 0; i < MAX_MONITORES_GRUPO; i++)
        sem_post(&s_monitores);
    
}

void fecharSala()
{
    /* A sala fica aberta por 60 seg */
    sleep(45);
    // sem_destroy(&s_sala);
    printf("O PROFESSOR FECHOU A SALA\n");
}

void *executarProfessor(void *)
{
    abrirSala();

    /* Avisar os alunos */

    avisarEstudantesMonitores();

    fecharSala();
}

/************************************************************************ /
 *                                ALUNOS                                *
 ************************************************************************/
void a_sairSala(int id)
{
    /* Aluno devolve um token para sair da sala */
    printf("ALUNO %i SAIU DA SALA\n", id);
    sem_post(&s_alunos);
    sem_post(&s_sala);
    total_alunos--;
}

void a_entrarSala(int id)
{
    sem_wait(&s_sala);
    printf("ALUNO %i ENTROU NA SALA\n", id);
}

void a_estudar(int id)
{
    sem_wait(&s_alunos);
    printf("ALUNO %i COMECOU A ESTUDAR\n", id);
    total_alunos++;
}

void *executarAlunos(void *id)
{
    int a_id = *((int *)id);

    a_entrarSala(a_id);

    a_estudar(a_id);
    /* Alunos ficam 10seg na sala */
    sleep(10);

    a_sairSala(a_id);
}

/************************************************************************ /
 *                               MONITORES                              *
 ************************************************************************/
void m_entrarSala(int id)
{
    sem_wait(&s_monitores);
    monitores_disponiveis++;
    sem_wait(&s_sala);
    printf("MONITOR %i ENTROU NA SALA\n", id);
}

void m_sairSala(int id)
{
    /* Monitor libera token para que outro monitor possa entra */
    sem_post(&s_monitores);
    monitores_disponiveis--;

    /* Monitor aguarda outro monitor entra na sala ou alguns alunos sairem */
    // while (((float)total_alunos / monitores_disponiveis) > MAX_ALUNOS_GRUPO)
    //     sleep(1);

    /* Monitor libera um TOKEN para outra thread entrar na sala */
    sem_post(&s_sala);
    printf("O MONITOR %i SAIU DA SALA\n", id);
}

void m_supervisionarAlunos()
{
    /* Liberando tokens para o semaforo dos ALUNOS */
    for (int i = 0; i < MAX_ALUNOS_GRUPO; i++)
    {
        sem_post(&s_alunos);
        sleep(1);
    }
}

void *executarMonitores(void *id)
{
    int m_id = *((int *)id);

    m_entrarSala(m_id);
    m_supervisionarAlunos(); // Libera MAX_ALUNOS_GRUPO para estudarem

    sleep(10); // Monitor fica 10seg na sala

    m_sairSala(m_id);
}

int main(int argc, char **argv)
{
    /* Criar threads para: Alunos, Monitores, Professores */
    pthread_t alunos[MAX_TURMA_ALUNOS], monitores[MAX_MONITORES_GRUPO], professor;

    sem_init(&s_alunos, 0, 0);    // Semaforo de ALUNOS iniciando bloqueado
    sem_init(&s_monitores, 0, 0); // Semaforo de MONITORES iniciando bloqueado

    /* Inicializando thread do PROFESSOR */
    pthread_create(&professor, NULL, executarProfessor, NULL);

    /* Inicializando threads ALUNOS */
    for (int i = 0; i < MAX_TURMA_ALUNOS; i++)
    {
        pthread_create(&alunos[i], NULL, executarAlunos, &i);
        sleep(1); // proximo aluno chega em 10seg
    }

    /* Inicializando threads de MONITORES */
    for (int i = 0; i < MAX_MONITORES_GRUPO; i++)
    {
        pthread_create(&monitores[i], NULL, executarMonitores, &i);
        sleep(10); // Proximo monitor chega em 10seg
    }

    for (int i = 0; i < MAX_TURMA_ALUNOS; i++)
        pthread_join(alunos[i], NULL);

    for (int i = 0; i < MAX_MONITORES_GRUPO; i++)
        pthread_join(monitores[i], NULL);
        
    pthread_join(professor, NULL);
    
    sem_destroy(&s_alunos);
    sem_destroy(&s_monitores);
    sem_destroy(&s_sala);

    return 0;
}
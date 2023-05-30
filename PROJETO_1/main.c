/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242
        Marcos Vinicios de Quadros - 2380560

    Descrição:
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

        Para visualização:
            Exibir uma mensagem para cada ação executada.

    Data:
        08 de Junho de 2023
*/

#include <stdbool.h>   // bool
#include <pthread.h>   // pthread_t
#include <semaphore.h> // sem_t
#include <stdio.h>     // printf()
#include <unistd.h>    // sleep()

/* Variaveis para teste com diferentes numeros de ALUNOS, MONITORES e PROFESSORES */
#define ALUNOS_POR_GRUPO 2
#define LIMITE_MONITORES 3
#define LIMITE_ALUNOS_SALA 6

/* Semaforos */
sem_t s_alunos;    // Controle dos alunos estudando
sem_t s_monitores; // Controle do limite de monitores
sem_t s_sala;      // Controle dos estudantes na sala (Alunos e Monitores)

/* Variaveis para controle de permição da entrada de alunos e monitores */
bool entrada_alunos = true;
bool entrada_monitores = true;

/* Variavel de controle para saída do monitores*/
int total_alunos = 0;
int monitores_disponiveis = 0;

/* Variavel de controle para finalizar o programa */
bool sala_cheia = true;

/************************************************************************
 *                               PROFESSOR                              *
 ************************************************************************/
void abrirSala()
{
    /* Libera o semaforo SALA p/ MONITORES e ALUNOS entrarem */
    printf("PROFESSOR ABRIU A SALA\n");
    sem_init(&s_sala, 0, LIMITE_ALUNOS_SALA);
}

void avisarEstudantesMonitores()
{
    entrada_monitores = false;
    printf("MONITORES NAO PODEM MAIS ENTRAR\n");
}

void avisarAlunos()
{
    entrada_alunos = false;
    printf("ALUNOS NAO PODEM MAIS ENTRAR\n");
}

void fecharSala()
{
    /* Aguarda a sala envaziar */
    while (sala_cheia)
        sleep(1);

    sem_destroy(&s_sala);
    printf("O PROFESSOR FECHOU A SALA\n");
}

void *executarProfessor(void *)
{
    /* Libera os ALUNOS e MONITORES p/ entrar */
    abrirSala();

    /* Sala fica aberta por 60seg */
    sleep(10);

    /* Monitores não poderão mais entrar na sala*/
    avisarEstudantesMonitores();

    /* Alunos não poderão mais entrar na sala*/
    avisarAlunos();

    fecharSala();
}

/************************************************************************ /
 *                                ALUNOS                                *
 ************************************************************************/
void a_sairSala(int id)
{
    /* Aluno devolve tokens token para sair da sala */
    printf("ALUNO %i SAIU DA SALA\n", id);
    sem_post(&s_alunos);
    sem_post(&s_sala);
    total_alunos--;
}

void a_entrarSala(int id)
{
    sleep(1); // tempo para verificação no prompt

    sem_wait(&s_alunos);
    total_alunos++;

    printf("ALUNO %i ENTROU NA SALA\n", id);
}

void a_estudar(int id)
{
    sem_wait(&s_sala);
    printf("ALUNO %i COMECOU A ESTUDAR\n", id);
}

void *executarAlunos(void *id)
{
    int a_id = *((int *)id);

    if (!entrada_alunos)
        return;

    printf("ALUNO %i ESPERANDO MONITOR\n", a_id);
    a_entrarSala(a_id);

    a_estudar(a_id);

    /* Tempo que os alunos ficam estudando */
    sleep(10);

    a_sairSala(a_id);
}

/************************************************************************ /
 *                               MONITORES                              *
 ************************************************************************/
void m_entrarSala(int id)
{
    if (!entrada_monitores)
        return;

    sem_wait(&s_monitores);
    monitores_disponiveis++;
    sem_wait(&s_sala);
    printf("MONITOR %i ENTROU NA SALA\n", id);
}

void m_sairSala(int id)
{
    /* Monitor libera token p/ que outro monitor possa entrar */
    sem_post(&s_monitores);
    monitores_disponiveis--;

    /* Monitor aguarda outro monitor entra na sala ou alguns alunos sairem */
    while (((float)total_alunos / monitores_disponiveis) > ALUNOS_POR_GRUPO)
    {
    }

    /* Monitor libera um TOKEN para outra thread entrar na sala */
    sem_post(&s_sala);
    printf("O MONITOR %i SAIU DA SALA\n", id);

    if (monitores_disponiveis == 0)
        sala_cheia = false;
}

void m_supervisionarAlunos()
{
    int liberar_tokens = ALUNOS_POR_GRUPO - (total_alunos % ALUNOS_POR_GRUPO);

    /* Liberando tokens para o semaforo dos ALUNOS */
    for (int i = 0; i < liberar_tokens; i++)
    {
        sem_post(&s_alunos);
        sleep(1);
    }
}

void *executarMonitores(void *id)
{
    int m_id = *((int *)id);

    m_entrarSala(m_id);
    m_supervisionarAlunos(); // Libera ALUNOS_POR_GRUPO para estudarem

    sleep(10); // Monitor fica 10seg na sala

    m_sairSala(m_id);
}

int main(int argc, char **argv)
{
    /* Criar threads para: Alunos, Monitores, Professores */
    pthread_t alunos[LIMITE_ALUNOS_SALA], monitores[LIMITE_MONITORES], professor;

    sem_init(&s_alunos, 0, 0);                   // Semaforo de ALUNOS iniciando bloqueado
    sem_init(&s_monitores, 0, LIMITE_MONITORES); // Semaforo de MONITORES iniciando bloqueado

    /* Inicializando thread do PROFESSOR */
    pthread_create(&professor, NULL, executarProfessor, NULL);

    /* Inicializando threads ALUNOS */
    for (int i = 0; i < LIMITE_ALUNOS_SALA; i++)
    {
        pthread_create(&alunos[i], NULL, executarAlunos, &i);
        sleep(1); // proximo aluno chega em 1seg
    }

    /* Inicializando threads de MONITORES */
    for (int i = 0; i < LIMITE_MONITORES; i++)
    {
        pthread_create(&monitores[i], NULL, executarMonitores, &i);
        sleep(8); // Proximo monitor chega em 8seg
    }

    pthread_join(professor, NULL);

    for (int i = 0; i < LIMITE_ALUNOS_SALA; i++)
        pthread_join(alunos[i], NULL);

    for (int i = 0; i < LIMITE_MONITORES; i++)
        pthread_join(monitores[i], NULL);

    sem_destroy(&s_alunos);
    sem_destroy(&s_monitores);

    return 0;
}
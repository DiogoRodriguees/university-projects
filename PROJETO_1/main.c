/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242
        Marcos Vinicios de Quadros - 2380560

    Descrição:
        como foi feito a implementação
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
sem_t s_alunos;          // Controle dos alunos estudando
sem_t s_saida_monitores; // Controle a saida de monitores
sem_t s_sala;            // Controle dos estudantes na sala (Alunos e Monitores)
sem_t mutex;
sem_t teste;

sem_t s_fechar_sala;

/* Variaveis para controle de permição da entrada de alunos e monitores */
bool entrada_alunos = true;
bool entrada_monitores = true;
bool monitor_deseja_sair = false;
/* Variavel de controle para saída do monitores*/
int total_alunos = 0;
int monitores_disponiveis = 0;

/************************************************************************
 *                               PROFESSOR                              *
 ************************************************************************/
void abrirSala()
{
    /* Libera o semaforo SALA p/ MONITORES e ALUNOS entrarem */
    sem_init(&s_sala, 0, LIMITE_ALUNOS_SALA);
}

void avisarEstudantesMonitores()
{
    // bloqueio de leitura
    sem_wait(&mutex);
    entrada_monitores = false;
    sem_post(&mutex);
}

void avisarAlunos()
{
    sem_wait(&mutex);
    entrada_alunos = false;
    sem_post(&mutex);
}

void fecharSala()
{
    sem_wait(&s_fechar_sala);
    sem_destroy(&s_sala);
}

void *executarProfessor(void *param)
{
    /* Libera os ALUNOS e MONITORES p/ entrar */
    abrirSala();
    printf("PROFESSOR ABRIU A SALA\n");

    /* Sala fica aberta por 60seg */
    sleep(16);

    /* Monitores não poderão mais entrar na sala*/
    avisarEstudantesMonitores();
    printf("MONITORES NAO PODEM MAIS ENTRAR\n");

    /* Alunos não poderão mais entrar na sala*/
    avisarAlunos();
    printf("ALUNOS NAO PODEM MAIS ENTRAR\n");

    fecharSala();
    printf("O PROFESSOR FECHOU A SALA\n");
}

/************************************************************************
 *                                ALUNOS                                *
 ************************************************************************/
void *executarAlunos(void *id)
{
    int a_id = *((int *)id);

    printf("ALUNO %i ESPERANDO MONITOR\n", a_id);

    sem_wait(&s_alunos);
    sem_wait(&s_sala);

    sem_wait(&mutex);
    if (entrada_alunos)
    {
        total_alunos++;
        sem_post(&mutex);
        printf("ALUNO %i ENTROU NA SALA E COMECOU A ESTUDAR\n", a_id);

        // tempo que o aluno permance na sala
        sleep(5);

        // saindo da sala
        sem_post(&s_alunos);
        sem_post(&s_sala);
        printf("ALUNO %i SAIU DA SALA\n", a_id);
        
        sem_wait(&mutex);
        total_alunos--;
        sem_post(&mutex);

        sem_wait(&mutex);
        if (total_alunos <= ((monitores_disponiveis - 1) * ALUNOS_POR_GRUPO))
            sem_post(&s_saida_monitores);
        sem_post(&mutex);
    }
    else
    {
        printf("O ALUNO %i NAO PODE MAIS ENTRAR NA SALA\n", a_id);
        sem_post(&mutex);
    }
}

/************************************************************************
 *                               MONITORES                              *
 ************************************************************************/
<<<<<<< HEAD
bool m_entrarSala(int id)
{

    sem_wait(&s_monitores);

    sem_wait(&s_sala);
    monitores_disponiveis++;

    if (!entrada_monitores)
        return true;
    printf("MONITOR %i ENTROU NA SALA\n", id);

    return false;
}

void m_sairSala(int id)
{
    int m_id = *((int *)id);
    /* Monitor libera token p/ que outro monitor possa entrar */
    sem_post(&s_monitores);

    monitores_disponiveis--;
    
    /* Monitor aguarda outro monitor entra na sala ou alguns alunos sairem */
    // while (((float)total_alunos / monitores_disponiveis) > ALUNOS_POR_GRUPO)
    // {
    // }

    /* Monitor libera um TOKEN para outra thread entrar na sala */
    sem_post(&s_sala);
    printf("O MONITOR %i SAIU DA SALA\n", m_id);

    if (monitores_disponiveis <= 0)
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

=======
>>>>>>> version_2
void *executarMonitores(void *id)
{
    int m_id = *((int *)id);

<<<<<<< HEAD
    if (m_entrarSala(m_id))
    {
        m_sairSala(m_id);
        return;
    }
    m_supervisionarAlunos(); // Libera ALUNOS_POR_GRUPO para estudarem
=======
    // verificar se pode entrar na sala
    sem_wait(&mutex);
    if (entrada_monitores)
    {
        monitores_disponiveis++;
>>>>>>> version_2

        // se existe monitor na sala, libera token de saida
        if (monitor_deseja_sair)
        {
            sem_post(&s_saida_monitores);
            monitor_deseja_sair = false;
        }
        else
        {
            for (int i = 0; i < ALUNOS_POR_GRUPO; i++)
                sem_post(&s_alunos);
        }
        printf("MONITOR %i ENTROU NA SALA\n", m_id);

        // liberando seção critica
        sem_post(&mutex);

        // tempo que o monitor permance na sala
        sleep(4);
        sem_wait(&mutex);
        monitor_deseja_sair = true;
        sem_post(&mutex);

        // saida libera pela entrada de um monitor ou saida de X alunos
        sem_wait(&s_saida_monitores);
        sem_wait(&mutex);
        monitores_disponiveis--;
        printf("MONITOR %i SAIU DA SALA\n", m_id);
        if (monitores_disponiveis == 0)
            sem_post(&s_fechar_sala);
        sem_post(&mutex);
    }
    else
    {
        printf("MONITOR %i NAO PODE MAIS ENTRAR NA SALA\n", m_id);
        sem_post(&mutex);
    }
}

int main(int argc, char **argv)
{
    /* Criar threads para: Alunos, Monitores, Professores */
    pthread_t alunos[LIMITE_ALUNOS_SALA], monitores[LIMITE_MONITORES], professor;

    sem_init(&s_alunos, 0, 0); // Semaforo de ALUNOS iniciando bloqueado
    sem_init(&teste, 0, 0);    // Semaforo de ALUNOS iniciando bloqueado
    sem_init(&mutex, 0, 1);
    sem_init(&s_fechar_sala, 0, 0);
    sem_init(&s_saida_monitores, 0, 0); // Semaforo de MONITORES iniciando bloqueado

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
        sleep(4); // Proximo monitor chega em 8seg
    }

    pthread_join(professor, NULL);

    for (int i = 0; i < LIMITE_ALUNOS_SALA; i++)
        pthread_join(alunos[i], NULL);

    for (int i = 0; i < LIMITE_MONITORES; i++)
        pthread_join(monitores[i], NULL);

    sem_destroy(&s_alunos);
    sem_destroy(&s_saida_monitores);

    return 0;
}
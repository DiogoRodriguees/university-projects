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
bool alunos_em_sala = true;

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
    entrada_monitores = false;
}

void avisarAlunos()
{
    entrada_alunos = false;
}

void fecharSala()
{
    /* Aguarda a sala envaziar */
    while (alunos_em_sala)
        sleep(1);

    sem_destroy(&s_sala);
}

void *executarProfessor(void *param)
{
    /* Libera os ALUNOS e MONITORES p/ entrar */
    abrirSala();
    printf("PROFESSOR ABRIU A SALA\n");

    /* Sala fica aberta por 60seg */
    sleep(12);

    /* Monitores não poderão mais entrar na sala*/
    avisarEstudantesMonitores();
    printf("MONITORES NAO PODEM MAIS ENTRAR\n");

    /* Alunos não poderão mais entrar na sala*/
    avisarAlunos();
    printf("ALUNOS NAO PODEM MAIS ENTRAR\n");

    fecharSala();
    printf("O PROFESSOR FECHOU A SALA\n");
}

/************************************************************************ /
 *                                ALUNOS                                *
 ************************************************************************/
void *executarAlunos(void *id)
{
    int a_id = *((int *)id);

    /* Entrar na sala */
    sem_wait(&s_alunos);
    sem_wait(&s_sala);
    
    /* Para o caso da entra de alunos ser bloqueadas */
    if (!entrada_alunos)
    {
        sem_post(&s_sala);
        sem_post(&s_alunos);
        printf("ALUNO %i NAO PODE MAIS ENTRAR NA SALA\n", a_id);
        return;
    }
    
    printf("ALUNO %i ENTROU NA SALA E COMECOU A ESTUDAR\n", a_id);

    /* Permanecer um tempo na sala */
    sleep(5);
    
    /* Sair da sala */
    sem_post(&s_sala);
    sem_post(&s_alunos);
    printf("ALUNO %i SAIU DA SALA\n", a_id);
}

/************************************************************************ /
 *                               MONITORES                              *
 ************************************************************************/
void *executarMonitores(void *id)
{
    int m_id = *((int *)id);

    /* Entrar na sala */
    sem_wait(&s_sala);
    monitores_disponiveis++;
    if (!entrada_alunos)
    {
        monitores_disponiveis--;
        sem_post(&s_sala);
        if (monitores_disponiveis == 0)
        {
            alunos_em_sala = false;
        }
        printf("MONITOR %i NAO PODE MAIS ENTRAR NA SALA\n", m_id);
        return;
    }
    printf("MONITOR %i ENTROU NA SALA\n", m_id);

    /* Supervisionar alunos */
    int liberar_tokens = ALUNOS_POR_GRUPO - (total_alunos % monitores_disponiveis);
    for (int i = 0; i < liberar_tokens; i++)
    {
        sem_post(&s_alunos);
    }

    /* Permanecer um tempo na sala */
    sleep(10);

    /* Sair da sala */
    monitores_disponiveis--;
    sem_post(&s_sala);
    
    /* Para garantir 1 monitor para X alunos */
    while (((float)total_alunos / monitores_disponiveis) > ALUNOS_POR_GRUPO)
    {
        sleep(1);
    }
    
    /* Ultimo monitor avisa que a sala esta vazia */
    if (monitores_disponiveis == 0)
    {
        alunos_em_sala = false;
    }
    printf("MONITOR %i SAIU DA SALA\n", m_id);
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
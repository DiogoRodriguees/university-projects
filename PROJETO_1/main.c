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

sem_t s_fechar_sala;

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
    // bloqueio de leitura
    sem_wait(&mutex);
    entrada_monitores = false;
    sem_post(&mutex);
}

void avisarAlunos()
{
    // bloqueio de leitura
    sem_wait(&mutex);
    entrada_alunos = false;
    sem_post(&mutex);
}

void fecharSala()
{
    /* Aguarda a sala envaziar */
    // debloqueio
    // evitar espera ocupada
    sem_wait(&s_fechar_sala);

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

/************************************************************************
 *                                ALUNOS                                *
 ************************************************************************/
void *executarAlunos(void *id)
{
    int a_id = *((int *)id);

    printf("ALUNO %i ESPERANDO MONITOR", a_id);
    sem_wait(&s_alunos); // Semaforo liberado pelo monitor
    sem_wait(&s_sala);   // Semaforo p/ controle de alunos na sala

    sem_wait(&mutex); // mutex de bloqueio p/ variavel total_alunos
    total_alunos++;

    /* Para o caso da entra de alunos ser bloqueadas */
    if (!entrada_alunos)
    {
        sem_post(&s_sala);
        sem_post(&s_alunos);
        printf("ALUNO %i NAO PODE MAIS ENTRAR NA SALA\n", a_id);
        // desbloqueio
        sem_post(&mutex);

        return;
    }
    // desbloqueio
    sem_post(&mutex);

    printf("ALUNO %i ENTROU NA SALA E COMECOU A ESTUDAR\n", a_id);

    /* Permanecer um tempo na sala */
    sleep(5);

    /* Sair da sala */
    sem_post(&s_sala);
    sem_post(&s_alunos);
    printf("ALUNO %i SAIU DA SALA\n", a_id);

    sem_wait(&mutex);
    total_alunos--;
    sem_post(&mutex);

    int alunos_por_grupo = ((float)total_alunos / monitores_disponiveis);
    if (total_alunos == 0)
    {
        sem_post(&s_saida_monitores);
    }
}

/************************************************************************ /
 *                               MONITORES                              *
 ************************************************************************/
void *executarMonitores(void *id)
{
    int m_id = *((int *)id);

    // monitor não contar como aluno na sala. Revisar o uso desse semaforo pelo monitores
    sem_wait(&s_sala); // monitor pega token p/ entrar na sala

    if (!entrada_alunos)
    {
        sem_post(&s_sala);
        if (monitores_disponiveis == 0)
        {
            alunos_em_sala = false;
        }
        printf("MONITOR %i NAO PODE MAIS ENTRAR NA SALA\n", m_id);
        return;
    }

    sem_wait(&mutex); // bloqueio p/ leitura da variavel monitores_disponiveis
    int alunos_por_grupo = ((float)total_alunos / monitores_disponiveis);
    if (alunos_por_grupo < ALUNOS_POR_GRUPO)
    {
        sem_post(&s_saida_monitores);
    }

    monitores_disponiveis++;
    sem_post(&mutex);

    printf("MONITOR %i ENTROU NA SALA\n", m_id);

    // esplicar melhor a ideia do calculo para liberação de tokens
    /* Libera X alunos para entrarem na sala */
    int alunos_por_monitor = total_alunos % monitores_disponiveis; // Alunos por monitor
    int liberar_tokens = ALUNOS_POR_GRUPO - alunos_por_monitor;    //
    for (int i = 0; i < liberar_tokens; i++)
        sem_post(&s_alunos);

    /* Permanecer um tempo na sala */
    sleep(10);

    sem_wait(&s_saida_monitores); // libera o monitor para sair da sala
    monitores_disponiveis--;
    sem_post(&s_sala);

    printf("MONITOR %i SAIU DA SALA\n", m_id);

    if (monitores_disponiveis == 0)
        sem_post(&s_fechar_sala); // libera o professor para fechar a sala
}

int main(int argc, char **argv)
{
    /* Criar threads para: Alunos, Monitores, Professores */
    pthread_t alunos[LIMITE_ALUNOS_SALA], monitores[LIMITE_MONITORES], professor;

    sem_init(&s_alunos, 0, 0); // Semaforo de ALUNOS iniciando bloqueado
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
        sleep(8); // Proximo monitor chega em 8seg
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
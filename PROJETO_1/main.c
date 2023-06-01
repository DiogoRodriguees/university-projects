/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242
        Marcos Vinicios de Quadros - 2380560

    Descrição:
        PROFESSOR:
            O professor inicia com a abertura da sala, liberando os alunos
            e monitores para entrarem. Após um determindado tempo(T_SALA_ABERTA)
            os professor avisa os alunos e monitores que não é possivel entrar na sala.
            Esse aviso ocorre atribuindo o valor TRUE para as variaveis: entrada_alunos e entrada_monitores.
            Em seguida ele aguarda e liberação do semaforo(s_fechar_sala) que é liberado pelo ultimo
            monitor que sair da sala, garantindo que não existe mais nenhum aluno na sala.

        MONITORES:
            Ao executar, o monitor verifica se pode entrar na sala. Caso seja possivel, ele verifica se
            existe algum monitor que deseja sair. Caso exista, ele libera esse monitor. Caso não exista
            ele libera(ALUNOS_POR_GRUPO) tokens para novos alunos entrarem. Após determinado tempo(T_MONITOR_SALA)
            o monitor sinaliza que deseja sair. Essa sinalização ocorre com a atribuição "TRUE" para a variavel(monitor_deseja_sair).
            Se o monitor for o ultimo a sair ele libera um token para o semaforo "s_fechar_sala", assim o professor termina sua execução
            fechando a sala.

        ALUNOS:
            Ao entrar os alunos aguardam até que os monitores liberem tokens para entrada ou algum aluno saia. Essa liberação
            ocorre pelo semaforo "s_alunos". Ao entra o aluno verifica se a entrada de alunos na sala é possivel. Caso seja possivel
            o aluno entra e permance um tempo(T_ALUNO_SALA) antes de sair da sala. Ao sair o aluno verifica se é possivel liberar
            um monitor para que ele saia. Para que o monitor seja libera o aluno deve saber se a quantidade de alunos na sala é menor
            que a quantidade de ((monitores - 1) * alunos por grupo).

        FUNÇÕES EXTRAS:
            Para simular um tempo entre as chegadas dos monitores, foi criado um variavel(T_CRIACAO_MONITOR) que é
            utilizada na função sleep antes que o proximo monitor seja criado.

    Data:
        08 de Junho de 2023
*/

#include <stdbool.h>   // bool
#include <pthread.h>   // pthread_t
#include <semaphore.h> // sem_t
#include <stdio.h>     // printf()
#include <unistd.h>    // sleep()

/* Variaveis para teste com diferentes numeros de ALUNOS, MONITORES e PROFESSORES */
#define LIMITE_ALUNOS_SALA 17
#define ALUNOS_POR_GRUPO 4
#define MONITORES 5

/* Tempo que as threads executam sleep */
#define T_ALUNO_SALA 5      // tempo que o aluno permance na sala
#define T_MONITOR_SALA 4    // tempo que o monitor permance na sala
#define T_SALA_ABERTA 30    // tempo que a sala permanece aberta
#define T_CRIACAO_MONITOR 6 // tempo para simular uma entrada tardia do monitor

/* Semaforos */
sem_t s_alunos;          // Controle dos alunos estudando
sem_t s_saida_monitores; // Controle da saida de monitores
sem_t s_sala;            // Controle dos estudantes na sala (Alunos e Monitores)
sem_t mutex;             // Controle das variaveis compartilhadas
sem_t s_fechar_sala;     // Libera o professor pra fechar a sala;

/* Variaveis para controle de permição da entrada de alunos e monitores */
bool entrada_alunos = true;       // o valor false é atribuido quando a entrada de alunos não é permitida
bool entrada_monitores = true;    // o valor false é atribuido quando a entrada de monitores não é permitida
bool monitor_deseja_sair = false; // valor true é atribuido quando um monitor deseja sair da sala

/* Controle de alunos e monitores na sala */
int total_alunos = 0;
int monitores_disponiveis = 0;

/************************************************************************
 *                               PROFESSOR                              *
 ************************************************************************/
void *executarProfessor(void *param)
{
    /* Libera os ALUNOS e MONITORES p/ entrar */
    sem_init(&s_sala, 0, LIMITE_ALUNOS_SALA);
    printf("PROFESSOR ABRIU A SALA\n");

    /* Sala fica aberta por 60seg */
    sleep(T_SALA_ABERTA);

    /* Monitores não poderão mais entrar na sala*/
    sem_wait(&mutex);
    entrada_monitores = false;
    printf("MONITORES NAO PODEM MAIS ENTRAR\n");

    /* Alunos não poderão mais entrar na sala*/
    entrada_alunos = false;
    printf("ALUNOS NAO PODEM MAIS ENTRAR\n");
    sem_post(&mutex);

    sem_wait(&s_fechar_sala);
    sem_destroy(&s_sala);
    printf("O PROFESSOR FECHOU A SALA\n");
}

/************************************************************************
 *                                ALUNOS                                *
 ************************************************************************/
void *executarAlunos(void *id)
{
    int a_id = *((int *)id);

    printf("ALUNO %i ESPERANDO MONITOR\n", a_id);

    sem_wait(&s_alunos); // semaforo liberado pela entrada de um monitor
    sem_wait(&s_sala);

    // entra na seção critica
    sem_wait(&mutex);
    if (entrada_alunos)
    {
        total_alunos++;

        // liberando seção critica
        sem_post(&mutex);
        printf("ALUNO %i ENTROU NA SALA E COMECOU A ESTUDAR\n", a_id);

        // tempo que o aluno permance na sala
        sleep(T_ALUNO_SALA);

        sem_post(&s_alunos); // libera token p/ outro aluno
        sem_post(&s_sala);
        printf("ALUNO %i SAIU DA SALA\n", a_id);

        // entra na seção critica
        sem_wait(&mutex);
        total_alunos--;
        sem_post(&mutex);

        // entra na seção critica
        sem_wait(&mutex);
        // caso a saida do aluno permita a saida de um monitor
        if (total_alunos <= ((monitores_disponiveis - 1) * ALUNOS_POR_GRUPO))
            sem_post(&s_saida_monitores);
        sem_post(&mutex);
    }
    else
    {
        printf("ALUNO %i NAO PODE MAIS ENTRAR NA SALA\n", a_id);
        sem_post(&s_alunos);
        sem_post(&mutex);
    }
}

/************************************************************************
 *                               MONITORES                              *
 ************************************************************************/
void *executarMonitores(void *id)
{
    int m_id = *((int *)id);

    // entra na seção critica
    sem_wait(&mutex);

    // verificar se pode entrar na sala
    if (entrada_monitores)
    {
        monitores_disponiveis++;

        // libera token se nenhum monitor deseja sair
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

        // libera seção critica
        sem_post(&mutex);

        // tempo que o monitor permance na sala
        sleep(T_MONITOR_SALA);

        // entra na seção critica
        sem_wait(&mutex);
        monitor_deseja_sair = true;
        sem_post(&mutex);

        // semaforo liberado pela saida de X alunos ou entrada de um monitor
        sem_wait(&s_saida_monitores);

        // entra na seção critica
        sem_wait(&mutex);
        monitores_disponiveis--;
        printf("MONITOR %i SAIU DA SALA\n", m_id);

        // o ultimo monitor libera o professor p/ fechar a sala
        if (monitores_disponiveis == 0)
            sem_post(&s_fechar_sala);

        // libera seção critica
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
    pthread_t alunos[LIMITE_ALUNOS_SALA], monitores[MONITORES], professor;

    sem_init(&mutex, 0, 1);             // controla leitura e escrita da variaveis compartilhadas
    sem_init(&s_alunos, 0, 0);          // controla a quantidade de alunos que podem entrar na sala
    sem_init(&s_fechar_sala, 0, 0);     // libera o professor p/ fechar a sala
    sem_init(&s_saida_monitores, 0, 0); // controla se o monitor pode sair

    /* Inicializando thread do PROFESSOR */
    pthread_create(&professor, NULL, executarProfessor, NULL);

    /* Inicializando threads ALUNOS */
    for (int i = 0; i < LIMITE_ALUNOS_SALA; i++)
    {
        pthread_create(&alunos[i], NULL, executarAlunos, &i);
        sleep(1); // proximo aluno chega em 1seg
    }

    /* Inicializando threads de MONITORES */
    for (int i = 0; i < MONITORES; i++)
    {
        int n_thread = i + 1;
        pthread_create(&monitores[i], NULL, executarMonitores, &n_thread);
        sleep(T_CRIACAO_MONITOR); // Proximo monitor chega em T_CRIACAO_MONITOR
    }

    pthread_join(professor, NULL);

    for (int i = 0; i < LIMITE_ALUNOS_SALA; i++)
        pthread_join(alunos[i], NULL);

    for (int i = 0; i < MONITORES; i++)
        pthread_join(monitores[i], NULL);

    sem_destroy(&s_alunos);
    sem_destroy(&s_saida_monitores);

    return 0;
}
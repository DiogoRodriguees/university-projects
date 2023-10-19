/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242
        Marcos Vinicios de Quadros - 2380560

    Descrição:
        PROFESSOR:
            O professor inicia com a abertura da sala, liberando os alunos
            e monitores para entrarem. Essa liberação ocorre liberando o semáforo "s_sala" e
            atribuindo TRUE para a variável sala_aberta. Após um determindado tempo(T_SALA_ABERTA)
            o professor avisa os alunos e monitores que não é possível entrar na sala.
            Esse aviso ocorre atribuindo o valor FALSE para a variável "sala_aberta".

            Em seguida ele aguarda a liberação do semáforo(s_fechar_sala) que é liberado pelo último
            monitor que sair da sala, garantindo que não existe mais nenhum aluno na sala.
            O professor ao sair também libera um token para os alunos que estão presos no semáforo, isso ocorre
            para quando nenhum monitor entrar na sala.

        MONITORES:
            Ao executar, o monitor verifica se pode entrar na sala. Caso seja possível, ele verifica se
            existe algum monitor que deseja sair. Caso exista, ele libera esse monitor. Caso não exista
            ele libera(ALUNOS_POR_GRUPO) tokens para novos alunos entrarem. Após determinado tempo(T_MONITOR_SALA)
            o monitor sinaliza que deseja sair. Essa sinalização ocorre com a atribuição "TRUE" para a variável(monitor_deseja_sair).
            Se o monitor for o último a sair ele libera um token para o semáforo "s_fechar_sala", assim o professor termina sua execução
            fechando a sala. Se ao sair o monitor mantiver a condição de X alunos por monitor, ele sai normalmente, caso contrario,
            ele tenta pegar um token de saida no semáforo(s_saida_monitores). Esse semáforo recebe post quando um monitor entra ou
            quando um aluno sai desde que a condição(X alunos por monitor) é satisfeita.

        ALUNOS:
            Ao entrar os alunos aguardam até que os monitores liberem tokens para entrada ou algum aluno saia. Essa liberação
            ocorre pelo método post no semáforo "s_alunos". Ao entra o aluno verifica se a entrada de alunos na sala é possível.
            Caso seja possível o aluno entra e permance um tempo(T_ALUNO_SALA) antes de sair da sala.
            Ao sair o aluno verifica se é possível liberar um monitor para que ele saia.
            Para que o monitor seja liberado o aluno deve saber se a quantidade de alunos na sala é menor
            que a quantidade de ((monitores - 1) * alunos por grupo). Essa verificação garante X alunos por monitor.


        FUNÇÕES EXTRAS:
            Para simular um tempo entre as chegadas dos monitores, foi criado um variavel(T_CRIACAO_MONITOR) que é
            utilizada na função sleep antes que o proximo monitor seja criado.
            O tempo que o monitor e aluno permanecem na sala é determinado pelas variáveis define (T_ALUNO_SALA,
            T_MONITOR_SALA) que são passadas como parâmetro nas funções sleep().

        EXPRESSOES:
            ((monitores - 1) * alunos por grupo) essa expressão calcula a quantidade de alunos por monitor caso um monitor saia agora.
            Essa verificação é utilizada para saber se com um monitor a menos a condição(X alunos por monitor) é satisfeita.
    Data:
        08 de Junho de 2023
*/

#include <stdbool.h>   // bool
#include <pthread.h>   // pthread_t
#include <semaphore.h> // sem_t
#include <stdio.h>     // printf()
#include <unistd.h>    // sleep()

/* Variaveis para teste com diferentes numeros de ALUNOS e MONITORES */
#define LIMITE_ALUNOS_SALA 20 // limite da sala e quantidade de alunos a ser criado
#define MONITORES 3           // quantidade de monitores a ser criado
#define ALUNOS_POR_GRUPO 3

/* Tempo que as threads executam sleep */
#define T_ALUNO_SALA 5      // tempo que o aluno permance na sala
#define T_MONITOR_SALA 5    // tempo que o monitor permance na sala
#define T_SALA_ABERTA 30    // tempo que a sala permanece aberta
#define T_CRIACAO_MONITOR 4 // tempo para simular uma entrada tardia do monitor

/* Semaforos */
sem_t s_alunos;          // Controle dos alunos estudando
sem_t s_saida_monitores; // Controle da saida de monitores
sem_t s_sala;            // Controle dos estudantes na sala
sem_t mutex;             // Controle das variaveis compartilhadas
sem_t s_fechar_sala;     // Libera o professor pra fechar a sala;

/* Variaveis para controle de permição da entrada de alunos e monitores */
bool sala_aberta = false;         // o valor false é atribuido quando a entrada de alunos não é permitida
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
    sem_wait(&mutex);
    sala_aberta = true;
    sem_post(&mutex);
    printf("PROFESSOR ABRIU A SALA\n");

    /* Sala fica aberta por 60seg */
    sleep(T_SALA_ABERTA);

    /* Monitores não poderão mais entrar na sala*/
    sem_wait(&mutex);
    printf("MONITORES NAO PODEM MAIS ENTRAR\n");

    /* Alunos não poderão mais entrar na sala*/
    sala_aberta = false;
    printf("ALUNOS NAO PODEM MAIS ENTRAR\n");
    sem_post(&s_alunos);
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
    sem_wait(&mutex);
    total_alunos++;
    sem_post(&mutex);

    sem_wait(&s_alunos); // semaforo liberado pela entrada de um monitor ou saida de um aluno
    sem_wait(&s_sala);

    // entra na seção critica
    sem_wait(&mutex);
    if (sala_aberta)
    {

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
        total_alunos--;
        sem_post(&s_alunos);
        sem_post(&s_sala);
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
    if (sala_aberta)
    {
        // a entrada de um monitor fecha o semaforo "s_fechar_sala"
        // sendo assim o professor precisa esperar o semaforo ser liberado
        if (monitores_disponiveis == 0)
            sem_wait(&s_fechar_sala);

        monitores_disponiveis++;

        // libera token se nenhum monitor deseja sair
        if (monitor_deseja_sair)
        {
            sem_post(&s_saida_monitores);
            monitor_deseja_sair = false;
        }
        else if (total_alunos > 0)
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

        sem_wait(&mutex);
        // verifica se a saida do monitor é permitida
        if (total_alunos > ((monitores_disponiveis - 1) * ALUNOS_POR_GRUPO))
        {
            monitor_deseja_sair = true;
            sem_post(&mutex);
            // semaforo liberado pela saida de X alunos ou entrada de um monitor
            sem_wait(&s_saida_monitores);
        }
        else
        {
            sem_post(&mutex);
        }

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
    sem_init(&s_fechar_sala, 0, 1);     // libera o professor p/ fechar a sala. inicia com 1 para o caso de ninguem entrar
    sem_init(&s_saida_monitores, 0, 0); // controla se o monitor pode sair

    /* Inicializando thread do PROFESSOR */
    pthread_create(&professor, NULL, executarProfessor, NULL);
    sleep(1);

    /* Inicializando threads ALUNOS */
    for (int i = 0; i < LIMITE_ALUNOS_SALA; i++)
    {
        pthread_create(&alunos[i], NULL, executarAlunos, &i);
        sleep(1); // proximo aluno chega em 1seg
    }

    /* Inicializando threads de MONITORES */
    for (int i = 0; i < MONITORES; i++)
    {
        pthread_create(&monitores[i], NULL, executarMonitores, &i);
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
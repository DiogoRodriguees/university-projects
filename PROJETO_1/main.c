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

#include <stdbool.h> // bool
#include <pthread.h> // pthread_t

/* Variaveis para teste com diferentes numeros de ALUNOS, MONITORES e PROFESSORES */
#define MAX_ALUNOS_GRUPO 10
#define MAX_MONITORES_GRUPO 1
#define MAX_TURMA_ALUNOS 30

/* Variaveis que serão inicializadas pelo monitor */
int estudantesSala;   // numero de estudantes na sala
bool grupoTemMonitor; // verifica se o grupo possui um monitor


int main()
{
    /* Criar threads para: Alunos, Monitores, Professores */
    pthread_t alunos, professores, monitores;

    return 0;
}
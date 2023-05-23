#include <stdbool.h> // bool

/* Variaveis para teste com diferentes numeros de ALUNOS, MONITORES e PROFESSORES */
#define MAX_ALUNOS_GRUPO 10
#define MAX_MONITORES_GRUPO 1
#define MAX_TURMA_ALUNOS 30

/* Variaveis que serão inicializadas pelo monitor */
int estudantesSala;   // numero de estudantes na sala
bool grupoTemMonitor; // verifica se o grupo possui um monitor

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

int main()
{
    /* Criar threads para: Alunos, Monitores, Professores */

    return 0;
}
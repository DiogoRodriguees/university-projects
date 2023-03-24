#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <unistd.h>    // fork()
#include <sys/types.h> // pid_t
#include <stdbool.h>

int main(int argc, char** argv)
{
    if(argc <= 1){
        printf("É necessario informar o numero de processos!\n");
        return 0;
    }

    int quantidadeProcessos = atoi(argv[1]);
    int vetor[] = {0, 7, 2, 3, 4, 5, 6, 7, 8, 9};
    int tamanhoVetor = sizeof(vetor) / sizeof(vetor[0]);
    int intervaloProcesso = tamanhoVetor / quantidadeProcessos;
    int sobraVetor = tamanhoVetor % quantidadeProcessos;
    int posInicial, posFinal;
    int valorBuscado = 7;
    int ultimoPid = 0;                  // guarda o PID do último processo filho que fez a busca

    
    printf("pstree -c -p %d \n\n", getpid());
    pid_t pid;
    pid = fork();

    // Loop responsável por criar os processos filhos
    for (int i = 0; i < quantidadeProcessos; i++)
    {
        if (pid && i != 0)
        {
            pid = fork();
        
        } 
        
        if (pid == 0 && ultimoPid != getpid()) {
            ultimoPid = getpid();
            
            // Define a primeira posição do processo
            int posInicial = i * intervaloProcesso;

            // Define a última posição que o processo irá buscar
            if(i == quantidadeProcessos -1) {
                posFinal = tamanhoVetor;
            } else {
                posFinal = (i+1) * intervaloProcesso; 
            }
            
            // Busca o valor procurado no vetor e exibe o pid do processo filho
            for (int j = posInicial; j < posFinal; j++) {
                if(vetor[j] == valorBuscado)
                {
                    printf("Processo filho PID: %d, encontrou o valor!\n", getpid());
                }
            }

        }
    }

    getchar();
    exit(0);
}

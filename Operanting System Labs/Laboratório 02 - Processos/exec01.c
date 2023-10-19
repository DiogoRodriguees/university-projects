#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <unistd.h>    // fork()
#include <sys/types.h> // pid_t

/*
    Descrição:
        Esse programa cria uma arvore de processos
        com a altura recebida por parametro.

    Autores:
        Diogo Rodrigues - 2380252
        Carolina Yumi Fujii - 2335468
        Marcos Vinicius de Quadros - 2380560

    Data de criação: 22/03/2023
*/

void createProcess(int level)
{
    if (level <= 1)
        return;

    pid_t pid;

    pid = fork();

    if (pid)
    {
        pid = fork();

        if (pid == 0)
            createProcess(level - 1);
    }
    else
    {
        createProcess(level - 1);
    }

    getchar();
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("O arquivo necessita de pelo menos uma entrada\n");
        return 0;
    }
    int levels = atoi(argv[1]); // passando o parametro de int para char

    printf("Digite o comando abaixo em outro terminal\n"); // exibi o processo pai
    printf("pstree -c -p %d \n\n", getpid());              // exibi o processo pai
    createProcess(levels);

    exit(0);
}
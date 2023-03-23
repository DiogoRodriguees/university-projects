#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <unistd.h>    // fork()
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // wait
#include <string.h>


/* 

verificar sintaxes das funçoes wait e exec
 */

int main(int argc, char* argv[])
{
    /* Comando */
    char cmd[] = "ls";
    char com[] = "-l";
    char x[] = "/bin/";
    strcat(x, cmd);
    
    /* cria processo pai */
    pid_t pid;
    pid = fork();


    int status; // variavel que verifica se o filho foi criado com sucesso


    if (pid)
    {
        printf("Eu Sou o Processo Pai - Filho PID :%d \n", pid);
        printf("PID : %d \n\n", getpid());
        wait(&status);
        printf("Processo Pai finalizado \n");
    }
    else
    {
        printf("Eu Sou o Processo Filho - PID : %d \n", getpid());
        execl(x, cmd, com, (char *)NULL); // executa os comando no shell
    }

    exit(0);
}
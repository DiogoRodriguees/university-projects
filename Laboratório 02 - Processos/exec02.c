#include <stdio.h>      // printf()
#include <stdlib.h>     // exit()
#include <unistd.h>     // fork()
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait
#include <string.h>     // strcpy(), strcat()

// verificar sintaxes das funçoes wait e exec

int main(int argc, char *argv[])
{
    char comando[10];
    strcpy(comando, argv[1]);

    // passando os complementos
    char complemento[50] = "";
    for (int i = 2; i < argc; i++)
    {
        char aux[50];
        strcpy(aux, argv[i]);
        strcat(complemento, aux);
    }

    // caminho onde será executado o comando
    char enderecoDeExecucao[] = "/bin/";
    strcat(enderecoDeExecucao, comando);

    // cria processo pai
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

        if(argc > 2){
            execl(enderecoDeExecucao, comando, complemento, (char *)NULL); // executa os comando no shell
        }else{
            execl(enderecoDeExecucao, comando, (char *)NULL); // executa os comando no shell
        }
    }

    exit(0);
}
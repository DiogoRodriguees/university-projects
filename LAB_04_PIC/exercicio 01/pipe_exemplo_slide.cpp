#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    pid_t pid;       // processo
    int mypipe[2];   // [0] - refenrencia o lado da leituro, [1] - referencia o lado da escrita
    char buffer[40]; // guardar as informações até que sejam lidas

    /* criação do pipe */
    if (pipe(mypipe))
    {
        std::cerr << "erro ao criar o pipe";
        return EXIT_FAILURE;
    }

    /* criação do processo filho */
    pid = fork();
    if (pid < (pid_t)0)
    {
        std::cerr << "erro ao executar o fork";
        return EXIT_FAILURE;
    }
    else if (pid == (pid_t)0)
    {
        /* entrou no processo filho */
        close(mypipe[1]);                              // fechando a referencia para escrita
        read(mypipe[0], buffer, sizeof(buffer));       // fazendo a leitura do buffer
        std::cout << "FILHO: " << buffer << std::endl; // imprimindo o conteudo enviado pelo pai
        fflush(stdout);
        return EXIT_SUCCESS;
    }
    else
    {
        /* entrou no processo pai */
        close(mypipe[0]);   // fechando a referencia para leitura
        
        std::cout << "PAI: Digite algo para enviar ao filho: ";
        scanf("%40[^\n]", buffer);
        write(mypipe[1], buffer, sizeof(buffer)); // escrevendo a mensagem no buffer

        wait(NULL);
        return EXIT_SUCCESS;
    }

    exit(0);
}
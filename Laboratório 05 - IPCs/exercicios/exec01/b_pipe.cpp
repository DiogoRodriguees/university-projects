#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid;

    char buffer[40];
    int mypipe[2];

    /* criar o pipe */
    if (pipe(mypipe))
    {
        std::cerr << "erro ao criar o pipe";
        return EXIT_FAILURE;
    }

    /* criar o processo */
    pid = fork();

    /* caso ocorra um erro na criação do processo */
    if (pid < (pid_t)-1)
    {
        std::cerr << "erro ao executar o fork";
        return EXIT_FAILURE;
    }
    /* processo filho irá executar o calculo */
    else if (pid == (pid_t)0)
    {
        // receber a expressao em formato string
        // separar os numeros do operador
        // realizar o calculo
        // exibir o resultado no terminal
    }
    /* processo pai recebe a expressão matematica e enviar para o filho */
    else
    {
        /* entrou no processo pai */
        close(mypipe[0]); // fechando a referencia para leitura

        std::cout << "PAI: Digite algo para enviar ao filho: ";
        scanf("%40[^\n]", buffer);
        write(mypipe[1], buffer, sizeof(buffer)); // escrevendo a mensagem no buffer

        wait(NULL);
        return EXIT_SUCCESS;
    }
    return 0;
}
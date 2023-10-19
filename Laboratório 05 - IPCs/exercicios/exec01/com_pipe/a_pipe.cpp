#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <cstdlib>
#include <string.h>

int verificaVogais(char caracter)
{
    char vogais[10] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};

    for (int i = 0; i < 10; i++)
        if (caracter == vogais[i])
            return true;

    return false;
}

int contaVogais(char *string)
{
    int contador = 0;
    int i = 0;

    while (string[i] != '\0')
    {
        if (verificaVogais(string[i]))
            contador++;

        i++;
    }

    return contador;
}
int contaConsoantes(char *string)
{
    int contador = 0;
    int i = 0;

    while (string[i] != '\0')
    {
        if (verificaVogais(string[i]))
        {
            i++;
            continue;
        }
        contador++;
        i++;
    }

    return contador;
}

int tamanhoString(char *string)
{
    int contador = 0;
    int i = 0;

    while (string[i] != '\0')
    {
        contador++;
        i++;
    }

    return contador;
}

int contaEspaço(char *string)
{
    int contador = 0;
    int i = 0;

    while (string[i] != '\0')
    {
        if (string[i] == ' ')
            contador++;
        i++;
    }

    return contador;
}

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

    /* caso o processo seja filho */
    if (pid == (pid_t)0)
    {
        int numeroUm, numeroDois;
        char operador;

        close(mypipe[1]);                        // fechando a referencia para escrita
        read(mypipe[0], buffer, sizeof(buffer)); // fazendo a leitura do buffer

        int vogais = contaVogais(buffer);
        int consoantes = contaConsoantes(buffer);
        int tamanho = tamanhoString(buffer);
        int espacos = contaEspaço(buffer);
        /* imprimir expressao original e resultado */
        std::cout << "FILHO: " << std::endl;
        std::cout << "Tamanho: " << tamanho << std::endl;
        std::cout << "Vogais: " << vogais << std::endl;
        std::cout << "Consoantes: " << consoantes << std::endl;
        std::cout << "Espaços: " << espacos << std::endl;

        fflush(stdout);
        return EXIT_SUCCESS;
    }
    /* caso o processo seja o pai */
    else
    {
        /* entrou no processo pai */
        close(mypipe[0]); // fechando a referencia para leitura

        std::cout << "PAI: insira uma string: ";
        scanf("%40[^\n]", buffer);
        write(mypipe[1], buffer, sizeof(buffer)); // escrevendo a mensagem no buffer

        wait(NULL);
        return EXIT_SUCCESS;
    }

    return 0;
}
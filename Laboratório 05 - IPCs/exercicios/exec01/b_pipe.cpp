#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <cstdlib>
#include <string.h>

int calcular(int numeroUm, int numeroDois, char operador)
{
    int resultado = 0;

    switch (operador)
    {
    case '+':
        resultado = numeroUm + numeroDois;
        break;

    case '-':
        resultado = numeroUm - numeroDois;
        break;

    case '/':
        resultado = numeroUm / numeroDois;
        break;

    case '*':
        resultado = numeroUm * numeroDois;
        break;
    }

    return resultado;
}

void convertExpressao(char *buffer, int *numeroUm, int *numeroDois, char *operador)
{
    int i = 0;
    char caracter;
    bool operadorEncontrado = false;
    char operadoresValidos[2] = {'*', '/'};
    
    /* zera os paramentros de saida */
    *numeroUm = 0;
    *numeroDois = 0;

    while (buffer[i] != '\0')
    {
        caracter = buffer[i++];
        
        if (caracter >= operadoresValidos[0] && caracter <= operadoresValidos[1]) {
            *operador = caracter;
            operadorEncontrado = true;
            continue;
        }
        if (operadorEncontrado) {
            *numeroDois *= 10;
            *numeroDois += atoi(&caracter);
        }
        else {
            *numeroUm *= 10;
            *numeroUm += atoi(&caracter);
        }
    }
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
        
        /* converte expressao para numero e operador */
        convertExpressao(buffer, &numeroUm, &numeroDois, &operador);
        
        /* calcular o resultado */
        int resultado = calcular(numeroUm, numeroDois, operador);
        
        /* imprimir expressao original e resultado */
        std::cout << "FILHO: " << numeroUm << " " << operador << " " << numeroDois << " = " << resultado << std::endl;
        
        fflush(stdout);
        return EXIT_SUCCESS;
    }
    /* caso o processo seja o pai */
    else
    {
        /* entrou no processo pai */
        close(mypipe[0]); // fechando a referencia para leitura

        std::cout << "PAI: insira uma expressao no formato (22+44): ";
        scanf("%40[^\n]", buffer);
        write(mypipe[1], buffer, sizeof(buffer)); // escrevendo a mensagem no buffer

        wait(NULL);
        return EXIT_SUCCESS;
    }
    return 0;
}
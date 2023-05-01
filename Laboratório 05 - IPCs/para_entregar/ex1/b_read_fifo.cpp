
/*
    Autores:
        Diogo Rodrigues dos Santos - 2380232
        Marcos Vinicius de Quadros - 2380560

    Descrição:
        O progama é responsavel por ler uma expressão matematica
        simples(8 * 8), realizar o calcula dessa expressão e
        exibir o resultado no terminal.

    Data:
        02 de Maio 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "/tmp/serverfifo"

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
        if(strcmp(&caracter, " ") == 0){
            continue;
        }

        if (caracter >= operadoresValidos[0] && caracter <= operadoresValidos[1])
        {
            *operador = caracter;
            operadorEncontrado = true;
            continue;
        }
        if (operadorEncontrado)
        {
            *numeroDois *= 10;
            *numeroDois += atoi(&caracter);
        }
        else
        {
            *numeroUm *= 10;
            *numeroUm += atoi(&caracter);
        }   
    }
}

int main(int argc, char **argv)
{
    int fd, bytes_read;
    char buf[512];

    // cria um FIFO se inexistente
    if ((mkfifo(SERVER_FIFO, 0664) == -1) && (errno != EEXIST))
    {
        perror("mkfifo");
        exit(1);
    }

    // abre um FIFO
    if ((fd = open(SERVER_FIFO, O_RDONLY)) == -1)
        perror("open");

    while (1)
    {
        // lê mensagem do FIFO
        memset(buf, '\0', sizeof(buf));
        if ((bytes_read = read(fd, buf, sizeof(buf))) == -1)
            perror("read");

        if (bytes_read == 0)
        {
            printf("-- None data...close and reopen fifo --\n");
            close(fd);
            fd = open(SERVER_FIFO, O_RDONLY);
            continue;
        }

        if (bytes_read > 0)
        {
            int numeroUm, numeroDois;
            char operador;

            /* converte expressao para numero e operador */
            convertExpressao(buf, &numeroUm, &numeroDois, &operador);

            /* calcular o resultado */
            int resultado = calcular(numeroUm, numeroDois, operador);
            
            // Imprime o resultado
            printf("Expressao recebida: %s\n", buf);
            printf("Resultado: %i\n", resultado);
        }
    }

    if (close(fd) == -1)
        perror("close");
}

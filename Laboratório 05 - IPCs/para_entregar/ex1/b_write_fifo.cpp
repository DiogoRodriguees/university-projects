
/*
    Autores:
        Diogo Rodrigues dos Santos - 2380232
        Marcos Vinicius de Quadros - 2380560

    Descrição:
        O progama é responsavel por escrever no FIFO
        uma expressão matematica simples(8 * 8).
        Essa expressão deve ser recebida por parametro.

    Data:
        02 de Maio 2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>

#define SERVER_FIFO "/tmp/serverfifo"
#define N 1

char buf[512];

int main(int argc, char **argv)
{

    int fd_server;

    if ((fd_server = open(SERVER_FIFO, O_WRONLY)) == -1)
    {
        perror("open: server fifo");
        return 1;
    }
    
    char string[20];
    
    int value = 0;
    while (value < N)
    {
        // cria uma mensagem
        value++;
        printf("Informe a expressao matematica sem espacos (Ex. 2*3): ");
        scanf("%s", string);
        sprintf(buf, string);
        printf("Sending: %s\n", buf);

        // envia mensagem para o fifo
        write(fd_server, buf, strlen(buf));

        // sleep(1);
    }

    if (close(fd_server) == -1)
    {
        perror("close");
        return 2;
    }

    return 0;
}

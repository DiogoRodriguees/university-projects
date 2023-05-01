#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "/tmp/serverfifo"

int main(int argc, char **argv)
{
    int fd_server, num_bytes_read; // descritor para o fifo
    char buf[512] = "mensagem de teste do buffer";

    /* cria um FIFO quando ele não exista */
    if ((mkfifo(SERVER_FIFO, 0664) == -1) && (errno != EEXIST))
    {
        perror("mkfifo");
        exit(1);
    }

    /* abre o fifo */
    if ((fd_server = open(SERVER_FIFO, O_RDONLY)) == -1)
    {
        perror("open");
    }

    /* lê e trata as mensagens do FIFO */
    while (1)
    {
        memset(buf, '\0', sizeof(buf));
        num_bytes_read = read(fd_server, buf, sizeof(buf));
        
        switch (num_bytes_read)
        {
        case -1:
            perror("-- read error -- ");
            break;

        case 0:
            printf("-- None data ... close and reopen fifo --\n");
            close(fd_server);
            fd_server = open(SERVER_FIFO, O_RDONLY);
            break;

        default:
            printf("Received %d bytes: %s\n", num_bytes_read, buf);
        }

        if (close(fd_server) == -1)
        {
            perror("close");
        }
    }

    exit(0);
}
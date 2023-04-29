#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "/tmp/serverfifo"
#define N 5
char buf[512];

int main(int argc, char **argv)
{
    int fd_server; // descritor do FIFO
    
    if( (fd_server = open (SERVER_FIFO, O_WRONLY)) == -1){
        perror("open error: server fifo");
        return 1;
    }
    
    int value = 0;
    
    while(value < N){
        // cria mensagens incrementalmente
        sprintf(buf, "Menssage %ld - Number %d", (long) getpid(), value++);
        printf("Sending: %s\n", buf);
        
        // envia a mensagem para o fifo
        write(fd_server, buf, strlen(buf));
        
        // somente para vizualização
        sleep(1);
        
    }
    
    if(close (fd_server) == -1) {
        perror("close error: server fifo");
        return 2;
    }
    
    exit(0);
}
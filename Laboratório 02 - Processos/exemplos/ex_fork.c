#include <stdio.h>        // printf()
#include <stdlib.h>       // exit()
#include <unistd.h>       // fork()
#include <sys/types.h>    // pid_t

int main( ){
    pid_t pid;
    int  valor = 0;
 	
	pid = fork( ); /* cria um processo e devolve pid do filho 
                      para o pai e 0 para o filho */

    if (pid) {
        /* este trecho é executado pelo pai */
		printf("Eu Sou o Processo Pai - Filho PID %d \n", pid);
        valor = 5;
        printf("Valor: %d \n", valor);
    }
    else {
        /* este trecho é executado pelo filho */
   		printf("Eu Sou o Processo Filho - Filho PID %d \n", pid);
        valor = 10;
        printf("Valor: %d \n", valor);
    }
    
    exit(0);        
}

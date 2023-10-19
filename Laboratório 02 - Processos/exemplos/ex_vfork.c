#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main( ){
	pid_t pid;
    int  valor = 5;
 	
   pid = vfork();
   if (pid){   /* este trecho é executado pelo pid */
		printf("Eu Sou o Processo Pai %d \n", pid);
        printf("Valor: %d \n", valor);
   }
   else
       {   /* este trecho é executado pelo filho */
   		printf("Eu Sou o Processo Filho %d \n", pid);
        scanf("%d", &valor);
        printf("Valor: %d \n", valor);
       }
   exit(0);        
}

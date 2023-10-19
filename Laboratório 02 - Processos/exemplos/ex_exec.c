/**
 * Exemplo de código com o execl/execv
 *  exec() family of functions replaces the current process image with a new process image.
 *  devolve -1 em caso de erro.
 *  https://www.systutorials.com/docs/linux/man/3p-execl/
 */

#include <unistd.h>
#include <stdio.h>

int main() {
   printf("Antes do exec\n");
   execl("/bin/ls", "ls", "-r", "-t", "-l", (char *) NULL);
   //char *cmd[] = { "ls", "-l", (char *)0 };
   //execv ("/bin/ls", cmd);
   printf("Depois do exec\n");
   
   return 0;
}


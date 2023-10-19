/**
 * zombies.c
 * Cria 10 processos filhos zombies
*/
#include <unistd.h>    // _exit()

int main(){
   for(int i=0; i<5; i++){
      if (fork() == 0) { // processo filho
          _exit(0); // and exit
      }
   }

   while(1);   // processo pai continua executando
}
 

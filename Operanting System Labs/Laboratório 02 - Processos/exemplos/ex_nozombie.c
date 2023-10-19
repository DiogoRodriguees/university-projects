/**
 * ex_nozombie.c
 * No zombie kids
*/
#include <unistd.h>    // _exit(), sleep()
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // wait()
#include <stdio.h>

int main(){
   pid_t pid;
   int status;

   for(int i=0; i<5; i++){
      if (fork() == 0) { // processo filho
          _exit(0); // and exit
      }
   }

   while (1) {
      pid = wait(&status);
      printf("-- Pai detecta processo %d foi finalizado com estado %d. \n", pid, status);
      sleep(1);
   }
}
 

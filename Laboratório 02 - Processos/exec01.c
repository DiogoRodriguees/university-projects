#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <unistd.h>    // fork()
#include <sys/types.h> // pid_t

void createProcess(int level)
{
    if (level <= 1)
        return;

    pid_t pid;
    pid = fork(); 

    if (pid)
    {
        printf("PID : %d \n\n", getpid()); // exibi o processo pai

        pid = fork();

        if (pid == 0) createProcess(level - 1);
    }
    else
    {
        createProcess(level - 1);
    }

    getchar();
}


int main()
{
    createProcess(5);
    exit(0);
}
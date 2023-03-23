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
        pid = fork();

        if (pid == 0)
            createProcess(level - 1);
    }
    else
    {
        createProcess(level - 1);
    }

    getchar();
}



int main(int argc, char* argv[])
{
    if(argc <= 1){
        printf("O arquivo necessita de pelo menos uma entrada\n");
        return 0;
    }
    int levels = atoi(argv[1]); // passando o parametro de int para char

    printf("pstree -c -p %d \n\n", getpid()); // exibi o processo pai
    createProcess(levels);

    exit(0);
}
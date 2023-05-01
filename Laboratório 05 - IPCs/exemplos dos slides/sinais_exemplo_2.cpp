#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t programe_executing = 1;

void catch_alarm(int sig)
{
    puts("Alarm!");
    programe_executing = 0;
    signal(sig, catch_alarm);
}

void timer(void)
{
    puts("Fazendo alguma coisa enquanto aguarda o alarmw");
}

int main(void)
{

    /* estabelece um tratador para os sinais SIGALRM */
    signal(SIGALRM, catch_alarm);

    /* define um alarme paara daqui a 10 seg */
    alarm(10);

    /* fica executando o loop ate ser interrompido pelo laço */
    while (programe_executing)
    {
        timer();
    }

    puts("Terminou!");

    return EXIT_SUCCESS;
}
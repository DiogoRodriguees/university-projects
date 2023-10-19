#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t programe_executing = 1;
int contador = 10;

void catch_alarm(int sig)
{
    /* atribui 0 para a variavel que fara interrupção do laço while */
    programe_executing = 0;
}

void timer(void)
{
    /* imprime a contagm regrassiva iniciado em 10 */
    printf("%i\n", contador--);

    /* aguarda 1 segundo */
    sleep(1);
}

void catch_interrupt(int sig)
{
    /* tradador do sinal de alarm */
    signal(SIGALRM, catch_alarm);

    /* limpando terminal */
    system("clear");

    /* aviso de interupção iniciada e disparo do alarm */
    puts("Interrompendo em 10 segundos");
    alarm(10);

    /* loop para a contagem dos segundos */
    while (programe_executing)
    {
        timer();
    }
}

int main(void)
{
    /* mensagem para o usuario*/
    puts("Aguardando sinal de interrupção: CTRL + C\n");

    /* estabelece um tratador para os sinais SIGINT */
    signal(SIGINT, catch_interrupt);

    while (programe_executing)
    {
        /* loop infinito até o sinal de interrupção ser recebido */
    }

    /* mensagem de aviso */
    puts("O programa foi interrompido");

    return EXIT_SUCCESS;
}
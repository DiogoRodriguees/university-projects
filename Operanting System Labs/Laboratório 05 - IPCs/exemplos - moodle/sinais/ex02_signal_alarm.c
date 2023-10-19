/*
 * Exemplo: http://www.gnu.org/software/libc/manual/
 *               html_node/Handler-Returns.html#Handler-Returns
 */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Flag que controla a terminação do loop. */
volatile sig_atomic_t programe_executing = 1;

/* Tratador para o sinal SIGALRM. Reseta o flag e se reabilita. */
void catch_alarm(int sig)
{
    puts("Alarme!");
    programe_executing = 0;
    signal(sig, catch_alarm);
}

void do_stuff(void)
{
    puts("Fazendo alguma coisa enquanto aguarda o alarme.");
    sleep(1);
}

int main(void)
{
    /* Estabelece um tratador para sinais SIGALRM. */
    signal(SIGALRM, catch_alarm);

    /* Define um alarme para daqui a 10 segundos.
     * Interromperá o laço. */
    alarm(10);

    /* Fica em loop executando. */
    while (programe_executing)
        do_stuff();

    puts("Terminou.");

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------
 * Programa:
 *    velha - Implementa o jogo da velha
 *
 * Funcionalidades: 
 *    - inicializa um jogo sem nenhuma marcação
 *    - desenha a velha
 *    - seta as jogadas
 *    - verifica ganhador ou empate 
 *    
 *
 * Sintaxe:  velha 
 *
 * Notas:     
 *    - apesar de possuir um main, suas funções podem ser invocadas 
 *------------------------------------------------------------------------
 */


#include <stdio.h>
#include "velha.h"

void inicio_velha () {
    memset(&velha,' ',sizeof(velha)); 
    contador = 0;
}
    
void desenha_velha () {
    printf ("-------------\n");
    printf ("| %c | %c | %c |\n", velha[0][0], velha[0][1], velha[0][2]);
    printf ("-------------\n");
    printf ("| %c | %c | %c |\n", velha[1][0], velha[1][1], velha[1][2]);
    printf ("-------------\n");
    printf ("| %c | %c | %c |\n", velha[2][0], velha[2][1], velha[2][2]);
    printf ("-------------\n");
}

char verifica_ganhador () {
    /* contabiliza o numero de lances */
    contador++;
    
    int i;
    for (i=0; i<3; i++) {
        if ((velha[i][0] == velha[i][1]) && (velha[i][1] == velha[i][2])) return velha[i][1];
        if ((velha[0][i] == velha[1][i]) && (velha[1][i] == velha[2][i])) return velha[1][i];
    } //for
    
    /* verifica diagonais */
    if ((velha[0][0] == velha[1][1]) && (velha[1][1] == velha[2][2])) return velha[1][1];
    if ((velha[0][2] == velha[1][1]) && (velha[1][1] == velha[2][0])) return velha[1][1];
    
    /* retorna 0 para empate */
    if (contador == 9) return 0;
    
    /* retorna 32 (espaço) para jogo indefinido */
    return ' ';
}


int marca_velha (int l, int c, char sinal) {
    if (velha[l][c] != ' ') return -1;
    velha[l][c] = sinal;
    return 1;
}

/*
main (int argc, char *argv[]) {
    int l, c;
    char vez='X';
    
    inicio_velha();
    desenha_velha();
    
    int i=0;
    while (1) {
	
	do {
	   printf ("\nLinha: ");
	   scanf("%i", &l);
	   printf ("Coluna: ");
	   scanf("%i", &c);
	   printf ("\n");
	} while (marca_velha(l,c,vez) != 1);
        
	desenha_velha();
	
	char resp = verifica_ganhador();
	if (resp != ' ') { 
	   if (resp == 0) printf ("\n\nEmpate!!!!!!!\n\n", resp);
	   else printf ("\n\nGanhador é %c\n\n", resp);
	   exit(1);
	} //if
	
	if (i%2 == 0) vez = '0';
	else vez = 'X';
	i++;
    } //while
} //main
*/

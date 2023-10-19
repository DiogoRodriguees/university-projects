/*
 * Especifica as variáveis e funções para o jogo da velha
 **/

char velha[3][3];
int contador;

void inicio_velha();                           /* inicializa as variáveis do jogo */
void desenha_velha();                          /* desenha o tabuleiro e as posições preenchidas */
char verifica_ganhador();                      /* verifica se há ganhador ou empate, retorna sinal ganhador */ 
int marca_velha (int l, int c, char sinal);    /* seta uma posição com um determinado sinal 
                                                 (retorna 1 sucesso e -1 posição inválida */


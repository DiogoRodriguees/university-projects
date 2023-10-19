/**
 * Exemplo de processo IO Bound
 * autor: Rodrigo Campiolo
 */

#include<stdio.h>

int main() {
   char pnome[30];
   char unome[30];

   printf("Digite primeiro nome: ");
   scanf("%s", pnome);
   
   printf("Digite último nome: "); 
   scanf("%s", unome);
   
   printf("Citação: \n\t%s, %s\n", unome, pnome);

   return 0;
}

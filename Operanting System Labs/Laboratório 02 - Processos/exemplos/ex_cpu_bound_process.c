/**
 * Exemplo de processo CPU Bound
 * autor: Rodrigo Campiolo
 */

int main() {

   int a = 0;

   while (1) {
      a = a + 1;
      if (a == 32000) a = 0;
   }

   return 0;
}

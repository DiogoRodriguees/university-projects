#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    
}TablePage;

void extractParams(int address, int *displacement, int *pageNumber)
{
}

void acessPage(int pageNumber, int displacement)
{
    
    
}

void readPage(int address)
{
    int displacement;
    int pageNumber;
    extractParams(address, &displacement, &pageNumber);

    acessPage(pageNumber, displacement);
}

void writePage(int address) {}

void searchInstruction(int address) {}

void executeOperation(int op, int address)
{
    switch (op)
    {
    case 0:
        readPage(address);
        break;
    case 1:
        writePage(address);
        break;
    case 2:
        searchInstruction(address);
        break;

    default:
        break;
    }
}

int main(int argc, char **argv)
{
    int size_ram = 0;
    int size_pages = 0;
    int size_process = 0;
    int alg = 0;

    printf("Informe o tamanho da memoria RAM: ");
    scanf("%i", &size_ram);

    printf("Informe o tamanho das paginas: ");
    scanf("%i", &size_pages);

    printf("Informe o tamanho do processo: ");
    scanf("%i", &size_process);

    printf("Informe o algoritimo a ser utilizado: \n");
    printf("1. FIFO\n");
    printf("2. Relogio\n");
    printf("3. Outro\n");
    scanf("%i", &alg);

    int ram[size_ram];

    int op;
    int address;

    if (alg < 1 || alg > 3)
    {
        printf("Voce escolheu uma opção invalida\n");
        return 0;
    }

    // recebendo os endereços a serem acessados
    system("clear");
    printf("OP - ADDRESS\n");
    while (size_process)
    {
        scanf("%i", &op);
        scanf("%i", &address);
        size_process--;

        if (op < 0 || op > 2)
        {
            printf("Voce inseriu uma op invalida\n");
            break;
        }

        executeOperation(op, address);
    }

    return 0;
}
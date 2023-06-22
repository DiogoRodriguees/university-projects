#include <stdio.h>
#include <stdlib.h>

// Estrutura para a tabela de páginas
typedef struct
{
    int pageNumber;
    int frameNumber;
    int R;
    int M;
    // Outros bits de controle, como R (Referenced) e M (Modified), podem ser adicionados aqui
} PageTableEntry;

// Estrutura para a memória RAM
typedef struct
{
    int frameNumber;
    int isOccupied;
    // Outros dados relevantes para a página, como o número do processo, podem ser adicionados aqui
} Frame;

PageTableEntry *tablePage;
Frame *ram;
int memorySize = 0;
int tableSize = 0;

// Função para encontrar uma página na tabela de páginas
int findPage(PageTableEntry *pageTable, int pageNumber, int tableSize)
{
    for (int i = 0; i < tableSize; i++)
    {
        if (pageTable[i].pageNumber == pageNumber)
        {
            return i; // Retorna o índice da página na tabela de páginas
        }
    }
    return -1; // Página não encontrada na tabela de páginas
}

// Função para substituir uma página na RAM
int replacePage(Frame *memory, int memorySize)
{
    // Implemente aqui o algoritmo de substituição de páginas desejado
    // Pode ser FIFO, LRU, Second-Chance, etc.
    // Retorne o número do frame selecionado para substituição
}

int findPageNumberInMemory(int frameNumber, int memorySize)
{
    for (int i = 0; i < memorySize; i++)
    {
        if (ram[i].frameNumber == frameNumber)
        {
            return i;
        }
    }
}

void fifoAlg(int op, int address)
{
    // encontrar pageNumber
    int pageNumber;

    // converter o page number em frame number
    int frameNumber;

    // verifica se a pagina ja esta na memoria
    int pageFound = findPageNumberInMemory(frameNumber, memorySize);

    // caso a pagina nao esteja na memoria fisica
    if (pageFound == -1)
    {
        // calcula a posição de inserção ou substituição (processo mais antigo)
        int subPosition;

        // colocando a nova pagina na memoria
        ram[subPosition].frameNumber = tablePage[findPage(tablePage, pageNumber, tableSize)].frameNumber;

        ram->isOccupied = 1;
    }
}

void insertProcessInTablePage(int address)
{
    // extrair o page number do address
    int pageNumber;

    // verifica se a pagina ta na tabela
    int pageFound = findPage(tablePage, pageNumber, tableSize);

    // se a pagina não exstiver na tabela, inserir.
    if (pageFound == -1)
    {
        // inserir pagina na tabela
    }
}

void acessPageWithAlgoritm(int op, int address, int alg)
{
    // encontra pagenumber

    // antes de acessar, colocar o processa na TB
    insertProcessInTablePage(address);

    // verifica se ta memoria
    if (alg == 1)
    {
        fifoAlg(op, address);
    }
}

// Função principal do simulador
int main()
{
    system("clear");

    // Configurações do simulador
    int ramSize = 1024;     // Tamanho da RAM em bytes
    int pageSize = 256;     // Tamanho das páginas em bytes
    int processSize = 2048; // Tamanho do processo em bytes
    int algorithm = 1;      // Número do algoritmo de substituição de páginas a ser simulado

    // Cálculo do número de páginas e frames
    int numPages = processSize / pageSize;
    int numFrames = ramSize / pageSize;

    printf("%d\n", numPages);
    printf("%d\n", numFrames);

    // Alocação dinâmica da tabela de páginas e da memória RAM
    tablePage = (PageTableEntry *)calloc(numPages, sizeof(PageTableEntry));
    ram = (Frame *)calloc(numFrames, sizeof(Frame));

    int totalPageFaults = 0;

    int op = 1, address = 0;

    // acessa a pagina utilizando o algoritmo de substituição informado
    acessPageWithAlgoritm(op, address, algorithm);

    return 0;
}
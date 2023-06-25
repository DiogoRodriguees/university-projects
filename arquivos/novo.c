#include <stdio.h>
#include <stdlib.h>



// Estrutura para a tabela de páginas
typedef struct {
    int pageNumber;
    int frameNumber;
    int R;
    int M;
    // Outros bits de controle, como R (Referenced) e M (Modified), podem ser adicionados aqui
} PageTableEntry;

// Estrutura para a memória RAM
typedef struct {
    int frameNumber;
    int isOccupied;
    // Outros dados relevantes para a página, como o número do processo, podem ser adicionados aqui
} Frame;

PageTableEntry* tablePage;
Frame* ram;
int tableSize = 0;

// Função para encontrar uma página na tabela de páginas
int findPage(PageTableEntry* pageTable, int pageNumber, int tableSize) {
    for (int i = 0; i < tableSize; i++) {
        if (pageTable[i].pageNumber == pageNumber) {
            return i;  // Retorna o índice da página na tabela de páginas
        }
    }
    return -1;  // Página não encontrada na tabela de páginas
}

// Função para substituir uma página na RAM
int replacePage(Frame* memory, int memorySize) {
    // Implemente aqui o algoritmo de substituição de páginas desejado
    // Pode ser FIFO, LRU, Second-Chance, etc.
    // Retorne o número do frame selecionado para substituição
}

void fifoAlg(int op, int address){
    // encontrar pageNumber
    int pageNumber;

    int pageFound = findPageNumberInMemory(pageNumber);

    if(pageFound == -1){
        if(findPageNotOcuped(ram)){
            // calcula a posição de inserção (processo mais antigo)
            int subPosition;
            
            // converter page number in frameNumber
            ram[subPosition].frameNumber = findPage(tablePage, pageNumber, tableSize);
        }

    }
}

void insertProcessInTablePage(int address){
    // extrair o page number do address  
    int pageNumber;

    // verifica se a pagina ta na tabela
    int pageFound = findPage(tablePage, pageNumber, tableSize);

    // se a pagina não exstiver na tabela, inserir.
    if(pageFound == -1){
        // inserir pagina na tabela
    }
}

void acessPageWithAlgoritm(int op, int address, int alg){
    // encontra pagenumber
    
    // antes de acessar, colocar o processa na TB
    insertProcessInTablePage(address);

    // verifica se ta memoria
    if(alg == 1){
        fifoAlg(op, address);
    }
}

// Função principal do simulador
int main() {
    // Configurações do simulador
    int ramSize = 1024;  // Tamanho da RAM em bytes
    int pageSize = 256;  // Tamanho das páginas em bytes
    int processSize = 2048;  // Tamanho do processo em bytes
    int algorithm = 1;  // Número do algoritmo de substituição de páginas a ser simulado

    // Cálculo do número de páginas e frames
    int numPages = processSize / pageSize;
    int numFrames = ramSize / pageSize;

    printf("%d\n", numPages);
    printf("%d\n", numFrames);

    // Alocação dinâmica da tabela de páginas e da memória RAM
    tablePage = (PageTableEntry*)calloc(numPages, sizeof(PageTableEntry));
    ram = (Frame*)calloc(numFrames, sizeof(Frame));

    int totalPageFaults = 0;

    int op, address;
    
    acessPageWithAlgoritm(op, address, algorithm);
    // while (1) {
    //     scanf("%d %x", &op, &address);
    //     if(op == 0 && address == 0) break;
    //     int pageNumber = address / pageSize;
    //     int pageIndex = findPage(pageTable, pageNumber, numPages);

    //     if (pageIndex == -1) {
    //         printf("Page fault for address %x\n", address);
    //         totalPageFaults++;

    //         // Implemente aqui o tratamento da falta de página
    //         // Você precisará substituir uma página na RAM e atualizar a tabela de páginas
    //     }

    //     // A página está presente na RAM, faça o acesso normalmente
    //     // Implemente aqui o restante do processamento do endereço
    // }

    return 0;

}
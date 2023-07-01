#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define PAGE_SIZE 1
#define RAM_SIZE 2
#define PROCESS_SIZE 8
#define ALGORITMO 2

int global_time = 0;

// representação das paginas
typedef struct
{
    // int page_number;
    int frame_number;
    int r_bit;
    int m_bit;
    int v_bit;
    int time_loaded;
} Page;

// NO utilizado na fila
typedef struct no
{
    int dado;
    struct no *prox;
} No;

typedef struct fila
{
    No *inicio;
    No *fim;
    int qtde;
} Fila;

Fila *fila_criar()
{
    Fila *f = (Fila *)malloc(sizeof(Fila));
    f->qtde = 0;
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

bool fila_inserir(Fila *f, int page_number)
{
    if (f == NULL)
        return false;

    No *novo = (No *)malloc(sizeof(No));
    if (f->qtde == 0)
    {
        f->inicio = novo;
        f->inicio->dado = page_number;
        f->inicio->prox = NULL;
    }
    else
    {
        f->fim->prox = novo;
    }
    f->fim = novo;
    f->fim->dado = page_number;
    f->fim->prox = NULL;
    f->qtde++;
}

bool fila_remover(Fila *f)
{
    if (f == NULL)
        return false;
    if (f->qtde == 0)
        return false;

    No *aux = f->inicio;
    f->inicio = f->inicio->prox;
    f->qtde--;
    free(aux);
    aux = NULL;

    return true;
}

void fila_destruir(Fila **enderecoFila)
{
    Fila *f = *enderecoFila;
    No *aux;

    while (f->inicio != NULL)
    {
        aux = f->inicio;
        f->inicio = f->inicio->prox;
        free(aux);
    }

    free(f);
    *enderecoFila = NULL;
}

// limpa os bits de referencia(r_bit) para 0
void update_bits(Page **page_table, int quantity_pages)
{
    for (int i = 0; i < quantity_pages; i++)
    {
        if (page_table[i])
        {
            page_table[i]->r_bit = 0;
        }
    }
}

void nru(Page **page_table, int **ram, int quantity_pages, int ram_size, int id)
{
    int position_sub_memory = -1; // posição que aponta para a memoria (-1 significa que a memoria possui espaço livre)
    int page_target = -1;         // posição da pagina vitima
    int class = -1;               // menor classe encontrada (-1 valor de inicialização)

    // verifica se tem memoria livre
    for (int i = 0; i < ram_size; i++)
    {
        if ((*ram)[i] == 0)
        {
            position_sub_memory = i;
            (*ram)[position_sub_memory] = 1; // Modifica o valor da RAM
            break;
        }
    }

    // não tem espaço na meoria
    if (position_sub_memory == -1)
    {
        // passa pela paginas tabela
        for (int i = 0; i < quantity_pages; i++)
        {
            printf("page: %d, bit R: %d, bit M: %d \n", i, page_table[i]->r_bit, page_table[i]->m_bit);
            if (page_table[i]->v_bit == 1)
            {
                // verifica se classe 0
                if (page_table[i]->r_bit == 0 && page_table[i]->m_bit == 0)
                {
                    page_target = i;
                    class = 0;
                    break;
                }

                // verifica se classe 1
                if (page_table[i]->r_bit == 0 && page_table[i]->m_bit == 1)
                {
                    // verifica se nenhuma class 1 foi encontrada até o memento
                    if (class != 1)
                    {
                        page_target = i;
                        class = 1;
                    }
                }

                // verifica se classe 2
                if (page_table[i]->r_bit == 1 && page_table[i]->m_bit == 0)
                {
                    // verifica se essa é a menor classe até o memento
                    if (class > 2)
                    {
                        page_target = i;
                        class = 2;
                    }
                }

                // verifica se classe 3
                if (page_table[i]->r_bit == 1 && page_table[i]->m_bit == 1)
                {
                    // verifica se nenhuma classe menor foi encontrada
                    if (class == -1 && class != 3)
                    {
                        page_target = i;
                        class = 3;
                    }
                }
            }
        }

        // busca frame number da pagina vitima
        position_sub_memory = page_table[page_target]->frame_number;

        printf("Pagina %d substituida.\n", page_target);

        // tira a referenciade memoria da pagina vitima
        page_table[page_target]->v_bit = 0;
        // page_table[page_target]->r_bit = 0;

        // referencia na memoria a pagina desejada
        page_table[id]->frame_number = position_sub_memory;
        page_table[id]->v_bit = 1;
        // page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_sub_memory);
    }
    // tem espaço na memoria
    else
    {
        // referencia a pagina na memoria
        page_table[id]->frame_number = position_sub_memory;
        page_table[id]->v_bit = 1;
        // page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_sub_memory);
    }
}

void fifo(Page **page_table, int **ram, int quantity_pages, int ram_size, Fila **fila, int id)
{
    int position_sub_memory = -1; // posição que aponta para a memoria (-1 significa que a memoria possui espaço livre)
    int page_target = -1;         // numero da pagina vitima

    // inseri na memoria ram enquanto tiver posição livre
    for (int i = 0; i < ram_size; i++)
    {
        if ((*ram)[i] == 0)
        {
            position_sub_memory = i;
            (*ram)[position_sub_memory] = 1; // Modifica o valor da RAM
            fila_inserir(*fila, id);
            break;
        }
    }

    // se não existe posição livre, substitui
    if (position_sub_memory == -1)
    {
            
        page_target = (*fila)->inicio->dado;
        position_sub_memory = page_table[page_target]->frame_number;

        // remove a página mais antiga
        fila_remover(*fila);
        fila_inserir(*fila, id);


        printf("Pagina %d substituida.\n", page_target);

        // pagina vitima deixa de apontar para memoria
        page_table[page_target]->v_bit = 0;

        // acessando pagina que deseja acessar memoria
        page_table[id]->frame_number = position_sub_memory;
        page_table[id]->v_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_sub_memory);
    }
    // existe posição livre
    else
    {
        // referencia a pagina desejada na memoria
        page_table[id]->frame_number = position_sub_memory;
        page_table[id]->v_bit = 1;
        page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_sub_memory);
    }
}

char *algoritmoEscohido(int num)
{
    switch (num)
    {
    case 1:
        return "NRU";
        break;
    case 2:
        return "FIFO";
        break;
    case 3:
        return "OUTRO";
        break;

    default:
        break;
    }

    return "NENHUM";
}

int main()
{
    int physical_address;
    system("clear");
    int refresh_interval = 4;

    int quantity_frames = RAM_SIZE / PAGE_SIZE;
    printf("\n%i\n", quantity_frames);

    int *RAM = (int *)calloc(quantity_frames, sizeof(int));

    int quantity_pages = PROCESS_SIZE / PAGE_SIZE;

    // se a quantidade de paginas for um numero quebrado, incrementa
    if (PROCESS_SIZE % PAGE_SIZE != 0)
    {
        quantity_pages++;
    }

    printf("Tamanho do processo: %d\n", PROCESS_SIZE);
    printf("Tamanho da pagina: %d\n", PAGE_SIZE);
    printf("Tamanho da ram: %d\n", RAM_SIZE);
    printf("Algoritmo: %s\n", algoritmoEscohido(ALGORITMO));
    printf("Quantidade de paginas: %d\n", quantity_pages);
    printf("Quantidade de quadros: %d\n", quantity_frames);

    printf("\n\nInsira a OP e o ADDRESS\n");
    Page **page_table = (Page **)malloc(quantity_pages * sizeof(Page *));

    for (int i = 0; i < quantity_pages; i++)
    {
        Page *new_page = (Page *)malloc(sizeof(Page));
        // new_page->page_number = i;
        new_page->frame_number = -1;
        new_page->r_bit = 0;
        new_page->m_bit = 0;
        new_page->v_bit = 0;
        new_page->time_loaded = global_time;
        page_table[i] = new_page;
    }

    int op;
    int address;

    Fila *fila = NULL;

    if (ALGORITMO == 2)
    {
        fila = fila_criar();
    }

    while (scanf("%d %x", &op, &address) == 2)
    {
        printf("Operacao: %d, Endereco Logico: 0x%x\n", op, address);

        int page_number = address / PAGE_SIZE;

        // calcula o deslocamento
        int offset = address % PAGE_SIZE;

        int page_fault = 0;

        if (op == 1)
        {
            page_table[page_number]->m_bit = 1;
        }

        if (!page_table[page_number]->v_bit)
        {
            page_fault++;

            // Lógica de substituição de página
            switch (ALGORITMO)
            {
            case 1:
                nru(page_table, &RAM, quantity_pages, quantity_frames, page_number);
                break;
            case 2:
                fifo(page_table, &RAM, quantity_pages, quantity_frames, &fila, page_number);
                break;
            case 3:
                // Implementar o 3º algoritmo
                break;
            default:
                printf("Erro: Algoritmo de substituicao de pagina invalido.\n");
            }

            // Cálcula o endereço físico
            physical_address = page_table[page_number]->frame_number * PAGE_SIZE + (offset);
            printf("Endereco Fisico: %d * %d + %x = 0x%x\n", page_table[page_number]->frame_number, PAGE_SIZE, offset, physical_address);
        }
        else
        {
            printf("Pagina %i ja esta no quadro %i.\n", page_number, page_table[page_number]->frame_number);
        }

        // intervalo de tempo que atribui 0 aos bits de referencia
        // funciona como o clock do sistema
        global_time++;
        if (global_time % refresh_interval == 0)
        {
            update_bits(page_table, quantity_pages);
        }

        printf("\n");
    }

    free(fila);
    free(page_table);
    return 0;
}
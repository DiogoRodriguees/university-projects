#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int global_time = 0;

typedef struct
{
    int page_number;
    int frame_number;
    int r_bit;
    int m_bit;
    int time_loaded;
} Page;

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

int find_page(Page **page_table, int page_number, int table_size)
{
    for (int i = 0; i < table_size; i++)
    {
        if (page_table[i]->page_number == page_number)
        {
            return page_table[i]->frame_number;
        }
    }
    return -1;
}

void update_bits(Page **page_table, int table_size)
{
    for (int i = 0; i < table_size; i++)
    {
        if (page_table[i])
        {
            page_table[i]->r_bit = 0;
            page_table[i]->m_bit = 0;
        }
    }
}

void nru(Page **page_table, int **RAM, int table_size, int ram_size, int id)
{

    int frame = -1;
    int victim = -1;
    int min_time = global_time;

    for (int i = 0; i < ram_size; i++)
    {
        if ((*RAM)[i] == 0)
        {
            frame = i;
            (*RAM)[frame] = 1; // Modifica o valor da RAM
            break;
        }
    }

    if (frame == -1)
    {
        for (int i = 0; i < table_size; i++)
        {
            if (page_table[i]->r_bit == 0 && page_table[i]->m_bit == 0)
            {
                frame = page_table[i]->frame_number;
                victim = i;
                break;
            }
            else if (page_table[i]->r_bit == 0 && page_table[i]->m_bit == 1)
            {
                frame = page_table[i]->frame_number;
                victim = i;
                break;
            }
            else if (page_table[i]->r_bit == 1 && page_table[i]->m_bit == 0)
            {
                frame = page_table[i]->frame_number;
                victim = i;
                break;
            }
            else if (page_table[i]->r_bit == 1 && page_table[i]->m_bit == 1)
            {
                frame = page_table[i]->frame_number;
                victim = i;
                break;
            }

            // // Seleciona a página com o menor tempo de carregamento
            // if (page_table[i]->time_loaded < min_time)
            // {
            //     min_time = page_table[i]->time_loaded;
            //     victim = i;
            // }
        }

        printf("Pagina %d substituida.\n", victim);
        page_table[victim]->frame_number = -1;
        page_table[victim]->r_bit = 0;

        page_table[id]->frame_number = frame;
        page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, frame);
    }
    else
    {
        page_table[id]->frame_number = frame;
        page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, frame);
    }

}

void fifo(Page **page_table, int **RAM, int table_size, int ram_size, Fila **fila, int id)
{
    
    int frame = -1;
    int victim = -1;
    for (int i = 0; i < ram_size; i++)
    {
        if ((*RAM)[i] == 0)
        {
            frame = i;
            fila_inserir(*fila, id);
            (*RAM)[frame] = 1; // Modifica o valor da RAM
            break;
        }
    }

    if (frame == -1)
    {
        for (int i = 0; i < table_size; i++)
        {
            if (page_table[i]->page_number == (*fila)->inicio->dado)
            {
                frame = page_table[i]->frame_number;
                victim = i;
                break;
            }
        }

        if (fila_remover(*fila))
        {
            fila_inserir(*fila, id);
        }
        else
        {
            printf("ERRO");
        }

        printf("Pagina %d substituida.\n", victim);
        page_table[victim]->frame_number = -1;
        page_table[victim]->r_bit = 0;

        page_table[id]->frame_number = frame;
        page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, frame);
    }
    else
    {
        page_table[id]->frame_number = frame;
        page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, frame);
    }

}

int main()
{
    int page_size = 0;
    int ram_size = 0;
    int process_size = 0;
    int algoritmo_substituicao = 2;
    int refresh_interval = 6;

    scanf("%d\n", &ram_size);
    scanf("%d\n", &page_size);
    scanf("%d\n", &process_size);
    scanf("%d\n", &algoritmo_substituicao);

    int *RAM = (int *)calloc(ram_size, sizeof(int));

    int table_size = process_size / page_size;
    if (process_size % page_size != 0)
    {
        table_size += 1;
    }

    printf("Tamanho da Tabela de página: %d\n\n", table_size);

    Page **page_table = (Page **)malloc(table_size * sizeof(Page *));

    for (int i = 0; i < table_size; i++)
    {
        Page *new_page = (Page *)malloc(sizeof(Page));
        new_page->page_number = i;
        new_page->frame_number = -1;
        new_page->r_bit = 0;
        new_page->m_bit = 0;
        new_page->time_loaded = global_time;
        page_table[i] = new_page;
    }

    int op;
    unsigned int address;

    Fila *fila = NULL;

    if (algoritmo_substituicao == 2)
    {
        fila = fila_criar();
    }

    while (scanf("%d %x", &op, &address) == 2)
    {
        printf("Operacao: %d, Endereco: 0x%x\n", op, address);
        int page_number = address / page_size;

        int offset = address % page_size;

        int frame_number = find_page(page_table, page_number, table_size);

        if (frame_number == -1)
        {
            // Lógica de substituição de página
            switch (algoritmo_substituicao)
            {
            case 1:
                nru(page_table, &RAM, table_size, ram_size, page_number);
                break;
            case 2:
                fifo(page_table, &RAM, table_size, ram_size, &fila, page_number);
                break;
            case 3:
                // Chamar a função 3º algoritmo
                break;
            default:
                printf("Erro: Algoritmo de substituicao de pagina invalido.\n");
            }

            frame_number = find_page(page_table, page_number, table_size);
        }

        if (frame_number == -1)
        {
            printf("Erro: Pagina nao encontrada na memoria.\n");
            return 0;
        }

        // page_table[frame_number]->r_bit = 1;

        unsigned int physical_address = frame_number * page_size + (address % page_size);
        printf("Endereco Fisico: 0x%x\n", physical_address);

        if (op == 1)
        {
            page_table[page_number]->m_bit = 1;
        }

        // intervalo de tempo que atribui 0 aos bits de referencia
        // funciona como o clook do sistema
        global_time++;
        if (global_time % refresh_interval == 0)
        {
            update_bits(page_table, table_size);
        }

        printf("\n");
    }

    free(page_table);
    return 0;
}
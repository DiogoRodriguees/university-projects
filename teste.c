#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int page_number;
    int r_bit;
    int m_bit;
    int time_loaded;
} Page;

int global_time = 0;

int find_page(Page **memory, int page_number, int ram_size)
{
    for (int i = 0; i < ram_size; i++)
    {
        if (memory[i] && memory[i]->page_number == page_number)
        {
            return i;
        }
    }
    return -1;
}

int find_victim(Page **memory, int ram_size)
{
    int min_time = global_time;
    int victim = -1;
    for (int i = 0; i < ram_size; i++)
    {
        if (!memory[i])
        {
            return i;
        }

        if (memory[i]->r_bit == 0 && memory[i]->m_bit == 0)
        {
            return i;
        }

        if (memory[i]->r_bit == 0 && memory[i]->m_bit == 1)
        {
            return i;
        }

        // if (memory[i]->r_bit == 1 && memory[i]->m_bit == 0)
        // {
        //     return i;
        // }

        // if (memory[i]->r_bit == 1 && memory[i]->m_bit == 1)
        // {
        //     return i;
        // }

        // Seleciona a página com o menor tempo de carregamento
        if (memory[i]->time_loaded < min_time)
        {
            min_time = memory[i]->time_loaded;
            victim = i;
        }

    }

    return victim;
}

void update_bits(Page **memory, int ram_size)
{
    for (int i = 0; i < ram_size; i++)
    {
        if (memory[i])
        {
            memory[i]->r_bit = 0;
            memory[i]->m_bit = 0;
        }
    }
}

void nru(Page **memory, int id, int ram_size)
{
    int index = find_page(memory, id, ram_size);
    if (index != -1)
    {
        return;
    }

    int victim = find_victim(memory, ram_size);
    if (victim == -1)
    {
        printf("Erro: Tabela de paginas cheia e nenhuma pagina vitima disponivel para substituicao.\n");
        exit(1);
    }

    if (memory[victim])
    {
        printf("Pagina %d substituida.\n", memory[victim]->page_number);
        free(memory[victim]);
    }

    Page *new_page = (Page *)malloc(sizeof(Page));
    new_page->page_number = id;
    new_page->r_bit = 1;
    new_page->m_bit = 0;
    new_page->time_loaded = global_time;
    memory[victim] = new_page;

    printf("Pagina %d carregada no quadro %d.\n", id, victim);
}


unsigned int logical_to_physical(Page **memory, unsigned int logical_address, int page_size, int ram_size)
{
    int page_number = logical_address / page_size;
    int offset = logical_address % page_size;
    int frame_number = find_page(memory, page_number, ram_size);

    if (frame_number == -1)
    {
        nru(memory, page_number, ram_size);
        frame_number = find_page(memory, page_number, ram_size);
    }

    if (frame_number == -1)
    {
        printf("Erro: Pagina nao encontrada na memoria.\n");
        return -1;
    }

    memory[frame_number]->r_bit = 1;

    return frame_number * page_size + offset;
}

int main()
{
    int page_size = 0;
    int ram_size = 0;
    int process_size = 0;
    int refresh_interval = 6;

    scanf("%d\n", &ram_size);
    scanf("%d\n", &page_size);
    scanf("%d\n", &process_size);

    // cria a memória com a quantidade referente as páginas 
    
    Page **memory = (Page **)malloc(ram_size * sizeof(Page *));

    for (int i = 0; i < ram_size; i++)
    {
        memory[i] = NULL;
    }

    int op;
    unsigned int address;

    while (scanf("%d %x", &op, &address) == 2)
    {
        printf("Operacao: %d, Endereco: 0x%x\n", op, address);
        int page_number = address / page_size;
        unsigned int physical_address = logical_to_physical(memory, address, page_size, ram_size);

        printf("Endereco Fisico: 0x%x\n", physical_address);

        if (op == 1)
        {
            int frame_number = find_page(memory, page_number, ram_size);
            if (frame_number != -1)
            {
                memory[frame_number]->m_bit = 1;
            }
        }
        

        // intervalo de tempo que atribui 0 aos bits de referencia 
        // funciona como o clook do sistema
        global_time++;
        if (global_time % refresh_interval == 0)
        {
            update_bits(memory, ram_size);
        }

        printf("\n");
    }

    free(memory);
    return 0;
}

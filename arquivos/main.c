/*
    Autores:
        Diogo Rodrigues - 2380242
        Marcos Vinicius de Quadros - 2380560

    Descrição

    Data:
        04 de Julho 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int global_time = 0; // variavel que funciona como o clock do sistema

/*****************************************************************************************
 *                                 Estrutura das Páginas                                 *
 *****************************************************************************************/
typedef struct
{
    // int page_number;
    int frame_number;
    int r_bit;
    int m_bit;
    int v_bit;
    int time_loaded;
} Page;

/*****************************************************************************************
 *                                 Implementaçã da Lista                                 *
 *****************************************************************************************/
typedef struct noh
{
    int dado;
    struct noh *prox;
    struct noh *ant;
} NO;

typedef struct
{
    NO *sentinela;
    int elements_amout;
} List;

List *create_list(int size)
{
    List *list = malloc(sizeof(List));
    list->elements_amout = 0;
    list->sentinela = malloc(sizeof(NO));
    list->sentinela->prox = list->sentinela;
    list->sentinela->ant = list->sentinela;

    return list;
}

void insert_list(List *l, int page_number, int pos)
{
    int i = 0;
    NO *inicio = l->sentinela->prox;

    NO *new_no = malloc(sizeof(NO));
    new_no->dado = page_number;

    if (inicio == l->sentinela)
    {
        new_no->prox = l->sentinela;
        new_no->ant = l->sentinela;

        l->sentinela->prox = new_no;
        l->sentinela->ant = new_no;

        l->elements_amout++;

        return;
    }

    if (pos == l->elements_amout)
    {
        l->sentinela->ant->prox = new_no;
        new_no->ant = l->sentinela->ant;
        l->sentinela->ant = new_no;
        new_no->prox = l->sentinela;

        l->elements_amout++;

        return;
    }

    while (inicio != l->sentinela)
    {
        if (i == pos)
        {
            inicio->ant->prox = new_no;
            new_no->ant = inicio->ant;

            inicio->ant = new_no;
            new_no->prox = inicio;
            break;
        }

        inicio = inicio->prox;
        i++;
    }

    l->elements_amout++;
}

void remove_list(List *l, int pos)
{
    int i = 0;
    NO *inicio = l->sentinela->prox;

    while (l->sentinela != inicio)
    {
        if (i == pos)
        {
            inicio->ant->prox = inicio->prox;
            inicio->prox->ant = inicio->ant;
            break;
        }
        i++;
        inicio = inicio->prox;
    }

    l->elements_amout--;
}

/*****************************************************************************************
 *                              Funções Auxiliares da Lista                              *
 *****************************************************************************************/
// busca uma pagina em determinada posição da lista
int search_page_list(List *l, int page_number)
{

    int i = 0;
    NO *inicio = l->sentinela->prox;

    while (inicio != l->sentinela)
    {
        if (i == page_number)
        {
            return inicio->dado;
        }

        inicio = inicio->prox;
        i++;
    }

    return i;
}

// verifica se a pegina esta na lista e retorna sua posição (-1 indica não encontrada)
int page_in_lst(List *l, int page_number)
{
    int i = 0;
    NO *inicio = l->sentinela->prox;

    while (inicio != l->sentinela)
    {
        if (inicio->dado == page_number)
        {
            return i;
        }
        inicio = inicio->prox;
        i++;
    }

    return -1;
}

/*****************************************************************************************
 *                                 Implementaçã da Fila                                  *
 *****************************************************************************************/
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

Fila *create_queue()
{
    Fila *f = (Fila *)malloc(sizeof(Fila));
    f->qtde = 0;
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

bool insert_queue(Fila *f, int page_number)
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

bool remove_queue(Fila *f)
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

/*****************************************************************************************
 *                           Algoritmo NRU - Not Recently Used                           *
 *****************************************************************************************/
void nru(Page **page_table, int **ram, int quantity_pages, int size_ram, int id, int op)
{
    int memory_position = -1;      // posição na memória (-1 indica: sem espaço livre) 
    int page_target = -1;          // posição da página que será substituida
    int class = -1;                // menor classe encontrada (-1 valor de inicialização)

    // verifica se tem memoria livre
    for (int i = 0; i < size_ram; i++)
    {
        if ((*ram)[i] == 0)
        {
            memory_position = i;
            (*ram)[memory_position] = 1; // marca a posição na memória como ocupada
            break;
        }
    }

    // não tem espaço na memoria
    if (memory_position == -1)
    {
        // busca pagina com a menor classe
        for (int i = 0; i < quantity_pages; i++)
        {
            if (page_table[i]->v_bit == 1)
            {
                // printf("Pos: %d \t Classe: %d \n", i, class);
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
                    if (class == -1 || class > 2)
                    {
                        page_target = i;
                        class = 2;
                    }
                }

                // verifica se classe 3
                if (page_table[i]->r_bit == 1 && page_table[i]->m_bit == 1)
                {
                    // verifica se nenhuma classe menor foi encontrada
                    if (class == -1)
                    {
                        page_target = i;
                        class = 3;
                    }
                }
            }
        }
        // busca frame number da pagina vitima
        memory_position = page_table[page_target]->frame_number;
        printf("Pagina %d substituida.\n", page_target);

        // tira a referenciade memoria da pagina vitima
        page_table[page_target]->v_bit = 0;
        page_table[page_target]->r_bit = 0;
        page_table[page_target]->frame_number = -1;

        // referencia na memoria a pagina desejada
        page_table[id]->frame_number = memory_position;
        page_table[id]->v_bit = 1;
        if (op == 1)
        {
            page_table[id]->m_bit = 1;
            page_table[id]->r_bit = 1;
        }
        else
        {
            page_table[id]->r_bit = 1;
        }

        printf("Pagina %d carregada no quadro %d.\n", id, memory_position);
    }
    // tem espaço na memoria
    else
    {
        // referencia a pagina na memoria
        page_table[id]->frame_number = memory_position;
        page_table[id]->v_bit = 1;
        if (op == 1)
        {
            page_table[id]->m_bit = 1;
            page_table[id]->r_bit = 1;
        }
        else
        {
            page_table[id]->r_bit = 1;
        }

        printf("Pagina %d carregada no quadro %d.\n", id, memory_position);
    }
}

/*****************************************************************************************
 *                                    Algoritmo FIFO                                     *
 *****************************************************************************************/
void fifo(Page **page_table, int **ram, int quantity_pages, int size_ram, Fila **fila, int id)
{
    int memory_position = -1;      // posição na memória (-1 indica: sem espaço livre)
    int page_target = -1;          // página que será substituida

    // inseri na memoria ram enquanto tiver posição livre
    for (int i = 0; i < size_ram; i++)
    {
        if ((*ram)[i] == 0)
        {
            memory_position = i;
            (*ram)[memory_position] = 1; // // marca a posição na memória como ocupada
            insert_queue(*fila, id);
            break;
        }
    }

    // se não existe posição livre, substitui
    if (memory_position == -1)
    {
        page_target = (*fila)->inicio->dado;
        memory_position = page_table[page_target]->frame_number;

        // remove a página mais antiga
        remove_queue(*fila);
        insert_queue(*fila, id);

        printf("Pagina %d substituida.\n", page_target);

        // pagina vitima deixa de apontar para memoria
        page_table[page_target]->v_bit = 0;
        page_table[page_target]->frame_number = -1;

        // acessando pagina que deseja acessar memoria
        page_table[id]->frame_number = memory_position;
        page_table[id]->v_bit = 1;

        printf("Pagina %d carregada no quadro %d.\n", id, memory_position);
    }
    // existe posição livre
    else
    {
        // referencia a pagina desejada na memoria
        page_table[id]->frame_number = memory_position;
        page_table[id]->v_bit = 1;

        printf("Pagina %d carregada no quadro %d.\n", id, memory_position);
    }
}

/*****************************************************************************************
 *                                Algoritmo Segunda Chance                               *
 *****************************************************************************************/
// busca a pagina mais antiga não referenciada
int find_old_page(List *l, Page **table)
{
    int i = 0;
    int inicio = l->sentinela->prox->dado; // page number da primeira pagina

    // busca a primeira página com bit R == 0
    while (table[inicio]->r_bit != 0)
    {
        table[inicio]->r_bit = 0;
        remove_list(l, 0);
        insert_list(l, inicio, l->elements_amout);

        inicio = search_page_list(l, 0);
    }

    return inicio;
}

void second_chance(Page **page_table, int *ram, int quantity_pages, int size_ram, List *list, int id)
{
    int memory_position = -1; // -1 indica que não existe quadros livres na memoria

    // verifica se tem memória livre (inseri a pegina no quadro caso esteja livre)
    for (int i = 0; i < size_ram; i++)
    {
        if (ram[i] == 0)
        {
            memory_position = i;
            insert_list(list, id, list->elements_amout);
            ram[memory_position] = 1;       // marca a posição na memória como ocupada
            break;
        }
    }

    // se não existe quadros livres na memória
    if (memory_position == -1)
    {
        // busca a pagina mais antiga não referenciada
        int page_old = find_old_page(list, page_table);

        // recebe o numero do quadro que a pagina vitima estava referenciada
        memory_position = page_table[page_old]->frame_number;

        // verifica se a pagina ja esta na lista (-1 indica: não está)
        int pos_in_list = page_in_lst(list, page_old);
        if (pos_in_list == -1)
        {
            insert_list(list, id, list->elements_amout);
        }
        else
        {
            remove_list(list, pos_in_list);
            insert_list(list, id, list->elements_amout);
        }

        printf("Pagina %d substituida.\n", page_old);

        // remove a referencia da pagina vitima
        page_table[page_old]->v_bit = 0;
        page_table[page_old]->r_bit = 0;
        page_table[page_old]->frame_number = -1;

        // referencia na memoria a nova pagina
        page_table[id]->frame_number = memory_position;
        page_table[id]->v_bit = 1;
        page_table[id]->r_bit = 1;

        printf("Pagina %d carregada no quadro %d.\n", id, memory_position);
        remove_list(list, page_old);
    }
    else
    {
        // referencia a pagina deseja na memória
        page_table[id]->frame_number = memory_position;
        page_table[id]->v_bit = 1;
        page_table[id]->r_bit = 1;

        printf("Pagina %d carregada no quadro %d.\n", id, memory_position);
    }
}

/*****************************************************************************************
 *                                  Funções Auxiliares                                   *
 *****************************************************************************************/
// retorna o algoritmo equivalente ao numero recebido
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
        return "SEGUNDA CHANCE";
        break;

    default:
        break;
    }

    return "NENHUM";
}

// imprimi a lista de endereços acessados
void print_list(List *l, Page **table)
{
    NO *inicio = l->sentinela->prox;

    while (inicio != l->sentinela)
    {
        printf("PG NUM:%i,  R:%i\n", inicio->dado, table[inicio->dado]->r_bit);
        inicio = inicio->prox;
    }
}

// imprimi a tabela de paginas
void print_table_page(Page **table, int size)
{
    printf("___________________________________________________\n");
    printf("   PAGE        FRAME       R      M      V   \n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < size; i++)
    {
        printf("    %i           %0*d         %i      %i      %i   \n", i, 2, table[i]->frame_number, table[i]->r_bit, table[i]->m_bit, table[i]->v_bit);
    }
    printf("---------------------------------------------------\n");
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

int main(int argc, char **argv)
{
    system("clear");
    int physical_address;     // endereço físico
    int page_fault = 0;       // total de substituições
    int refresh_interval = 2; // tempo para zerar os bits R

    int page_size;
    int size_ram;
    int size_process;
    int algoritmo;

    // verifica se um arquivo de entrada foi recebido
    if (argc != 2)
    {
        printf("Insira uma entrada de teste.\n");
        return 0;
    }

    char string_discart[50];

    FILE *entrada = fopen(argv[1], "r");
    fscanf(entrada, "%[^=]=%d\n", string_discart, &page_size);
    fscanf(entrada, "%[^=]=%d\n", string_discart, &size_ram);
    fscanf(entrada, "%[^=]=%d\n", string_discart, &size_process);
    fscanf(entrada, "%[^=]=%d\n", string_discart, &algoritmo);

    int quantity_pages = size_process / page_size; // calculo do total de paginas
    int quantity_frames = size_ram / page_size;    // calculo do total de frames
    int *RAM = (int *)calloc(quantity_frames, sizeof(int));

    // se a quantidade de paginas for um numero quebrado, incrementa
    if (size_process % page_size != 0)
    {
        quantity_pages++;
    }

    printf("Tamanho da Pagina: %d\n", page_size);
    printf("Tamanho da Mem RAM: %d\n", size_ram);
    printf("Tamanho do Processo: %d\n\n", size_process);

    printf("Quantidade de Paginas: %d\n", quantity_pages);
    printf("Quantidade de Quadros: %d\n\n", quantity_frames);

    printf("Algoritmo: %s\n", algoritmoEscohido(algoritmo));
    printf("\nInsira a OP e o ADDRESS\n\n");
    Page **page_table = (Page **)malloc(quantity_pages * sizeof(Page *));

    for (int i = 0; i < quantity_pages; i++)
    {
        Page *new_page = (Page *)malloc(sizeof(Page));
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

    // cria uma fila para uso do algoritmo 2(FIFO)
    if (algoritmo == 2)
    {
        fila = create_queue();
    }

    List *list;
    // cria uma lista para uso do algoritmo 3(NRU - Not Recently Used)
    if (algoritmo == 3)
    {
        list = create_list(quantity_frames);
    }

    // arquivo de entrada com as OPs e ADDRESS
    while (fscanf(entrada, "%[^=]=%d %[^=]=%x", string_discart, &op, string_discart, &address) != EOF)
    {
        printf("Operacao: %d, Endereco Logico: 0x%x\n", op, address);

        int page_number = address / page_size; // calculo do page number
        int offset = address % page_size;      // calculo do deslocamento
        physical_address = page_table[page_number]->frame_number * page_size + offset;

        // verifica se a pagina esta na memória (v_bit == 1, indica: está na memória)
        if (!page_table[page_number]->v_bit)
        {
            printf("Ocorreu page fault\n");
            page_fault++;

            // Lógica de substituição de página
            switch (algoritmo)
            {
            case 1:
                nru(page_table, &RAM, quantity_pages, quantity_frames, page_number, op);
                break;
            case 2:
                fifo(page_table, &RAM, quantity_pages, quantity_frames, &fila, page_number);
                break;
            case 3:
                second_chance(page_table, RAM, quantity_pages, quantity_frames, list, page_number);
                break;
            default:
                printf("Erro: Algoritmo de substituicao de pagina invalido.\n");
            }

            // Cálcula o endereço físico
            physical_address = page_table[page_number]->frame_number * page_size + (offset);
            printf("Endereco Fisico: %d * %d + %x = 0x%x\n", page_table[page_number]->frame_number, page_size, offset, physical_address);
        }
        else
        {
            if (algoritmo == 1 && op == 1)
            {
                page_table[page_number]->m_bit = 1;
                page_table[page_number]->r_bit = 1;
            }
            else if (algoritmo == 1 && op != 1)
            {
                page_table[page_number]->r_bit = 1;
            }
            printf("Pagina %i ja esta no quadro %i.\n", page_number, page_table[page_number]->frame_number);
        }

        if (algoritmo == 1)
        {
            global_time++;

            // zera os bits Rs se global_time for multiplo do refresh interval
            if (global_time % refresh_interval == 0)
            {
                update_bits(page_table, quantity_pages);
            }
        }

        print_table_page(page_table, quantity_pages);
        printf("\n");
    }

    printf("Total de page fault: %d\n", page_fault);
    free(fila);
    free(list);
    free(RAM);
    free(page_table);
    return 0;
}
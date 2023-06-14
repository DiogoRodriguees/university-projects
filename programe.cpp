/*
    Autores:
        Diogo Rodrigues dos Santos - 2380242
        Marcos Vinicius de Quadros - 2380560

    Descrição:

    Data:
        04 de Julho 2023
*/

typedef struct Data
{
  int ref_bit = 0;
  struct Data *nextData;
  struct Data *previousData;
} Data;

typedef struct DataList
{
  Data *sentinela;
  Data *clockPointer;
} DataList;

int main(int argc, char **argv)
{

  return 0;
}
## Laboratorio 03 - Manipulação de Threads
Os arquivos deste laboratório estão separados da seguinte maneira:  

    ./exec01.c - contém os arquivos do exercício 1
    ./exec02.c - contém os arquivos do exercício 2

    ./matrizTexto - arquivo com a matriz que será utilizada caso não seja informado outro aarquivo.
    ./respostas.txt - contém as resposta do que foi pedido no laboratório
    
    ./matriz.c e ./matriz.h - contém as funções auxiliares que foram disponibilizadas pelo professor.


### Como compilar
no prompt de comando, digite:  
* `make first` para compilar o exercício 1
* `make matriz` para compilar o exercício 2

### Como executar
no prompt de comando, digite:  
* `./exec1` para executar o exercício 1. 
* `./main numero_de_linhas numero_de_colunas numero_de_threads` para executar o exercício 2. 

### Exemplo de Uso
    ~$ make matriz
    ~$ ./main 10 10 4

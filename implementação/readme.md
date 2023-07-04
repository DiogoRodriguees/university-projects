## Sobre os Algoritmos Implementados
    FIFO:
        O algoritmo inicia com a criação de 3 váriaveis( page_target: receve o número da página que será substituída,
        memory_position: guarda o numero do quadro que a pagina vitima esta referenciada(-1 indica: sem quadros livres)).

        O próximo passo é verificar se existe algum quadro livre, caso exista, a pagina é
        referenciada para esse quadro. Caso não exista(memory_position == -1) será feita a substituição.

        Para a substituioção, é atribuida para a variavel page_target o numero da pagina vitima.

        Em seguida, é atribuido para a variavel memory_position frame que a pagina vitima estava.

        O próximo passo é remover a pagina vitima da fila e adicionar o nova pagina.

        Por fim, os bits da nova pagina são atualizados e ela é referenciada na memoria.

    NRU:
        O algoritmo inicia com a criação de 3 váriaveis(class: recebe a classe
        mais baixa até o momento, page_target: receve o número da página que será substituída,
        memory_position: guarda o numero do quadro que a pagina vitima esta referenciada(-1 indica: sem quadros livres)).

        O próximo passo é verificar se existe algum quadro livre, caso exista, a pagina é
        referenciada para esse quadro. Caso não exista(memory_position == -1) será feita a substituição.

        Para identificar a pagina vitma, o algoritmo busca entre as paginas que estão na memória, a que
        possui a menor classe.

        Em seguida é atribuido para a variavel memory_position o frame que a pagina vitma estava referenciando.

        Por fim, os bits da nova pagina são atualizados e a pagina é referenciada.

    Segunda Chance:
        O algoritmo inicia com a criação de 3 váriaveis( page_target: receve o número da página que será substituída,
        memory_position: guarda o numero do quadro que a pagina vitima esta referenciada(-1 indica: sem quadros livres)).

        O próximo passo é verificar se existe algum quadro livre, caso exista, a pagina é
        referenciada para esse quadro. Caso não exista(memory_position == -1) será feita a substituição.

        O proximo passo é encontra na lista a pagina mais antiga na referenciada.
        Em seguida é feito a atribuição do numero do frame da pagina vitima para a variavel memory_position.

        Encontrada a pagina vitima, a nova pagina e inserida na lista. Caso ela ja esteja na lista, é removida e inserida
        novamente, sendo tratada como uma nova pagina.

        Por fim os bits da nova pagina são atualizados e ela é referenciada na memoria.


## Como Compilar

No diretório do arquivo "main.c", digite o comando **make** no prompt de comando. Esse comando irá compilar o arquivo main.c e gerar um executavel "main.exe".

```bash
    $ make
```

## Como executar

Digite no prompt de comando o nome do executavel(./main.exe), seguido do arquivo de configurações(Ex. entrada2.conf).

```bash
    $ ./main.exe entrada2.conf
```

## Exemplo de Execução

```bash
    # compilar o arquivo main.c
    $ make

    # executa o programa com o arquivo de configurações entrada2.conf
    $ ./main.exe entrada2.conf
```

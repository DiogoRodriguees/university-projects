<h1 align="center">Algoritmos de Substituição de Páginas</h1>
Projeto proposto na disciplina de sistemas operacionais. Tem como objetivo simular 3 algoritmos de substituição de páginas.
Os algoritmos escolhidos para serem implementados foram: FIFO, NRU e Segunda Chance.

## Sobre os Algoritmos
### NRU - Not Recently Used
Esse algoritmo dividi as páginas acessadas em classes. A classe mais páginas é escolhida como vítima para ser substituida. As páginas recebem suas classes seguindo a seguinte regra:
* Classe 0: bit R=0, bit M=0
* Classe 1: bit R=0, bit M=1
* Classe 2: bit R=1, bit M=0
* Classe 3: bit R=1, bit M=1

### FIFO - First in, First out
Esse algoritmo utiliza uma filas de páginas acessadas. A página vitima sempre é a primeira da fila. Páginas acessadas sempre vão para o fim da fila.

### Segunda Chance
Muito parecido com o FIFO, o algoritmo da segunda chance utiliza uma lista de páginas acessadas. Para escolher uma página vitima, deve-se pegar a primeira página da lista que tiver o bit R=0. Caso a página tenha o bit R=1, o valor 0 é atribuido para esse bit e a página é adicionada ao final da lista.


## Como Executar o Projeto
```bash
  # clonando o repoositório
  $ git clone https://github.com/DiogoRodriguees/ALGORITMOS_SUBSTITUICAO_PAGINAS.git

  # abrindo o diretório do arquivo principal
  $ cd ./implementacao

  # copilação
  $ make

  # execução
  $ ./main.exe entrada1.conf
```
## Autores

| [<img src="https://avatars.githubusercontent.com/diogorodriguees" width=115><br><sub>Diogo Rodrigues</sub>](https://github.com/diogorodriguees) |  [<img src="https://avatars.githubusercontent.com/MarcosQuadross" width=115><br><sub>Marcos Vinicius de Quadros</sub>](https://github.com/MarcosQuadross) | 
| :---: | :---: | 

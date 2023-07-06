<h1 align="center">Parser - Analisador Sintático</h1>
Este projeto foi proposto na disciplina de Automos, Linguagens Formais e Computabilidade. Tem como objetivo implementar uma analisador sintático para expressões matemáticas.

## Como Executar
Antes de executar o projeto, é necessário que a biblioteca `automata-lib` esteja devidamente instalada. 
```bash
  # intalação via comand line
  $ pip install automata-lib 
```

Após ter instalado a biblioteca, siga as instruções de execução abaixo:
```bash
  # clonando repositorio
  $ git clone https://github.com/DiogoRodriguees/PARSER.git

  # abrindo o diretorio do arquivo principal
  $ cd ./implementação/python

  # executando com teste unitário
  $ python AnalisadorSintatico.py ./teste-exp/teste-001.exp

  # executando todos os teste
  $ make
```

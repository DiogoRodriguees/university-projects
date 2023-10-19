# Projeto 1
    PROFESSOR:
        O professor inicia com a abertura da sala, liberando os alunos
        e monitores para entrarem. Essa liberação ocorre liberando o semáforo "s_sala" e
        atribuindo TRUE para a variável sala_aberta. Após um determindado tempo(T_SALA_ABERTA)
        o professor avisa os alunos e monitores que não é possível entrar na sala.
        Esse aviso ocorre atribuindo o valor FALSE para a variável "sala_aberta".

        Em seguida ele aguarda a liberação do semáforo(s_fechar_sala) que é liberado pelo último
        monitor que sair da sala, garantindo que não existe mais nenhum aluno na sala.
        O professor ao sair também libera um token para os alunos que estão presos no semáforo, isso ocorre
        para quando nenhum monitor entrar na sala.

    MONITORES:
        Ao executar, o monitor verifica se pode entrar na sala. Caso seja possível, ele verifica se
        existe algum monitor que deseja sair. Caso exista, ele libera esse monitor. Caso não exista
        ele libera(ALUNOS_POR_GRUPO) tokens para novos alunos entrarem. Após determinado tempo(T_MONITOR_SALA)
        o monitor sinaliza que deseja sair. Essa sinalização ocorre com a atribuição "TRUE" para a variável(monitor_deseja_sair).
        Se o monitor for o último a sair ele libera um token para o semáforo "s_fechar_sala", assim o professor termina sua execução
        fechando a sala. Se ao sair o monitor mantiver a condição de X alunos por monitor, ele sai normalmente, caso contrario,
        ele tenta pegar um token de saida no semáforo(s_saida_monitores). Esse semáforo recebe post quando um monitor entra ou
        quando um aluno sai desde que a condição(X alunos por monitor) é satisfeita.

    ALUNOS:
        Ao entrar os alunos aguardam até que os monitores liberem tokens para entrada ou algum aluno saia. Essa liberação
        ocorre pelo método post no semáforo "s_alunos". Ao entra o aluno verifica se a entrada de alunos na sala é possível.
        Caso seja possível o aluno entra e permance um tempo(T_ALUNO_SALA) antes de sair da sala.
        Ao sair o aluno verifica se é possível liberar um monitor para que ele saia.
        Para que o monitor seja liberado o aluno deve saber se a quantidade de alunos na sala é menor
        que a quantidade de ((monitores - 1) * alunos por grupo). Essa verificação garante X alunos por monitor.


    FUNÇÕES EXTRAS:
        Para simular um tempo entre as chegadas dos monitores, foi criado um variavel(T_CRIACAO_MONITOR) que é
        utilizada na função sleep antes que o proximo monitor seja criado.
        O tempo que o monitor e aluno permanecem na sala é determinado pelas variáveis define (T_ALUNO_SALA,
        T_MONITOR_SALA) que são passadas como parâmetro nas funções sleep().

    EXPRESSOES:
        ((monitores - 1) * alunos por grupo) essa expressão calcula a quantidade de alunos por monitor caso um monitor saia agora.
        Essa verificação é utilizada para saber se com um monitor a menos a condição(X alunos por monitor) é satisfeita.

## Como compilar

```bash
$ make main
```

## Como executar

```bash
$ ./main.o
```

## Exemplo

```bash
# compilando arquivo
$ make main

# executando
$ ./main.o
```

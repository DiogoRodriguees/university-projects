#include <signal.h> // signal()
#include <stdio.h>  // printf(), puts()
#include <stdlib.h> // malloc
#include <string.h> // strcpy()

typedef struct Expressao
{
    int numero_um;
    char operador;
    int numero_dois;
} Expressao;

Expressao *expressao;
char *caminho_do_arquivo;

/* Função executado ao receber um sinal SIGHUP */
void reportar_disconexao_usuario(int);

/* Verifica se os para esperados pelo programa foram recebidos */
bool verificar_entradas(int);

/* Atribui para as variaveis os atributos lidos no arquivo texto recebido ao executar o programa */
void preencher_atributos();

int main(int argc, char **argv)
{
    /* Verifica se os parametros esperados foram inseridos */
    if (verificar_entradas(argc))
        return 0;

    /* Recebe o caminho do arquivo */
    caminho_do_arquivo = argv[1];

    expressao = (Expressao *)malloc(sizeof(Expressao));

    /* Tratamento para o sinal SIGHUP */
    signal(SIGHUP, reportar_disconexao_usuario);

    /* Atribuindo os valores do arquivo para as variaveis da struct  */
    preencher_atributos();

    /* Imprimindo os atributos antes de receber o sinal */
    printf("%i %c %i\n", expressao->numero_um, expressao->operador, expressao->numero_dois);

    /* Tempo para o usuario alterar o arquivo */
    printf("Altere o arquivo de entrada %s para visualizar as mudanças em 20 segundos!\n", caminho_do_arquivo);
    puts("Matenha o padrao do arquivo(numero operador numero). Exemplo: 44 + 3");
    sleep(20);

    /* O sinal de SIGHUP é recebido */
    raise(SIGHUP);

    /* Imprimindo os valores atualizados dos atributos */
    printf("%i %c %i\n", expressao->numero_um, expressao->operador, expressao->numero_dois);

    return 0;
}

void preencher_atributos()
{
    /* Abrinfo o aquivo para leitura */
    FILE *arquivo_com_atributos = fopen(caminho_do_arquivo, "r");

    char numero_um[20];
    char operador[20];
    char numero_dois[20];

    /* lendo o arquivo com os atributos */
    fscanf(arquivo_com_atributos, "%s", numero_um);
    fscanf(arquivo_com_atributos, "%s", operador);
    fscanf(arquivo_com_atributos, "%s", numero_dois);

    /* Convertendo os atributos lidas */
    expressao->numero_dois = atoi(numero_dois);
    expressao->numero_um = atoi(numero_um);
    expressao->operador = operador[0];

    /* Fechando arquivo após leitura */
    fclose(arquivo_com_atributos);
}

bool verificar_entradas(int quantidade_parametros_recebidos)
{
    if (quantidade_parametros_recebidos < 2)
    {
        puts("São esperados os seguintes parametros: diretorio_do_arquivo_de_atributos");
        return true;
    }

    return false;
}

void reportar_disconexao_usuario(int signal)
{
    preencher_atributos();
}
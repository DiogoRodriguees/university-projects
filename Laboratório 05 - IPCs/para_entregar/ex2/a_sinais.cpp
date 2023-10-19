/*
    Autores:
        Diogo Rodrigues dos Santos - 2380232
        Marcos Vinicius de Quadros - 2380560

    Descrição:
        O programa deve receber um diretorio para um arquivo txt,
        esse arquivo será lido a seus dados serão atribuidos para
        as variaveis internas.

        Um sinal de SIGHUP será disparado e, quando isso acontecer,
        o programa irá reabrir o arquivo e fazer as atribuições novamente

    Data:
        02 de Maio 2023
*/

#include <signal.h> // signal()
#include <stdio.h>  // printf(), puts()
#include <stdlib.h> // malloc
#include <string.h> // strcpy()
#include <unistd.h>

int executing_programe = 1;

char conteudo_arquivo[1024] = "";

char *caminho_do_arquivo;

/* Função executado ao receber um sinal SIGHUP */
void reportar_disconexao_usuario(int);

/* Verifica se os para esperados pelo programa foram recebidos */
bool verificar_entradas(int);

/* Atribui para as variaveis os atributos lidos no
arquivo texto recebido ao executar o programa */
void preencher_atributos();

int main(int argc, char **argv)
{
    // Verifica se os parametros esperados foram inseridos
    if (verificar_entradas(argc))
        return 0;

    // Recebe o caminho do arquivo
    caminho_do_arquivo = argv[1];

    // Tratamento para o sinal SIGHUP
    signal(SIGHUP, reportar_disconexao_usuario);

    // Atribuindo os valores do arquivo para as variaveis da struct
    preencher_atributos();
    
    printf("CONTEUDO ANTES DO SINAL:\n%s\n", conteudo_arquivo);
    printf("\nFaça uma alteração no arquivo antes de enviar o sinal\n");
    printf("Digite o comando \"kill -SIGHUP %d\" em outro terminal.\n", getpid());

    // loop infito até receber o sinal SIGHUP
    while (executing_programe)
    {
        sleep(1);
    }

    // Imprimindo os valores atualizados dos atributos
    printf("\nCONTEUDO DEPOIS DO SINAL:\n");
    printf("%s\n", conteudo_arquivo);

    return 0;
}

void preencher_atributos()
{
    /* Abrinfo o aquivo para leitura */
    FILE *arquivo_com_atributos = fopen(caminho_do_arquivo, "r");

    char conteudo_lido[1024] = "";

    // Resetando a variavel global
    strcpy(conteudo_arquivo, "");

    // Guarda a quantidade de bits lido (-1 significa final do arquivo)
    int fim_do_arquivo = 0;

    // Loop para copia dos dados no arquivo
    do
    {
        fim_do_arquivo = fscanf(arquivo_com_atributos, "%s", conteudo_lido);
        if (fim_do_arquivo != -1)
        {
            strcat(conteudo_arquivo, conteudo_lido);
            strcat(conteudo_arquivo, " ");
        }

    } while (fim_do_arquivo != -1);

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
    executing_programe = 0;
}
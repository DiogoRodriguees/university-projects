/**
 * Envia mensagens para um processo servidor com sockets UNIX
 * SOURCE: https://troydhanson.github.io/network/Unix_domain_sockets.html
 * With the abstract socket address, you don't need to maintain the socket
 * file (e.g. unlink) in the file system. And there is no file permissions control over it.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// char *socket_path = "./socket";
char *socket_path = "./hidden";
executing_programe = 1;

]enum PalavrasValidasPT {

    ESTUDAR,
    SISTEMAS,
    OPERACIONAIS,
    DIVERTIDO

} PalavrasValidas;

enum PalavrasValidasEN
{

    STUDY,
    SYSTEM,
    OPERATING,
    FUN

};

/* Separa as propriedades do modelo de tradução ISO */
void separar_expressao(char origem_p[], char destino_p[], char palavra_p[], char expressao[])
{
    int i = 0;
    bool origem_identificado = false;
    bool destino_identificado = false;
    char origem[10];
    char destino[10];
    char palavra[10];

    printf("Expressao %s\n", expressao);

    while (expressao[i] != '\0')
    {
        char caracter = expressao[i];

        // printf("caracter: %c\n", caracter);

        if (strcmp(&caracter, "-") == 0)
        {
            origem_identificado = true;
            i++;
            continue;
        }
        if (strcmp(&caracter, ":") == 0)
        {
            destino_identificado = true;
            i++;
            continue;
        }

        if (!origem_identificado)
        {
            strcat(origem, &caracter);
        }
        else if (!destino_identificado)
        {
            strcat(destino, &caracter);
        }
        else
        {
            strcat(palavra, &caracter);
        }

        i++;
    }
}

/* Traduz mensagem recebida no padrão ISO(pt-en:palavra) */
char *traduzir_mensagem()
{
    char *palavra_traduzida = "palavra";
    char lingua_origem[2];
    char lingua_destino[2];
    char palavra[20];

    separar_expressao(lingua_origem, lingua_destino, palavra, "pt-en:palavra");
    printf("%s\n %s\n %s\n", lingua_origem, lingua_destino, palavra);
    sleep(5);
    // variavel para lingua de origem
    // variavel para lingua de tradução
    // varival para a palavra a ser traduzida

    // função que traduz a palavra(lingua_origem, lingua_destino, palavra_para_traduzir)

    return palavra_traduzida;
}

void start_system()
{
    while (executing_programe)
    {
        
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    char buf[100];
    int fd, // descritor do socket
        cl, // identifica conexao do cliente
        rc; // bytes recebidos

    if (argc > 1)
        socket_path = argv[1];

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    if (*socket_path == '\0')
    {
        *addr.sun_path = '\0';
        strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
    }
    else
    {
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
        unlink(socket_path); // desvincular path se existe
    }

    /* mapeia o socket para o socket_path */
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind error");
        exit(-1);
    }

    /* configura para aguardar conexões */
    if (listen(fd, 5) == -1)
    {
        perror("listen error");
        exit(-1);
    }

    while (1)
    {
        /* aguarda conexões dos clientes */
        if ((cl = accept(fd, NULL, NULL)) == -1)
        {
            perror("accept error");
            continue;
        }

        /* lê dados envidos pelos clientes */
        while ((rc = read(cl, buf, sizeof(buf))) > 0)
        {
            char *traducao;
            if (fork() == 0)
            {
                start_system();
            }

            printf("Traducao: %s\n", traducao);
            printf("read %u bytes: %.*s\n", rc, rc, traducao);
        }

        /* trata erros */
        if (rc == -1)
        {
            perror("read");
            exit(-1);
        }
        else if (rc == 0)
        {
            printf("EOF\n");
            close(cl);
        }
    }

    return 0;
}
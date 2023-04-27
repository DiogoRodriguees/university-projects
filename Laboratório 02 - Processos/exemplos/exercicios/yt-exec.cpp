#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int variavelGlobal = 0;

int main() {
    int variavelFuncao = 20 ;   

    std:: string identidade;

    // fork() cria um processo filho baseado no processo pai
    pid_t pid = fork();

    if(pid == 0){
        // estou dentro do filho
        identidade = "Processo filho";
        variavelGlobal++;
        variavelFuncao++;
    }else if(pid < 0) {
        // FALHA
        // não foi possivel criar o filho
        std::cerr << "falha ao criar";
        exit(1);
    }else {
        // estou dentro do pai
        identidade = "Processo pai";
    }
    std::cout << identidade;

    std::cout  << "variavel global"  << variavelGlobal;

    std::cout << "variavel de funcao" <<    std::endl;

    return 0;
}
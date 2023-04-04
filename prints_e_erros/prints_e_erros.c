#include "prints_e_erros.h"
#include <string.h>

void imprime_erro_padrao(void){
    printf("Erro\n");
    exit(1);
}

void imprime_erro_desalocar(void){
    printf("Erro ao desalocar\n");
    exit(1);
}

void imprime_erro_alocacao(void){
    printf("Erro de alocação\n");
    exit(1);
}

void imprime_erro_arquivo(void){
    printf("Falha no processamento do arquivo.\n");
    exit(1);
}
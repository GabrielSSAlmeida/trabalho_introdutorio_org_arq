#include "prints_e_erros.h"
#include <string.h>

void ErroPadrao(void){
    printf("Erro\n");
    exit(1);
}

void ErroDesalocar(void){
    printf("Erro ao desalocar\n");
    exit(1);
}

void ErroAlocacao(void){
    printf("Erro de alocação\n");
    exit(1);
}

void ErroArquivo(void){
    printf("Falha no processamento do arquivo.\n");
    exit(1);
}
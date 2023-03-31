#include "prints_e_erros.h"
#include <string.h>

void imprime_erro_padrao(void){
    printf("Erro\n");
}

void imprime_erro_alocacao(){
    printf("Erro de alocação\n");
    exit(1);
}
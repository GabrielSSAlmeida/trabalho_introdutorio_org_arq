#include "cabecalhoB*.h" 
#include "../prints_e_erros/prints_e_erros.h"

//Aloca e inicializa a struct cabecalhoIndice 
CABECALHO_B *CabecalhoBCriar(void){
    CABECALHO_B *cabecalho = (CABECALHO_B*) malloc(sizeof(CABECALHO_B));

    //inicializa cabecalho
    if(cabecalho != NULL){
        cabecalho->status = '0';
        cabecalho->noRaiz = -1;
        cabecalho->proxRRN = 0;
        cabecalho->nroNiveis = 0;
        cabecalho->nroChaves = 0;

        for (int i = 0; i < 60; i++){
            cabecalho->lixo[i] = '$';
        }
    }
    else{
        ErroAlocacao();
    }
    return cabecalho;
}

//Escreve o cabecalho no arquivo binário
void CabecalhoBEscreve(FILE *arqBin, CABECALHO_B *cabecalho){
    fwrite(&(cabecalho->status), sizeof(char), 1, arqBin);
    fwrite(&(cabecalho->noRaiz), sizeof(int), 1, arqBin);
    fwrite(&(cabecalho->proxRRN), sizeof(int), 1, arqBin);
    fwrite(&(cabecalho->nroNiveis), sizeof(int), 1, arqBin);
    fwrite(&(cabecalho->nroChaves), sizeof(int), 1, arqBin);
    fwrite(&(cabecalho->lixo), sizeof(char), 59, arqBin);
}

//Função para desalocar a struct cabecalho
void CabecalhoBDesalocar(CABECALHO_B *cabecalho){
    if(cabecalho != NULL){
        free(cabecalho);
    }
    else{
        ErroDesalocar();
    }
}

bool CabecalhoBVerificaStatus(CABECALHO_B *cabecalho){
    if((cabecalho->status) == '0'){
        ErroArquivo();
        return false;
    }
    return true;
}

bool CabecalhoBReescreve(FILE *arqInd, CABECALHO_B *cabecalho){
    fseek(arqInd, 0, SEEK_SET); 
    EscreveCabecalhoIndice(arqInd, cabecalho);
}
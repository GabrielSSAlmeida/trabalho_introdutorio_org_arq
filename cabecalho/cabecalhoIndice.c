#include "cabecalhoIndice.h" 
#include "../prints_e_erros/prints_e_erros.h"


struct cabecalhoIndice{
    char status;                    //indica consistencia do arquivo (0 - inconsistente, 1 - consistente) 
    int nroRegArq;                  // armazena o numero de registros no arquivo (total, sem removidos)                
};

//Aloca e inicializa a struct cabecalhoIndice 
CABECALHO_INDICE *CabecalhoIndiceCriar(void){
    CABECALHO_INDICE *cabecalhoIndice = (CABECALHO_INDICE*) malloc(sizeof(CABECALHO_INDICE));

    //inicializa cabecalho
    if(cabecalhoIndice != NULL){
        cabecalhoIndice->status = '0';
        cabecalhoIndice->nroRegArq = 0;
    }
    else{
        ErroAlocacao();
    }
    return cabecalhoIndice;
}

//Escreve o cabecalho no arquivo binário
void EscreveCabecalhoIndice(FILE *arqBin, CABECALHO_INDICE *cabecalho){
    fwrite(&(cabecalho->status), sizeof(char), 1, arqBin);
    fwrite(&(cabecalho->nroRegArq), sizeof(int), 1, arqBin);
}

//Função para desalocar a struct cabecalho
void DesalocaCabecalhoIndice(CABECALHO_INDICE *cabecalho){
    if(cabecalho != NULL){
        free(cabecalho);
    }
    else{
        ErroDesalocar();
    }
}


void LeCabecalhoDoArqIndice(CABECALHO_INDICE *cabecalho, FILE *arq){
    fread(&(cabecalho->status), sizeof(char), 1, arq);
    fread(&(cabecalho->nroRegArq), sizeof(int), 1, arq);
}

bool VerificaStatusIndice(CABECALHO_INDICE *cabecalho){
    if((cabecalho->status) == '0'){
        ErroArquivo();
        return false;
    }
    return true;
}



//Funçoes de atualizar
//Atualiza o campo Status
void AtualizaStatusIndice(CABECALHO_INDICE *cabecalho, char status){
    if(cabecalho != NULL)
        (cabecalho->status) = status;
}
//Atualiza o campo NroRegArq
void AtualizaNroRegArqIndice(CABECALHO_INDICE *cabecalho, int nroRegArq){
    if(cabecalho != NULL)
        (cabecalho->nroRegArq) = nroRegArq;   
}

//Funções GET
//Retorna o campo status ou \0 se der errado
char GetStatusIndice(CABECALHO_INDICE *cabecalho){
    if(cabecalho != NULL)
        return (cabecalho->status);
    return '\0';
}
//Retorna o campo NroRegArq ou -1 se der errado
int GetNroRegArqIndice(CABECALHO_INDICE *cabecalho){
    if(cabecalho != NULL)
        return (cabecalho->nroRegArq);
    return -1;
}
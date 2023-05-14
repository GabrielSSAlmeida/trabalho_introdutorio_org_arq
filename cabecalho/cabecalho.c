#include "cabecalho.h" 
#include "../prints_e_erros/prints_e_erros.h"


struct cabecalho{
    char status;                    //indica consistencia do arquivo (0 - inconsistente, 1 - consistente) 
    long int proxByteOffset;        //indica o valor do proximo ByteOffset 
    int nroRegArq;                  // armazena o numero de registros no arquivo (total, inclusive removidos)                
    int nroRegRem;                  // armazena o numero de registros removidos
};

//Aloca e inicializa a struct cabecalho 
CABECALHO *CabecalhoCriar(void){
    CABECALHO *cabecalho = (CABECALHO*) malloc(sizeof(CABECALHO));

    //sizeof(CABECALHO) não funciona corretamente, então somamos os bytes
    long int somaBytes = sizeof(cabecalho->status) + sizeof(cabecalho->proxByteOffset) 
    + sizeof(cabecalho->nroRegArq) + sizeof(cabecalho->nroRegRem); 

    //inicializa cabecalho
    if(cabecalho != NULL){
        cabecalho->status = '0';
        cabecalho->proxByteOffset = somaBytes;
        cabecalho->nroRegArq = 0;
        cabecalho->nroRegRem = 0;

    }
    else{
        ErroAlocacao();
    }
    return cabecalho;
}

//Escreve o cabecalho no arquivo binário
void EscreveCabecalho(FILE *arqBin, CABECALHO *cabecalho){
    fwrite(&(cabecalho->status), sizeof(char), 1, arqBin);
    fwrite(&(cabecalho->proxByteOffset), sizeof(long int), 1, arqBin);
    fwrite(&(cabecalho->nroRegArq), sizeof(int), 1, arqBin);
    fwrite(&(cabecalho->nroRegRem), sizeof(int), 1, arqBin);
}

//Função para desalocar a struct cabecalho
void DesalocaCabecalho(CABECALHO *cabecalho){
    if(cabecalho != NULL){
        free(cabecalho);
    }
    else{
        ErroDesalocar();
    }
}


void LeCabecalhoDoArqBinario(CABECALHO *cabecalho, FILE *arq){
    fread(&(cabecalho->status), sizeof(char), 1, arq); 
    fread(&(cabecalho->proxByteOffset), sizeof(long int), 1, arq); 
    fread(&(cabecalho->nroRegArq), sizeof(int), 1, arq); 
    fread(&(cabecalho->nroRegRem), sizeof(int), 1, arq); 
}

bool VerificaStatus(CABECALHO *cabecalho){
    if((cabecalho->status) == '0'){
        ErroArquivo();
        return false;
    }
    return true;
}

//Após todas as alterações, volta ao início do arquivo para reescrever o cabeçalho
bool ArqBinReescreveCabecalho(FILE *arqBin, CABECALHO *cabecalho){
    fseek(arqBin, 0, SEEK_SET); 
    EscreveCabecalho(arqBin, cabecalho);
}


//Funçoes de atualizar
//Atualiza o campo Status
void AtualizaStatus(CABECALHO *cabecalho, char status){
    if(cabecalho != NULL)
        (cabecalho->status) = status;
}
//Atualiza o campo NroRegArq
void AtualizaNroRegArq(CABECALHO *cabecalho, int nroRegArq){
    if(cabecalho != NULL)
        (cabecalho->nroRegArq) = nroRegArq;   
}
//Atualiza o campo NroRegRem
void AtualizaNroRegRem(CABECALHO *cabecalho, int nroRegRem){
    if(cabecalho != NULL)
        (cabecalho->nroRegRem) = nroRegRem;
}
//Atualiza o campo proxByteOffset
void AtualizaByteOffset(CABECALHO *cabecalho, int byteOffset){
    if(cabecalho != NULL)
        (cabecalho->proxByteOffset) = byteOffset;
}

//Funções GET
//Retorna o campo status ou \0 se der errado
char GetStatus(CABECALHO *cabecalho){
    if(cabecalho != NULL)
        return (cabecalho->status);
    return '\0';
}
//Retorna o campo NroRegArq ou -1 se der errado
int GetNroRegArq(CABECALHO *cabecalho){
    if(cabecalho != NULL)
        return (cabecalho->nroRegArq);
    return -1;
}
//Retorna o campo NroRegRem ou -1 se der errado
int GetNroRegRem(CABECALHO *cabecalho){
    if(cabecalho != NULL)
        return (cabecalho->nroRegRem);
    return -1;
}
//Retorna o campo proxByteOffset ou -1 se der errado
int GetByteOffset(CABECALHO *cabecalho){
    if(cabecalho != NULL)
        return (cabecalho->proxByteOffset);
    return -1;
}
#ifndef CABECALHO_H
    #define CABECALHO_H
    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>

    typedef struct cabecalho CABECALHO;

    //Aloca e inicializa a struct cabecalho 
    CABECALHO *CabecalhoCriar(void);
    //Escreve o cabecalho no arquivo binário
    void EscreveCabecalho(FILE *arqBin, CABECALHO *cabecalho);
    //Função para desalocar a struct cabecalho
    void DesalocaCabecalho(CABECALHO *cabecalho);
    //Retorna ao início do arquivo e reescreve o cabeçalho
    bool ArqBinReescreveCabecalho(FILE *arqBin, CABECALHO *cabecalho);

    //Le cabecalho do binario e salva no parametro 'cabecalho'
    void LeCabecalhoDoArqBinario(CABECALHO *cabecalho, FILE *arq);

    bool VerificaStatus(CABECALHO *cabecalho);


    //Funções de atualizar campos do cabecalho
    void AtualizaStatus(CABECALHO *cabecalho, char status);
    void AtualizaNroRegArq(CABECALHO *cabecalho, int nroRegArq);
    void AtualizaNroRegRem(CABECALHO *cabecalho, int nroRegRem);
    void AtualizaByteOffset(CABECALHO *cabecalho, int byteOffset);

    //Funções get campos do cabecalho
    char GetStatus(CABECALHO *cabecalho);
    int GetNroRegArq(CABECALHO *cabecalho);
    int GetNroRegRem(CABECALHO *cabecalho);
    int GetByteOffset(CABECALHO *cabecalho);


#endif
#ifndef CABECALHOINDICE_H
    #define CABECALHOINDICE_H
    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>

    typedef struct cabecalhoIndice CABECALHO_INDICE;

    //Aloca e inicializa a struct cabecalho 
    CABECALHO_INDICE *CabecalhoIndiceCriar(void);
    //Escreve o cabecalho no arquivo binário
    void EscreveCabecalhoIndice(FILE *arqBin, CABECALHO_INDICE *cabecalho);
    //Função para desalocar a struct cabecalho
    void DesalocaCabecalhoIndice(CABECALHO_INDICE *cabecalho);
    //Le cabecalho do binario e salva no parametro 'cabecalho'
    void LeCabecalhoDoArqIndice(CABECALHO_INDICE *cabecalho, FILE *arq);
    //Verifica se o status está consistente
    bool VerificaStatusIndice(CABECALHO_INDICE *cabecalho);
    //Retorna ao início do arquivo para reescrever cabeçalho
    bool ArqIndReescreveCabecalho(FILE *arqInd, CABECALHO_INDICE *cabecalho);

    //Funções de atualizar campos do cabecalho
    void AtualizaStatusIndice(CABECALHO_INDICE *cabecalho, char status);
    void AtualizaNroRegArqIndice(CABECALHO_INDICE *cabecalho, int nroRegArq);

    //Funções get campos do cabecalho
    char GetStatusIndice(CABECALHO_INDICE *cabecalho);
    int GetNroRegArqIndice(CABECALHO_INDICE *cabecalho);


#endif
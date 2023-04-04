#ifndef REGISTRO_H
    #define REGISTRO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    typedef struct cabecalho CABECALHO;
    typedef struct dados DADOS;

    //Funções para o cabecalho
    CABECALHO *CabecalhoCriar(void);
    void DesalocaCabecalho(CABECALHO *cabecalho);
    void DesalocaRegistro(DADOS *registro);
    
    //Funções para a funcionalidade 1
    DADOS *LerRegCsv(FILE *arquivoCSV, int *flagFuncionou);
    void EscreverRegBin(FILE *arquivoBIN, DADOS *registro, CABECALHO *cabecalho);
    void EscreveCabecalho(FILE *arqBin, CABECALHO *cabecalho, char status);

    //Funções para a funcionalidade 2
    void ImprimirBinario(FILE *arqBin);


#endif
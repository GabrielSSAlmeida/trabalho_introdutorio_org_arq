#ifndef REGISTRO_H
    #define REGISTRO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../cabecalho/cabecalho.h"

    typedef struct dados DADOS;

    int LerCamposFixosRegBinario(FILE *arqBin, DADOS *registro);
    void DesalocaRegistro(DADOS *registro);
    
    //Funções para a funcionalidade 1
    DADOS *LerRegistroCsv(FILE *arquivoCSV, int *flagFuncionou);
    void EscreverRegistroBin(FILE *arquivoBIN, DADOS *registro, CABECALHO *cabecalho);
   

    //Função para a funcionalidade 2
    bool ImprimirBinario(FILE *arqBin);
    void ImprimeRegistroBinario(FILE *arqBin, DADOS *registro);


#endif
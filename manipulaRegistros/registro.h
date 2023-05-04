#ifndef REGISTRO_H
    #define REGISTRO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../cabecalho/cabecalho.h"

    typedef struct dados DADOS;

    DADOS *RegistroCriar(void);
    int LerRegBinario(FILE *arqBin, DADOS *registro, int *offsetlido);
    void DesalocaRegistro(DADOS *registro);
    void DesalocaCamposVariaveis(DADOS *registro);
    
    //Funções para a funcionalidade 1
    DADOS *LerRegistroCsv(FILE *arquivoCSV, int *flagFuncionou);
    void EscreverRegistroBin(FILE *arquivoBIN, DADOS *registro, CABECALHO *cabecalho);
   

    //Função para a funcionalidade 2
    bool ImprimirBinario(FILE *arqBin);
    void ImprimeRegistroBinario(FILE *arqBin, DADOS *registro);

    //Função Get
    char GetRegistroRemovido(DADOS *registro);
    int GetRegistroIdCrime(DADOS *registro);
    int GetRegistroNroArtigo(DADOS *registro);
#endif
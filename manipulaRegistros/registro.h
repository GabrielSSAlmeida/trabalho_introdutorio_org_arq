#ifndef REGISTRO_H
    #define REGISTRO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../cabecalho/cabecalho.h"
    #include "../arquivos/arquivos.h"
    //#include "../manipulaIndices/string/indiceString.h"

    typedef struct dados DADOS;

    DADOS *RegistroCriar(void);
    int LerRegBinario(FILE *arqBin, DADOS *registro, int *offsetlido);
    void DesalocaRegistro(DADOS *registro);
    void DesalocaVetorRegistro(DADOS **registro, int tamanho);
    void DesalocaCamposVariaveis(DADOS *registro);
    
    //Funções para a funcionalidade 1
    DADOS *LerRegistroCsv(FILE *arquivoCSV, int *flagFuncionou);
    void EscreverRegistroBin(FILE *arquivoBIN, DADOS *registro, CABECALHO *cabecalho);
   

    //Função para a funcionalidade 2
    bool ImprimirBinario(FILE *arqBin);
    void ImprimeRegistroBinario(DADOS *registro);

    //Funcionalidade 4
    DADOS *LeRegistroPorByteOffset(FILE *arqBin, long int byteOffset);

    //Funcionalidade 6
    void RecebeRegistro(DADOS *registro);
    bool InsereRegistro(DADOS *registro, char *arqEntrada, char *arqIndice, char *campo, char *dado);

    //Funções Get do registro de Dados
    char GetRegistroRemovido(DADOS *registro);
    int GetRegistroIdCrime(DADOS *registro);
    int GetRegistroNroArtigo(DADOS *registro);
    char *GetRegistroDataCrime(DADOS *registro);
    char *GetRegistroMarcaCelular(DADOS *registro);
    char *GetRegistroLugarCrime(DADOS *registro);
    char *GetRegistroDescricaoCrime(DADOS *registro);
#endif
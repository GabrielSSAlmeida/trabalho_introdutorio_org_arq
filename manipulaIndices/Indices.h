#ifndef INDICES_H
    #define INDICES_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../cabecalho/cabecalhoIndice.h"
    #include "../manipulaRegistros/registro.h"

    typedef struct dadosIndiceInt DADOS_INT;
    typedef struct dadosIndiceString DADOS_STR;

    bool CriaIndiceInteiro(char arqEntrada[], char arqSaida[], char campo[]);
    bool CriaIndiceString(char arqEntrada[], char arqSaida[], char campo[]);
    DADOS_INT *RegistroDadosIntCriar(void);
    DADOS_INT *VetorIndicesIntCriar(int tamanho);
    DADOS_STR *RegistroDadosStringCriar(void);
    DADOS_STR *VetorIndicesStringCriar(int tamanho);

    bool InsereCampoIntEmIndices(DADOS_INT *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset);
    bool InsereCampoStringEmIndices(DADOS_STR *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset);
    
    void mergeSortIndiceInt(DADOS_INT* vetor, int inicio, int fim);
    void mergeSortIndiceString(DADOS_STR* vetor, int inicio, int fim);


#endif
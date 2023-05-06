#ifndef INDICES_STRING_H
    #define INDICES_STRING_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../../cabecalho/cabecalhoIndice.h"
    #include "../../manipulaRegistros/registro.h"

    typedef struct dadosIndiceString DADOS_STR;


    bool CriaIndiceString(char arqEntrada[], char arqSaida[], char campo[]);

    DADOS_STR *RegistroDadosStringCriar(void);
    DADOS_STR *VetorIndicesStringCriar(int tamanho);


    bool InsereCampoStringEmIndices(DADOS_STR *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset);


    void mergeSortIndiceString(DADOS_STR* vetor, int inicio, int fim);

#endif
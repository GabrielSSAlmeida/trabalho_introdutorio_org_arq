#ifndef INDICE_INTEIRO_H
    #define INDICE_INTEIRO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../../cabecalho/cabecalhoIndice.h"
    #include "../../manipulaRegistros/registro.h"

    typedef struct dadosIndiceInt DADOS_INT;


    //aloca um registro de indice do tipo inteiro
    DADOS_INT *RegistroDadosIntCriar(void);
    //aloca um vetor de DADOS_INT
    DADOS_INT *VetorIndicesIntCriar(int tamanho);

    //Cria um arquivo de indice que a chave de busca é do tipo inteiro
    bool CriaIndiceInteiro(char arqEntrada[], char arqSaida[], char campo[]);

    //Preenche o vetor de indices, ou seja, insere os campos chaveBusca e byteOffset em cada pos do vetor
    bool InsereCampoIntEmIndices(DADOS_INT *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset);

    //ordena o vetor de indices do tipo inteiro
    void mergeSortIndiceInt(DADOS_INT* vetor, int inicio, int fim);



#endif
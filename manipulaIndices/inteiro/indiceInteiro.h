#ifndef INDICE_INTEIRO_H
    #define INDICE_INTEIRO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../../cabecalho/cabecalhoIndice.h"
    #include "../../manipulaRegistros/registro.h"
    #include "../../parBusca/parBusca.h"

    struct dadosIndiceInt{
        int chaveBusca;                    
        long int byteOffset;            //número relativo do registro do arquivo de dados referente à chave de busca
    };

    typedef struct dadosIndiceInt DADOS_INT;


    //aloca um registro de indice do tipo inteiro
    DADOS_INT *IndiceDadosIntCriar(void);
    //aloca um vetor de DADOS_INT
    DADOS_INT *VetorIndicesIntCriar(int tamanho);

    //Cria um arquivo de indice que a chave de busca é do tipo inteiro
    bool CriaIndiceInteiro(char arqEntrada[], char arqSaida[], char campo[]);

    //Preenche o vetor de indices, ou seja, insere os campos chaveBusca e byteOffset em cada pos do vetor
    bool InsereCampoIntEmIndices(DADOS_INT *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset);

    //ordena o vetor de indices do tipo inteiro
    void mergeSortIndiceInt(DADOS_INT* vetor, int inicio, int fim);

    //le um registro do arquivo de indice para inteiros
    DADOS_INT LerRegIndiceInt(FILE *arqIndice, DADOS_INT *registroIndice);

    //escreve no arquivo de indice para inteiros
    void EscreveArqIndiceInt(FILE* arqIndice, DADOS_INT indice, int*nroRegistros);

    //preenche o vetor com os indices do arquivo
    void PreencheVetorIndicesINT(FILE *arqIndice, DADOS_INT *vetor, int tamanho);

    //insere ordenado no vetor
    void InsereVetorIndicesOrdenadoINT(DADOS_INT *vetorIndices, DADOS_INT *registroIndice, int tamanho);

    //Copia os dados de acordo com o tipoCampo pedido
    void CopiaChaveEByteOffsetINT(DADOS *registro, DADOS_INT *registroIndice, int byteoffset, int tipoCampo);   


    long int* BuscaBinariaIndiceInt(char *nomeArqIndice, int valorBuscado, long int *vetorByteOffset);

#endif
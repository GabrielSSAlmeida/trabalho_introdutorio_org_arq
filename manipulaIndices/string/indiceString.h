#ifndef INDICES_STRING_H
    #define INDICES_STRING_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../../cabecalho/cabecalhoIndice.h"
    #include "../../manipulaRegistros/registro.h"


    struct dadosIndiceString{
        char chaveBusca[12];                    
        long int byteOffset;            //número relativo do registro do arquivo de dados referente à chave de busca
    };

    typedef struct dadosIndiceString DADOS_STR;


    bool CriaIndiceString(char arqEntrada[], char arqSaida[], char campo[]);

    DADOS_STR *IndiceDadosStringCriar(void);
    DADOS_STR *VetorIndicesStringCriar(int tamanho);


    bool InsereCampoStringEmIndices(DADOS_STR *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset);

    DADOS_STR LerRegIndiceString(FILE *arqIndice, DADOS_STR *registroIndice);

    void EscreveArqIndiceString(FILE* arqIndice, DADOS_STR indice, int* nroRegistro);

    void mergeSortIndiceString(DADOS_STR* vetor, int inicio, int fim);


    //FUnção que dado um arquivo de indice e uma chave de busca, retorna todos os bytesOffsets que satisfazem
    long int* BuscaBinariaIndiceString(char *nomeArqIndice, char *valorBuscado, long int *vetorByteOffset);
    
    //preenche o vetor com os indices do arquivo
    void PreencheVetorIndicesSTR(FILE *arqIndice, DADOS_STR *vetor, int tamanho);

    //Remove fisicamente um registro do arquivo de indice
    void RemoveArquivoIndiceString(char *nomeArqIndice, long int byteOffset);
    
    void CopiaChaveEByteOffsetSTR(DADOS *registro, DADOS_STR *registroIndice, int byteoffset, int tipoCampo);

    void InsereVetorIndicesOrdenadoSTR(DADOS_STR *vetorIndices, DADOS_STR *registroIndice, int tamanho);


#endif
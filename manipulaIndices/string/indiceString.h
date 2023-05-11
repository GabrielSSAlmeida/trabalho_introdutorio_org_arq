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

    long int* BuscaBinariaIndiceString(char *nomeArqIndice, char *valorBuscado, long int *vetorByteOffset);

#endif
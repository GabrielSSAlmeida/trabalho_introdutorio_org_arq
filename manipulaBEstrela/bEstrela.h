#ifndef B_ESTRELA_H
    #define B_ESTRELA_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "../manipulaRegistros/registro.h"
    #include "../parBusca/parBusca.h"

    #define MAXCHAVES 4
    #define TAM_PAGE 76
    #define NIL -1

    typedef enum {PROMOTION, NO_PROMOTION, ERROR} ValoresRetorno;
    typedef enum { ACHOU, NAO_ACHOU } buscaRetorno;
    
    struct chave {
        int C;
        long int Pr;
    };
    typedef struct chave CHAVE;

    struct btPage
    {
        int nivel;
        int n; //Numero de chaves presentes no no
        int P[MAXCHAVES+1]; //RRN dos nos filhos
        CHAVE chaves[MAXCHAVES];
    };
    typedef struct btPage BTPAGE;
    
    DADOS** BuscaIndiceArvore(char *nomeArqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, int *qtdEncontrados);
#endif
#ifndef B_ESTRELA_H
    #define B_ESTRELA_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define MAXCHAVES 4
    #define TAM_PAGE 76

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
    

#endif
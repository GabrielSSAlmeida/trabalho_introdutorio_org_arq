#ifndef CABECALHO_B_ESTRELA_H
    #define CABECALHO_B_ESTRELA_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    #define TAM_CABECALHO 76

    struct cabecalhoBestrela
    {
        char status;
        int noRaiz;
        int proxRRN;
        int nroNiveis;
        int nroChaves;
        char lixo[59];
    };
    
    typedef struct cabecalhoBestrela CABECALHO_B;
    
    CABECALHO_B *CabecalhoBCriar(void);

    void CabecalhoBEscreve(FILE *arqBin, CABECALHO_B *cabecalho);

    void CabecalhoBDesalocar(CABECALHO_B *cabecalho);

    bool CabecalhoBVerificaStatus(CABECALHO_B *cabecalho);

    bool CabecalhoBReescreve(FILE *arqInd, CABECALHO_B *cabecalho);
    
#endif
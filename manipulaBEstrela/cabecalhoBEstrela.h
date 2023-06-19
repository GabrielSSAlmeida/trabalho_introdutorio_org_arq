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
    
    //Cria Cabecalho do arquivo de indice da Arvore B*
    CABECALHO_B *CabecalhoBCriar(void);

    //Leitura do Cabecalho do arquivo de indice da Arvore B*
    void CabecalhoBLer(CABECALHO_B *cabecalho, FILE *arq);

    //Escrita do Cabecalho do arquivo de indice da Arvore B*
    void CabecalhoBEscreve(FILE *arqBin, CABECALHO_B *cabecalho);

    //Desaloca Cabecalho do arquivo de indice da Arvore B*
    void CabecalhoBDesalocar(CABECALHO_B *cabecalho);

    //Verifica consistência do arquivo de indice da Arvore B*
    bool CabecalhoBVerificaStatus(CABECALHO_B *cabecalho);

    //Reescreve consistência do arquivo de indice da Arvore B*
    bool CabecalhoBReescreve(FILE *arqInd, CABECALHO_B *cabecalho);
    
#endif
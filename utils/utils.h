#ifndef UTILS_H
    #define UTILS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../manipulaIndices/Indices.h"

    //Funções fornecidas
    void readline(char *string);
    void binarioNaTela(char *nomeArquivoBinario);
    void scan_quote_string(char *str);

    //Funções criadas
    void RecebeEntradaFunc1(char str1[], char str2[]);
    void RecebeEntradaFunc2(char str1[]);
    void RecebeEntradaFunc3(char arqEntrada[], char campo[], char dado[], char arqIndice[]);

    void ImprimeCampoVariavel(char *string);
    void ImprimeMarcaCelular(char marcaCelular[]);
    void ImprimeDataCrime(char dataCrime[]);
    char *LerCampoVariavel(FILE *arqBin, int *offsetlido);


    char *LerStringVariavel(FILE *arq);
    void LerStringFixa(FILE *arq, char *vetor, int tamanho);
    int LerInteiroVariavel(FILE *arq);

    int VerificaDado(char dado[]);
    int TipoChaveBusca(char campo[]);

    void strncpySem0(char *string1, char *string2, int n);

    //Ler e ignorar a primeira linha do arquivo
    void IgnoraLinha(FILE *arq);
#endif
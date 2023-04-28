#ifndef UTILS_H
    #define UTILS_H
    #include <stdio.h>
    #include <stdlib.h>

    //Funções fornecidas
    void readline(char *string);
    void binarioNaTela(char *nomeArquivoBinario);
    void scan_quote_string(char *str);

    //Funções criadas
    void RecebeEntradaFunc1(char str1[], char str2[]);
    void RecebeEntradaFunc2(char str1[]);
    void ImprimeCampoVariavel(char *string);
    void ImprimeMarcaCelular(char marcaCelular[]);
    void ImprimeDataCrime(char dataCrime[]);
    void LerCampoVariavel(FILE *arqBin, char **string);


    char *LerStringVariavel(FILE *arq);
    void LerStringFixa(FILE *arq, char *vetor, int tamanho);
    int LerInteiroVariavel(FILE *arq);

    //Ler e ignorar a primeira linha do arquivo
    void IgnoraLinha(FILE *arq);
#endif
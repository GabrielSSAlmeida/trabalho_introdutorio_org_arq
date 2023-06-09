#ifndef UTILS_H
    #define UTILS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../manipulaIndices/inteiro/indiceInteiro.h"
    #include "../manipulaIndices/string/indiceString.h"


    typedef enum TipoDados { inteiro, string } TipoDados;

    typedef enum TipoCampo{
        erro = -1,
        idCrime,
        numeroArtigo,
        dataCrime,
        marcaCelular,
        lugarCrime,
        descricaoCrime
    } TipoCampo;

    //Funções fornecidas
    void readline(char *string);
    void binarioNaTela(char *nomeArquivoBinario);
    void scan_quote_string(char *str);

    //Funções criadas
    void RecebeEntradaFunc1(char str1[], char str2[]);
    void RecebeEntradaFunc2(char str1[]);
    void RecebeEntradaFunc3e8(char arqEntrada[], char arqIndice[], char campo[], char dado[]);
    void RecebeEntradaFuncs4a7(char arqEntrada[], char arqIndice[], char campo[], char dado[], int *n);

    void ImprimeCampoVariavel(char *string);
    void ImprimeMarcaCelular(char marcaCelular[]);
    void ImprimeDataCrime(char dataCrime[]);
    char *LerCampoVariavel(FILE *arqBin, long int *offsetlido);


    char *LerStringVariavel(FILE *arq);
    void LerStringFixa(FILE *arq, char *vetor, int tamanho);
    int LerInteiroVariavel(FILE *arq);

    TipoDados VerificaDado(char dado[]);
    TipoCampo TipoChaveBusca(char campo[]);

    void strncpySem0(char *string1, char *string2, int n);
    void strcpySem0Variavel(char *string1, char *string2);
    void strcpySem0Fixa(char *string1, char *string2);
    int strcmpAtePipe(char *string1, char *string2);
    void strcpySem0(char *string1, char *string2);
    void strncpySemBarra0(char *string1, char *string2, int n);


    void avancaCursosEmCampoVariavel(FILE *arqBin);

    //Ler e ignorar a primeira linha do arquivo
    void IgnoraLinha(FILE *arq);
#endif
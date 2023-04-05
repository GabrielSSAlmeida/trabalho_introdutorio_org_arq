#ifndef AUXILIAR_H
    #define AUXILIAR_H
    #include <stdio.h>
    #include <stdlib.h>

    //Funções fornecidas
    void readline(char *string);
    void binarioNaTela(char *nomeArquivoBinario);
    void scan_quote_string(char *str);

    //Funções criadas
    void RecebeEntradaFunc1(char str1[], char str2[]);
    void RecebeEntradaFunc2(char str1[]);
    void ImprimeCampoVariavel(FILE *arqBin, char *caracter);
    void LerCampoVariavel(FILE *arqBin, char *caracter);

#endif
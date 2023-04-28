#ifndef ARQUIVOS_H
    #define ARQUIVOS_H
    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include "../manipulaRegistros/registro.h"
    #include "../cabecalho/cabecalho.h"

    /*
        Abre arquivo e retorna se foi bem sucedido.
        Função usa parâmetros variados para fechar arquivos já abertos.
    */ 
    bool AbreArquivo(FILE **arq, char nomeArquivo[], char modo[], ...);

    //Le registros do csv e escreve no binario
    void CsvParaBin(FILE *arqCsv, FILE *arqBin, CABECALHO *cabecalho_saida);

#endif
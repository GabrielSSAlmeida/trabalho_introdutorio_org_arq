#ifndef FUNCIONALIDADES_H
    #define FUNCIONALIDADES_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "../utils/utils.h"
    #include "../parBusca/parBusca.h"
    #include "../manipulaRegistros/registro.h"
    #include "../prints_e_erros/prints_e_erros.h"
    #include "../arquivos/arquivos.h"
    #include "../cabecalho/cabecalho.h"
    #include "../manipulaIndices/inteiro/indiceInteiro.h"
    #include "../manipulaIndices/string/indiceString.h"
    #include "../manipulaBEstrela/bEstrela.h"
    

    bool funcionalidade1(char *nomeArqEntrada, char *nomeArqSaida);
    bool funcionalidade2(char *nomeArqEntrada);
    bool funcionalidade3(char *nomeArqEntrada, char *nomeArqSaida, char *campoIndexado, char *dado);
    bool funcionalidade4(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int qtdBuscas);
    bool funcionalidade5(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int qtdBuscas);
    bool funcionalidade6(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int n);
    bool funcionalidade7(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int n);
    bool funcionalidade8(char *nomeArqBinario, char *nomeArqIndice, char *dado);
    bool funcionalidade9(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int qtdBuscas);
    bool funcionalidade10(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int n);

#endif
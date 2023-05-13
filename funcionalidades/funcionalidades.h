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

    

    bool funcionalidade1();
    bool funcionalidade2();
    bool funcionalidade3();
    bool funcionalidade4(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int qtdBuscas);
    bool funcionalidade5(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int qtdBuscas);
    bool funcionalidade6(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int n);
    bool funcionalidade7(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int n);


#endif
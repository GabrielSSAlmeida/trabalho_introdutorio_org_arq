#ifndef INDICES_H
    #define INDICES_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../cabecalho/cabecalhoIndice.h"

    typedef struct dadosIndiceInt DADOS_INT;
    typedef struct dadosIndiceString DADOS_STR;

    bool CriaIndiceInteiro(char arqEntrada[], char arqSaida[], char campo[]);
    bool CriaIndiceString(char arqEntrada[], char arqSaida[], char campo[]);
    DADOS_INT *RegistroDadosIntCriar(void);

#endif
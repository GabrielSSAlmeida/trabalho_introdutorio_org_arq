#ifndef PAR_BURCA_H
    #define PAR_BURCA_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../prints_e_erros/prints_e_erros.h"
    #include "../utils/utils.h"

    //Uma struct auxiliar para armazenar todos os pares de busca na funcionalidade 4
    typedef struct paresBusca PARES_BUSCA;

    PARES_BUSCA *VetorParesBuscaCriar(int tamanho);
    char *GetNomeCampo(PARES_BUSCA *vetor, int pos);
    char *GetValorCampoString(PARES_BUSCA *vetor, int pos);
    void AtualizaValorCampoInt(PARES_BUSCA *vetor, int pos, int valor);
    bool DecideOrdemBusca(PARES_BUSCA *vetor, int tamanho, char *campoIndexado);
    void DesalocaParesBusca(PARES_BUSCA *vetor);


    
#endif
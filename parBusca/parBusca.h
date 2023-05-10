#ifndef PAR_BURCA_H
    #define PAR_BURCA_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../prints_e_erros/prints_e_erros.h"
    #include "../utils/utils.h"

    //Uma struct auxiliar para armazenar todos os pares de busca na funcionalidade 4
    typedef struct paresBusca PARES_BUSCA;

    //Cria um vetor de PARES_BUSCA
    PARES_BUSCA *VetorParesBuscaCriar(int tamanho);
    //Funções GET
    char *GetNomeCampo(PARES_BUSCA *vetor, int pos);
    char *GetValorCampoString(PARES_BUSCA *vetor, int pos);
    int GetValorCampoInt(PARES_BUSCA *vetor, int pos);
    int GetTipoCampo(PARES_BUSCA *vetor, int pos);
    //Funções SET
    void AtualizaTipoCampo(PARES_BUSCA *vetor, int pos, int valor);
    void AtualizaValorCampoInt(PARES_BUSCA *vetor, int pos, int valor);
    void DesalocaParesBusca(PARES_BUSCA *vetor);

    //==================================BUSCA====================================================
    //Le da entrada padrão cada par de busca em uma busca e armazena na struct
    void RecebeParesBusca(int qtdPares, PARES_BUSCA *paresBusca);

    //Retorna true se existe um par para realizar a busca binaria na primeira posição
    bool DecideOrdemBusca(PARES_BUSCA *vetor, int tamanho, char *campoIndexado);
    //Escolhe se vai fazer uma busca binaria no arquivo de indices ou uma busca sequencial no binario
    bool MetodoDeBusca(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, char *campoIndexado);

    /*Realiza uma busca binaria no arquivo de indices e depois verifica as outras buscas no arquivo binario
    imprime as buscas que passarem nos testes*/
    bool BuscaBinariaIndices(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares);

    /*Realiza uma busca sequencial direto no arquivo binario
    imprime as buscas que passarem nos testes*/
    bool BuscaSequencialBinario(char *nomeArqBin, PARES_BUSCA *paresBusca, int qtdPares);
    //=============================================================================================

    //==================================REMOCAO====================================================
    //Escolhe se vai remover a partir de uma busca binaria no arquivo de indices ou uma busca sequencial no binario
    bool MetodoDeRemocao(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, char *campoIndexado);

    bool RemocaoBinariaIndicesBinario(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares);

    bool RemocaoSequencialBinario(char *nomeArqBin, PARES_BUSCA *paresBusca, int qtdPares);
    //=============================================================================================
#endif
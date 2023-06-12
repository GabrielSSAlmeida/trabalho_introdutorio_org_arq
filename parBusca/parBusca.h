#ifndef PAR_BUSCA_H
    #define PAR_BUSCA_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../manipulaRegistros/registro.h"
    #include "../prints_e_erros/prints_e_erros.h"
    #include "../utils/utils.h"
    

    //Uma struct auxiliar para armazenar todos os pares de busca na funcionalidade 4
    struct paresBusca{
        char nomeCampo[15];
        //So sera usado um valorCampo de cada vez.
        char valorCampoString[200];
        int valorCampoInt;
        TipoCampo tipoCampo; 
        /*  0 = idCampo
            1 = numeroArtigo
            2 = dataCrime
            3 = marcaCelular
            4 = lugarCrime
            5 = descricaoCrime
        */
    };


    //Uma struct auxiliar para armazenar todos os pares de busca na funcionalidade 4
    typedef struct paresBusca PARES_BUSCA;


    typedef DADOS** (*BuscaIndice)(char*, char*, PARES_BUSCA*, int, int*);

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
    bool RecebeParesBusca(int qtdPares, PARES_BUSCA *paresBusca);

    //Retorna true se existe um par para realizar a busca binaria na primeira posição
    bool DecideOrdemBusca(PARES_BUSCA *vetor, int tamanho, char *campoIndexado);
    //Escolhe se vai fazer uma busca binaria no arquivo de indices ou uma busca sequencial no binario
    DADOS** MetodoDeBusca(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, 
    int qtdPares, char *campoIndexado, int *qtdEncontrados,BuscaIndice buscaIndice);

    bool MetodoDeAtualizacao(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, 
    PARES_BUSCA *paresAtualizacoes, int qtdPares, int qtdAtualizacoes, char *campoIndexado, char *dado);

    /*Realiza uma busca binaria no arquivo de indices e depois verifica as outras buscas no arquivo binario
    imprime as buscas que passarem nos testes*/
    bool BuscaBinariaIndices(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares);

    /*Realiza uma busca sequencial direto no arquivo binario
    imprime as buscas que passarem nos testes*/
    DADOS** BuscaSequencialBinario(char *nomeArqBin, PARES_BUSCA *paresBusca, int qtdPares, int *qtdEncontrados);

    void VerificaTodosCriteriosBusca(int inicio, int qtdPares, DADOS *registro, int *passou, PARES_BUSCA *paresBusca);
    //=============================================================================================

    //==================================REMOCAO====================================================
    //Escolhe se vai remover a partir de uma busca binaria no arquivo de indices ou uma busca sequencial no binario
    bool MetodoDeRemocao(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, char *campoIndexado);

    //Realiza uma busca binaria no arquivo de indices e depois verifica as outras buscas no arquivo binario
    //imprime as buscas que passarem nos testes
    bool RemocaoBinariaIndicesBinario(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares);

    bool RemocaoSequencialBinario(char *nomeArqBin, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, int tipoCampoIndice);

    //=============================================================================================

    //Func7

    bool AtualizacaoBinariaIndices(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, 
    PARES_BUSCA *paresBusca, PARES_BUSCA *atualizacoes, int qtdPares, int qtdAtualizacoes);

    bool AtualizacaoSequencialBinario(char *nomeArqBin, char *nomeArqIndice, char *campoIndexado, char *dado, PARES_BUSCA *paresBusca, 
    PARES_BUSCA *atualizacoes, int qtdPares, int qtdAtualizacoes);

    bool AtualizaRegistroBinario(DADOS *registro, PARES_BUSCA *atualizacoes, int qtdAtualizacoes);
#endif
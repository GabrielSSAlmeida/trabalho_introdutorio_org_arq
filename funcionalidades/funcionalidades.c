#include "funcionalidades.h"


bool funcionalidade1(char *nomeArqEntrada, char *nomeArqSaida){
    FILE *arqCsv;
    FILE *arqBin;

    //Abre arquivos csv e binario
    //Se a abertura falhar para o código 
    if(!AbreArquivo(&arqCsv, nomeArqEntrada, "r", NULL)) return false;
    if(!AbreArquivo(&arqBin, nomeArqSaida, "wb+", arqCsv, NULL)) return false;

    //Cria o cabecalho que será usado no arquivo binario
    CABECALHO *cabecalho_saida = CabecalhoCriar();
    
    //Ler e ignorar a primeira linha do arquivo CSV
    IgnoraLinha(arqCsv);

    //atualiza o status do cabecalho
    AtualizaStatus(cabecalho_saida, '0');

    //escreve cabecalho no arquivo binario
    EscreveCabecalho(arqBin,cabecalho_saida);

    //fecha arquivo para salvar status
    fclose(arqBin);

    //abre o arquivo novamente
    if(!AbreArquivo(&arqBin, nomeArqSaida, "rb+", NULL)) return false;

    //ajusta o ponteiro para dps do cabecalho
    fseek(arqBin, GetByteOffset(cabecalho_saida), SEEK_SET);

    //Le registros do csv e escreve no binario
    CsvParaBin(arqCsv, arqBin, cabecalho_saida);
    
    //atualiza o status do cabecalho
    AtualizaStatus(cabecalho_saida, '1');
    
    //atualiza o cabecalho
    fseek(arqBin, 0, SEEK_SET);
    EscreveCabecalho(arqBin,cabecalho_saida);

    //fecha arquivos
    fclose(arqCsv);
    fclose(arqBin);

    DesalocaCabecalho(cabecalho_saida);

    return true;
}


bool funcionalidade2(char *nomeArqEntrada){
    FILE *arqBin;

    //abre arquivo binario para funcionalidade 2
    if(!AbreArquivo(&arqBin, nomeArqEntrada, "rb", NULL)) return false;

    //imprime os registro do arquivo binario na tela
    if(!ImprimirBinario(arqBin)){
        fclose(arqBin);
        return false;
    }

    fclose(arqBin);

    return true;
}

bool funcionalidade3(char *nomeArqEntrada, char *nomeArqSaida, char *campoIndexado, char *dado){

    //verifica se o tipo de dado é string ou int
    int dadoVerificado = VerificaDado(dado);

    switch(dadoVerificado){
        //caso int
        case 0:{
            CriaIndiceInteiro(nomeArqEntrada, nomeArqSaida, campoIndexado);
            break;
        }
        //caso string
        case 1:{
            CriaIndiceString(nomeArqEntrada, nomeArqSaida, campoIndexado);
            break;
        }
        default:
            return false;
            break;
    }

    return true;
}

bool funcionalidade4(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int qtdBuscas){
    for (int i = 0; i < qtdBuscas; i++)
    {
        int qtdPares;
        scanf("%d ", &qtdPares);

        //cria um vetor com os pares utilizados para busca
        PARES_BUSCA *paresBusca = VetorParesBuscaCriar(qtdPares);
        RecebeParesBusca(qtdPares, paresBusca); 
        
        printf("Resposta para a busca %d\n", i+1);

        //if(!MetodoDeBusca(arqEntrada, nomeArqIndice, paresBusca, qtdPares, campoIndexado, BuscaBinariaIndices)) return false;

        DesalocaParesBusca(paresBusca);
    }
    
    return true;
}

bool funcionalidade5(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int qtdBuscas){    
    for (int i = 0; i < qtdBuscas; i++)
    {
        int qtdPares;
        scanf("%d ", &qtdPares);

        PARES_BUSCA *paresBusca = VetorParesBuscaCriar(qtdPares);

        RecebeParesBusca(qtdPares, paresBusca); 

        if(!MetodoDeRemocao(arqEntrada, nomeArqIndice, paresBusca, qtdPares, campoIndexado)) return false;

        DesalocaParesBusca(paresBusca);
        
    }
    
    return true;
    
}

bool funcionalidade6(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int n){
    for(int i=0; i<n; i++){
        DADOS *registro_auxiliar = RegistroCriar();
        RecebeRegistro(registro_auxiliar);
        InsereRegistro(registro_auxiliar, arqEntrada, nomeArqIndice, campoIndexado, dado);

        DesalocaCamposVariaveis(registro_auxiliar);
        DesalocaRegistro(registro_auxiliar);
    }
}

bool funcionalidade7(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int n){
    for (int i = 0; i < n; i++)
    {
        
        int qtdPares;
        scanf("%d ", &qtdPares);

        PARES_BUSCA *paresBusca = VetorParesBuscaCriar(qtdPares);

        if(!RecebeParesBusca(qtdPares, paresBusca)) return false;
        
        int qtdAtualizacoes;
        scanf("%d ", &qtdAtualizacoes);

        PARES_BUSCA *paresAtualizacoes = VetorParesBuscaCriar(qtdAtualizacoes);

        if(!RecebeParesBusca(qtdAtualizacoes, paresAtualizacoes)){
            free(paresBusca);
            free(paresAtualizacoes);
            return false;
        }         
        
        if(!MetodoDeAtualizacao(arqEntrada, nomeArqIndice, paresBusca, paresAtualizacoes, qtdPares, qtdAtualizacoes, campoIndexado, dado)) return false;
        
        DesalocaParesBusca(paresBusca);
        DesalocaParesBusca(paresAtualizacoes);
        
    }
}


bool funcionalidade8(char *nomeArqEntrada, char *nomeArqSaida, char *campoIndexado, char *dado){

    //verifica se o tipo de dado é string ou int
    TipoDados dadoVerificado = VerificaDado(dado);

    switch(dadoVerificado){
        //caso int
        case inteiro:{
            (nomeArqEntrada, nomeArqSaida, campoIndexado);
            break;
        }
        default:
            return false;
            break;
    }

    return true;
}


bool funcionalidade9(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, int qtdBuscas){
    for (int i = 0; i < qtdBuscas; i++)
    {
        int qtdPares;
        scanf("%d ", &qtdPares);

        //cria um vetor com os pares utilizados para busca
        PARES_BUSCA *paresBusca = VetorParesBuscaCriar(qtdPares);
        RecebeParesBusca(qtdPares, paresBusca); 
        
        printf("Resposta para a busca %d\n", i+1);

        int qtdRegistros = 0;
        DADOS** registros = MetodoDeBusca(arqEntrada, nomeArqIndice, paresBusca, qtdPares, campoIndexado, &qtdRegistros, BuscaIndiceArvore);

        DesalocaParesBusca(paresBusca);

        if(registros != NULL){
            for (int j = 0; j < qtdRegistros; j++)
            {
                ImprimeRegistroBinario((registros)[j]);
            }
            
            DesalocaVetorRegistro(registros, qtdRegistros);
        }
    }
    
    return true;
}
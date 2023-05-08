#include "parBusca.h"

//Uma struct auxiliar para armazenar todos os pares de busca na funcionalidade 4
struct paresBusca{
    char nomeCampo[15];
    //So sera usado um valorCampo de cada vez.
    char valorCampoString[200];
    int valorCampoInt;
    int tipoCampo; 
    /*  0 = idCampo
        1 = numeroArtigo
        2 = dataCrime
        3 = marcaCelular
        4 = lugarCrime
        5 = descricaoCrime
    */
};

//aloca um registro de indice do tipo inteiro
PARES_BUSCA *VetorParesBuscaCriar(int tamanho){
    PARES_BUSCA *vetor = (PARES_BUSCA*) calloc(tamanho, sizeof(PARES_BUSCA));
    if(vetor != NULL){
        //inicializa os itens do vetor
        for (int i = 0; i < tamanho; i++)
        {
            vetor[i].valorCampoInt = -1;
            vetor[i].tipoCampo = -1;
        }
        
        return vetor;
    }
    else{
        ErroAlocacao();
        return NULL;
    }
}

char *GetNomeCampo(PARES_BUSCA *vetor, int pos){
    if(vetor != NULL)
        return vetor[pos].nomeCampo;
    return NULL;
}

char *GetValorCampoString(PARES_BUSCA *vetor, int pos){
    if(vetor != NULL)
        return vetor[pos].valorCampoString;
    return NULL;
}

int GetTipoCampo(PARES_BUSCA *vetor, int pos){
    if(vetor != NULL)
        return vetor[pos].tipoCampo;
    return -1;
}

void AtualizaTipoCampo(PARES_BUSCA *vetor, int pos, int valor){
    if(vetor != NULL)
        vetor[pos].tipoCampo = valor;
}


int GetValorCampoInt(PARES_BUSCA *vetor, int pos){
    if(vetor != NULL)
        return vetor[pos].valorCampoInt;
    return -1;
}

void AtualizaValorCampoInt(PARES_BUSCA *vetor, int pos, int valor){
    if(vetor != NULL)
        vetor[pos].valorCampoInt = valor;
}

//Desalocar o vetor de Pares Busca
void DesalocaParesBusca(PARES_BUSCA *vetor){
    free(vetor);
}

//Retorna true se existe for para realizar a busca binaria na primeira posição
bool DecideOrdemBusca(PARES_BUSCA *vetor, int tamanho, char *campoIndexado){

    int tipoChaveBusca = TipoChaveBusca(campoIndexado);

    for (int i = 0; i < tamanho; i++)
    {   
        //Se tiver a chave de busca do campo indexado, joga ele para o começo do vetor(pois deve ser buscado primeiro)
        if(TipoChaveBusca(vetor[i].nomeCampo) == tipoChaveBusca){
            PARES_BUSCA aux = vetor[0];
            vetor[0] = vetor[i];
            vetor[i] = aux;

            //Existe um par de busca que corresponde à chave de busca do arquivo de indices
            return true;
        }
    }

    
    return false;
    
}
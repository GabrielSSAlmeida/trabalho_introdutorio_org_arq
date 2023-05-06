#include "parBusca.h"

//Uma struct auxiliar para armazenar todos os pares de busca na funcionalidade 4
struct paresBusca{
    char nomeCampo[12];
    //So sera usado um valorCampo de cada vez.
    char valorCampoString[100];
    int valorCampoInt;
};

//aloca um registro de indice do tipo inteiro
PARES_BUSCA *VetorParesBuscaCriar(int tamanho){
    PARES_BUSCA *vetor = (PARES_BUSCA*) calloc(tamanho, sizeof(PARES_BUSCA));
    if(vetor != NULL){
        //inicializa os itens do vetor
        for (int i = 0; i < tamanho; i++)
        {
            vetor[i].valorCampoInt = -1;
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

void AtualizaValorCampoInt(PARES_BUSCA *vetor, int pos, int valor){
    if(vetor != NULL)
        vetor[pos].valorCampoInt = valor;
}

//Desalocar o vetor de Pares Busca
void DesalocaParesBusca(PARES_BUSCA *vetor){
    free(vetor);
}


bool DecideOrdemBusca(PARES_BUSCA *vetor, int tamanho, char *campoIndexado){

    int tipoChaveBusca = TipoChaveBusca(campoIndexado);

    for (int i = 0; i < tamanho; i++)
    {   
        //Se tiver a chave de busca do campo indexado, joga ele para o começo do vetor(pois deve ser buscado primeiro)
        if(TipoChaveBusca(vetor[i].nomeCampo) == tipoChaveBusca && i!=0){
            /* for (int j = i; j > 0 ; j--)
            {
                PARES_BUSCA aux = vetor[j];
                vetor[j] = vetor[j-1];
                vetor[j-1] = aux;
            } */

            PARES_BUSCA aux = vetor[0];
            vetor[0] = vetor[i];
            vetor[i] = aux;

            //Existe um par de busca que corresponde à chave de busca do arquivo de indices
            return true;
        }
    }

    
    return false;
    
}
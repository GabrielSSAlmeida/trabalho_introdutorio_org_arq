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


void BuscaSequencialBinario(char *nomeArqBin, PARES_BUSCA *paresBusca, int qtdPares){
    FILE *arqBin;
    if(!AbreArquivo(&arqBin, nomeArqBin, "r", NULL)) return ;

    CABECALHO *cabecalho_aux = CabecalhoCriar();

    //le o cabecalho do arquivo binario
    LeCabecalhoDoArqBinario(cabecalho_aux, arqBin);

    DADOS *registro_aux = RegistroCriar();

    //não usa
    int aux;
    int flag = LerRegBinario(arqBin, registro_aux, &aux);
    int i;
    int qtdRegistrosImpressos = 0;
    for(i=0; flag!=0; i++){
        int passou = 1;//Verifica se passou em todos os criterios de busca
        
        for (int j = 0; j < qtdPares; j++)
        {
            int tipoCampo = GetTipoCampo(paresBusca, j);
            int len = strlen(GetValorCampoString(paresBusca, j));

            switch (tipoCampo)
            {
                case 0:
                    if(GetRegistroIdCrime(registro_aux) != GetValorCampoInt(paresBusca, j))
                        passou = 0;
                    break;
                case 1:
                    if(GetRegistroNroArtigo(registro_aux) != GetValorCampoInt(paresBusca, j))
                        passou = 0;
                    break;
                case 2:
                    if(strncmp(GetRegistroDataCrime(registro_aux), GetValorCampoString(paresBusca, j), len) != 0 )
                        passou = 0;
                    break;
                case 3:
                    if(strncmp(GetRegistroMarcaCelular(registro_aux), GetValorCampoString(paresBusca, j), len) != 0 )
                        passou = 0;
                    break;
                case 4:
                    if(strcmpAtePipe(GetRegistroLugarCrime(registro_aux), GetValorCampoString(paresBusca, j)) != 0 )
                        passou = 0;
                    break;
                case 5:
                    if(strcmpAtePipe(GetRegistroDescricaoCrime(registro_aux), GetValorCampoString(paresBusca, j)) != 0 )
                        passou = 0;
                    break;
                default:
                    break;
            }
        }

        if(passou){
            ImprimeRegistroBinario(registro_aux);
            qtdRegistrosImpressos++;
        }
        
        DesalocaCamposVariaveis(registro_aux);

        flag = LerRegBinario(arqBin, registro_aux, &aux); 
    }


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_aux);
    DesalocaRegistro(registro_aux);

    //se nao existem registros no arquivo
    if(i==0 || qtdRegistrosImpressos == 0) ErroRegistro();
    fclose(arqBin);
}
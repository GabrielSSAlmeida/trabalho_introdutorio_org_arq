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


void RecebeParesBuscaFunc4(int qtdPares, PARES_BUSCA *paresBusca){
    for (int j = 0; j < qtdPares; j++)
    {
        char *nomeCampo = GetNomeCampo(paresBusca, j);
        scanf("%s ", nomeCampo);
        int tipoCampo = TipoChaveBusca(nomeCampo);

        AtualizaTipoCampo(paresBusca, j, tipoCampo);
        if (tipoCampo == 0 || tipoCampo == 1){ //O campo é do tipo int
            int valorCampoInt;
            scanf("%d ", &valorCampoInt);
            AtualizaValorCampoInt(paresBusca, j, valorCampoInt);

        }else if(tipoCampo >= 2){
            scan_quote_string(GetValorCampoString(paresBusca, j));

        }else{//Caso seja -1
            ErroArquivo();
        }
    }
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


bool MetodoDeBusca(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, char *campoIndexado){
    if(DecideOrdemBusca(paresBusca, qtdPares, campoIndexado)){ //O metodo é a busca binaria
        if(!BuscaBinariaBinario(arqEntrada, nomeArqIndice, paresBusca, qtdPares)) return false;
    }else{ //o metodo é a busca sequencial
        BuscaSequencialBinario(arqEntrada, paresBusca, qtdPares);
    }

    return true;
}

bool BuscaBinariaBinario(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares){
    //Vai indicar os byteOffset dos registros encontrados
    //0 indica o byteOffset que não passou em todos os casos de busca
    //-1 indica o fim do vetor
    long int *vetorByteOffset  = calloc(1, sizeof(long int));

    FILE *arqBin;
    if(!AbreArquivo(&arqBin, arqEntrada, "r", NULL)) return false;

    int tipoCampo = GetTipoCampo(paresBusca, 0);

    if (tipoCampo == 0 || tipoCampo == 1){ //O campo é do tipo int
        vetorByteOffset =  BuscaBinariaIndiceInt(nomeArqIndice, GetValorCampoInt(paresBusca, 0), vetorByteOffset);

    }else if(tipoCampo >= 2){ //O campo é do tipo string
        vetorByteOffset =  BuscaBinariaIndiceString(nomeArqIndice, GetValorCampoString(paresBusca, 0), vetorByteOffset);
    }

    int contador = 1;

    /*  A partir das buscas retornadas da busca binaria, verifica se esses campos
        satisfazem os outros criterios de busca
    */
    while (contador < qtdPares){
        int tipoCampo = GetTipoCampo(paresBusca, contador);

        for (int j = 0; vetorByteOffset[j] != -1 ; j++)
        {
            DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);

            switch (tipoCampo)
            {
                case 0:
                    if(GetRegistroIdCrime(registro_aux) != GetValorCampoInt(paresBusca, contador)){
                        vetorByteOffset[j] = 0;
                    }
                    break;
                
                case 1:
                    if(GetRegistroNroArtigo(registro_aux) != GetValorCampoInt(paresBusca, contador)){
                        vetorByteOffset[j] = 0;
                    }
                    break;
                
                case 2:
                    if(strncmp(GetRegistroDataCrime(registro_aux), 
                    GetValorCampoString(paresBusca, contador), 10) != 0){
                        vetorByteOffset[j] = 0;
                    }
                    break;

                case 3:
                    if(strncmp(GetRegistroMarcaCelular(registro_aux), 
                    GetValorCampoString(paresBusca, contador), 12) !=0){
                        vetorByteOffset[j] = 0;
                    }
                    break;

                case 4:
                    if(strcmpAtePipe(GetRegistroLugarCrime(registro_aux), 
                    GetValorCampoString(paresBusca, contador))!= 0){
                        vetorByteOffset[j] = 0;
                    }
                    break;
                
                case 5:
                    if(strcmpAtePipe(GetRegistroDescricaoCrime(registro_aux), 
                    GetValorCampoString(paresBusca, contador)) != 0){
                        vetorByteOffset[j] = 0;
                    }
                    break;
                default:
                    break;
            }

            DesalocaRegistro(registro_aux);
        }
        
        contador++;
    }

    //Não encontrou o registro
    if(vetorByteOffset[0] == -1){
        ErroRegistro();
    }else{
        int passou = 0;
        int tamanho = 0;
        //Percorre vetor de byteoffset e le os registros
        for (int j = 0; vetorByteOffset[j] != -1; j++)
        {
            if(vetorByteOffset[j] != 0){
                passou = 1;
                DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);
                ImprimeRegistroBinario(registro_aux);

                DesalocaRegistro(registro_aux);
            }
        }
        
        if(!passou){
            ErroRegistro();
        }
    }

    
    fclose(arqBin);
    free(vetorByteOffset);
    return true;
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
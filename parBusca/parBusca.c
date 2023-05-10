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


//Le da entrada padrão cada par de busca em uma busca e armazena na struct
void RecebeParesBusca(int qtdPares, PARES_BUSCA *paresBusca){
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

//Retorna true se existe um par para realizar a busca binaria na primeira posição
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

//Escolhe se vai fazer uma busca binaria no arquivo de indices ou uma busca sequencial no binario
bool MetodoDeBusca(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, char *campoIndexado){
    if(DecideOrdemBusca(paresBusca, qtdPares, campoIndexado)){ //O metodo é a busca binaria
        if(!BuscaBinariaIndices(arqEntrada, nomeArqIndice, paresBusca, qtdPares)) return false;
    }else{ //o metodo é a busca sequencial
        if(!BuscaSequencialBinario(arqEntrada, paresBusca, qtdPares)) return false;
    }

    return true;
}

//Realiza uma busca binaria no arquivo de indices e depois verifica as outras buscas no arquivo binario
//imprime as buscas que passarem nos testes
bool BuscaBinariaIndices(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares){
    //Vai indicar os byteOffset dos registros encontrados
    //-1 indica o fim do vetor
    long int *vetorByteOffset  = calloc(1, sizeof(long int));

    FILE *arqBin;
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;

    int tipoCampo = GetTipoCampo(paresBusca, 0);

    if (tipoCampo == 0 || tipoCampo == 1){ //O campo é do tipo int
        vetorByteOffset =  BuscaBinariaIndiceInt(nomeArqIndice, GetValorCampoInt(paresBusca, 0), vetorByteOffset);

    }else if(tipoCampo >= 2){ //O campo é do tipo string
        vetorByteOffset =  BuscaBinariaIndiceString(nomeArqIndice, GetValorCampoString(paresBusca, 0), vetorByteOffset);
    }

   int qtdRegistrosImpressos=0;
   //Se tiver apenas 1 criterio de busca(no caso, a busca pelo indice) ja imprime
   if(qtdPares == 1){
        for (int j = 0; vetorByteOffset[j] != -1; j++)
        {
            DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);
            if(GetRegistroRemovido(registro_aux) != '1'){
                ImprimeRegistroBinario(registro_aux);
                qtdRegistrosImpressos++;
            }
            DesalocaRegistro(registro_aux);
        }
        
   }
    /*  A partir das buscas retornadas da busca binaria, verifica se esses campos
        satisfazem os outros criterios de busca
    */
   else{
        for (int j = 0; vetorByteOffset[j] != -1; j++){
            int passou = 1;
            
            DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);

            for (int i = 1; i < qtdPares; i++)
            {
                int tipoCampo = GetTipoCampo(paresBusca, i);

                switch (tipoCampo)
                {
                    case 0:
                        if(GetRegistroIdCrime(registro_aux) != GetValorCampoInt(paresBusca, i)){
                            passou = 0;
                        }
                        break;
                    
                    case 1:
                        if(GetRegistroNroArtigo(registro_aux) != GetValorCampoInt(paresBusca, i)){
                            passou = 0;
                        }
                        break;
                    
                    case 2:{

                        int len = strlen(GetValorCampoString(paresBusca, i));
                        if(strncmp(GetRegistroDataCrime(registro_aux), 
                        GetValorCampoString(paresBusca, i), len) != 0){
                            passou = 0;
                        }
                        break;
                    }
                        

                    case 3:{
                        int len = strlen(GetValorCampoString(paresBusca, i));
                        if(strncmp(GetRegistroMarcaCelular(registro_aux), 
                        GetValorCampoString(paresBusca, i), len) !=0){
                            passou = 0;
                        }
                        break;
                    }
                    case 4:
                        if(strcmpAtePipe(GetRegistroLugarCrime(registro_aux), 
                        GetValorCampoString(paresBusca, i))!= 0){
                            passou = 0;
                        }
                        break;
                    
                    case 5:
                        if(strcmpAtePipe(GetRegistroDescricaoCrime(registro_aux), 
                        GetValorCampoString(paresBusca, i)) != 0){
                            passou = 0;
                        }
                        break;
                    default:
                        break;
                }

            }

            if(passou && GetRegistroRemovido(registro_aux) != '1'){
                ImprimeRegistroBinario(registro_aux);
                qtdRegistrosImpressos++;
            }
               
            DesalocaRegistro(registro_aux);
        
        }
   }
   if(qtdRegistrosImpressos == 0) ErroRegistro();
   
    
    fclose(arqBin);
    free(vetorByteOffset);
    return true;
}

//Realiza uma busca sequencial direto no arquivo binario
//imprime as buscas que passarem nos testes
bool BuscaSequencialBinario(char *nomeArqBin, PARES_BUSCA *paresBusca, int qtdPares){
    FILE *arqBin;
    if(!AbreArquivo(&arqBin, nomeArqBin, "rb", NULL)) return false;

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

        if(passou && GetRegistroRemovido(registro_aux) != '1'){
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
    return true;
}




//Escolhe se vai remover a partir de uma busca binaria no arquivo de indices ou uma busca sequencial no binario
bool MetodoDeRemocao(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, char *campoIndexado){

    if(DecideOrdemBusca(paresBusca, qtdPares, campoIndexado)){ //O metodo é a busca binaria
        if(!RemocaoBinariaIndicesBinario(arqEntrada, nomeArqIndice, paresBusca, qtdPares)) return false;
    }else{ //o metodo é a busca sequencial
        if(!RemocaoSequencialBinario(arqEntrada, paresBusca, qtdPares)) return false;
    }

    return true;
}


bool RemocaoBinariaIndicesBinario(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares){
    //Vai indicar os byteOffset dos registros encontrados
    //0 indica um registro que não passou em todos os casos
    //-1 indica o fim do vetor
    long int *vetorByteOffset  = calloc(1, sizeof(long int));

    FILE *arqBin;
    if(!AbreArquivo(&arqBin, arqEntrada, "rb+", NULL)) return false;

    int tipoCampoIndice = GetTipoCampo(paresBusca, 0);

    if (tipoCampoIndice == 0 || tipoCampoIndice == 1){ //O campo é do tipo int
        vetorByteOffset =  BuscaBinariaIndiceInt(nomeArqIndice, GetValorCampoInt(paresBusca, 0), vetorByteOffset);

    }else if(tipoCampoIndice >= 2){ //O campo é do tipo string
        vetorByteOffset =  BuscaBinariaIndiceString(nomeArqIndice, GetValorCampoString(paresBusca, 0), vetorByteOffset);
    }

   int qtdRegistrosRemovidos=0;
   //Se tiver apenas 1 criterio de busca(no caso, a busca pelo indice) ja imprime
   if(qtdPares == 1){
        char remove = '1';

        for (int j = 0; vetorByteOffset[j] != -1; j++)
        {
            fseek(arqBin, vetorByteOffset[j], SEEK_SET);
            fwrite(&(remove), sizeof(char), 1, arqBin);
            
            if (tipoCampoIndice == 0 || tipoCampoIndice == 1){ //O campo é do tipo int
                RemoveArquivoIndiceInt(nomeArqIndice, vetorByteOffset[j]);
                qtdRegistrosRemovidos++;

            }else if(tipoCampoIndice >= 2){ //O campo é do tipo string
                RemoveArquivoIndiceString(nomeArqIndice, vetorByteOffset[j]);
                qtdRegistrosRemovidos++;
            } 
        }
        
   }
    /*  A partir das buscas retornadas da busca binaria, verifica se esses campos
        satisfazem os outros criterios de busca
    */
   else{
        for (int j = 0; vetorByteOffset[j] != -1; j++){
            int passou = 1;
            
            DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);

            for (int i = 1; i < qtdPares; i++)
            {
                int tipoCampo = GetTipoCampo(paresBusca, i);

                switch (tipoCampo)
                {
                    case 0:
                        if(GetRegistroIdCrime(registro_aux) != GetValorCampoInt(paresBusca, i)){
                            passou = 0;
                            vetorByteOffset[j] = 0;
                        }
                        break;
                    
                    case 1:
                        if(GetRegistroNroArtigo(registro_aux) != GetValorCampoInt(paresBusca, i)){
                            passou = 0;
                             vetorByteOffset[j] = 0;
                        }
                        break;
                    
                    case 2:{

                        int len = strlen(GetValorCampoString(paresBusca, i));
                        if(strncmp(GetRegistroDataCrime(registro_aux), 
                        GetValorCampoString(paresBusca, i), len) != 0){
                            passou = 0;
                             vetorByteOffset[j] = 0;
                        }
                        break;
                    }
                        

                    case 3:{
                        int len = strlen(GetValorCampoString(paresBusca, i));
                        if(strncmp(GetRegistroMarcaCelular(registro_aux), 
                        GetValorCampoString(paresBusca, i), len) !=0){
                            passou = 0;
                             vetorByteOffset[j] = 0;
                        }
                        break;
                    }
                    case 4:
                        if(strcmpAtePipe(GetRegistroLugarCrime(registro_aux), 
                        GetValorCampoString(paresBusca, i))!= 0){
                            passou = 0;
                             vetorByteOffset[j] = 0;
                        }
                        break;
                    
                    case 5:
                        if(strcmpAtePipe(GetRegistroDescricaoCrime(registro_aux), 
                        GetValorCampoString(paresBusca, i)) != 0){
                            passou = 0;
                            vetorByteOffset[j] = 0;
                        }
                        break;
                    default:
                        break;
                }

            }

            if(passou && GetRegistroRemovido(registro_aux) != '1'){
                //remover
                fseek(arqBin, vetorByteOffset[j], SEEK_SET);
                fwrite(&(remove), sizeof(char), 1, arqBin);
                
                if (tipoCampoIndice == 0 || tipoCampoIndice == 1){ //O campo é do tipo int
                    RemoveArquivoIndiceInt(nomeArqIndice, vetorByteOffset[j]);
                    qtdRegistrosRemovidos++;

                }else if(tipoCampoIndice >= 2){ //O campo é do tipo string
                    RemoveArquivoIndiceString(nomeArqIndice, vetorByteOffset[j]);
                    qtdRegistrosRemovidos++;
                } 
                qtdRegistrosRemovidos++;
            }
               
            DesalocaRegistro(registro_aux);
        
        }
   }
   if(qtdRegistrosRemovidos == 0) ErroRegistro();
   
    
    fclose(arqBin);
    free(vetorByteOffset);
    binarioNaTela(arqEntrada);
    binarioNaTela(nomeArqIndice);
    return true;
}



bool RemocaoSequencialBinario(char *nomeArqBin, PARES_BUSCA *paresBusca, int qtdPares){
    
}





#include "parBusca.h"

// //Uma struct auxiliar para armazenar todos os pares de busca na funcionalidade 4
// struct paresBusca{
//     char nomeCampo[15];
//     //So sera usado um valorCampo de cada vez.
//     char valorCampoString[200];
//     int valorCampoInt;
//     int tipoCampo; 
//     /*  0 = idCampo
//         1 = numeroArtigo
//         2 = dataCrime
//         3 = marcaCelular
//         4 = lugarCrime
//         5 = descricaoCrime
//     */
// };

//aloca um registro de indice do tipo inteiro
PARES_BUSCA *VetorParesBuscaCriar(int tamanho){
    PARES_BUSCA *vetor = (PARES_BUSCA*) calloc(tamanho, sizeof(PARES_BUSCA));
    if(vetor != NULL){
        //inicializa os itens do vetor
        for (int i = 0; i < tamanho; i++)
        {
            vetor[i].valorCampoInt = -1;
            vetor[i].tipoCampo = -1;
            for(int j=0; j<200; j++)
                vetor[i].valorCampoString[j] = '$';
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
bool RecebeParesBusca(int qtdPares, PARES_BUSCA *paresBusca){
    for (int j = 0; j < qtdPares; j++)
    {
        char *nomeCampo = GetNomeCampo(paresBusca, j);
        scanf("%s ", nomeCampo);
        int tipoCampo = TipoChaveBusca(nomeCampo);

        AtualizaTipoCampo(paresBusca, j, tipoCampo);
        if (tipoCampo == 0 || tipoCampo == 1){ //O campo é do tipo int
            int valorCampoInt;
            char aux[15];
            scanf("%s ", aux);
            if(strcmp(aux, "NULO") != 0)
                valorCampoInt = atoi(aux);
            else
                valorCampoInt = -1;

            AtualizaValorCampoInt(paresBusca, j, valorCampoInt);
            

        }else if(tipoCampo >= 2){
            scan_quote_string(GetValorCampoString(paresBusca, j));

        }else{//Caso seja -1
            return false;
            ErroArquivo();
        }
    }
    return true;
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
    //Decide qual método de busca utilizar
    if(DecideOrdemBusca(paresBusca, qtdPares, campoIndexado)){ 
        //O metodo é a busca binaria
        if(!BuscaBinariaIndices(arqEntrada, nomeArqIndice, paresBusca, qtdPares)) return false;
    }else{ 
        //o metodo é a busca sequencial
        if(!BuscaSequencialBinario(arqEntrada, paresBusca, qtdPares)) return false;
    }

    return true;
}

//Realiza uma busca binaria no arquivo de indices e depois verifica as outras buscas no arquivo binario
//imprime as buscas que passarem nos testes
bool BuscaBinariaIndices(char *nomeArqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares){
    //Vai indicar os byteOffset dos registros encontrados
    //-1 indica o fim do vetor
    long int *vetorByteOffset  = calloc(1, sizeof(long int));

    FILE *arqBin;
    if(!AbreArquivo(&arqBin, nomeArqEntrada, "rb", NULL)) return false;

    //identifica qual campo será usado na busca por índice
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
            
            DADOS *registroAux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);

            for (int i = 1; i < qtdPares; i++)
            {
                int tipoCampo = GetTipoCampo(paresBusca, i);

                switch (tipoCampo)
                {
                    case 0:
                        if(GetRegistroIdCrime(registroAux) != GetValorCampoInt(paresBusca, i)){
                            passou = 0;
                        }
                        break;
                    
                    case 1:
                        if(GetRegistroNroArtigo(registroAux) != GetValorCampoInt(paresBusca, i)){
                            passou = 0;
                        }
                        break;
                    
                    case 2:{

                        int len = strlen(GetValorCampoString(paresBusca, i));
                        if(strncmp(GetRegistroDataCrime(registroAux), 
                        GetValorCampoString(paresBusca, i), len) != 0){
                            passou = 0;
                        }
                        break;
                    }
                        

                    case 3:{
                        int len = strlen(GetValorCampoString(paresBusca, i));
                        if(strncmp(GetRegistroMarcaCelular(registroAux), 
                        GetValorCampoString(paresBusca, i), len) !=0){
                            passou = 0;
                        }
                        break;
                    }
                    case 4:
                        if(strcmpAtePipe(GetRegistroLugarCrime(registroAux), 
                        GetValorCampoString(paresBusca, i))!= 0){
                            passou = 0;
                        }
                        break;
                    
                    case 5:
                        if(strcmpAtePipe(GetRegistroDescricaoCrime(registroAux), 
                        GetValorCampoString(paresBusca, i)) != 0){
                            passou = 0;
                        }
                        break;
                    default:
                        break;
                }

            }

            if(passou && GetRegistroRemovido(registroAux) != '1'){
                ImprimeRegistroBinario(registroAux);
                qtdRegistrosImpressos++;
            }
               
            DesalocaRegistro(registroAux);
        
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
    long int aux;
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
        int tipoCampoIndice = TipoChaveBusca(campoIndexado);
        if(!RemocaoSequencialBinario(arqEntrada, nomeArqIndice, paresBusca, qtdPares, tipoCampoIndice)) return false;
    }

    return true;
}

bool MetodoDeAtualizacao(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca,
PARES_BUSCA *paresAtualizacoes, int qtdPares, int qtdAtualizacoes, char *campoIndexado, char *dado){
    if(DecideOrdemBusca(paresBusca, qtdPares, campoIndexado)){ //O metodo é a busca binaria
        if(!AtualizacaoBinariaIndices(arqEntrada, nomeArqIndice, campoIndexado, dado, paresBusca, paresAtualizacoes, qtdPares, qtdAtualizacoes)) return false;
    }else{ //o metodo é a busca sequencial
        if(!AtualizacaoSequencialBinario(arqEntrada, nomeArqIndice, campoIndexado, dado, paresBusca, paresAtualizacoes, qtdPares, qtdAtualizacoes)) return false;
    }

    return true;
}


bool RemocaoBinariaIndicesBinario(char *arqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares){
    //Vai indicar os byteOffset dos registros encontrados
    //0 indica um registro que não passou em todos os casos
    //-1 indica o fim do vetor
    long int *vetorByteOffset  = calloc(1, sizeof(long int));
    bool isNulo = false;

    FILE *arqBin;
    if(!AbreArquivo(&arqBin, arqEntrada, "rb+", NULL)) return false;

    CABECALHO *cabecalho_aux = CabecalhoCriar();

    //le o cabecalho do arquivo binario
    LeCabecalhoDoArqBinario(cabecalho_aux, arqBin);


    int tipoCampoIndice = GetTipoCampo(paresBusca, 0);

    if (tipoCampoIndice == 0 || tipoCampoIndice == 1){ //O campo é do tipo int
        vetorByteOffset =  BuscaBinariaIndiceInt(nomeArqIndice, GetValorCampoInt(paresBusca, 0), vetorByteOffset);

    }else if(tipoCampoIndice >= 2){ //O campo é do tipo string
        vetorByteOffset =  BuscaBinariaIndiceString(nomeArqIndice, GetValorCampoString(paresBusca, 0), vetorByteOffset);
    }

   int qtdRegistrosRemovidos=0;
   //Se tiver apenas 1 criterio de busca(no caso, a busca pelo indice) ja remove
   if(qtdPares == 1){
        char remove = '1';

        for (int j = 0; vetorByteOffset[j] != -1; j++)
        {   
            DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);
            isNulo = TestaNulo(registro_aux, tipoCampoIndice);
            fseek(arqBin, vetorByteOffset[j], SEEK_SET);
            fwrite(&(remove), sizeof(char), 1, arqBin);
            
            if (!isNulo && (tipoCampoIndice == 0 || tipoCampoIndice == 1)){ //O campo é do tipo int
                RemoveArquivoIndiceInt(nomeArqIndice, vetorByteOffset[j]);
            }else if(!isNulo && (tipoCampoIndice >= 2)){ //O campo é do tipo string
                RemoveArquivoIndiceString(nomeArqIndice, vetorByteOffset[j]);
            }
            qtdRegistrosRemovidos++;
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
                char remove = '1';

                isNulo = TestaNulo(registro_aux, tipoCampoIndice);
                
                //remover
                fseek(arqBin, vetorByteOffset[j], SEEK_SET);
                fwrite(&(remove), sizeof(char), 1, arqBin);
                
                if (!isNulo && (tipoCampoIndice == 0 || tipoCampoIndice == 1)){ //O campo é do tipo int
                    RemoveArquivoIndiceInt(nomeArqIndice, vetorByteOffset[j]);
                }else if(!isNulo && (tipoCampoIndice >= 2)){ //O campo é do tipo string
                    RemoveArquivoIndiceString(nomeArqIndice, vetorByteOffset[j]);
                }
                qtdRegistrosRemovidos++;
            }
               
            DesalocaRegistro(registro_aux);
        
        }
    }
    
   //ATUALIZAR STATUS CABECALHOOOOOOOO binario!!!!!!!
    fclose(arqBin);


    int qtdRem = GetNroRegRem(cabecalho_aux) + qtdRegistrosRemovidos;
    AtualizaNroRegRem(cabecalho_aux, qtdRem);
    if(!AbreArquivo(&arqBin, arqEntrada, "rb+", NULL)) return false;
    EscreveCabecalho(arqBin, cabecalho_aux);
    fclose(arqBin);
    

    DesalocaCabecalho(cabecalho_aux);
    free(vetorByteOffset);
    return true;
}
    


bool RemocaoSequencialBinario(char *nomeArqBin, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, int tipoCampoIndice){
    FILE *arqBin;
    if(!AbreArquivo(&arqBin, nomeArqBin, "rb+", NULL)) return false;
    bool isNulo = false;
    
    //ATUALIZAR STATUS DO ARQUIVO

    CABECALHO *cabecalho_aux = CabecalhoCriar();

    //le o cabecalho do arquivo binario
    LeCabecalhoDoArqBinario(cabecalho_aux, arqBin);

    DADOS *registro_aux = RegistroCriar();


    long int byteOffsetLido = 17;
    long int byteOffsetAnterior = 17;
    int flag = LerRegBinario(arqBin, registro_aux, &byteOffsetLido);
    int i;
    int qtdRegistrosRemovidos = 0;
    
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
            char remove = '1';

            //remover
            fseek(arqBin, byteOffsetAnterior, SEEK_SET);
            fwrite(&(remove), sizeof(char), 1, arqBin);

            isNulo = TestaNulo(registro_aux, tipoCampoIndice);

            //avança o cursor lendo os campos
            avancaCursorSemRemovido(arqBin, registro_aux);
            
            
            if (!isNulo && (tipoCampoIndice == 0 || tipoCampoIndice == 1)){ //O campo é do tipo int
                RemoveArquivoIndiceInt(nomeArqIndice, byteOffsetAnterior);
            }else if(!isNulo && (tipoCampoIndice >= 2)){ //O campo é do tipo string
                RemoveArquivoIndiceString(nomeArqIndice, byteOffsetAnterior);
            } 
            qtdRegistrosRemovidos++;
        }

        byteOffsetAnterior = byteOffsetLido;
        
        DesalocaCamposVariaveis(registro_aux);

        flag = LerRegBinario(arqBin, registro_aux, &byteOffsetLido); 
    }

    fclose(arqBin);

    //se nao existem registros no arquivo
    if(!isNulo && (i==0 || qtdRegistrosRemovidos == 0)) ErroRegistro();

    //ATUALIZAR CABCALHOOOOOOOO binario!!!!!!!
    int qtdRem = GetNroRegRem(cabecalho_aux) + qtdRegistrosRemovidos;

    AtualizaNroRegRem(cabecalho_aux, qtdRem);

    if(!AbreArquivo(&arqBin, nomeArqBin, "rb+", NULL)) return false;

    EscreveCabecalho(arqBin, cabecalho_aux);



    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_aux);
    DesalocaRegistro(registro_aux);

    fclose(arqBin);
    return true;
}










bool AtualizaRegistroBinario(DADOS *registro, PARES_BUSCA *atualizacoes, int qtdAtualizacoes){
    for(int i=0; i<qtdAtualizacoes; i++){
        switch(GetTipoCampo(atualizacoes, i)){
            //caso idCrime
            case 0:{
                registro->idCrime = GetValorCampoInt(atualizacoes, i);
                break;
            }
            //caso numeroArtigo
            case 1:{
                registro->numeroArtigo = GetValorCampoInt(atualizacoes, i);
                break;
            }
            //caso dataCrime
            case 2:{
                
                strcpySem0(registro->dataCrime, GetValorCampoString(atualizacoes, i));
                
                break;
            }
            //caso marcaCelular
            case 3:{
               
                strcpySem0(registro->marcaCelular, GetValorCampoString(atualizacoes, i));
                
                break;
            }
            //caso lugarCrime
            case 4:{
                registro->lugarCrime = realloc(registro->lugarCrime, (strlen(GetValorCampoString(atualizacoes, i)) + 1));
                strcpySem0Variavel(registro->lugarCrime, GetValorCampoString(atualizacoes, i));
                break;
            }
            //caso descricaoCrime
            case 5:{
                registro->descricaoCrime = realloc(registro->descricaoCrime, strlen(GetValorCampoString(atualizacoes, i)) + 1);
                strcpySem0Variavel(registro->descricaoCrime, GetValorCampoString(atualizacoes, i));
                break;
            }
        }
    }

    return true;
}






//Realiza uma busca binaria no arquivo de indices e depois verifica as outras buscas no arquivo binario
//atualiza as buscas que passarem nos testes
bool AtualizacaoBinariaIndices(char *arqEntrada, char *nomeArqIndice, char *campoIndexado, char *dado, PARES_BUSCA *paresBusca, PARES_BUSCA *atualizacoes, int qtdPares, int qtdAtualizacoes){
    //Vai indicar os byteOffset dos registros encontrados
    //-1 indica o fim do vetor
    long int *vetorByteOffset  = calloc(1, sizeof(long int));

    FILE *arqBin;
    if(!AbreArquivo(&arqBin, arqEntrada, "rb+", NULL)) return false;

    CABECALHO *cabecalho = CabecalhoCriar();
    LeCabecalhoDoArqBinario(cabecalho, arqBin);

    int tipoCampo = GetTipoCampo(paresBusca, 0);

    if (tipoCampo == 0 || tipoCampo == 1){ //O campo é do tipo int
        vetorByteOffset =  BuscaBinariaIndiceInt(nomeArqIndice, GetValorCampoInt(paresBusca, 0), vetorByteOffset);

    }else if(tipoCampo >= 2){ //O campo é do tipo string
        vetorByteOffset =  BuscaBinariaIndiceString(nomeArqIndice, GetValorCampoString(paresBusca, 0), vetorByteOffset);
    }


    int qtdRegistrosAtualizados=0;
    //Se tiver apenas 1 criterio de busca(no caso, a busca pelo indice) ja atualiza
    if(qtdPares == 1){
        for (int j = 0; vetorByteOffset[j] != -1; j++)
        {
            DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);
            if(GetRegistroRemovido(registro_aux) != '1'){
                //verifica se o campo indexado era nulo antes da atualizacao
                bool EraNulo = false;
                EraNulo = TestaNulo(registro_aux, TipoChaveBusca(campoIndexado));

                int tamanhoAntigo = TamanhoRegistro(registro_aux);
            
                AtualizaRegistroBinario(registro_aux, atualizacoes, qtdAtualizacoes);

                int diferencaTamanhos = TamanhoRegistro(registro_aux) - tamanhoAntigo;

                if(diferencaTamanhos > 0){
                    PARES_BUSCA *paresbuscaAUX = VetorParesBuscaCriar(1);
                    paresbuscaAUX[0].valorCampoInt = GetRegistroIdCrime(registro_aux);
                    strcpy(paresbuscaAUX[0].nomeCampo,"idCrime");
                    strcpy(paresbuscaAUX[0].valorCampoString, "");
                    paresbuscaAUX[0].tipoCampo = 0;

                    //remove logicamente o registro
                    
                    MetodoDeRemocao(arqEntrada, nomeArqIndice, paresbuscaAUX, 1, campoIndexado);
                    

                    //insere registro
                    InsereRegistro(registro_aux, arqEntrada, nomeArqIndice, campoIndexado, dado);

                    
                    free(paresbuscaAUX);
                }else{
                    //lembrar de arrumar o cabecalho na escreverregistrobin
                    CABECALHO *cabecalho_aux = CabecalhoCriar();

                    fseek(arqBin, vetorByteOffset[j], SEEK_SET);
                    EscreverRegistroBinSemDelimitador(arqBin, registro_aux, cabecalho_aux);

                    int diferenca = TamanhoRegistro(registro_aux) - tamanhoAntigo;
                    //preenche com lixo
                    for(int i=0; i<abs(diferenca); i++){
                        char aux = '$';
                        fwrite(&aux, 1, sizeof(char), arqBin);
                    }
                    //escreve o delimitador do registro
                    char aux = '#';
                    fwrite(&aux, sizeof(char), 1, arqBin);

                    DesalocaCabecalho(cabecalho_aux);

                    //verifica se o campo indexado virou nulo depois da atualizacao
                    bool isNulo = false;
                    isNulo = TestaNulo(registro_aux, TipoChaveBusca(campoIndexado));

                    if (TipoChaveBusca(campoIndexado) == 0 || TipoChaveBusca(campoIndexado) == 1){ //O campo é do tipo int
                        //AtualizaArquivoIndiceInt(nomeArqIndice, atualizacoes->valorCampoInt, vetorByteOffset[j]);
                        if(!EraNulo)
                            RemoveArquivoIndiceInt(nomeArqIndice, vetorByteOffset[j]);
                        
                            
                        if(!isNulo)
                            InsereRegistroNoArqInd(registro_aux, nomeArqIndice, dado, TipoChaveBusca(campoIndexado), vetorByteOffset[j]);
                        
                            
                    }else if(TipoChaveBusca(campoIndexado) >= 2){ //O campo é do tipo string
                        //AtualizaArquivoIndiceString(nomeArqIndice, atualizacoes->valorCampoString, vetorByteOffset[j]);
                        if(!EraNulo)
                            RemoveArquivoIndiceString(nomeArqIndice, vetorByteOffset[j]);
                        
                            
                        if(!isNulo)
                            InsereRegistroNoArqInd(registro_aux, nomeArqIndice, dado, TipoChaveBusca(campoIndexado), vetorByteOffset[j]);
                        
                            
                    }
                }

                qtdRegistrosAtualizados++;
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
                //verifica se o campo indexado era nulo antes da atualizacao
                bool EraNulo = false;
                EraNulo = TestaNulo(registro_aux, TipoChaveBusca(campoIndexado));

                int tamanhoAntigo = TamanhoRegistro(registro_aux);
            

                AtualizaRegistroBinario(registro_aux, atualizacoes, qtdAtualizacoes);
                
                

                int diferencaTamanhos = TamanhoRegistro(registro_aux) - tamanhoAntigo;
                
                if(diferencaTamanhos > 0){
                    PARES_BUSCA *paresbuscaAUX = VetorParesBuscaCriar(1);
                    paresbuscaAUX[0].valorCampoInt = GetRegistroIdCrime(registro_aux);
                    paresbuscaAUX[0].tipoCampo = 0;
                    strcpy(paresbuscaAUX[0].valorCampoString, "");
                    strcpy(paresbuscaAUX[0].nomeCampo, "idCrime");

                    //remove logicamente o registro
                    MetodoDeRemocao(arqEntrada, nomeArqIndice, paresbuscaAUX, 1, campoIndexado);

                    //insere registro
                    InsereRegistro(registro_aux, arqEntrada, nomeArqIndice, campoIndexado, dado);
                    
                    free(paresbuscaAUX);
                }else{
                    CABECALHO *cabecalho_aux = CabecalhoCriar();

                    fseek(arqBin, vetorByteOffset[j], SEEK_SET);
                    EscreverRegistroBinSemDelimitador(arqBin, registro_aux, cabecalho_aux);

                    int diferenca = TamanhoRegistro(registro_aux) - tamanhoAntigo;
                    
                    for(int i=0; i < abs(diferenca); i++){
                        char aux = '$';
                        fwrite(&aux, 1, sizeof(char), arqBin);
                    }
                    //escreve o delimitador do registro
                    char aux = '#';
                    fwrite(&aux, sizeof(char), 1, arqBin);

                    DesalocaCabecalho(cabecalho_aux);

                    //DecideOrdemBusca(atualizacoes, qtdAtualizacoes, campoIndexado);

                    //verifica se o campo indexado virou nulo depois da atualizacao
                    bool isNulo = false;
                    isNulo = TestaNulo(registro_aux, TipoChaveBusca(campoIndexado));


                    if (TipoChaveBusca(campoIndexado) == 0 || TipoChaveBusca(campoIndexado) == 1){ //O campo é do tipo int
                        //AtualizaArquivoIndiceInt(nomeArqIndice, atualizacoes->valorCampoInt, vetorByteOffset[j]);
                        if(!EraNulo)
                            RemoveArquivoIndiceInt(nomeArqIndice, vetorByteOffset[j]);
                        
                            
                        if(!isNulo)
                            InsereRegistroNoArqInd(registro_aux, nomeArqIndice, dado, TipoChaveBusca(campoIndexado), vetorByteOffset[j]);
                        
                            
                    }else if(TipoChaveBusca(campoIndexado) >= 2){ //O campo é do tipo string
                        //AtualizaArquivoIndiceString(nomeArqIndice, atualizacoes->valorCampoString, vetorByteOffset[j]);
                        if(!EraNulo)
                            RemoveArquivoIndiceString(nomeArqIndice, vetorByteOffset[j]);
                            
                        if(!isNulo)
                            InsereRegistroNoArqInd(registro_aux, nomeArqIndice, dado, TipoChaveBusca(campoIndexado), vetorByteOffset[j]);
                            
                    }
                }

                qtdRegistrosAtualizados++;
            }
               
            DesalocaRegistro(registro_aux);
        
        }
   }
//    if(qtdRegistrosAtualizados == 0) ErroRegistro();
   
    DesalocaCabecalho(cabecalho);
    fclose(arqBin);
    free(vetorByteOffset);
    return true;
}








//Realiza uma busca sequencial direto no arquivo binario
//atualiza as buscas que passarem nos testes
bool AtualizacaoSequencialBinario(char *nomeArqBin, char *nomeArqIndice, char *campoIndexado, char *dado, PARES_BUSCA *paresBusca, PARES_BUSCA *atualizacoes, int qtdPares, int qtdAtualizacoes){
    FILE *arqBin;
    if(!AbreArquivo(&arqBin, nomeArqBin, "rb+", NULL)) return false;

    CABECALHO *cabecalho_aux = CabecalhoCriar();

    //le o cabecalho do arquivo binario
    LeCabecalhoDoArqBinario(cabecalho_aux, arqBin);

    DADOS *registro = RegistroCriar();

    long int byteoffset = 17;
    long int byteoffsetanterior = 17;
    int flag = LerRegBinario(arqBin, registro, &byteoffset);
    int i;
    int qtdRegistrosAtualizados = 0;
    for(i=0; flag!=0; i++){
        int passou = 1;//Verifica se passou em todos os criterios de busca
        
        for (int j = 0; j < qtdPares; j++)
        {
            int tipoCampo = GetTipoCampo(paresBusca, j);
            int len = strlen(GetValorCampoString(paresBusca, j));

            switch (tipoCampo)
            {
                case 0:
                    if(GetRegistroIdCrime(registro) != GetValorCampoInt(paresBusca, j))
                        passou = 0;
                    break;
                case 1:
                    if(GetRegistroNroArtigo(registro) != GetValorCampoInt(paresBusca, j))
                        passou = 0;
                    break;
                case 2:
                    if(strncmp(GetRegistroDataCrime(registro), GetValorCampoString(paresBusca, j), len) != 0 )
                        passou = 0;
                    break;
                case 3:
                    if(strncmp(GetRegistroMarcaCelular(registro), GetValorCampoString(paresBusca, j), len) != 0 )
                        passou = 0;
                    break;
                case 4:
                    if(strcmpAtePipe(GetRegistroLugarCrime(registro), GetValorCampoString(paresBusca, j)) != 0 )
                        passou = 0;
                    break;
                case 5:
                    if(strcmpAtePipe(GetRegistroDescricaoCrime(registro), GetValorCampoString(paresBusca, j)) != 0 )
                        passou = 0;
                    break;
                default:
                    break;
            }
        }

        if(passou && GetRegistroRemovido(registro) != '1'){
            

            //verifica se o campo indexado era nulo antes da atualizacao
            bool EraNulo = false;
            EraNulo = TestaNulo(registro, TipoChaveBusca(campoIndexado));

            int tamanhoAntigo = TamanhoRegistro(registro);

            int idAuxiliar = GetRegistroIdCrime(registro);

            AtualizaRegistroBinario(registro, atualizacoes, qtdAtualizacoes);

            int diferencaTamanhos = TamanhoRegistro(registro) - tamanhoAntigo;

            if(diferencaTamanhos > 0){
                
                PARES_BUSCA *paresbuscaAUX = VetorParesBuscaCriar(1);
                
                paresbuscaAUX[0].valorCampoInt = idAuxiliar;
                strcpy(paresbuscaAUX[0].nomeCampo, "idCrime");
                paresbuscaAUX[0].tipoCampo = 0;
                strcpy(paresbuscaAUX[0].valorCampoString, "");

                //remove logicamente o registro
                MetodoDeRemocao(nomeArqBin, nomeArqIndice, paresbuscaAUX, 1, campoIndexado);


                //insere registro
                InsereRegistro(registro, nomeArqBin, nomeArqIndice, campoIndexado, dado);

                free(paresbuscaAUX);
            }else{
                //lembrar de arrumar o cabecalho na escreverregistrobin
                CABECALHO *cabecalho_aux = CabecalhoCriar();

                fseek(arqBin, byteoffsetanterior, SEEK_SET);
                EscreverRegistroBinSemDelimitador(arqBin, registro, cabecalho_aux);

                int diferenca = TamanhoRegistro(registro) - tamanhoAntigo;
                
                for(int i=0; i<abs(diferenca); i++){
                    char aux = '$';
                    fwrite(&aux, 1, sizeof(char), arqBin);
                }
                //escreve o delimitador do registro
                char aux = '#';
                fwrite(&aux, sizeof(char), 1, arqBin);

                //verifica se o campo indexado virou nulo depois da atualizacao
                bool isNulo = false;
                isNulo = TestaNulo(registro, TipoChaveBusca(campoIndexado));

                if (TipoChaveBusca(campoIndexado) == 0 || TipoChaveBusca(campoIndexado) == 1){ //O campo é do tipo int
                    //AtualizaArquivoIndiceInt(nomeArqIndice, atualizacoes->valorCampoInt, byteoffsetanterior);
                    if(!EraNulo)
                        RemoveArquivoIndiceInt(nomeArqIndice, byteoffsetanterior);
                    if(!isNulo)
                        InsereRegistroNoArqInd(registro, nomeArqIndice, dado, TipoChaveBusca(campoIndexado), byteoffsetanterior);
                }else if(TipoChaveBusca(campoIndexado) >= 2){ //O campo é do tipo string
                    //AtualizaArquivoIndiceString(nomeArqIndice, atualizacoes->valorCampoString, byteoffsetanterior);
                    if(!EraNulo)
                        RemoveArquivoIndiceString(nomeArqIndice, byteoffsetanterior);
                    if(!isNulo)
                        InsereRegistroNoArqInd(registro, nomeArqIndice, dado, TipoChaveBusca(campoIndexado), byteoffsetanterior);
                }

                DesalocaCabecalho(cabecalho_aux);
            }

            qtdRegistrosAtualizados++;
        }
        
        DesalocaCamposVariaveis(registro);

        byteoffsetanterior = byteoffset;
        flag = LerRegBinario(arqBin, registro, &byteoffset); 
    }

    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_aux);
    DesalocaRegistro(registro);

    //se nao existem registros no arquivo
    // if(i==0 || qtdRegistrosAtualizados == 0) ErroRegistro();
    fclose(arqBin);
    return true;
}

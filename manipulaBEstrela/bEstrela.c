#include "bEstrela.h"
#include "../arquivos/arquivos.h"
#include "../cabecalho/cabecalho.h"
#include "../manipulaRegistros/registro.h"
#include "../manipulaIndices/inteiro/indiceInteiro.h"

BTPAGE *PaginaCriar(void){
    BTPAGE *pagina = (BTPAGE*) calloc(1, sizeof(BTPAGE));

    //inicializa pagina
    if(pagina != NULL){
        pagina->nivel = 1;
        pagina->n = 0;

        for (int i = 0; i < MAXCHAVES+1; i++)
        {
            pagina->P[i] = -1;
            
            if(i < MAXCHAVES){
                pagina->chaves[i].C = -1;
                pagina->chaves[i].Pr = -1;
            }
        }   
    }
    else{
        ErroAlocacao();
    }
    return pagina;
}

BTPAGE *PaginaCriarInicializado(CHAVE chave, int pEsquerdo, int pDireito, int nivel, int n){
    BTPAGE *pagina = (BTPAGE*) calloc(1, sizeof(BTPAGE));

    //inicializa pagina
    if(pagina != NULL){
        pagina->nivel = nivel;
        pagina->n = n;
        
        for (int i = 0; i < MAXCHAVES+1; i++)
        {
            pagina->P[i] = -1;
            
            if(i < MAXCHAVES){
                pagina->chaves[i].C = -1;
                pagina->chaves[i].Pr = -1;
            }
        }

        pagina->chaves[0] = chave;
        pagina->P[0] = pEsquerdo;
        pagina->P[1] = pDireito; 
    }
    else{
        ErroAlocacao();
    }
    return pagina;
}

void LerPagina(FILE* arqArvore, int CURRENT_RRN, BTPAGE* pagina){
    //Va ate a pagina
    //Pula o cabecalho
    fseek(arqArvore, TAM_PAGE+(CURRENT_RRN*TAM_PAGE), SEEK_SET);


    fread(&(pagina->nivel), 4, 1, arqArvore);
    fread(&(pagina->n), 4, 1, arqArvore);

    
    int numPagina = (pagina->n);

    for (int i = 0; i < numPagina; i++)
    {
        fread(&(pagina->P[i]), 4, 1, arqArvore);
        fread(&(pagina->chaves[i].C), 4, 1, arqArvore);
        fread(&(pagina->chaves[i].Pr), 8, 1, arqArvore);
    }

    fread(&(pagina->P[numPagina]), 4, 1, arqArvore);
}

void EscrevePagina(FILE* arqArvore, int CURRENT_RRN, BTPAGE* pagina){
    //Va ate a pagina
    //Pula o cabecalho
    fseek(arqArvore, TAM_PAGE+(CURRENT_RRN*TAM_PAGE), SEEK_SET);


    fwrite(&(pagina->nivel), 4, 1, arqArvore);
    fwrite(&(pagina->n), 4, 1, arqArvore);


    for (int i = 0; i < MAXCHAVES; i++)
    {
        fwrite(&(pagina->P[i]), 4, 1, arqArvore);
        fwrite(&(pagina->chaves[i].C), 4, 1, arqArvore);
        fwrite(&(pagina->chaves[i].Pr), 8, 1, arqArvore);
    }

    fread(&(pagina->P[MAXCHAVES]), 4, 1, arqArvore);
}


bool ArvoreCriar(char nomeArqBin[], char arqIndiceArvore[]){
    FILE *arvore, *dados;
    if(!AbreArquivo(&arvore, arqIndiceArvore, "wb+", NULL)) return false;

    CABECALHO_B *cabecalhoArvore = CabecalhoBCriar();
    CabecalhoBEscreve(arvore, cabecalhoArvore);

    if(!AbreArquivo(&dados, nomeArqBin, "rb", NULL)) return false;
    CABECALHO *cabecalhoDados = CabecalhoCriar();
    LeCabecalhoDoArqBinario(cabecalhoDados, dados);

    //Percorre arquivo binario lendo os registros
    long int offsetProximoRegistro = 17; //será o offset do proximo registro durante o loop
    long int offsetInicioRegistro = 17;
    DADOS *registroAuxiliar = RegistroCriar();

    //A flag verifica se o id foi lido e serve para saber se chegou no final do arquivo
    int flag = LerRegBinario(dados, registroAuxiliar, &offsetProximoRegistro);


    int i;
    //le registros até o final do arquivo, inserindo no vetor de indices
    for(i = 0; flag != 0; i++){
        if(GetRegistroRemovido(registroAuxiliar) == '0'){

            //INSERCAO NA ARVORE!!!
            ArvoreInserir(arvore, registroAuxiliar, cabecalhoArvore, offsetInicioRegistro);
        }

        DesalocaCamposVariaveis(registroAuxiliar);

        offsetInicioRegistro = offsetProximoRegistro;

        flag = LerRegBinario(dados, registroAuxiliar, &offsetProximoRegistro); 
    }

    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();


    DesalocaRegistro(registroAuxiliar);

    fclose(arvore);
    fclose(dados);
    CabecalhoBDesalocar(cabecalhoArvore);
    DesalocaCabecalho(cabecalhoDados);

    return true;
}

bool ArvoreInserir(FILE *arvore, DADOS *registro, CABECALHO_B *cabecalho, long int byteoffset){
    int root = cabecalho->noRaiz;

    CHAVE key;
    key.C = registro->idCrime;
    key.Pr = byteoffset;

    CHAVE promo_key;
    int promo_r_child;

    int proxRRN = cabecalho->proxRRN;

    if(Insert(arvore, root, key, &promo_key, &promo_r_child) == PROMOTION){
        //crie nova página raiz com key:=PROMO_KEY, l_child:=ROOT, r_child:=PROMO_R_CHILD
        BTPAGE* novaPagina = PaginaCriarInicializado(promo_key, root, promo_r_child, 1, 1);
        //faça ROOT igual ao RRN da nova página raiz
        //Acho que o RRN da nova pagina é o prox RRN do cabecalho
        root = proxRRN;

        //Att proxRRN ddo cabecalho;
        cabecalho->proxRRN = (proxRRN+1);

        EscrevePagina(arvore, root, novaPagina);

        free(novaPagina);
    }

    //atualiza RRN raiz
    cabecalho->noRaiz = root;
    CabecalhoBReescreve(arvore, cabecalho);
}

bool ProcuraChavePagina(BTPAGE *pagina, int chave, int *posicao){
    //Procura no vetor de chaves por busca binaria
    bool encontrou;
    *posicao = BuscaBinaria(pagina, 0, (pagina->n)-1, chave, &encontrou);
    return encontrou;
}

ValoresRetorno Insert(FILE *arqArvore, int CURRENT_RRN, CHAVE KEY, CHAVE *PROMO_KEY, int *PROMO_R_CHILD){

    //Cria Paginas Vazias
    BTPAGE *pagina = PaginaCriar();
    BTPAGE *novaPagina = PaginaCriar();
    int posicaoPagina;
    CHAVE chavePromovida;
    int rrnPromovido;

    //se nó não existe, promove a chave
    if(CURRENT_RRN == NIL){
        *PROMO_KEY = KEY;
        *PROMO_R_CHILD = NIL;

        free(pagina);
        free(novaPagina);
        return PROMOTION;
    }else{
        //leia página CURRENT_RRN e armazene em PAGE
        LerPagina(arqArvore, CURRENT_RRN, pagina);

        //procure por KEY em PAGE
        if(ProcuraChavePagina(pagina, KEY.C, &posicaoPagina)) return ERROR;
        
        //passo recursivo
        ValoresRetorno RETURN_VALUE = Insert(arqArvore, pagina->P[posicaoPagina], KEY, &chavePromovida, &rrnPromovido);

        if(RETURN_VALUE == NO_PROMOTION || RETURN_VALUE == ERROR) return RETURN_VALUE;

        //se ainda existe espaço na página
        else if(pagina->n < MAXCHAVES){
            //Inserir Chave e RRN Promovidos
            InsereChave(pagina, MAXCHAVES, KEY, rrnPromovido);
            EscrevePagina(arqArvore, CURRENT_RRN, pagina);
            free(pagina);
            free(novaPagina);

            return NO_PROMOTION;
        }
        else{
            //redistribuicao / split
        }
    }


    
}

int InsereChave(BTPAGE *pagina, int tamanho, CHAVE chave, int RRN_Direita){
    //algoritmo usado esta disponivel em https://www.sanfoundry.com/c-program-insert-element-specified-position-array/
    int pos;

    for(int i=0; i<tamanho; i++){
        if(pagina->chaves[i].C == NIL){
            pagina->chaves[i] = chave;
            return i;
        }

        if(chave.C < pagina->chaves[i].C){
            pos = i;
            break;
        }
    }

    if(pos != tamanho){
        int deslocamento = tamanho - pos - 1;

        for(int i=0; i<deslocamento; i++){
           pagina->chaves[tamanho - i - 1] = pagina->chaves[tamanho - i - 2];
           pagina->P[tamanho - i] = pagina->P[tamanho - i - 1];
        }
    }
    pagina->chaves[pos] = chave;
    pagina->P[pos+1] = RRN_Direita;
    pagina->n = (pagina->n) + 1;
    return pos;
}








int BuscaBinaria(BTPAGE* pagina, int posicaoInicial, int posicaoFinal, int chave, bool *encontrou){
	int centro  = (int)((posicaoInicial+posicaoFinal)/2);
    while(posicaoInicial <= posicaoFinal){ //log n
		centro = (int)((posicaoInicial+posicaoFinal)/2);

		if (chave == pagina->chaves[centro].C){
            *encontrou = true;
			return centro; //valor encontrado- retorna a posicao no vetor de chaves
        }
		if (chave < pagina->chaves[centro].C) //se o número existir estará na primeira metade
			posicaoFinal = centro - 1;
		if (chave > pagina->chaves[centro].C) //se o número existir estará na segunda metade
			posicaoInicial = centro + 1;
	}
	//valor não encontrado = retorna a posicao no vetor de RRNs
    *encontrou = false;
    
    if (chave < pagina->chaves[centro].C) //se o número existir estará na primeira metade
		return (centro);
	if (chave > pagina->chaves[centro].C) //se o número existir estará na segunda metade
		return (centro+1);
}


buscaRetorno Pesquisa(FILE* arqArvore, int RRN, int chave, int *encontra_RRN, int *encontra_pos){
    if(RRN == NIL)
        return NAO_ACHOU;
    else{
        BTPAGE *pagina = PaginaCriar(); 
        LerPagina(arqArvore, RRN, pagina);
        //procure KEY em PAGE, fazendo POS igual a posição em que KEY ocorre ou deveria ocorrer
        int pos = -1;
        if(ProcuraChavePagina(pagina, chave, &pos)){
            *encontra_RRN = RRN;
            *encontra_pos = pos;
            free(pagina);
            return ACHOU;
        }else{
            int RRN_busca = pagina->P[pos];
            free(pagina);
            return Pesquisa(arqArvore, RRN_busca, chave, encontra_RRN, encontra_pos);
        }
    }
}

long int BuscaArvore(char *nomeArqIndice, int valorBuscado){
    FILE *arqArvore;
    CABECALHO_B *cabecalhoArvoreAux = CabecalhoBCriar();

    //abre o arquivo de indice
    if(!AbreArquivo(&arqArvore, nomeArqIndice, "rb", NULL)) return -1;

    CabecalhoBLer(cabecalhoArvoreAux, arqArvore);

    int RRN_encontrado = -1;
    int POS_encontrada = -1;
    buscaRetorno retorno = Pesquisa(arqArvore, cabecalhoArvoreAux->noRaiz, 
    valorBuscado, &RRN_encontrado, &POS_encontrada);

    if(retorno == ACHOU){
        BTPAGE *pagina = PaginaCriar();
        LerPagina(arqArvore, RRN_encontrado, pagina);

        CabecalhoBDesalocar(cabecalhoArvoreAux);
        fclose(arqArvore);
        long int byteOffset = pagina->chaves[POS_encontrada].Pr;
        free(pagina);
        return byteOffset;
    }

    
    CabecalhoBDesalocar(cabecalhoArvoreAux);
    fclose(arqArvore);
    return -1;
}


DADOS** BuscaIndiceArvore(char *nomeArqEntrada, char *nomeArqIndice, PARES_BUSCA *paresBusca, int qtdPares, int *qtdEncontrados){
    //Vai indicar os byteOffset dos registros encontrados
    //-1 indica o fim do vetor
    long int byteOffset;

    FILE *arqBin;
    if(!AbreArquivo(&arqBin, nomeArqEntrada, "rb", NULL)) return false;

    //identifica qual campo será usado na busca por índice
    TipoCampo tipoCampo = GetTipoCampo(paresBusca, 0);

    if(tipoCampo == idCrime){
        byteOffset =  BuscaArvore(nomeArqIndice, GetValorCampoInt(paresBusca, 0));
    }

    //Cria um vetor de registros
    DADOS** registrosProcurado = VetorRegistrosCriar(1);

    int qtdRegistrosEncontrados=0;
    //Se tiver apenas 1 criterio de busca(no caso, a busca pelo indice) ja imprime
    if(qtdPares == 1){
        if(byteOffset != -1){
            DADOS *registroAux = LeRegistroPorByteOffset(arqBin, byteOffset);
            if(GetRegistroRemovido(registroAux) != '1'){
                registrosProcurado[qtdRegistrosEncontrados] = registroAux;
                qtdRegistrosEncontrados++;
            }else{
                DesalocaRegistro(registroAux);
            }
        } 
    }
        /*  A partir das buscas retornadas da busca binaria, verifica se esses campos
            satisfazem os outros criterios de busca
        */
    else{
        if (byteOffset != -1){
            int passou = 1;
            
            DADOS *registroAux = LeRegistroPorByteOffset(arqBin, byteOffset);

            VerificaTodosCriteriosBusca(1, qtdPares, registroAux, &passou, paresBusca);

            if(passou && GetRegistroRemovido(registroAux) != '1'){
                registrosProcurado[qtdRegistrosEncontrados] = registroAux;
                qtdRegistrosEncontrados++;
            }else{
                DesalocaRegistro(registroAux);
            }
        }
    }

    *qtdEncontrados = qtdRegistrosEncontrados;

    fclose(arqBin);

    if(qtdRegistrosEncontrados == 0){
        DesalocaVetorRegistro(registrosProcurado, qtdRegistrosEncontrados);
        return NULL;
    }

    return registrosProcurado;
}




/*
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
            DADOS *registroAux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);
            if(GetRegistroRemovido(registroAux) != '1'){
                ImprimeRegistroBinario(registroAux);
                qtdRegistrosImpressos++;
            }
            DesalocaRegistro(registroAux);
        }
        
   }

   else{
        for (int j = 0; vetorByteOffset[j] != -1; j++){
            int passou = 1;
            
            DADOS *registroAux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);

            VerificaTodosCriteriosBusca(1, qtdPares, registroAux, &passou, paresBusca);

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

    CABECALHO *cabecalhoBinAux = CabecalhoCriar();

    //le o cabecalho do arquivo binario
    LeCabecalhoDoArqBinario(cabecalhoBinAux, arqBin);

    DADOS *registroAux = RegistroCriar();

    long int boffAux;//Neste caso nn utiliza o numero de byteoffset lidos
    int flag = LerRegBinario(arqBin, registroAux, &boffAux);
    int i;
    int qtdRegistrosImpressos = 0;
    for(i=0; flag!=0; i++){
        int passou = 1;//Verifica se passou em todos os criterios de busca
        
        VerificaTodosCriteriosBusca(0, qtdPares, registroAux, &passou, paresBusca);

        if(passou && GetRegistroRemovido(registroAux) != '1'){
            ImprimeRegistroBinario(registroAux);
            qtdRegistrosImpressos++;
        }
        
        DesalocaCamposVariaveis(registroAux);

        flag = LerRegBinario(arqBin, registroAux, &boffAux); 
    }


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalhoBinAux);
    DesalocaRegistro(registroAux);

    //se nao existem registros no arquivo
    if(i==0 || qtdRegistrosImpressos == 0) ErroRegistro();
    fclose(arqBin);
    return true;
} */
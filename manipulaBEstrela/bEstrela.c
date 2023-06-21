#include "bEstrela.h"
#include "../arquivos/arquivos.h"
#include "../cabecalho/cabecalho.h"
#include "../manipulaRegistros/registro.h"
#include "../manipulaIndices/inteiro/indiceInteiro.h"


//FUNÇÕES BÁSICA DA ÁRVORE B*

//Cria página de disco
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

//Cria página inicializada
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

//Define dados da página para os de inicialização
void ResetaPagina(BTPAGE *pagina){
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

//Realiza a leitura de uma página de disco
void LerPagina(FILE* arqArvore, int RRN_Atual, BTPAGE* pagina){
    //Va ate a pagina
    //Pula o cabecalho
    fseek(arqArvore, TAM_PAGE+(RRN_Atual*TAM_PAGE), SEEK_SET);


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

//Escreve uma página de disco
void EscrevePagina(FILE* arqArvore, int RRN_Atual, BTPAGE* pagina){
    //Va ate a pagina
    //Pula o cabecalho
    fseek(arqArvore, TAM_PAGE+(RRN_Atual*TAM_PAGE), SEEK_SET);


    fwrite(&(pagina->nivel), 4, 1, arqArvore);
    fwrite(&(pagina->n), 4, 1, arqArvore);


    for (int i = 0; i < MAXCHAVES; i++)
    {
        fwrite(&(pagina->P[i]), 4, 1, arqArvore);
        fwrite(&(pagina->chaves[i].C), 4, 1, arqArvore);
        fwrite(&(pagina->chaves[i].Pr), 8, 1, arqArvore);
    }

    fwrite(&(pagina->P[MAXCHAVES]), 4, 1, arqArvore);
}


//Cria árvore de indices
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

            //Insercao na arvore
            ArvoreInserir(arvore, registroAuxiliar, cabecalhoArvore, offsetInicioRegistro);
        }

        DesalocaCamposVariaveis(registroAuxiliar);

        offsetInicioRegistro = offsetProximoRegistro;

        flag = LerRegBinario(dados, registroAuxiliar, &offsetProximoRegistro); 
    }

    DesalocaRegistro(registroAuxiliar);

    fseek(arvore, 0, SEEK_SET);
    CabecalhoBLer(cabecalhoArvore, arvore);
    cabecalhoArvore->status = '1';
    CabecalhoBReescreve(arvore, cabecalhoArvore);

    fclose(arvore);
    fclose(dados);
    CabecalhoBDesalocar(cabecalhoArvore);
    DesalocaCabecalho(cabecalhoDados);

    binarioNaTela(arqIndiceArvore);
    return true;
}

//inserção na árvore
bool ArvoreInserir(FILE *arvore, DADOS *registro, CABECALHO_B *cabecalho, long int byteoffset){
    int root = cabecalho->noRaiz;

    CHAVE key;
    key.C = registro->idCrime;
    key.Pr = byteoffset;

    CHAVE promoChave;
    int promoFilho;

    //passo recursivo
    ValoresRetorno retornoInsercao = Insert(arvore, root, key, &promoChave, &promoFilho, -1);

    fseek(arvore, 0, SEEK_SET);
    CabecalhoBLer(cabecalho, arvore);

    int proxRRN = cabecalho->proxRRN;
    if(retornoInsercao == PROMOTION){
        //crie nova página raiz com key:=promoChave, l_child:=ROOT, r_child:=promoFilho
        
        BTPAGE* novaPagina = PaginaCriarInicializado(promoChave, root, promoFilho, cabecalho->nroNiveis + 1, 1);
        //faça ROOT igual ao RRN da nova página raiz
        root = proxRRN;

        //Att proxRRN ddo cabecalho;
        cabecalho->proxRRN = (proxRRN+1);
        cabecalho->nroNiveis = cabecalho->nroNiveis + 1;

        EscrevePagina(arvore, root, novaPagina);

        free(novaPagina);
    }

    if(retornoInsercao != ERROR)
        cabecalho->nroChaves = cabecalho->nroChaves + 1;

    //atualiza RRN raiz
    cabecalho->noRaiz = root;
    CabecalhoBReescreve(arvore, cabecalho);
}




//FUNÇÕES DE MANIPULAÇÃO DOS VETORES DE CHAVES

//copia o conteudo de origem em destino
void copiaVetorChave(CHAVE *vetorOrigem, int inicioOrigem, int fimOrigem, CHAVE *vetorDestino, int inicioDestino){
    int j = inicioDestino;
    for (int i = inicioOrigem; i <= fimOrigem; i++)
    {
        vetorDestino[j] = vetorOrigem[i];
        j++;
    }
    
}

//copia o conteudo de origem em destino
void copiaVetorPonteiro(int *vetorOrigem, int inicioOrigem, int fimOrigem, int *vetorDestino, int inicioDestino){
    int j = inicioDestino;
    for (int i = inicioOrigem; i <= fimOrigem; i++)
    {
        vetorDestino[j] = vetorOrigem[i];
        j++;
    }
    
}


//Busca chave em página
bool ProcuraChavePagina(BTPAGE *pagina, int chave, int *posicao){
    //Procura no vetor de chaves por busca binaria
    bool encontrou;
    *posicao = BuscaBinaria(pagina, 0, (pagina->n)-1, chave, &encontrou);
    return encontrou;
}



//FUNÇÕES DO SPLIT 1 PARA 2

//Realiza split 1 para 2
void Split_1to2(FILE *arqArvore, CHAVE chave_in, int RRN_in, BTPAGE *pagina, 
                CHAVE *promoChave, int *promoFilho, BTPAGE *novaPagina){
    int pEst[MAXCHAVES+2];
    CHAVE chavesEst[MAXCHAVES+1];

    //prepara para split
    copiaVetorChave(pagina->chaves, 0, MAXCHAVES-1, chavesEst, 0);
    copiaVetorPonteiro(pagina->P, 0, MAXCHAVES, pEst, 0);

    InsereChave(chavesEst, pEst, MAXCHAVES+1, chave_in, RRN_in);

    novaPagina->nivel = pagina->nivel;

    CABECALHO_B *cabecalho = CabecalhoBCriar();
    fseek(arqArvore, 0, SEEK_SET);
    CabecalhoBLer(cabecalho, arqArvore);

    int RRN_novapagina = cabecalho->proxRRN;

    //Pega a chave do meio
    int metade = (int)(MAXCHAVES+1)/2;
    *promoChave = chavesEst[metade];
    *promoFilho = RRN_novapagina;

    //Deixa toda a pagina com -1
    ResetaPagina(pagina);

    //Primeiro ponteiro da pagina e da nova pagina
    pagina->P[0] = pEst[0];
    novaPagina->P[0] = pEst[metade+1];

    //Insere as chaves nas paginas com seus ponteiros a direita
    for (int i = 0; i < metade; i++)
    {
        InsereChavePagina(pagina, MAXCHAVES, chavesEst[i], pEst[i+1]);
        InsereChavePagina(novaPagina, MAXCHAVES, chavesEst[metade+1+i], pEst[metade+2+i]);
    }
    

    cabecalho->proxRRN = RRN_novapagina + 1;
    CabecalhoBReescreve(arqArvore, cabecalho);

    free(cabecalho);
}





//FUNÇÕES DA REDISTRIBUIÇÃO

void ConstroiVetorEstendido(BTPAGE *paginaMenor, int qtdMenor, BTPAGE *paginaMaior, 
                            int qtdMaior, BTPAGE *pai, int posPai, CHAVE *destinoC, int *destinoP){
    copiaVetorChave(paginaMenor->chaves, 0, qtdMenor-1, destinoC, 0);
    destinoC[qtdMenor] = pai->chaves[posPai];
    copiaVetorChave(paginaMaior->chaves, 0, qtdMaior-1, destinoC, qtdMenor+1);

    copiaVetorPonteiro(paginaMenor->P, 0, qtdMenor, destinoP, 0);
    copiaVetorPonteiro(paginaMaior->P, 0, qtdMaior, destinoP, qtdMenor+1);
}

//separa metades para redistribuição
void RedistribuiMetadeVetor(CHAVE *chaves, int ponteiros[], int tamanhoVetor, BTPAGE *paginaMenor, BTPAGE *paginaMaior){
    paginaMenor->P[0] = ponteiros[0];
    paginaMaior->P[0] = ponteiros[tamanhoVetor/2 + 1];

    for (int i = 0; i < tamanhoVetor; i++)
    {
        if(i < (tamanhoVetor /2)){
            InsereChavePagina(paginaMenor, MAXCHAVES, chaves[i], ponteiros[i+1]);
        }
        else if(i > (tamanhoVetor /2)){
            InsereChavePagina(paginaMaior, MAXCHAVES, chaves[i], ponteiros[i+1]);
        }
    }
}

//Realiza redistribuição
bool Redistribuicao(FILE *arqArvore, CHAVE chave_in, int RRN_in, BTPAGE *pagina, int RRN_Atual, int RRN_pai, lado lado){
    BTPAGE *paginaPai = PaginaCriar();
    BTPAGE *paginaIrma = PaginaCriar();

    LerPagina(arqArvore, RRN_pai, paginaPai);
    int posicao;
    ProcuraChavePagina(paginaPai, chave_in.C, &posicao);

    int posIrma;
    int posPai;
    int RRN_Irma;

    //ajusta variaveis dependendo do lado
    if(lado == ESQUERDA){
        posIrma = posicao-1;
        posPai = posIrma;
        if(posIrma < 0){
            free(paginaPai);
            free(paginaIrma);
            return false;
        }
        RRN_Irma = paginaPai->P[posIrma];
    }
    else{
        posIrma = posicao+1;
        posPai = posIrma-1;
        if(posIrma > MAXCHAVES){
            free(paginaPai);
            free(paginaIrma);  
            return false;
        }
        RRN_Irma = paginaPai->P[posIrma];
    }
    
    //se for raiz, não é distribuição
    if(RRN_Irma == -1){
        free(paginaPai);
        free(paginaIrma); 
        return false;
    }

    LerPagina(arqArvore, RRN_Irma, paginaIrma);

    int qtdChavesPagina = pagina->n;
    int qtdChavesPaginaIrma = paginaIrma->n;

    if(qtdChavesPaginaIrma >= MAXCHAVES){
        free(paginaPai);
        free(paginaIrma); 
        return false;
    }
    
    //construção dos vetores estendidos

    int tamanhoVetor = qtdChavesPagina + qtdChavesPaginaIrma + 2;
    CHAVE chaveEst[tamanhoVetor];
    int pEst[tamanhoVetor+1];

    if(lado == ESQUERDA)
        ConstroiVetorEstendido(paginaIrma, qtdChavesPaginaIrma, 
        pagina, qtdChavesPagina, paginaPai, posPai, chaveEst, pEst);
    else
        ConstroiVetorEstendido(pagina, qtdChavesPagina, paginaIrma, qtdChavesPaginaIrma, 
        paginaPai, posPai, chaveEst, pEst);
    
    //promove chave do meio
    InsereChave(chaveEst, pEst, tamanhoVetor, chave_in, RRN_in);

    paginaPai->chaves[posPai] = chaveEst[tamanhoVetor/2];

    ResetaPagina(pagina);
    ResetaPagina(paginaIrma);

    //distribui chaves
    if(lado == ESQUERDA)
        RedistribuiMetadeVetor(chaveEst, pEst, tamanhoVetor, paginaIrma, pagina);
    else
        RedistribuiMetadeVetor(chaveEst, pEst, tamanhoVetor, pagina, paginaIrma);
    

    EscrevePagina(arqArvore, RRN_Atual, pagina);
    EscrevePagina(arqArvore, RRN_Irma, paginaIrma);
    EscrevePagina(arqArvore, RRN_pai, paginaPai);

    free(paginaPai);
    free(paginaIrma);

    return true;
}





//FUNÇÕES DO SPLIT 2 PARA 3

//Distribui chaves do split 2 para 3
void Split3Vetores(CHAVE *chaves, int ponteiros[], int tamanhoVetor, BTPAGE *pagina1, BTPAGE *pagina2, BTPAGE* pagina3){
    pagina1->P[0] = ponteiros[0];
    pagina2->P[0] = ponteiros[tamanhoVetor/3 + 1];
    pagina3->P[0] = ponteiros[((tamanhoVetor/3)*2)+2];

    for (int i = 0; i < tamanhoVetor; i++)
    {
        if(i < (tamanhoVetor/3)){
            InsereChavePagina(pagina1, MAXCHAVES, chaves[i], ponteiros[i+1]);
        }
        else if(i > (tamanhoVetor/3) && i < ((tamanhoVetor/3)*2)+1){
            InsereChavePagina(pagina2, MAXCHAVES, chaves[i], ponteiros[i+1]);
        }else if (i > ((tamanhoVetor/3)*2)+1){
            InsereChavePagina(pagina3, MAXCHAVES, chaves[i], ponteiros[i+1]);
        }
    }
}

bool Split_2to3(FILE *arqArvore, CHAVE chave_in, int RRN_in, CHAVE *promoChave, int *promoFilho, BTPAGE* novaPagina,
                 BTPAGE *pagina, int RRN_Atual, int RRN_pai, lado lado){
    BTPAGE *paginaPai = PaginaCriar();
    LerPagina(arqArvore, RRN_pai, paginaPai);


    int posicao;
    ProcuraChavePagina(paginaPai, chave_in.C, &posicao);
        
    int posIrma;
    int posPai;
    int RRN_Irma;

    //ajusta variaveis dependendo do lado
    if(lado == DIREITA){
        posIrma = posicao+1;
        posPai = posIrma-1;
        if(paginaPai->P[posIrma] == -1 || posIrma > MAXCHAVES){
            free(paginaPai);
            return false;
        }
        RRN_Irma = paginaPai->P[posIrma];
    }
    else{
        posIrma = posicao-1;
        posPai = posIrma;
        if(posIrma < 0){
            free(paginaPai);
            return false;
        }
        RRN_Irma = paginaPai->P[posIrma];
    }
    
    //se for raiz
    if(RRN_Irma == -1){
        free(paginaPai);
        return false;
    }

    BTPAGE *paginaIrma = PaginaCriar();

    LerPagina(arqArvore, RRN_Irma, paginaIrma);


    //construção dos vetores estendidos
    int tamanhoVetor = (MAXCHAVES*2) + 2;
    CHAVE chaveEst[tamanhoVetor];
    int pEst[tamanhoVetor+1];


    if(lado == ESQUERDA)
        ConstroiVetorEstendido(paginaIrma, MAXCHAVES, pagina, MAXCHAVES, 
        paginaPai, posPai, chaveEst, pEst);
    else
        ConstroiVetorEstendido(pagina, MAXCHAVES, paginaIrma, MAXCHAVES, 
        paginaPai, posPai, chaveEst, pEst);
    
    InsereChave(chaveEst, pEst, tamanhoVetor, chave_in, RRN_in);


    //Cria nova pagina
    novaPagina->nivel = pagina->nivel;

    CABECALHO_B *cabecalho = CabecalhoBCriar();
    fseek(arqArvore, 0, SEEK_SET);
    CabecalhoBLer(cabecalho, arqArvore);

    int RRN_novapagina = cabecalho->proxRRN;
    cabecalho->proxRRN = RRN_novapagina + 1;

    //Primeiro promovido
    paginaPai->chaves[posPai] = chaveEst[tamanhoVetor/3];

    //Segundo Promovido
    *promoChave = chaveEst[((tamanhoVetor/3)*2)+1];
    *promoFilho = RRN_novapagina;

    ResetaPagina(pagina);
    ResetaPagina(paginaIrma);

    //separa chaves
    if(lado == ESQUERDA)
        Split3Vetores(chaveEst, pEst, tamanhoVetor, paginaIrma, pagina, novaPagina);
    else
        Split3Vetores(chaveEst, pEst, tamanhoVetor, pagina, paginaIrma, novaPagina);

    EscrevePagina(arqArvore, RRN_Atual, pagina);
    EscrevePagina(arqArvore, RRN_Irma, paginaIrma);
    EscrevePagina(arqArvore, RRN_pai, paginaPai);
    EscrevePagina(arqArvore, RRN_novapagina, novaPagina);
    
    free(paginaIrma);
    free(paginaPai);

    CabecalhoBReescreve(arqArvore, cabecalho);
    free(cabecalho);
    return true;
}




//FUNÇÕES DE INSERÇÃO

//Função recursiva da inserção na árvore
ValoresRetorno Insert(FILE *arqArvore, int RRN_Atual, CHAVE KEY, CHAVE *promoChave, int *promoFilho, int RRN_pai){

    //Cria Paginas Vazias
    BTPAGE *pagina = PaginaCriar();
    BTPAGE *novaPagina = PaginaCriar();
    int posicaoPagina;
    CHAVE chavePromovida;
    int rrnPromovido;

    //se nó não existe, promove a chave
    if(RRN_Atual == NIL){
        *promoChave = KEY;
        *promoFilho = NIL;

        free(novaPagina);
        free(pagina);
        return PROMOTION;
    }else{
        //leia página RRN_Atual e armazene em PAGE
        LerPagina(arqArvore, RRN_Atual, pagina);

        //procure por KEY em PAGE
        if(ProcuraChavePagina(pagina, KEY.C, &posicaoPagina)){
            free(novaPagina);
            free(pagina);
            return ERROR;
        } 
        
        //passo recursivo
        ValoresRetorno RETURN_VALUE = Insert(arqArvore, pagina->P[posicaoPagina], KEY, &chavePromovida, &rrnPromovido, RRN_Atual);

        if(RETURN_VALUE == NO_PROMOTION || RETURN_VALUE == ERROR){
            free(novaPagina);
            free(pagina);
            return RETURN_VALUE;
        }
        //se ainda existe espaço na página
        else if(pagina->n < MAXCHAVES){
            LerPagina(arqArvore, RRN_Atual, pagina);
            //Inserir Chave e RRN Promovidos
            InsereChavePagina(pagina, MAXCHAVES, chavePromovida, rrnPromovido);
            EscrevePagina(arqArvore, RRN_Atual, pagina);

            free(novaPagina);
            free(pagina);

            return NO_PROMOTION;
        }
        else{
            LerPagina(arqArvore, RRN_Atual, pagina);
            
            //Tenta redistribuição à esquerda
            if(RRN_pai != -1 && Redistribuicao(arqArvore, chavePromovida, rrnPromovido, 
                pagina, RRN_Atual, RRN_pai, ESQUERDA)){
                free(novaPagina);
                free(pagina);
                return NO_PROMOTION;
                
            }
            //Se não der, redistribuição à direita
            else if(RRN_pai != -1 && Redistribuicao(arqArvore, chavePromovida, rrnPromovido, 
                    pagina, RRN_Atual, RRN_pai, DIREITA)){
                free(novaPagina);
                free(pagina);
                return NO_PROMOTION;
            }
            else{
                //Caso não seja possivel redistribuição
                //split 1-to-2 (NÓ RAIZ)
                if(RRN_pai == -1){
                    Split_1to2(arqArvore, chavePromovida, rrnPromovido, pagina, promoChave, promoFilho, novaPagina);
                    //escreva PAGE no arquivo na posição RRN_Atual
                    //escreva NEWPAGE no arquivo na posição promoFilho
                    EscrevePagina(arqArvore, RRN_Atual, pagina);
                    EscrevePagina(arqArvore, *promoFilho, novaPagina);
                }else{

                    //split 2-to-3 (DEMAIS NÓS) à direita
                    if(Split_2to3(arqArvore, chavePromovida, rrnPromovido, promoChave, promoFilho, 
                        novaPagina, pagina, RRN_Atual, RRN_pai, DIREITA)){
                        
                    }
                    //se não der, split 2-to-3 (DEMAIS NÓS) à esquerda
                    else if(Split_2to3(arqArvore, chavePromovida, rrnPromovido, promoChave, promoFilho,
                        novaPagina, pagina, RRN_Atual, RRN_pai, ESQUERDA)){

                    }
                }       
                free(novaPagina);
                free(pagina);
                return PROMOTION;  
            }
        }
    } 
}

//Insere chave nos vetores respectivos
int InsereChave(CHAVE *chaves, int *ponteiros, int tamanho, CHAVE chave, int RRN_Direita){
    //algoritmo usado esta disponivel em https://www.sanfoundry.com/c-program-insert-element-specified-position-array/
    int pos;

    for(int i=0; i<tamanho; i++){
        if(i == tamanho -1){
            pos = i;
            break;
        }

        if(chaves[i].C == NIL){
            chaves[i] = chave;
            ponteiros[i+1] = RRN_Direita;
            return i;
        }

        if(chave.C < chaves[i].C){
            pos = i;
            break;
        }

    }

    if(pos != tamanho){
        int deslocamento = tamanho - pos - 1;

        for(int i=0; i<deslocamento; i++){
           chaves[tamanho - i - 1] = chaves[tamanho - i - 2];
           ponteiros[tamanho - i] = ponteiros[tamanho - i - 1];
        }
    }
    chaves[pos] = chave;
    ponteiros[pos+1] = RRN_Direita;
    
    return pos;
}

//Insere chave na página
int InsereChavePagina(BTPAGE *pagina, int tamanho, CHAVE chave, int RRN_Direita){
    int retorno = InsereChave(pagina->chaves, pagina->P, tamanho, chave, RRN_Direita);
    pagina->n = (pagina->n) + 1;
    return retorno;
}







//FUNÇÕES DE BUSCA

//Busca binária na página de disco
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
    
    if (chave < pagina->chaves[centro].C)
		return (centro);
	if (chave > pagina->chaves[centro].C)
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

//busca na árvore B*
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
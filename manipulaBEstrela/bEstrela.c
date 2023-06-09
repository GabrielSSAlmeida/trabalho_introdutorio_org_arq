#include "bEstrela.h"
#include "../arquivos/arquivos.h"
#include "cabecalhoBEstrela.h"
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
        pagina->chaves[0] = chave;
        pagina->P[0] = pEsquerdo;
        pagina->P[1] = pDireito;


        //ARRUMAR     TEM UM ERRO
        for (int i = 1; i < MAXCHAVES+1; i++)
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

void lerPagina(FILE* arqArvore, int CURRENT_RRN, BTPAGE* pagina){
    //Va ate a pagina
    fseek(arqArvore, CURRENT_RRN*TAM_PAGE, SEEK_SET);


    fread(&(pagina->nivel), 4, 1, arqArvore);
    fread(&(pagina->n), 4, 1, arqArvore);

    for (int i = 0; i < (pagina->n); i++)
    {
        fread(&(pagina->P[i]), 4, 1, arqArvore);
        fread(&(pagina->chaves[i].C), 4, 1, arqArvore);
        fread(&(pagina->chaves[i].Pr), 8, 1, arqArvore);
    }
    
    if((pagina->n) == 4){
        fread(&(pagina->chaves[4].Pr), 8, 1, arqArvore);
    }


}


bool ArvoreCriar(char nomeArquivo[], char arquivoDados[]){
    FILE *arvore, *dados;
    if(!AbreArquivo(&arvore, nomeArquivo, "wb", NULL)) return false;

    CABECALHO_B *cabecalhoArvore = CabecalhoBCriar();
    CabecalhoBEscreve(arvore, cabecalhoArvore);

    if(!AbreArquivo(&dados, arquivoDados, "rb", NULL)) return false;
    CABECALHO *cabecalhoDados = CabecalhoCriar();
    LeCabecalhoDoArqBinario(cabecalhoDados, dados);

    //Percorre arquivo binario lendo os registros
    long int offsetProximoRegistro = 17; //será o offset do proximo registro durante o loop
    long int offsetInicioRegistro = 17;
    DADOS *registroAuxiliar = RegistroCriar();

    //A flag verifica se o id foi lido e serve para saber se chegou no final do arquivo
    int flag = LerRegBinario(dados, registroAuxiliar, &offsetProximoRegistro);

    //pos é utilizada para armazenar a posicao que o campo deve ser inserido
    int pos = 0;
    int i;
    //le registros até o final do arquivo, inserindo no vetor de indices
    for(i = 0; flag != 0; i++){
        if(GetRegistroRemovido(registroAuxiliar) == '0'){




            //INSERCAO NA ARVORE!!!




            pos++;
        }

        DesalocaCamposVariaveis(registroAuxiliar);

        offsetInicioRegistro = offsetProximoRegistro;

        flag = LerRegBinario(dados, registroAuxiliar, &offsetProximoRegistro); 
    }

    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();
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

        free(novaPagina);
    }


    //atualiza RRN raiz
    cabecalho->noRaiz = root;
    CabecalhoBReescreve(arvore, cabecalho);

}

ValoresRetorno Insert(FILE*arqArvore, int CURRENT_RRN, CHAVE KEY, CHAVE *PROMO_KEY, int *PROMO_R_CHILD){

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

        return PROMOTION;
    }else{
        //leia página CURRENT_RRN e armazene em PAGE
        lerPagina(arqArvore, CURRENT_RRN, pagina);
        //procure por KEY em PAGE

        //faça POS igual a posição em que KEY ocorre ou deveria ocorrer
    }


    free(pagina);
    free(novaPagina);
}
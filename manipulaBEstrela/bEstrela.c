#include "bEstrela.h"
#include "../arquivos/arquivos.h"
#include "cabecalhoBEstrela.h"
#include "../cabecalho/cabecalho.h"
#include "../manipulaRegistros/registro.h"
#include "../manipulaIndices/inteiro/indiceInteiro.h"

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

    DADOS_INT key;
    key.chaveBusca = registro->idCrime;
    key.byteOffset = byteoffset;

    DADOS_INT promo_key;
    int promo_r_child;

    if(Insert(root, key, &promo_key, &promo_r_child) == PROMOTION){
        //crie nova página raiz com key:=PROMO_KEY, l_child:=ROOT, r_child:=PROMO_R_CHILD
        //faça ROOT igual ao RRN da nova página raiz
    }


    //atualiza RRN raiz
    cabecalho->noRaiz = root;
    CabecalhoBReescreve(arvore, cabecalho);

}

ValoresRetorno Insert(int CURRENT_RRN, DADOS_INT KEY, DADOS_INT *PROMO_KEY, int *PROMO_R_CHILD){
    //se nó não existe, promove a chave
    if(CURRENT_RRN == NIL){
        *PROMO_KEY = KEY;
        *PROMO_R_CHILD = NIL;

        return PROMOTION;
    }else{
        //leia página CURRENT_RRN e armazene em PAGE
        //procure por KEY em PAGE
        //faça POS igual a posição em que KEY ocorre ou deveria ocorrer
    }


}
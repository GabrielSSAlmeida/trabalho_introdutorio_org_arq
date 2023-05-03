#include "Indices.h"
#include "../utils/utils.h"
#include "../prints_e_erros/prints_e_erros.h"

struct dadosIndiceInt{
    int chaveBusca;                    
    long int byteOffset;            //número relativo do registro do arquivo de dados referente à chave de busca
};

struct dadosIndiceString{
    char chaveBusca[12];                    
    long int byteOffset;            //número relativo do registro do arquivo de dados referente à chave de busca
};

bool CriaIndiceInteiro(char arqEntrada[], char arqSaida[], char campo[]){
    FILE *arqBin;
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalho = CabecalhoIndiceCriar();

    //abre os arquivos recebidos na entrada
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", arqBin, NULL)) return false;

    //Escreve cabecalho
    EscreveCabecalhoIndice(arqIndice, cabecalho);

    //fecha para atualizar em disco
    fclose(arqIndice);

    //abre novamente para uso
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", NULL)) return false;

    //ajusta o ponteiro para depois do cabecalho
    fseek(arqBin, 5, SEEK_SET);

}

bool CriaIndiceString(char arqEntrada[], char arqSaida[], char campo[]){
    FILE *arqBin;
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalho = CabecalhoIndiceCriar();

    //abre os arquivos recebidos na entrada
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", arqBin, NULL)) return false;

    //Escreve cabecalho
    EscreveCabecalhoIndice(arqIndice, cabecalho);

    //fecha para atualizar em disco
    fclose(arqIndice);

    //abre novamente para uso
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", NULL)) return false;

    //ajusta o ponteiro para dps do cabecalho
    fseek(arqBin, 5, SEEK_SET);



}
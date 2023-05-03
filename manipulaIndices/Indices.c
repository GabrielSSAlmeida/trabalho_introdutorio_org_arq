#include "Indices.h"
#include "../utils/utils.h"
#include "../arquivos/arquivos.h"
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
    fseek(arqIndice, 5, SEEK_SET);

    //Aloca cabecalho e registro auxiliar para percorrer arquivo binario
    CABECALHO *cabecalho_registro = CabecalhoCriar();
    DADOS *registro_auxiliar = RegistroCriar();

    LeCabecalhoDoArqBinario(cabecalho_registro, arqBin);

    //Calcula o numero de registros não removidos e cria um vetor para os indices
    int nroRegistros = GetNroRegArq(cabecalho_registro) - GetNroRegRem(cabecalho_registro);
    DADOS_INT indices[nroRegistros];

    int tipoCampo = -1;
    if(strcmp(campo, "idCrime") == 0){
        tipoCampo = 0;
    }else if(strcmp(campo, "numeroArtigo") == 0){
        tipoCampo = 1;
    }else{
        ErroArquivo();
        return false;
    }



    //Percorre arquivo binario lendo os registros
    int flag = LerRegBinario(arqBin, registro_auxiliar);
    int i;
    for(i=0; flag!=0; i++){
        if(GetRegistroRemovido(registro_auxiliar) == '0'){
            
        }

        


        DesalocaCamposVariaveis(registro_auxiliar);

        flag = LerRegBinario(arqBin, registro_auxiliar); 
    }


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_registro);
    DesalocaRegistro(registro_auxiliar);

    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();
    return true;
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
    fseek(arqIndice, 5, SEEK_SET);



}
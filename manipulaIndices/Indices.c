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
    int offsetlido = 17; //será o offset do proximo registro durante o loop
    int offsetanterior = 17;

    int flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido);
    int i;
    for(i=0; flag!=0; i++){
        if(GetRegistroRemovido(registro_auxiliar) == '0'){
            InsereCampoIntEmIndices(indices, registro_auxiliar, i, tipoCampo, offsetanterior);
        }

        


        DesalocaCamposVariaveis(registro_auxiliar);

        offseranterior = offsetlido;

        flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido); 
    }


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_registro);
    DesalocaRegistro(registro_auxiliar);

    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();
    return true;
}


bool InsereCampoIntEmIndices(DADOS_INT *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset){
    DADOS_INT *registroDados = RegistroDadosIntCriar();
    
    switch(campo){
        //caso idCrime
        case 0:{
            registroDados->chaveBusca = GetRegistroIdCrime(registro_auxiliar);
            registroDados->byteOffset = byteoffset;

            vetor[posicao].chaveBusca = registroDados->chaveBusca;
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;   
        }
            
        //caso numeroArtigo
        case 1:{
            registroDados->chaveBusca = GetRegistroNroArtigo(registro_auxiliar);
            registroDados->byteOffset = byteoffset;

            vetor[posicao].chaveBusca = registroDados->chaveBusca;
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }     
    }

    free(registroDados);
}

bool InsereCampoStringEmIndices(DADOS_STR *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset){
    DADOS_STR *registroDados = RegistroDadosIntCriar();
    
    switch(campo){
        //caso dataCrime
        case 0:{
            strncpySem0(registroDados->chaveBusca, GetRegistroDataCrime(registro_auxiliar));
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;   
        }
            
        //caso marcaCelular
        case 1:{
            strncpySem0(registroDados->chaveBusca, GetRegistroMarcaCelular(registro_auxiliar));
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }    

        //caso lugarCrime
        case 2:{
            strncpySem0(registroDados->chaveBusca, GetRegistroLugarCrime(registro_auxiliar));
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }  

        //caso descricaoCrime
        case 3:{
            strncpySem0(registroDados->chaveBusca, GetRegistroDescricaoCrime(registro_auxiliar));
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }   
    }

    free(registroDados);
}

DADOS_INT *RegistroDadosIntCriar(void){
    DADOS_INT *registro = (DADOS_INT*) calloc(1, sizeof(DADOS_INT));

    //inicializa registro
    if(registro != NULL){
        registro->chaveBusca = 0;
        registro->byteOffset = 0;
    }
    else{
        ErroAlocacao();
    }
    return registro;
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

    //ajusta o ponteiro para depois do cabecalho
    fseek(arqIndice, 5, SEEK_SET);

    //Aloca cabecalho e registro auxiliar para percorrer arquivo binario
    CABECALHO *cabecalho_registro = CabecalhoCriar();
    DADOS *registro_auxiliar = RegistroCriar();

    LeCabecalhoDoArqBinario(cabecalho_registro, arqBin);

    //Calcula o numero de registros não removidos e cria um vetor para os indices
    int nroRegistros = GetNroRegArq(cabecalho_registro) - GetNroRegRem(cabecalho_registro);
    DADOS_STR indices[nroRegistros];

    int tipoCampo = -1;
    if(strcmp(campo, "dataCrime") == 0){
        tipoCampo = 0;
    }else if(strcmp(campo, "marcaCelular") == 0){
        tipoCampo = 1;
    }else if(strcmp(campo, "lugarCrime") == 0){
        tipoCampo = 2;
    }else if(strcmp(campo, "descricaoCrime") == 0){
        tipoCampo = 3;
    }else{
        ErroArquivo();
        return false;
    }



    //Percorre arquivo binario lendo os registros
    int offsetlido = 17; //será o offset do próximo registro durante o loop
    int offsetanterior = 17;

    int flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido);
    int i;
    for(i=0; flag!=0; i++){
        if(GetRegistroRemovido(registro_auxiliar) == '0'){
            InsereCampoStringEmIndices(indices, registro_auxiliar, i, tipoCampo, offsetanterior);
        }

        


        DesalocaCamposVariaveis(registro_auxiliar);

        offseranterior = offsetlido;

        flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido); 
    }


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_registro);
    DesalocaRegistro(registro_auxiliar);

    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();
    return true;

}
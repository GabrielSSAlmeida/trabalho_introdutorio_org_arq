#include "funcionalidades.h"


bool funcionalidade1(){
    char arqEntrada[32], arqSaida[32];
    FILE *arqCsv;
    FILE *arqBin;

    //abertura dos arquivos para funcionalidade 1
    RecebeEntradaFunc1(arqEntrada, arqSaida);
    
    //Abre arquivos csv e binario
    //Se a abertura falhar para o código 
    if(!AbreArquivo(&arqCsv, arqEntrada, "r", NULL)) return false;
    if(!AbreArquivo(&arqBin, arqSaida, "wb+", arqCsv, NULL)) return false;

    //Cria o cabecalho que será usado no arquivo binario
    CABECALHO *cabecalho_saida = CabecalhoCriar();
    
    //Ler e ignorar a primeira linha do arquivo CSV
    IgnoraLinha(arqCsv);

    //atualiza o status do cabecalho
    AtualizaStatus(cabecalho_saida, '0');

    //escreve cabecalho no arquivo binario
    EscreveCabecalho(arqBin,cabecalho_saida);

    //fecha arquivo para salvar status
    fclose(arqBin);

    //abre o arquivo novamente
    if(!AbreArquivo(&arqBin, arqSaida, "wb+", NULL)) return false;

    //ajusta o ponteiro para dps do cabecalho
    fseek(arqBin, GetByteOffset(cabecalho_saida), SEEK_SET);

    //Le registros do csv e escreve no binario
    CsvParaBin(arqCsv, arqBin, cabecalho_saida);
    
    //atualiza o status do cabecalho
    AtualizaStatus(cabecalho_saida, '1');
    
    //atualiza o cabecalho
    fseek(arqBin, 0, SEEK_SET);
    EscreveCabecalho(arqBin,cabecalho_saida);

    //fecha arquivos
    fclose(arqCsv);
    fclose(arqBin);
    
    binarioNaTela(arqSaida);

    DesalocaCabecalho(cabecalho_saida);

    return true;
}


bool funcionalidade2(){
    char arqEntrada[32];
    FILE *arqBin;

    RecebeEntradaFunc2(arqEntrada);

    //abre arquivo binario para funcionalidade 2
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;

    //imprime os registro do arquivo binario na tela
    if(!ImprimirBinario(arqBin)){
        fclose(arqBin);
        return false;
    }

    fclose(arqBin);

    return true;
}

bool funcionalidade3(){
    char arqEntrada[32];
    char arqSaida[32];
    char campo[32];
    char dado[32];

    RecebeEntradaFunc3(arqEntrada, campo, dado, arqSaida);

    //verifica se o tipo de dado é string ou int
    int dadoVerificado = VerificaDado(dado);

    switch(dadoVerificado){
        //caso int
        case 0:{
            CriaIndiceInteiro(arqEntrada, arqSaida, campo);
            break;
        }
        //caso string
        case 1:{
            CriaIndiceString(arqEntrada, arqSaida, campo);
            break;
        }
    }

    return true;
}
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

    //Le registros do csv e escreve no binario
    CsvParaBin(arqCsv, arqBin, cabecalho_saida);
    
    //atualiza o status do cabecalho
    AtualizaStatus(cabecalho_saida, '1');
    
    //atualiza o cabecalho
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
#include "arquivos.h"
#include "../prints_e_erros/prints_e_erros.h"


/*
    Abre arquivo e retorna se foi bem sucedido.
    Função usa parâmetros variados para fechar arquivos já abertos.
*/ 
bool AbreArquivo(FILE **arq, char nomeArquivo[], char modo[], ...){
    *arq = fopen(nomeArquivo, modo);
    //Se abertura falhar, verifica todos os parâmetros variados e fechar os arquivos já abertos
    if(*arq == NULL){
        va_list args;
        va_start(args, modo);

        FILE *aux;
        //Passa por todos os parâmetros variados até encontrar NULL
        while ((aux = va_arg(args, FILE *))){
            fclose(aux);
        }
        
        ErroArquivo();
        return false;
    }
    return true;
}

//Le registros do csv e escreve no binario
void CsvParaBin(FILE *arqCsv, FILE *arqBin, CABECALHO *cabecalho_saida){
    int flag;
    //leitura dos registros um a um no arquivo CSV
    while(feof(arqCsv) == 0){
        DADOS *registro = LerRegistroCsv(arqCsv, &flag);
        //se não conseguir ler registro, desaloca (trata o erro de uma linha ter só "\n")
        if(flag != -1)
            //se conseguir, escreve
            EscreverRegistroBin(arqBin, registro, cabecalho_saida);

        DesalocaRegistro(registro);
    }
}

//Verifica se o arquivo está consistente
bool VerificaStatusArqBin(char *nomeArq){
    FILE *arq;
    char aux;

    if(!AbreArquivo(&arq, nomeArq, "rb")) return false;

    fread(&aux, 1, 1, arq);

    switch(aux){
        case '0':
            fclose(arq);
            return false;
            break;
        case '1':
            fclose(arq);
            return true;
            break;
    }
}
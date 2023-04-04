#include "auxiliar.h"

//recebe nome dos arquivos na funcionalidade 1
void RecebeEntradaFunc1(char str1[], char str2[]){
    scanf("%s", str1);
    scanf("%s", str2);
}

//recebe nome dos arquivos na funcionalidade 2
void RecebeEntradaFunc2(char str1[]){
    scanf("%s", str1);
}

//imprime campos variaveis na funcionalidade 2
void ImprimeCampoVariavel(FILE *arqBin, char *caracter){
    fread(caracter, 1, 1, arqBin);
    do{
        if(*caracter == '|'){
            printf("NULO");
            break;
        }
        printf("%c", *caracter);
        fread(caracter, 1, 1, arqBin);
    }while(*caracter != '|');
        
    printf(", ");
}
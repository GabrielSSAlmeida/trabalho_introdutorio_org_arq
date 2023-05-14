/*
- Gabriel Sousa Santos de Almeida - 13837432 (100%)
- Luís Henrique Giorgetti Dantas - 13782369 (100%)
- João Gabriel Manfre Nazar - 13733652  (100%)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades/funcionalidades.h"


int main(){
    int variavelMenu;

    scanf("%d", &variavelMenu);

    char arqEntrada[32];
    char nomeArqIndice[32];
    char campoIndexado[32];
    char dado[32];
    int n;

    switch(variavelMenu){
        case 1:{
            funcionalidade1();
            break;
        }
        case 2:{   
            funcionalidade2();
            break;
        }
        case 3:{
            funcionalidade3();
            break;
        }
        case 4:{
            RecebeEntradaFunc4e5(arqEntrada, campoIndexado, dado, nomeArqIndice, &n);

            if(!(VerificaStatusArqBin(arqEntrada) && VerificaStatusArqBin(nomeArqIndice))){
                ErroArquivo();
                break;
            }

            funcionalidade4(arqEntrada, nomeArqIndice, campoIndexado, dado, n);
            break;
        }
        case 5:{
            RecebeEntradaFunc4e5(arqEntrada, campoIndexado, dado, nomeArqIndice, &n);

            if(!(VerificaStatusArqBin(arqEntrada) && VerificaStatusArqBin(nomeArqIndice))){
                ErroArquivo();
                break;
            }

            funcionalidade5(arqEntrada, nomeArqIndice, campoIndexado, dado, n);
            binarioNaTela(arqEntrada);
            binarioNaTela(nomeArqIndice);
            break;
        }
        case 6:{
            RecebeEntradaFunc6(arqEntrada, campoIndexado, dado, nomeArqIndice, &n);

            if(!(VerificaStatusArqBin(arqEntrada) && VerificaStatusArqBin(nomeArqIndice))){
                ErroArquivo();
                break;
            }

            funcionalidade6(arqEntrada, nomeArqIndice, campoIndexado, dado, n);
            binarioNaTela(arqEntrada);
            binarioNaTela(nomeArqIndice);
            break;
        }
        case 7:{
            RecebeEntradaFunc6(arqEntrada, campoIndexado, dado, nomeArqIndice, &n);

            if(!(VerificaStatusArqBin(arqEntrada) && VerificaStatusArqBin(nomeArqIndice))){
                ErroArquivo();
                break;
            } 
            


            if(funcionalidade7(arqEntrada, nomeArqIndice, campoIndexado, dado, n)){
                binarioNaTela(arqEntrada);
                binarioNaTela(nomeArqIndice); 
            }
        }
    }

    return 0;
}
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

    char nomeArquivo1[32];
    char nomeArquivo2[32];
    char campoIndexado[32];
    char dado[32];
    int n;

    switch(variavelMenu){
        case 1:{
            RecebeEntradaFunc1(nomeArquivo1, nomeArquivo2);

            funcionalidade1(nomeArquivo1, nomeArquivo2);
            binarioNaTela(nomeArquivo2);
            break;
        }
        case 2:{
            RecebeEntradaFunc2(nomeArquivo1);
            funcionalidade2(nomeArquivo1);
            break;
        }
        case 3:{
            RecebeEntradaFunc3(nomeArquivo1, campoIndexado, dado, nomeArquivo2);

            if(!(VerificaStatusArqBin(nomeArquivo1))){
                ErroArquivo();
                break;
            }

            funcionalidade3(nomeArquivo1, nomeArquivo2, campoIndexado, dado);

            binarioNaTela(nomeArquivo2);
            break;
        }
        case 4:{
            RecebeEntradaFuncs4a7(nomeArquivo1, nomeArquivo2, campoIndexado, dado, &n);

            if(!(VerificaStatusArqBin(nomeArquivo1) && VerificaStatusArqBin(nomeArquivo2))){
                ErroArquivo();
                break;
            }

            funcionalidade4(nomeArquivo1, nomeArquivo2, campoIndexado, dado, n);
            break;
        }
        case 5:{
            RecebeEntradaFuncs4a7(nomeArquivo1, nomeArquivo2, campoIndexado, dado, &n);

            if(!(VerificaStatusArqBin(nomeArquivo1) && VerificaStatusArqBin(nomeArquivo2))){
                ErroArquivo();
                break;
            }

            funcionalidade5(nomeArquivo1, nomeArquivo2, campoIndexado, dado, n);
            binarioNaTela(nomeArquivo1);
            binarioNaTela(nomeArquivo2);
            break;
        }
        case 6:{
            RecebeEntradaFuncs4a7(nomeArquivo1, nomeArquivo2, campoIndexado, dado, &n);

            if(!(VerificaStatusArqBin(nomeArquivo1) && VerificaStatusArqBin(nomeArquivo2))){
                ErroArquivo();
                break;
            }

            funcionalidade6(nomeArquivo1, nomeArquivo2, campoIndexado, dado, n);
            binarioNaTela(nomeArquivo1);
            binarioNaTela(nomeArquivo2);
            break;
        }
        case 7:{
            RecebeEntradaFuncs4a7(nomeArquivo1, nomeArquivo2, campoIndexado, dado, &n);

            if(!(VerificaStatusArqBin(nomeArquivo1) && VerificaStatusArqBin(nomeArquivo2))){
                ErroArquivo();
                break;
            } 

            if(funcionalidade7(nomeArquivo1, nomeArquivo2, campoIndexado, dado, n)){
                binarioNaTela(nomeArquivo1);
                binarioNaTela(nomeArquivo2); 
            }
        }
    }

    return 0;
}
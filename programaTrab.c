#include <stdio.h>
#include <stdlib.h>
#include "auxiliares/auxiliar.h"
#include "manipulaRegistros/registro.h"

int main(){
    int variaMenu;

    scanf("%d", &variaMenu);

    switch(variaMenu){
        case 1:{
            char arqEntrada[32], arqSaida[32];
        
            RecebeEntradaFunc1(arqEntrada, arqSaida);

            printf("%s %s\n", arqEntrada, arqSaida);

            CABECALHO *cabecalho_saida = criar_cabecalho();
            
            //FILE *arqCsv = fopen(arqEntrada, "r");
            //loop
                //DADOS* registro = LerRegCsv(arqCsv);
                //EscreverRegBin();
            

            //fclose(arqCsv);
            desaloca_cabecalho(cabecalho_saida);
            break;
        }
        case 2:{   
            char arqEntrada[32];

            RecebeEntradaFunc2(arqEntrada);

            //ImprimirBinario();

            break;
        }
    }

    return 0;
}



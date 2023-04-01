/*
- Gabriel Sousa Santos de Almeida - 13837432
- Luís Henrique Giorgetti Dantas - 13782369
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
            
            FILE *arqCsv = fopen(arqEntrada, "r");

            //Ler a primeira linha
            fscanf(arqCsv, "%[^\r\n]s"); 
            fscanf(arqCsv, "%[\r\n]s");
            
            //loop
            DADOS* registro = LerRegCsv(arqCsv);
                //EscreverRegBin();
            
            
            fclose(arqCsv);
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



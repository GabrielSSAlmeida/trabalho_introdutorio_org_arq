/*
- Gabriel Sousa Santos de Almeida - 13837432
- Luís Henrique Giorgetti Dantas - 13782369
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares/auxiliar.h"
#include "manipulaRegistros/registro.h"
#include "prints_e_erros/prints_e_erros.h"





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
            FILE *arqBin = fopen(arqSaida, "wb+");
            if(arqCsv == NULL || arqBin == NULL)
                imprime_erro_arquivo();
            
            //Ler a primeira linha
            fscanf(arqCsv, "%*[^\r\n]s"); 
            fscanf(arqCsv, "%*[\r\n]s");


            while(feof(arqCsv) == 0){
                int flag;
                DADOS *registro = LerRegCsv(arqCsv, &flag);
                if(flag != -1){
                    EscreverRegBin(arqBin, registro, cabecalho_saida);
                }

                desaloca_registro(registro);
            }

            imprime_cabecalho(arqBin,cabecalho_saida);



            fclose(arqCsv);
            fclose(arqBin);
            
            binarioNaTela(arqSaida);

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



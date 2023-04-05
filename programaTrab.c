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
    int variavelMenu;

    scanf("%d", &variavelMenu);

    switch(variavelMenu){
        case 1:{
            char arqEntrada[32], arqSaida[32];
        
            //abertura dos arquivos para funcionalidade 1
            RecebeEntradaFunc1(arqEntrada, arqSaida);
            
            FILE *arqCsv = fopen(arqEntrada, "r");
            if(arqCsv == NULL){
                ErroArquivo();
                break;
            }   

            FILE *arqBin = fopen(arqSaida, "wb+");
            if(arqCsv == NULL){
                fclose(arqCsv);
                ErroArquivo();
                break;
            }
            
            CABECALHO *cabecalho_saida = CabecalhoCriar();

            //Ler e ignorar a primeira linha do arquivo CSV
            fscanf(arqCsv, "%*[^\r\n]s"); 
            fscanf(arqCsv, "%*[\r\n]s");

            //escreve cabecalho no arquivo binario
            EscreveCabecalho(arqBin,cabecalho_saida, '0');

            //leitura dos registros no arquivo CSV
            int flag;
            while(feof(arqCsv) == 0){
                DADOS *registro = LerRegistroCsv(arqCsv, &flag);
                //se não conseguir ler registro, desaloca (trata o erro de uma linha ter só "\n")
                if(flag != -1)
                    EscreverRegistroBin(arqBin, registro, cabecalho_saida);

                DesalocaRegistro(registro);
            }

            //atualiza o cabecalho
            EscreveCabecalho(arqBin,cabecalho_saida, '1');

            //fecha arquivos
            fclose(arqCsv);
            fclose(arqBin);
            
            binarioNaTela(arqSaida);

            DesalocaCabecalho(cabecalho_saida);
            break;
        }
        case 2:{   
            char arqEntrada[32];

            //abre arquivo binario para funcionalidade 2
            RecebeEntradaFunc2(arqEntrada);
            FILE *arqBin = fopen(arqEntrada, "rb");
            if(arqBin == NULL){
                ErroArquivo();
                break;
            }

            //imprime os registro do arquivo binario na tela
            ImprimirBinario(arqBin);

            fclose(arqBin);

            break;
        }
    }

    return 0;
}



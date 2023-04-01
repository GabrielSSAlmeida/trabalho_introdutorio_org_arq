#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares/auxiliar.h"
#include "manipulaRegistros/registro.h"

char *ler_ate_virgula(FILE *arq){
    char c;
    char *vetor =NULL;
    int num_carac = 0;

    fscanf(arq, "%c", &c); 
    while(c != ','){    
        num_carac++;
    
        vetor = realloc(vetor, sizeof(char)* num_carac);
        if (vetor == NULL)
        {
            printf("Erro");
            exit(1);
        }
        vetor[num_carac-1] = c;


        
        if (fscanf(arq, "%c", &c) == EOF)
        {
            break;
        }
        
    }

    num_carac++;
    
    vetor = realloc(vetor, sizeof(char)* num_carac);
    if (vetor == NULL)
    {
        printf("Erro");
        exit(1);
    }
    vetor[num_carac-1] = '\0';

    return vetor;    
}







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
            //loop
            //DADOS* registro = LerRegCsv(arqCsv);
                //EscreverRegBin();
            
            int idCrime;                    
            char dataCrime[11];
            int numeroArtigo;
            char marcaCelular[13];
            char *lugarCrime;
            char *descricaoCrime;

            char *linha;
            //Ler a primeira linha
            fscanf(arqCsv, "%*[^\r\n]s"); 
            fscanf(arqCsv, "%*[\r\n]s");
            while(feof(arqCsv)==0){

                fscanf(arqCsv, " %m[^\n]s", &linha);

                printf("%s\n", linha);

                char *pt = strtok(linha, ",");
                while(pt){
                    if(pt == NULL){
                        printf("asjdhas");
                    }
                    printf("token: %s\n", pt);
                    pt = strtok(NULL, ",");
                }


                free(linha);
                /* fscanf(arqCsv, " %d,%10s,%d,", &idCrime, dataCrime, &numeroArtigo);


                lugarCrime = ler_ate_virgula(arqCsv);
                descricaoCrime = ler_ate_virgula(arqCsv);

                fscanf(arqCsv, "%s", marcaCelular);

                printf("%d %s %d %s %s %s\n", idCrime, dataCrime, numeroArtigo, lugarCrime, descricaoCrime, marcaCelular); */

            }
            

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



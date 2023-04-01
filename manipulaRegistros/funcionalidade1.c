#include "registro.h"
#include "../prints_e_erros/prints_e_erros.h"

struct cabecalho{
    char status;                    //indica consistencia do arquivo (0 - inconsistente, 1 - consistente) 
    long int proxByteOffset;        //indica o valor do proximo ByteOffset 
    int nroRegArq;                  // armazena o numero de registros no arquivo (total, inclusive removidos)                
    int nroRegRem;                  // armazena o numero de registros removidos
};

struct dados{
    char removido;                  //indica se a struct foi removida (1 = removido, 0 = nao)
    int idCrime;                    
    char dataCrime[10];
    int numeroArtigo;
    char marcaCelular[12];
    char *lugarCrime;
    char *descricaoCrime;
    char delimitador;               //delimitador do fim da struct
};

//================================================================================
CABECALHO *criar_cabecalho(void){
    CABECALHO *cabecalho = (CABECALHO*) malloc(sizeof(CABECALHO));

    if(cabecalho != NULL){
        cabecalho->proxByteOffset = 0;
        cabecalho->nroRegArq = 0;
        cabecalho->nroRegRem = 0;
    }
    else{
        imprime_erro_alocacao();
    }
    return cabecalho;
}

DADOS *criar_registro(void){
    DADOS *registro = (DADOS*) malloc(sizeof(DADOS));

    if(registro != NULL){
        registro->removido = 0;
        registro->delimitador = '#';
    }
    else{
        imprime_erro_alocacao();
    }
    return registro;
}


void desaloca_cabecalho(CABECALHO *cabecalho){
    if(cabecalho != NULL){
        free(cabecalho);
    }
    else{
        imprime_erro_padrao();
    }
}

char *ler_ate_virgula(FILE *arq){
    char c;
    char *vetor =NULL;
    int num_carac = 0;

    fscanf(arq, "%c", &c); 

    if(c == ',')
        return NULL;

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

void ler_data(FILE *arq, char *vetor){
    char c;
    int num_carac = 0;

    fscanf(arq, "%c", &c); 

    if(c == ','){
        for(int i=0; i<10; i++){
            vetor[i] = '$';
        }
    }

    while(c != ','){    
        num_carac++;
    
        vetor[num_carac-1] = c;
        
        if (fscanf(arq, "%c", &c) == EOF){
            break;
        }
    }

    vetor[9] = '\0';
   
}



DADOS *LerRegCsv(FILE *arquivoCSV){
    char vetorData[10];
    int auxId;
    DADOS *registro = criar_registro();

    fscanf(arquivoCSV, "%d,", &auxId);
    ler_data(arquivoCSV, vetorData);

    printf("%d %s", auxId, registro->dataCrime);



}

/* void imprime_cabecalho(CABECALHO *cabecalho){
    printf("%ld %d %d", cabecalho->proxByteOffset, cabecalho->nroRegArq, cabecalho->nroRegRem);
} */
//================================================================================



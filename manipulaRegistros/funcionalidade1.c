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
    char *vetor = NULL;
    int num_carac = 0;

    vetor = malloc(sizeof(char));
    if(vetor == NULL){
        imprime_erro_alocacao();
    }

    fscanf(arq, "%c", &c); 

    if(c == ','){
        *vetor = '|';
        return vetor;
    }

    while(c != ','){    
        num_carac++;
    
        vetor = realloc(vetor, sizeof(char)* num_carac);
        if (vetor == NULL){
            imprime_erro_alocacao();
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
        imprime_erro_alocacao();
    }
    vetor[num_carac-1] = '|';   

    return vetor;    
}

void ler_string_fixa(FILE *arq, char *vetor, int tamanho){
    char c;
    int num_carac = 0;

     

    //preenche vetor com $ para tratar vazios
    for(int i=0; i<tamanho; i++){
        vetor[i] = '$';
    }

    //retorno serve para verificacao de EOF
    char retorno = fscanf(arq, "%c", &c);

    while(c != ',' && c != '\n' && c != '\r' && retorno != EOF){    
        num_carac++;
    
        vetor[num_carac-1] = c;
        
        if (fscanf(arq, "%c", &c) == EOF){
            break;
        }
    }
   
}

int ler_numeroArtigo(FILE *arq){
    char c;
    char *vetor;
    int num_carac = 0;

    fscanf(arq, "%c", &c); 

    vetor = malloc(sizeof(char));
    if(vetor == NULL){
        imprime_erro_alocacao();
    }

    if(c == ',')
        return -1;

    while(c != ','){    
        num_carac++;
        
        vetor = realloc(vetor, sizeof(char) * num_carac);
        if (vetor == NULL){
            imprime_erro_alocacao();
        }
        vetor[num_carac-1] = c;


        
        if (fscanf(arq, "%c", &c) == EOF)
        {
            break;
        }
        
    }

    num_carac++;
    
    vetor = realloc(vetor, sizeof(char)* num_carac);
    if (vetor == NULL){
        imprime_erro_alocacao();
    }
    vetor[num_carac-1] = '\0';

    return (atoi(vetor));
   
}


DADOS *LerRegCsv(FILE *arquivoCSV){
    DADOS *registro = criar_registro();

    fscanf(arquivoCSV, "%d,", &(registro->idCrime));
    ler_string_fixa(arquivoCSV, registro->dataCrime, 10);
    registro->numeroArtigo = ler_numeroArtigo(arquivoCSV);
    registro->lugarCrime = ler_ate_virgula(arquivoCSV);
    registro->descricaoCrime = ler_ate_virgula(arquivoCSV);
    ler_string_fixa(arquivoCSV, registro->marcaCelular, 12);
    
    printf("\n\n%d %s %d %s %s %s\n\n", registro->idCrime, registro->dataCrime, 
    registro->numeroArtigo, registro->lugarCrime, 
    registro->descricaoCrime, registro->marcaCelular);

    return registro;

}


void EscreverRegBin(FILE *arquivoBIN, DADOS *registro){
    
    fwrite(registro->removido, sizeof(char), 1, arquivoBIN);
    fwrite(registro->idCrime, sizeof(int), 1, arquivoBIN);
    fwrite(registro->dataCrime, sizeof(registro->dataCrime), 1, arquivoBIN);
    fwrite(registro->numeroArtigo, sizeof(int), 1, arquivoBIN);
    fwrite(registro->marcaCelular, sizeof(registro->marcaCelular), 1, arquivoBIN);
    
    //tamanho variaveis
    int i;

    for(i=0; registro->lugarCrime[i] != '|'; i++)
        fwrite(registro->lugarCrime[i], sizeof(char), 1, arquivoBIN);
    fwrite(registro->lugarCrime[i], sizeof(char), 1, arquivoBIN);

    for(i=0; registro->descricaoCrime[i] != '|'; i++)
        fwrite(registro->descricaoCrime[i], sizeof(char), 1, arquivoBIN);
    fwrite(registro->descricaoCrime[i], sizeof(char), 1, arquivoBIN);

    fwrite(registro->delimitador, sizeof(char), 1, arquivoBIN);

}

//================================================================================



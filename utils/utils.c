#include "utils.h"
#include "../prints_e_erros/prints_e_erros.h"

//recebe nome dos arquivos na funcionalidade 1
void RecebeEntradaFunc1(char str1[], char str2[]){
    scanf("%s", str1);
    scanf("%s", str2);
}

//recebe nome dos arquivos na funcionalidade 2
void RecebeEntradaFunc2(char str1[]){
    scanf("%s", str1);
}


void ImprimeMarcaCelular(char marcaCelular[]){
    //imprime marca do celular
    for (int i = 0; i < 12; i++){
        if(marcaCelular[i] == '$'){
            if(i==0)
                printf("NULO");
            break;
        }    
                
        printf("%c", marcaCelular[i]);
    }

    printf("\n");
}

void ImprimeDataCrime(char dataCrime[]){
    //imprime data do crime
    for (int i = 0; i < 10; i++){
        if(dataCrime[i] == '$'){
            printf("NULO");
            break;
        }
            
        printf("%c", dataCrime[i]);
    }
    printf(", ");
}




//imprime campos variaveis na funcionalidade 2
void ImprimeCampoVariavel(FILE *arqBin, int id){
    char caracter = ' ';
    fread(&caracter, 1, 1, arqBin);
    do{
        if(caracter == '|'){
            printf("NULO");
            break;
        }
        printf("%c", caracter);
        fread(&caracter, 1, 1, arqBin);
    }while(caracter != '|');
        
    printf(", ");
}

//Serve para avançar o cursor nos campos de tamanho variavel sem imprimir
void LerCampoVariavel(FILE *arqBin, int id){
    if(id == 636){
        printf(" ");
    }
    char caracter = ' ';
    fread(&caracter, 1, 1, arqBin);
    do{
        if(caracter == '|'){
            break;
        }
        fread(&caracter, 1, 1, arqBin);
    }while(caracter != '|');
}



char *LerStringVariavel(FILE *arq){
    char caracter;
    char *vetor = NULL;
    int numeroCaracteres = 0;

    //aloca e inicializa vetor usada na leitura
    vetor = calloc(1, sizeof(char));
    if(vetor == NULL){
        ErroAlocacao();
    }

    char retorno = fscanf(arq, "%c", &caracter); 

    //trata o caso de campo vazio ou fim de arquivo
    if(retorno == EOF || caracter == ','){
        *vetor = '|';
        return vetor;
    }

    //lê até o fim do campo adicionando ao vetor
    while(caracter != ','){    
        numeroCaracteres++;
    
        vetor = (char *) realloc(vetor, sizeof(char)* numeroCaracteres);
        if (vetor == NULL){
            ErroAlocacao();
        }
        vetor[numeroCaracteres-1] = caracter;
        
        if (fscanf(arq, "%c", &caracter) == EOF)
        {
            break;
        }
    }

    //adiciona o "|" no final do campo
    numeroCaracteres++;
    
    vetor = (char *) realloc(vetor, sizeof(char)* numeroCaracteres);
    if (vetor == NULL)
    {
        ErroAlocacao();
    }
    vetor[numeroCaracteres-1] = '|';  

    return vetor;    
}


void LerStringFixa(FILE *arq, char *vetor, int tamanho){
    char caracter;
    int numeroCaracteres = 0;

    //retorno serve para verificação de EOF
    char retorno = fscanf(arq, "%c", &caracter);

    //lê char por char até fim do campo, adicionando ao vetor
    while(retorno != EOF && caracter != ',' && caracter != '\n' && caracter != '\r'){    
        numeroCaracteres++;
    
        vetor[numeroCaracteres-1] = caracter;
        
        if (fscanf(arq, "%c", &caracter) == EOF){
            break;
        }
    }
   
}

//lê um inteiro como string e converte novamente para inteiro (necessária para tratar o caso de um inteiro nulo no Nro do Artigo)
int LerInteiroVariavel(FILE *arq){
    char caracter;
    int numeroCaracteres = 0;
    char *vetor;

    char retorno = fscanf(arq, "%c", &caracter); 

    vetor = calloc(1, sizeof(char));
    if(vetor == NULL){ 
        ErroAlocacao();
    }

    //trata campos vazios
    if(retorno == EOF || caracter == ','){
        free(vetor);
        return -1;
    }

    //lê até o fim do campo e adiciona no vetor
    while(caracter != ','){    
        numeroCaracteres++;
        
        vetor = (char *) realloc(vetor, sizeof(char) * numeroCaracteres);
        if (vetor == NULL){
            ErroAlocacao();
        }
        vetor[numeroCaracteres-1] = caracter;

        if (fscanf(arq, "%c", &caracter) == EOF)
        {
            break;
        } 
    }

    //adiciona o "\0 na string"
    numeroCaracteres++;
    
    vetor = (char *) realloc(vetor, sizeof(char)* numeroCaracteres);
    if (vetor == NULL){
        ErroAlocacao();
    }
    vetor[numeroCaracteres-1] = '\0';
    
    //copia a string e converte para um inteiro ao retornar da função
    char estatico[numeroCaracteres];
    for(int i=0; i<numeroCaracteres; i++)
        estatico[i] = vetor[i];

    free(vetor);

    return (atoi(estatico));  
}

//Ler e ignorar a primeira linha do arquivo
void IgnoraLinha(FILE *arq){
    fscanf(arq, "%*[^\r\n]s"); 
    fscanf(arq, "%*[\r\n]s");
}
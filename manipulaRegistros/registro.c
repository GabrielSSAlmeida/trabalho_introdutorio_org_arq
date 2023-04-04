#include "registro.h"
#include "../auxiliares/auxiliar.h"
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
CABECALHO *CabecalhoCriar(void){
    CABECALHO *cabecalho = (CABECALHO*) malloc(sizeof(CABECALHO));

    //sizeof(CABECALHO) não funciona corretamente, então somamos os bytes
    long int somaBytes = sizeof(cabecalho->status) + sizeof(cabecalho->proxByteOffset) + sizeof(cabecalho->nroRegArq) + sizeof(cabecalho->nroRegRem); 

    //inicializa cabecalho
    if(cabecalho != NULL){
        cabecalho->status = '0';
        cabecalho->proxByteOffset = somaBytes;
        cabecalho->nroRegArq = 0;
        cabecalho->nroRegRem = 0;

    }
    else{
        ErroAlocacao();
    }
    return cabecalho;
}

DADOS *RegistroCriar(void){
    DADOS *registro = (DADOS*) calloc(1, sizeof(DADOS));

    //inicializa registro
    if(registro != NULL){
        registro->removido = '0';
        registro->idCrime = 0;
        registro->numeroArtigo = 0;
        registro->lugarCrime = NULL;
        registro->descricaoCrime = NULL;
        registro->delimitador = '#';

        //preenche vetor com $ para tratar vazios
        for(int i=0; i<10; i++){
            registro->dataCrime[i] = '$';
        }

        for(int i=0; i<12; i++){
            registro->marcaCelular[i] = '$';
        }
    }
    else{
        ErroAlocacao();
    }
    return registro;
}

void DesalocaRegistro(DADOS *registro){
    if(registro != NULL){
        free(registro->lugarCrime);
        free(registro->descricaoCrime);
        free(registro);
    }
    else{
        ErroDesalocar();
    }
}

void DesalocaCabecalho(CABECALHO *cabecalho){
    if(cabecalho != NULL){
        free(cabecalho);
    }
    else{
        ErroDesalocar();
    }
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

//Essa função faz a leitura de um registro do arquivo CSV
DADOS *LerRegistroCsv(FILE *arquivoCSV, int *flagFuncionou){
    DADOS *registro = RegistroCriar();

    //se retornar -1 a leitura não funcionou, ou seja, não há registro
    *flagFuncionou = fscanf(arquivoCSV, "%d,", &(registro->idCrime));

    LerStringFixa(arquivoCSV, registro->dataCrime, 10);
    registro->numeroArtigo = LerInteiroVariavel(arquivoCSV);
    registro->lugarCrime = LerStringVariavel(arquivoCSV);
    registro->descricaoCrime = LerStringVariavel(arquivoCSV);
    LerStringFixa(arquivoCSV, registro->marcaCelular, 12);

    return registro;
}

//Essa função escreve um registro no arquivo binário
void EscreverRegistroBin(FILE *arquivoBIN, DADOS *registro, CABECALHO *cabecalho){
    long int somaBytes = 0;

    //escreve campos de tamanho fixo
    fwrite(&(registro->removido), sizeof(char), 1, arquivoBIN);
    fwrite(&(registro->idCrime), sizeof(int), 1, arquivoBIN);
    fwrite(&(registro->dataCrime), sizeof(registro->dataCrime), 1, arquivoBIN);
    fwrite(&(registro->numeroArtigo), sizeof(int), 1, arquivoBIN);
    fwrite(&(registro->marcaCelular), sizeof(registro->marcaCelular), 1, arquivoBIN);
    somaBytes+= 31; //Nro de bytes dos campos de tamanho fixo

    //escreve campos de tamanho variável
    int i;

        //escreve lugar do crime
    for(i=0; registro->lugarCrime[i] != '|'; i++){
        fwrite(&(registro->lugarCrime[i]), sizeof(char), 1, arquivoBIN);
        somaBytes++;
    }
    fwrite(&(registro->lugarCrime[i]), sizeof(char), 1, arquivoBIN);
    somaBytes++;

        //escreve descricao do crime
    for(i=0; registro->descricaoCrime[i] != '|'; i++){
        fwrite(&(registro->descricaoCrime[i]), sizeof(char), 1, arquivoBIN);
        somaBytes++;
    }
    fwrite(&(registro->descricaoCrime[i]), sizeof(char), 1, arquivoBIN);
    somaBytes++;

        //escreve o delimitador do registro
    fwrite(&(registro->delimitador), sizeof(char), 1, arquivoBIN);
    somaBytes++;

        //atualiza dados do cabecalho
    (cabecalho->proxByteOffset) += somaBytes;
    (cabecalho->nroRegArq) += 1; 
}

void EscreveCabecalho(FILE *arqBin, CABECALHO *cabecalho, char status){
    //atualiza o status do cabecalho
    cabecalho->status = status;
    
    //volta para o inicio do arquivo para atualizar o cabecalho
    fseek(arqBin, 0, SEEK_SET);

    fwrite(&(cabecalho->status), sizeof(char), 1, arqBin);
    fwrite(&(cabecalho->proxByteOffset), sizeof(long int), 1, arqBin);
    fwrite(&(cabecalho->nroRegArq), sizeof(int), 1, arqBin);
    fwrite(&(cabecalho->nroRegRem), sizeof(int), 1, arqBin);
}

//Essa função imprime os registros do arquivo binário na tela
void ImprimirBinario(FILE *arqBin){
    char cabecalho[17];

    char removido;
    int idCrime;
    char dataCrime[10];
    int numeroArtigo;
    char marcaCelular[12];

    //Ler e ignorar o cabeçalho
    fread(cabecalho, 17, 1, arqBin); 
    
    while(fread(&removido, 1, 1, arqBin)!=0){
        fread(&idCrime, 4, 1, arqBin);
        fread(dataCrime, 10, 1, arqBin);
        fread(&numeroArtigo, 4, 1, arqBin);
        fread(marcaCelular, 12, 1, arqBin);

        printf("\n %d, ", idCrime);

        //imprime data do crime
        for (int i = 0; i < 10; i++){
                if(dataCrime[i] == '$'){
                    printf("NULO");
                    break;
                }
                    
                printf("%c", dataCrime[i]);
        }
        printf(", ");

        //imprime Numero do Artigo
        (numeroArtigo == -1) ? printf("NULO, "):printf("%d, ", numeroArtigo);

        char caracter;

        //imprime cidade
        ImprimeCampoVariavel(arqBin, &caracter);

        //imprime descricao
        ImprimeCampoVariavel(arqBin, &caracter);

        //imprime marca do celular
        for (int i = 0; i < 12; i++){
            if(marcaCelular[i] == '$'){
                if(i==0)
                    printf("NULO");
                break;
            }    
                    
            printf("%c", marcaCelular[i]);
        }

        //lê o delimitador do registro
        fread(&caracter, 1, 1, arqBin);
    }

    printf("\n");
}

//================================================================================



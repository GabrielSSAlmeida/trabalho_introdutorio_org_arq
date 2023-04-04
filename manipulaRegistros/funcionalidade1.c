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
CABECALHO *CabecalhoCriar(void){
    CABECALHO *cabecalho = (CABECALHO*) malloc(sizeof(CABECALHO));

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
    char c;
    char *vetor = NULL;
    int num_carac = 0;

    vetor = calloc(1, sizeof(char));
    if(vetor == NULL){
        ErroAlocacao();
    }

    char retorno = fscanf(arq, "%c", &c); 

    //trata o caso de campo vazio ou fim de arquivo
    if(retorno == EOF || c == ','){
        *vetor = '|';
        return vetor;
    }

    //lê até o fim do campo adicionando ao vetor
    while(c != ','){    
        num_carac++;
    
        vetor = (char *) realloc(vetor, sizeof(char)* num_carac);
        if (vetor == NULL){
            ErroAlocacao();
        }
        vetor[num_carac-1] = c;


        
        if (fscanf(arq, "%c", &c) == EOF)
        {
            break;
        }
        
    }

    //adiciona o "|" no final do campo
    num_carac++;
    
    vetor = (char *) realloc(vetor, sizeof(char)* num_carac);
    if (vetor == NULL)
    {
        ErroAlocacao();
    }
    vetor[num_carac-1] = '|';  

    return vetor;    
}

void LerStringFixa(FILE *arq, char *vetor, int tamanho){
    char c;
    int num_carac = 0;

    //preenche vetor com $ para tratar vazios
    for(int i=0; i<tamanho; i++){
        vetor[i] = '$';
    }

    //retorno serve para verificação de EOF
    char retorno = fscanf(arq, "%c", &c);

    while(retorno != EOF && c != ',' && c != '\n' && c != '\r'){    
        num_carac++;
    
        vetor[num_carac-1] = c;
        
        if (fscanf(arq, "%c", &c) == EOF){
            break;
        }
    }
   
}

int LerInteiroVariavel(FILE *arq){
    char c;
    int num_carac = 0;
    char *vetor;

    char retorno = fscanf(arq, "%c", &c); 

    vetor = calloc(1, sizeof(char));
    if(vetor == NULL){ 
        ErroAlocacao();
    }

    //trata campos vazios
    if(retorno == EOF || c == ','){
        free(vetor);
        return -1;
    }

    //lê até o fim do campo e adiciona no vetor
    while(c != ','){    
        num_carac++;
        
        vetor = (char *) realloc(vetor, sizeof(char) * num_carac);
        if (vetor == NULL){
            ErroAlocacao();
        }
        vetor[num_carac-1] = c;


        
        if (fscanf(arq, "%c", &c) == EOF)
        {
            break;
        }
        
    }

    //adiciona o "\0 na string"
    num_carac++;
    
    vetor = (char *) realloc(vetor, sizeof(char)* num_carac);
    if (vetor == NULL){
        ErroAlocacao();
    }
    vetor[num_carac-1] = '\0';
    
    //copia a string e converte para um inteiro ao retornar
    char estatico[num_carac];
    for(int i=0; i<num_carac; i++)
        estatico[i] = vetor[i];

    free(vetor);

    return (atoi(estatico));
   
}

//Essa função faz a leitura de um registro do arquivo CSV
DADOS *LerRegCsv(FILE *arquivoCSV, int *flagFuncionou){
    DADOS *registro = RegistroCriar();


    int retorno = fscanf(arquivoCSV, "%d,", &(registro->idCrime));
    LerStringFixa(arquivoCSV, registro->dataCrime, 10);
    registro->numeroArtigo = LerInteiroVariavel(arquivoCSV);
    registro->lugarCrime = LerStringVariavel(arquivoCSV);
    registro->descricaoCrime = LerStringVariavel(arquivoCSV);
    LerStringFixa(arquivoCSV, registro->marcaCelular, 12);
    

    *flagFuncionou = retorno;

    return registro;
}

//Essa função escreve um registro no arquivo binário
void EscreverRegBin(FILE *arquivoBIN, DADOS *registro, CABECALHO *cabecalho){
    long int somaBytes = 0;

    //campos de tamanho fixo
    fwrite(&(registro->removido), sizeof(char), 1, arquivoBIN);
    fwrite(&(registro->idCrime), sizeof(int), 1, arquivoBIN);
    fwrite(&(registro->dataCrime), sizeof(registro->dataCrime), 1, arquivoBIN);
    fwrite(&(registro->numeroArtigo), sizeof(int), 1, arquivoBIN);
    fwrite(&(registro->marcaCelular), sizeof(registro->marcaCelular), 1, arquivoBIN);
    somaBytes+= 31; //Nro de bytes dos campos de tamanho fixo

    //campos de tamanho variável
    int i;

    for(i=0; registro->lugarCrime[i] != '|'; i++){
        fwrite(&(registro->lugarCrime[i]), sizeof(char), 1, arquivoBIN);
        somaBytes++;
    }
    fwrite(&(registro->lugarCrime[i]), sizeof(char), 1, arquivoBIN);
    somaBytes++;

    for(i=0; registro->descricaoCrime[i] != '|'; i++){
        fwrite(&(registro->descricaoCrime[i]), sizeof(char), 1, arquivoBIN);
        somaBytes++;
    }
    fwrite(&(registro->descricaoCrime[i]), sizeof(char), 1, arquivoBIN);
    somaBytes++;

    fwrite(&(registro->delimitador), sizeof(char), 1, arquivoBIN);
    somaBytes++;


    (cabecalho->proxByteOffset) += somaBytes;
    (cabecalho->nroRegArq) += 1; 

}

void EscreveCabecalho(FILE *arqBin, CABECALHO *cabecalho, char status){
    cabecalho->status = '1';
    
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

    
    fread(cabecalho, 17, 1, arqBin); //Ler e ignorar o cabeçalho

    
    while(fread(&removido, 1, 1, arqBin)!=0){
        fread(&idCrime, 4, 1, arqBin);
        fread(dataCrime, 10, 1, arqBin);
        fread(&numeroArtigo, 4, 1, arqBin);
        fread(marcaCelular, 12, 1, arqBin);


        printf("\n %c %d %d ", removido, idCrime, numeroArtigo);

        //imprime data do crime
        for (int i = 0; i < 10; i++){
                if(dataCrime[i] == '$')
                    break;
                printf("%c", dataCrime[i]);
        }
        printf(" ");

        //imprime marca do celular
        for (int i = 0; i < 12; i++){
                if(marcaCelular[i] == '$')
                    break;
                printf("%c", marcaCelular[i]);
        }

            printf(" ");
        char charCidade;

        //imprime cidade
        do{
            fread(&charCidade, 1, 1, arqBin);
            if(charCidade == '|')
                break;
            printf("%c", charCidade);
        }while(charCidade != '|');

            printf(" ");

        //imprime descricao
        do{
            fread(&charCidade, 1, 1, arqBin);
            if(charCidade == '|')
                break;
            printf("%c", charCidade);
        }while(charCidade != '|');

        fread(&charCidade, 1, 1, arqBin);
    }
}

//================================================================================



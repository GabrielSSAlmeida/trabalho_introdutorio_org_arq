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

void RecebeEntradaFunc3e8(char arqEntrada[], char arqIndice[], char campo[], char dado[]){
    scanf("%s %s %s %s", arqEntrada, campo, dado, arqIndice);
}

void RecebeEntradaFuncs4a7(char arqEntrada[], char arqIndice[], char campo[], char dado[], int *n){
    scanf("%s %s %s %s %d", arqEntrada, campo, dado, arqIndice, n);
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
void ImprimeCampoVariavel(char *string){
    int contador = 0;
    do{
        if(string[contador] == '|'){
            printf("NULO");
            break;
        }
        printf("%c", string[contador]);
        contador++;
    }while(string[contador] != '|');
        
    printf(", ");
}

void avancaCursosEmCampoVariavel(FILE *arqBin){
    char caracter;

    fread(&caracter, 1, 1, arqBin);
    while(caracter != '|'){
        fread(&caracter, 1, 1, arqBin);
    }
}


//Serve para avançar o cursor nos campos de tamanho variavel sem imprimir
char *LerCampoVariavel(FILE *arqBin, long int *offsetlido){
    char caracter;
    char *vetor = NULL;
    int numeroCaracteres = 0;

    //aloca e inicializa vetor usada na leitura
    vetor = calloc(1, sizeof(char));
    if(vetor == NULL){
        ErroAlocacao();
    }

    fread(&caracter, 1, 1, arqBin);
    vetor[0] = caracter;
    numeroCaracteres++;
    while(caracter != '|'){
        fread(&caracter, 1, 1, arqBin);

        vetor = (char *) realloc(vetor, sizeof(char)* numeroCaracteres+1);
        if (vetor == NULL){
            ErroAlocacao();
        }

        vetor[numeroCaracteres] = caracter;
        numeroCaracteres++;
    }

    *offsetlido += numeroCaracteres;
    
    return vetor;
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

//verifica se o tipo de dado é string ou int (return 0 == int) (return 1 == string)
TipoDados VerificaDado(char dado[]){
    if(strcmp(dado, "inteiro") == 0) return inteiro;
    if(strcmp(dado, "string") == 0) return string;
}

TipoCampo TipoChaveBusca(char campo[]){
    TipoCampo tipoCampo = erro;
    if(strcmp(campo, "idCrime") == 0){
        tipoCampo = idCrime;
    }else if(strcmp(campo, "numeroArtigo") == 0){
        tipoCampo = numeroArtigo;
    }else if(strcmp(campo, "dataCrime") == 0){
        tipoCampo = dataCrime;
    }else if(strcmp(campo, "marcaCelular") == 0){
        tipoCampo = marcaCelular;
    }else if(strcmp(campo, "lugarCrime") == 0){
        tipoCampo = lugarCrime;
    }else if(strcmp(campo, "descricaoCrime") == 0){
        tipoCampo = descricaoCrime;
    }else{
        ErroArquivo();
    }

    return tipoCampo;
}

//strncpy sem copiar o '|'
//mudar nome para SemPipe
void strncpySem0(char *string1, char *string2, int n){
    for(int i = 0; i<n && string2[i] != '|'; i++)
        string1[i] = string2[i];
}

void strncpySemBarra0(char *string1, char *string2, int n){
    for(int i = 0; i<n && string2[i] != '\0'; i++)
        string1[i] = string2[i];
}

void strcpySem0ComFinal(char *string1, char *string2, char final){
    int i=0;    
    for(; string2[i] != '\0'; i++)
        string1[i] = string2[i];
    string1[i]=final;
    
}

void strcpySem0(char *string1, char *string2){
    int i=0;    
    for(; string2[i] != '\0'; i++)
        string1[i] = string2[i];
}

void strcpySem0Variavel(char *string1, char *string2){
    strcpySem0ComFinal(string1, string2, '|');
}

void strcpySem0Fixa(char *string1, char *string2){
    strcpySem0ComFinal(string1, string2, '$');
}


int strcmpAtePipe(char *string1, char *string2){
    int pos1= 0;
    int pos2 = 0;
    while (string1[pos1] != '|' || string2[pos2] != '\0')
    {
        if(string1[pos1] != string2[pos2]){
            break;
        }
        pos1++;
        pos2++;
    }

    if(string1[pos1] == '|' && string2[pos2] == '\0'){
        pos1--;
        pos2--;
    }

    //trata caso de strings nulas
    if(pos1 == -1 && pos2 == -1) return 0;

    char s1 = string1[pos1];
    char s2 = string2[pos2];
    int diferenca =  (int)s1 - (int)s2;
    return diferenca;
    
}



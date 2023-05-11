#include "registro.h"
#include "../utils/utils.h"
#include "../prints_e_erros/prints_e_erros.h"

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


DADOS *RegistroCriar(void){
    DADOS *registro = (DADOS*) calloc(1, sizeof(DADOS));

    //inicializa registro
    if(registro != NULL){
        registro->removido = '0';
        registro->idCrime = -1;
        registro->numeroArtigo = -1;
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


void DesalocaCamposVariaveis(DADOS *registro){
    if(registro != NULL){
        if(registro->lugarCrime != NULL && registro->descricaoCrime != NULL){
            free(registro->lugarCrime);
            free(registro->descricaoCrime);
            registro->lugarCrime = NULL;
            registro->descricaoCrime = NULL;
        }
    }
}

void DesalocaRegistro(DADOS *registro){
    if(registro != NULL){
        DesalocaCamposVariaveis(registro);
        free(registro);
    }
    else{
        ErroDesalocar();
    }
}

void DesalocaVetorRegistro(DADOS **registro, int tamanho){
    if(registro != NULL){
        for (int i = 0; i < tamanho; i++)
        {
            DesalocaCamposVariaveis(registro[i]);
            free(registro[i]);
        }
        free(registro);
    }
    else{
        ErroDesalocar();
    }
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
    AtualizaByteOffset(cabecalho, GetByteOffset(cabecalho)+somaBytes); 
    AtualizaNroRegArq(cabecalho, GetNroRegArq(cabecalho)+1);
}


int LerRegBinario(FILE *arqBin, DADOS *registro, int *offsetlido){
    int aux = fread(&(registro->removido), 1, 1, arqBin);

    if(aux == 0){
        return aux;
    }

    fread(&(registro->idCrime), 4, 1, arqBin);
    fread((registro->dataCrime), 10, 1, arqBin);
    fread(&(registro->numeroArtigo), 4, 1, arqBin);
    fread((registro->marcaCelular), 12, 1, arqBin);
    

    registro->lugarCrime = LerCampoVariavel(arqBin, offsetlido);
    registro->descricaoCrime = LerCampoVariavel(arqBin, offsetlido);

    //lê o delimitador do registro
    fread(&(registro->delimitador), 1, 1, arqBin);

    *offsetlido += 32;
    
    return aux;
}


void ImprimeRegistroBinario(DADOS *registro){
    if(registro->removido == '0'){

        //imprime o id do crime
        printf("%d, ", registro->idCrime);

        //imprime data do crime
        ImprimeDataCrime(registro->dataCrime);

        //imprime Numero do Artigo
        (registro->numeroArtigo == -1) ? printf("NULO, "):printf("%d, ", registro->numeroArtigo);

        //imprime cidade
        ImprimeCampoVariavel(registro->lugarCrime);

        //imprime descricao
        ImprimeCampoVariavel(registro->descricaoCrime);

        //imprime marca do celular
        ImprimeMarcaCelular(registro->marcaCelular);

    }
    
}

bool ImprimirBinario(FILE *arqBin){
    //alocacao de auxiliares
    CABECALHO *cabecalho_aux = CabecalhoCriar();
    DADOS *registro_aux = RegistroCriar();
    
    //le o cabecalho do arquivo binario
    LeCabecalhoDoArqBinario(cabecalho_aux, arqBin);
    
    //verifica se o status do arquivo é consistente
    if(!VerificaStatus(cabecalho_aux)){
        //desaloca os auxiliares criados
        DesalocaCabecalho(cabecalho_aux);
        DesalocaRegistro(registro_aux);
        return false;
    }

    //é passado no parametro da funcao, mas nao sera utilizado nesse momento
    int aux;

    int flag = LerRegBinario(arqBin, registro_aux, &aux);

    int i;
    for(i=0; flag!=0; i++){
        
        ImprimeRegistroBinario(registro_aux);
        DesalocaCamposVariaveis(registro_aux);

        flag = LerRegBinario(arqBin, registro_aux, &aux); 
    }


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_aux);
    DesalocaRegistro(registro_aux);

    //se nao existem registros no arquivo
    if(i==0) ErroRegistro();

    
    return true;

}


DADOS *LeRegistroPorByteOffset(FILE *arqBin, long int byteOffset){
    //vai ate o byteoffset do registro buscado
    fseek(arqBin, byteOffset, SEEK_SET);

    DADOS *registro = RegistroCriar();

    int tamanho_registro;
    int flag = LerRegBinario(arqBin, registro, &tamanho_registro);

    if(flag == 0){
        ErroArquivo();
        return NULL;
    }

    return registro;
}

//recebe registro na entrada da func6
void RecebeRegistro(DADOS *registro){
    char string_aux[200];
    char data_aux[10];

    //recebe idCrime
    scan_quote_string(string_aux);

    if(strcmp(string_aux, "")!=0)
        registro->idCrime = atoi(string_aux);
    else
        registro->idCrime = -1;
    
    
    //recebe dataCrime
    scan_quote_string(string_aux);
    strcpySem0Fixa(registro->dataCrime, string_aux);

    // if(strcmp(string_aux, "")==0){
    //     for(int i=0; i<10; i++)
    //         registro->dataCrime[i] = '$';
    // }

    //recebe numeroArtigo
    scan_quote_string(string_aux);

    if(strcmp(string_aux, "")!=0)
        registro->numeroArtigo = atoi(string_aux);
    else   
        registro->numeroArtigo = -1;
    
    //recebe lugarCrime
    scan_quote_string(string_aux);

    registro->lugarCrime = (char *)malloc((strlen(string_aux)+1)*sizeof(char));

    strcpySem0Variavel(registro->lugarCrime, string_aux);

    //recebe descricaoCrime
    scan_quote_string(string_aux);

    registro->descricaoCrime = (char *)malloc((strlen(string_aux)+1)*sizeof(char));

    strcpySem0Variavel(registro->descricaoCrime, string_aux);

    //recebe marcaCelular
    scan_quote_string(string_aux);
    
    // for(int i=11; i>=strlen(string_aux); i--)
    //     registro->marcaCelular[i] = '$';
    

    strcpySem0Fixa(registro->marcaCelular, string_aux);

    // if(strcmp(string_aux, "")==0){
    //     for(int i=0; i<12; i++)
    //         registro->marcaCelular[i] = '$';
    // }
}

bool InsereRegistroNoArqBin(DADOS *registro, char *arqEntrada, long int *byteoffset){
    //INSERCAO NO BINARIO
    
    FILE *arqBin;

    //atualiza o status do cabecalho
    if(!AbreArquivo(&arqBin, arqEntrada, "rb+", NULL)) return false;
    CABECALHO *cabecalho = CabecalhoCriar();
    LeCabecalhoDoArqBinario(cabecalho, arqBin);
    AtualizaStatus(cabecalho, '0');
    fseek(arqBin, 0, SEEK_SET);
    EscreveCabecalho(arqBin, cabecalho);
    fclose(arqBin);

    if(!AbreArquivo(&arqBin, arqEntrada, "rb+", NULL)) return false;

    *byteoffset = GetByteOffset(cabecalho);

    //insere no proximo byteoffset
    fseek(arqBin, GetByteOffset(cabecalho), SEEK_SET);
    EscreverRegistroBin(arqBin, registro, cabecalho);

    //atualiza status do cabecalho e fecha arquivo
    AtualizaStatus(cabecalho, '1');
    fseek(arqBin, 0, SEEK_SET);
    EscreveCabecalho(arqBin, cabecalho);
    DesalocaCabecalho(cabecalho);
    fclose(arqBin);
}

bool InsereRegistro(DADOS *registro, char *arqEntrada, char *nomeArqIndice, char *campo, char *dado){
    //salva o byteoffset do registro após inserido no arq bin (sera usado no arquivo de indices)
    long int byteoffsetanterior = 0;

    InsereRegistroNoArqBin(registro, arqEntrada, &byteoffsetanterior);


    //INSERCAO NO ARQUIVO DE INDICE
    FILE *arqIndice;

    //interpreta qual o tipo de campo exigido (string ou int)
    int tipoCampo = TipoChaveBusca(campo);

    //atualiza status do arquivo
    if(!AbreArquivo(&arqIndice, nomeArqIndice, "rb+", NULL)) return false;
    CABECALHO_INDICE *cabecalho_indice = CabecalhoIndiceCriar();
    LeCabecalhoDoArqIndice(cabecalho_indice, arqIndice);
    AtualizaStatusIndice(cabecalho_indice, '0');
    fseek(arqIndice, 0, SEEK_SET);
    EscreveCabecalhoIndice(arqIndice, cabecalho_indice);
    fclose(arqIndice);

    if(!AbreArquivo(&arqIndice, nomeArqIndice, "rb+", NULL)) return false;

    //inicia insercao
    int tipoDado = VerificaDado(dado);
    switch(tipoDado){
        //caso int
        case 0:{

            int nroReg = GetNroRegArqIndice(cabecalho_indice);
            DADOS_INT *vetorIndices = VetorIndicesIntCriar(nroReg+1);
            DADOS_INT *registroIndice_aux = IndiceDadosIntCriar();
            DADOS_INT *registroIndice_aux2 = IndiceDadosIntCriar();

            switch(tipoCampo){
                //caso idCrime
                case 0:{
                    registroIndice_aux->chaveBusca = registro->idCrime;
                    registroIndice_aux->byteOffset = byteoffsetanterior;
                    break;
                }

                //caso numeroArtigo
                case 1:{
                    registroIndice_aux->chaveBusca = registro->numeroArtigo;
                    registroIndice_aux->byteOffset = byteoffsetanterior;
                    break;
                }

            }
            
            LeCabecalhoDoArqIndice(cabecalho_indice, arqIndice);

            //Preenche o vetor de indices
            for (int i = 0; i < nroReg; i++)
            {
                vetorIndices[i] = LerRegIndiceInt(arqIndice, registroIndice_aux2);
            }

            //insere ordenado
            if(vetorIndices[nroReg-1].chaveBusca < registroIndice_aux->chaveBusca){
                vetorIndices[nroReg].chaveBusca = registroIndice_aux->chaveBusca;
                vetorIndices[nroReg].byteOffset = registroIndice_aux->byteOffset;
            }
            for(int i=0; i < nroReg; i++){
                if(vetorIndices[i].chaveBusca > registroIndice_aux->chaveBusca){
                    for(int j=nroReg; j>i; j--){
                        vetorIndices[j] = vetorIndices[j-1];
                    }
                    vetorIndices[i].chaveBusca = registroIndice_aux->chaveBusca;
                    vetorIndices[i].byteOffset = registroIndice_aux->byteOffset;
                    break;
                }
            }

            //escrever no arquivo de indice
            fclose(arqIndice);

            //ATUALIZA CABECALHO INDICE
            AtualizaStatusIndice(cabecalho_indice, '1');
            AtualizaNroRegArqIndice(cabecalho_indice, nroReg+1);

            //abre para escrita
            if(!AbreArquivo(&arqIndice, nomeArqIndice, "wb+", NULL)) return false;

            //Coloca o cursor no começo do arq para sobrescrever o cabecalho
            EscreveCabecalhoIndice(arqIndice, cabecalho_indice);

            //essa variavel conta no sentido negativo (0, -1, -2, -3) por conta de outra parte do código
            int contaNulos = 0;

            for (int j = 0; j < nroReg+1; j++)
                EscreveArqIndiceInt(arqIndice, vetorIndices[j], &contaNulos);
            
            nroReg = (nroReg+1) + contaNulos;

            //ATUALIZA CABECALHO INDICE
            AtualizaNroRegArqIndice(cabecalho_indice, nroReg);
            fseek(arqIndice, 0, SEEK_SET);
            EscreveCabecalhoIndice(arqIndice, cabecalho_indice);



            free(vetorIndices);
            free(registroIndice_aux);
            free(registroIndice_aux2);
            break;
        }

        //caso string
        case 1:{
            int nroReg = GetNroRegArqIndice(cabecalho_indice);
            DADOS_STR *vetorIndices = VetorIndicesStringCriar(nroReg+1);
            DADOS_STR *registroIndice_aux = IndiceDadosStringCriar();
            DADOS_STR *registroIndice_aux2 = IndiceDadosStringCriar();

            switch(tipoCampo){
                //caso dataCrime
                case 2:{
                    strncpySem0(registroIndice_aux->chaveBusca, registro->dataCrime, 12);
                    registroIndice_aux->byteOffset = byteoffsetanterior;
                    break;
                }

                //caso marcaCelular
                case 3:{
                    strncpySem0(registroIndice_aux->chaveBusca, registro->marcaCelular, 12);
                    registroIndice_aux->byteOffset = byteoffsetanterior;
                    break;
                }

                //caso lugarCrime
                case 4:{
                    strncpySem0(registroIndice_aux->chaveBusca, registro->lugarCrime, 12);
                    registroIndice_aux->byteOffset = byteoffsetanterior;
                    break;
                }

                //caso descricaoCrime
                case 5:{
                    strncpySem0(registroIndice_aux->chaveBusca, registro->descricaoCrime, 12);
                    registroIndice_aux->byteOffset = byteoffsetanterior;
                    break;
                }

            }

            LeCabecalhoDoArqIndice(cabecalho_indice, arqIndice);

            //Preenche o vetor de indices
            for (int i = 0; i < nroReg; i++)
            {
                vetorIndices[i] = LerRegIndiceString(arqIndice, registroIndice_aux2);
            }

            //insere ordenado
            if(strncmp(vetorIndices[nroReg-1].chaveBusca, registroIndice_aux->chaveBusca, 12) < 0 ){
                strncpySem0(vetorIndices[nroReg].chaveBusca, registroIndice_aux->chaveBusca, 12);
                vetorIndices[nroReg].byteOffset = registroIndice_aux->byteOffset;
            }
            for(int i=0; i < nroReg; i++){
                if(vetorIndices[i].chaveBusca > registroIndice_aux->chaveBusca){
                    for(int j=nroReg; j>i; j--){
                        vetorIndices[j] = vetorIndices[j-1];
                    }
                    strncpySem0(vetorIndices[i].chaveBusca, registroIndice_aux->chaveBusca, 12);
                    vetorIndices[i].byteOffset = registroIndice_aux->byteOffset;
                    break;
                }
            }

            //escrever no arquivo de indice
            fclose(arqIndice);

            //ATUALIZA CABECALHO INDICE
            AtualizaStatusIndice(cabecalho_indice, '1');
            AtualizaNroRegArqIndice(cabecalho_indice, nroReg+1);

            //abre para escrita
            if(!AbreArquivo(&arqIndice, nomeArqIndice, "wb+", NULL)) return false;

            //Coloca o cursor no começo do arq para sobrescrever o cabecalho
            EscreveCabecalhoIndice(arqIndice, cabecalho_indice);

            //essa variavel conta no sentido negativo (0, -1, -2, -3) por conta de outra parte do código
            int contaNulos = 0;

            for (int j = 0; j < nroReg+1; j++)
                EscreveArqIndiceString(arqIndice, vetorIndices[j], &contaNulos);
            
            nroReg = (nroReg+1) + contaNulos;

            //ATUALIZA CABECALHO INDICE
            AtualizaNroRegArqIndice(cabecalho_indice, nroReg);
            fseek(arqIndice, 0, SEEK_SET);
            EscreveCabecalhoIndice(arqIndice, cabecalho_indice);



            free(vetorIndices);
            free(registroIndice_aux);
            free(registroIndice_aux2);

            break;
        }

        default:
            break;
    }

    DesalocaCabecalhoIndice(cabecalho_indice);
    fclose(arqIndice);

}




char GetRegistroRemovido(DADOS *registro){
    if(registro != NULL){
        return (registro->removido);
    }
    return '\0';
}

int GetRegistroIdCrime(DADOS *registro){
    if(registro != NULL){
        return (registro->idCrime);
    }
    return -1;
}

int GetRegistroNroArtigo(DADOS *registro){
    if(registro != NULL){
        return (registro->numeroArtigo);
    }
    return -1;
}

char *GetRegistroDataCrime(DADOS *registro){
    if(registro != NULL){
        return (registro->dataCrime);
    }
    return NULL;
}

char *GetRegistroMarcaCelular(DADOS *registro){
    if(registro != NULL){
        return (registro->marcaCelular);
    }
    return NULL;
}

char *GetRegistroLugarCrime(DADOS *registro){
    if(registro != NULL){
        return (registro->lugarCrime);
    }
    return NULL;
}

char *GetRegistroDescricaoCrime(DADOS *registro){
    if(registro != NULL){
        return (registro->descricaoCrime);
    }
    return NULL;
}
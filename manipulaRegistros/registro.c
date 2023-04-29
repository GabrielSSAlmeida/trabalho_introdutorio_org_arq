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


int LerCamposFixosRegBinario(FILE *arqBin, DADOS *registro){
    int aux = fread(&(registro->removido), 1, 1, arqBin);
    
    fread(&(registro->idCrime), 4, 1, arqBin);
    fread((registro->dataCrime), 10, 1, arqBin);
    fread(&(registro->numeroArtigo), 4, 1, arqBin);
    fread((registro->marcaCelular), 12, 1, arqBin);

    return aux;
}


void ImprimeRegistroBinario(FILE *arqBin, DADOS *registro){
    if(registro->removido == '0'){

        //imprime o id do crime
        printf("%d, ", registro->idCrime);

        //imprime data do crime
        ImprimeDataCrime(registro->dataCrime);

        //imprime Numero do Artigo
        (registro->numeroArtigo == -1) ? printf("NULO, "):printf("%d, ", registro->numeroArtigo);

        //imprime cidade
        ImprimeCampoVariavel(arqBin, registro->idCrime);

        //imprime descricao
        ImprimeCampoVariavel(arqBin, registro->idCrime);

        //imprime marca do celular
        ImprimeMarcaCelular(registro->marcaCelular);

    }else{
        //Serve para avançar o cursor nos campos de tamanho variavel sem imprimir
        LerCampoVariavel(arqBin, registro->idCrime);
        LerCampoVariavel(arqBin, registro->idCrime);
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


    int flag = LerCamposFixosRegBinario(arqBin, registro_aux);

    int i;
    for(i=0; flag!=0; i++){
        
        ImprimeRegistroBinario(arqBin, registro_aux);

        //lê o delimitador do registro
        fread(&(registro_aux->delimitador), 1, 1, arqBin);

        flag = LerCamposFixosRegBinario(arqBin, registro_aux);
         
    }

    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_aux);
    DesalocaRegistro(registro_aux);

    //se nao existem registros no arquivo
    if(i==0) ErroRegistro();

    
    return true;

}
#include "Indices.h"
#include "../utils/utils.h"
#include "../arquivos/arquivos.h"
#include "../prints_e_erros/prints_e_erros.h"

struct dadosIndiceInt{
    int chaveBusca;                    
    long int byteOffset;            //número relativo do registro do arquivo de dados referente à chave de busca
};

struct dadosIndiceString{
    char chaveBusca[12];                    
    long int byteOffset;            //número relativo do registro do arquivo de dados referente à chave de busca
};



//Criação das Structs
//aloca um registro de indice do tipo inteiro
DADOS_INT *RegistroDadosIntCriar(void){
    DADOS_INT *registro = (DADOS_INT*) calloc(1, sizeof(DADOS_INT));

    //inicializa registro
    if(registro != NULL){
        registro->chaveBusca = 0;
        registro->byteOffset = 0;
    }
    else{
        ErroAlocacao();
    }
    return registro;
}
//aloca um vetor de DADOS_INT
DADOS_INT *VetorIndicesIntCriar(int tamanho){
    DADOS_INT *vetor = (DADOS_INT*) calloc(tamanho, sizeof(DADOS_INT));
    if(vetor != NULL){
        return vetor;
    }
    else{
        ErroAlocacao();
        return NULL;
    }
}

//aloca um registro de indice do tipo String
DADOS_STR *RegistroDadosStringCriar(void){
    DADOS_STR *registro = (DADOS_STR*) calloc(1, sizeof(DADOS_STR));

    //inicializa registro
    if(registro != NULL){
        for (int i = 0; i < 12; i++)
        {
            registro->chaveBusca[i] = '$';
        }
        
        registro->byteOffset = 0;
    }
    else{
        ErroAlocacao();
    }
    return registro;
}
//aloca um vetor de DADOS_STR
DADOS_STR *VetorIndicesStringCriar(int tamanho){
    DADOS_STR *vetor = (DADOS_STR*) calloc(tamanho, sizeof(DADOS_STR));
    if(vetor != NULL){
        return vetor;
    }
    else{
        ErroAlocacao();
        return NULL;
    }
}

void EscreveArqIndiceInt(FILE* arqIndice, DADOS_INT indice){
    if(indice.chaveBusca != -1){
        fwrite(&(indice.chaveBusca), sizeof(int), 1, arqIndice);
        fwrite(&(indice.byteOffset), sizeof(long int), 1, arqIndice);
    }
}



bool CriaIndiceInteiro(char arqEntrada[], char arqSaida[], char campo[]){
    FILE *arqBin;
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalho = CabecalhoIndiceCriar();

    //Aloca cabecalho e registro auxiliar para percorrer arquivo binario
    CABECALHO *cabecalho_registro = CabecalhoCriar();
    DADOS *registro_auxiliar = RegistroCriar();


    //abre os arquivos recebidos na entrada
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", arqBin, NULL)) return false;


    //Escreve cabecalho de indices
    EscreveCabecalhoIndice(arqIndice, cabecalho);
    //fecha para atualizar em disco
    fclose(arqIndice);
    //abre novamente para uso
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", NULL)) return false;
    //ajusta o ponteiro para depois do cabecalho
    fseek(arqIndice, 5, SEEK_SET);

   
    //Le cabeçalho do arquivo binario e avança cursor
    LeCabecalhoDoArqBinario(cabecalho_registro, arqBin);


    //Calcula o numero de registros não removidos e cria um vetor para os indices
    int nroRegistros = GetNroRegArq(cabecalho_registro) - GetNroRegRem(cabecalho_registro);
    DADOS_INT *indices = VetorIndicesIntCriar(nroRegistros);


    //Decide qual o tipo da chave de busca (talvez por em utils)
    int tipoCampo = -1;
    if(strcmp(campo, "idCrime") == 0){
        tipoCampo = 0;
    }else if(strcmp(campo, "numeroArtigo") == 0){
        tipoCampo = 1;
    }else{
        ErroArquivo();
        return false;
    }



    //Percorre arquivo binario lendo os registros
    int offsetlido = 17; //será o offset do proximo registro durante o loop
    int offsetanterior = 17;

    int flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido);
    int i;
    for(i=0; flag!=0; i++){
        if(GetRegistroRemovido(registro_auxiliar) == '0'){
            InsereCampoIntEmIndices(indices, registro_auxiliar, i, tipoCampo, offsetanterior);
        }

        DesalocaCamposVariaveis(registro_auxiliar);

        offsetanterior = offsetlido;

        flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido); 
    }

    //ordenar
    mergeSortIndice(indices, 0, nroRegistros-1);
    //escrever no binario
    for (int i = 0; i < nroRegistros; i++)
    {
        EscreveArqIndiceInt(arqIndice, indices[i]);
        printf("%d %ld\n", indices[i].chaveBusca, indices[i].byteOffset);
    }
    


    //ATUALIZA CABECALHO INDICE
    AtualizaStatusIndice(cabecalho, '1');
    AtualizaNroRegArqIndice(cabecalho, nroRegistros);
    //Coloca o cursor no começo do arq para sobrescrever o cabcecalho
    fseek(arqIndice, 0, SEEK_SET); 
    EscreveCabecalhoIndice(arqIndice, cabecalho);

    //fecha para atualizar em disco
    DesalocaCabecalhoIndice(cabecalho);


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_registro);
    DesalocaRegistro(registro_auxiliar);

    free(indices);

    binarioNaTela(arqSaida);

    fclose(arqBin);
    fclose(arqIndice);
    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();

    return true;
}


bool InsereCampoIntEmIndices(DADOS_INT *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset){
    DADOS_INT *registroDados = RegistroDadosIntCriar();
    
    switch(campo){
        //caso idCrime
        case 0:{
            registroDados->chaveBusca = GetRegistroIdCrime(registro_auxiliar);
            registroDados->byteOffset = byteoffset;

            vetor[posicao].chaveBusca = registroDados->chaveBusca;
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;   
        }
            
        //caso numeroArtigo
        case 1:{
            registroDados->chaveBusca = GetRegistroNroArtigo(registro_auxiliar);
            registroDados->byteOffset = byteoffset;

            vetor[posicao].chaveBusca = registroDados->chaveBusca;
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }     
    }

    free(registroDados);
}


bool CriaIndiceString(char arqEntrada[], char arqSaida[], char campo[]){
    FILE *arqBin;
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalho = CabecalhoIndiceCriar();

    //abre os arquivos recebidos na entrada
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", arqBin, NULL)) return false;

    //Escreve cabecalho
    EscreveCabecalhoIndice(arqIndice, cabecalho);

    //fecha para atualizar em disco
    fclose(arqIndice);

    //abre novamente para uso
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", NULL)) return false;

    //ajusta o ponteiro para depois do cabecalho
    fseek(arqIndice, 5, SEEK_SET);

    //Aloca cabecalho e registro auxiliar para percorrer arquivo binario
    CABECALHO *cabecalho_registro = CabecalhoCriar();
    DADOS *registro_auxiliar = RegistroCriar();

    LeCabecalhoDoArqBinario(cabecalho_registro, arqBin);

    //Calcula o numero de registros não removidos e cria um vetor para os indices
    int nroRegistros = GetNroRegArq(cabecalho_registro) - GetNroRegRem(cabecalho_registro);
    DADOS_STR *indices = VetorIndicesStringCriar(nroRegistros);

    int tipoCampo = -1;
    if(strcmp(campo, "dataCrime") == 0){
        tipoCampo = 0;
    }else if(strcmp(campo, "marcaCelular") == 0){
        tipoCampo = 1;
    }else if(strcmp(campo, "lugarCrime") == 0){
        tipoCampo = 2;
    }else if(strcmp(campo, "descricaoCrime") == 0){
        tipoCampo = 3;
    }else{
        ErroArquivo();
        return false;
    }



    //Percorre arquivo binario lendo os registros
    int offsetlido = 17; //será o offset do próximo registro durante o loop
    int offsetanterior = 17;

    int flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido);
    int i;
    for(i=0; flag!=0; i++){
        if(GetRegistroRemovido(registro_auxiliar) == '0'){
            InsereCampoStringEmIndices(indices, registro_auxiliar, i, tipoCampo, offsetanterior);
        }

        


        DesalocaCamposVariaveis(registro_auxiliar);

        offsetanterior = offsetlido;

        flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido); 
    }

    for (int i = 0; i < nroRegistros; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            printf("%c", indices[i].chaveBusca[j]);
        }
        
        printf(" %ld\n", indices[i].byteOffset);
    }


    //ATUALIZA CABECALHO INDICE
    AtualizaStatusIndice(cabecalho, '1');
    AtualizaNroRegArqIndice(cabecalho, nroRegistros);
    //Coloca o cursor no começo do arq para sobrescrever o cabcecalho
    fseek(arqIndice, 0, SEEK_SET); 
    EscreveCabecalhoIndice(arqIndice, cabecalho);
    DesalocaCabecalhoIndice(cabecalho);


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_registro);
    DesalocaRegistro(registro_auxiliar);

    free(indices);

    fclose(arqBin);
    fclose(arqIndice);
    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();
    return true;

}

bool InsereCampoStringEmIndices(DADOS_STR *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset){
    DADOS_STR *registroDados = RegistroDadosStringCriar();
    
    switch(campo){
        //caso dataCrime
        case 0:{
            strncpySem0(registroDados->chaveBusca, GetRegistroDataCrime(registro_auxiliar), 10);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;   
        }
            
        //caso marcaCelular
        case 1:{
            strncpySem0(registroDados->chaveBusca, GetRegistroMarcaCelular(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }    

        //caso lugarCrime
        case 2:{
            strncpySem0(registroDados->chaveBusca, GetRegistroLugarCrime(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }  

        //caso descricaoCrime
        case 3:{
            strncpySem0(registroDados->chaveBusca, GetRegistroDescricaoCrime(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }   
    }

    free(registroDados);
}




void intercala(DADOS_INT* vetor, int inicio, int centro, int fim){
	DADOS_INT* vetorAux = VetorIndicesIntCriar((fim-inicio)+1);

	int i = inicio;	// indice da primeira lista ordenada
	int j = centro+1; //indice da segunda lista ordenada
	int k = 0; //indice do vetor auxiliar

	//compara e intercala os elementos do menor para o maior
	while(i <= centro && j <= fim){
		if (vetor[i].chaveBusca  < vetor[j].chaveBusca){
            //verifica qual � o elemento menor entre as duas listas
			vetorAux[k] = vetor[i];
			i++; //proximo elemento da primeira metade
		}else if(vetor[i].chaveBusca == vetor[j].chaveBusca){
            if (vetor[i].byteOffset <= vetor[j].byteOffset){
                
                vetorAux[k] = vetor[i];
                i++; //proximo elemento da primeira metade
            }
            else{
                vetorAux[k] = vetor[j];
                j++; //proximo elemento da segunda metade
            }
        }
		else{
			vetorAux[k] = vetor[j];
			j++; //proximo elemento da segunda metade
		}
		k++;
	}

	while(i <= centro){//h� elementos na primeira metade ainda?
		vetorAux[k] = vetor[i];
		i++;
		k++;
	}

	while(j <= fim){//h� elementos na segunda metade ainda?
		vetorAux[k] = vetor[j];
		j++;
		k++;
	}

	//atualizando o vetor original com o vetor auxiliar(ordenado)
	for(i = inicio, k = 0; i <= fim; i++,k++)
		vetor[i] = vetorAux[k];
	free(vetorAux);
}

//Método de ordenação MergeSort
void mergeSortIndice(DADOS_INT* vetor, int inicio, int fim){
	//caso base
	if (fim <= inicio) return;

	//processo de divis�o
	int centro = (int)((inicio+fim)/2.0);
	mergeSortIndice(vetor, inicio, centro);
	mergeSortIndice(vetor, centro+1, fim);

	//processo de conquista
	intercala(vetor, inicio, centro, fim);
}


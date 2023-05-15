#include "indiceString.h"
#include "../../utils/utils.h"
#include "../../arquivos/arquivos.h"
#include "../../prints_e_erros/prints_e_erros.h"

//aloca um registro de indice do tipo String
DADOS_STR *IndiceDadosStringCriar(void){
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

//Escreve os indices no binario e atualiza o numero de registros para os casos de chaves nulas
void EscreveArqIndiceString(FILE* arqIndice, DADOS_STR indice, int* nroRegistro){
    if(indice.chaveBusca[0] != '$'){
        fwrite(&(indice.chaveBusca), sizeof(char), 12, arqIndice);
        fwrite(&(indice.byteOffset), sizeof(long int), 1, arqIndice);
    }else{
        (*nroRegistro)--;
    }
}

DADOS_STR LerRegIndiceString(FILE *arqIndice, DADOS_STR *registroIndice){
    fread(&(registroIndice->chaveBusca), sizeof(char), 12, arqIndice);
    fread(&(registroIndice->byteOffset), sizeof(long int), 1, arqIndice);

    return (*registroIndice);
}


bool CriaIndiceString(char nomeArqEntrada[], char nomeArqSaida[], char campo[]){
    FILE *arqBin;
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalhoIndice = CabecalhoIndiceCriar();

    //abre os arquivos recebidos na entrada
    if(!AbreArquivo(&arqBin, nomeArqEntrada, "rb", NULL)) return false;
    if(!AbreArquivo(&arqIndice, nomeArqSaida, "wb+", arqBin, NULL)) return false;

    //Escreve cabecalho
    EscreveCabecalhoIndice(arqIndice, cabecalhoIndice);

    //fecha para atualizar em disco
    fclose(arqIndice);

    //abre novamente para uso
    if(!AbreArquivo(&arqIndice, nomeArqSaida, "rb+", NULL)) return false;

    //ajusta o ponteiro para depois do cabecalho
    fseek(arqIndice, 5, SEEK_SET);

    //Aloca cabecalho e registro auxiliar para percorrer arquivo binario
    CABECALHO *cabecalhoRegistro = CabecalhoCriar();
    DADOS *registroAuxiliar = RegistroCriar();

    LeCabecalhoDoArqBinario(cabecalhoRegistro, arqBin);

    //Calcula o numero de registros não removidos e cria um vetor para os indices
    int nroRegistros = GetNroRegArq(cabecalhoRegistro) - GetNroRegRem(cabecalhoRegistro);
    DADOS_STR *indices = VetorIndicesStringCriar(nroRegistros);


    //Decide qual o tipo da chave de busca 
    int tipoCampo = TipoChaveBusca(campo);
    

    //Percorre arquivo binario lendo os registros
    long int offsetProximoRegistro = 17; //será o offset do próximo registro durante o loop
    long int offsetInicioRegistro = 17;

    //usada para identificar momento de parada (EOF)
    int flag = LerRegBinario(arqBin, registroAuxiliar, &offsetProximoRegistro);

    //armazena a posição da próxima inserção
    int pos=0;
    int i;
    for(i = 0; flag != 0; i++){
        if(GetRegistroRemovido(registroAuxiliar) == '0'){
            InsereCampoStringEmIndices(indices, registroAuxiliar, pos, tipoCampo, offsetInicioRegistro);
            pos++;
        }

        DesalocaCamposVariaveis(registroAuxiliar);

        offsetInicioRegistro = offsetProximoRegistro;

        flag = LerRegBinario(arqBin, registroAuxiliar, &offsetProximoRegistro); 
    }
    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();

    //ordenar
    mergeSortIndiceString(indices, 0, nroRegistros-1);

    //É preciso uma cópia para não interferir no for
    int copiaNroRegistro = nroRegistros;
    for (int i = 0; i < nroRegistros; i++)
    {
        EscreveArqIndiceString(arqIndice, indices[i], &copiaNroRegistro);
    }
    

    //Atualiza cabeçalho índice
    AtualizaStatusIndice(cabecalhoIndice, '1');
    AtualizaNroRegArqIndice(cabecalhoIndice, copiaNroRegistro);
    ArqIndReescreveCabecalho(arqIndice, cabecalhoIndice);
    
    //desaloca os auxiliares criados
    DesalocaCabecalhoIndice(cabecalhoIndice);
    DesalocaCabecalho(cabecalhoRegistro);
    DesalocaRegistro(registroAuxiliar);

    free(indices);

    fclose(arqBin);
    fclose(arqIndice);

    return true;

}

bool InsereCampoStringEmIndices(DADOS_STR *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset){
    DADOS_STR *registroDados = IndiceDadosStringCriar();
    
    switch(campo){
        //caso dataCrime
        case 2:{
            strncpySem0(registroDados->chaveBusca, GetRegistroDataCrime(registro_auxiliar), 10);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;   
        }
            
        //caso marcaCelular
        case 3:{
            strncpySem0(registroDados->chaveBusca, GetRegistroMarcaCelular(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }    

        //caso lugarCrime
        case 4:{
            strncpySem0(registroDados->chaveBusca, GetRegistroLugarCrime(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }  

        //caso descricaoCrime
        case 5:{
            strncpySem0(registroDados->chaveBusca, GetRegistroDescricaoCrime(registro_auxiliar), 12);
            registroDados->byteOffset = byteoffset;

            strncpySem0(vetor[posicao].chaveBusca, registroDados->chaveBusca, 12);
            vetor[posicao].byteOffset = registroDados->byteOffset;
            break;    
        }   
    }

    free(registroDados);
}


void intercalaString(DADOS_STR* vetor, int inicio, int centro, int fim){
	DADOS_STR* vetorAux = VetorIndicesStringCriar((fim-inicio)+1);

	int i = inicio;	// indice da primeira lista ordenada
	int j = centro+1; //indice da segunda lista ordenada
	int k = 0; //indice do vetor auxiliar

	//compara e intercala os elementos do menor para o maior
	while(i <= centro && j <= fim){
		if (strncmp(vetor[i].chaveBusca, vetor[j].chaveBusca, 12) < 0){
            //verifica qual � o elemento menor entre as duas listas
			vetorAux[k] = vetor[i];
			i++; //proximo elemento da primeira metade
		}else if(strncmp(vetor[i].chaveBusca, vetor[j].chaveBusca, 12) == 0){
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
void mergeSortIndiceString(DADOS_STR* vetor, int inicio, int fim){
	//caso base
	if (fim <= inicio) return;

	//processo de divis�o
	int centro = (int)((inicio+fim)/2.0);
	mergeSortIndiceString(vetor, inicio, centro);
	mergeSortIndiceString(vetor, centro+1, fim);

	//processo de conquista
	intercalaString(vetor, inicio, centro, fim);
}

long int* buscaBinariaString(DADOS_STR* vetor, int posicaoInicial, int posicaoFinal, char *chave, long int *vetorByteOffset){
    int posInicialGlobal = posicaoInicial;
    int posFinalGlobal = posicaoFinal;
    int len = strlen(chave); //para nn ler os '$'
    int tam_vetor = 1;
	while(posicaoInicial <= posicaoFinal){
		int centro = (int)((posicaoInicial+posicaoFinal)/2);

		if (strncmp(chave, vetor[centro].chaveBusca, len)==0) {
            int centroAux = centro;
            
            //volta ate o primeiro que satisfaz
            while (centroAux >= posInicialGlobal && strncmp(chave, vetor[centroAux].chaveBusca, len)==0 )
            {
                centroAux--;
            }

            centroAux++;
            //ao encontrar um que satisfaz, verifica todos acima dele que sejam iguais 
            while (centroAux <= posFinalGlobal && strncmp(chave, vetor[centroAux].chaveBusca, len)==0 )
            {
                vetorByteOffset[tam_vetor-1] = vetor[centroAux].byteOffset;
                tam_vetor++;
                vetorByteOffset = (long int *) realloc(vetorByteOffset, sizeof(long int) * tam_vetor);
                centroAux++;
            } 
              
            break;
        }
			
		else if (strncmp(chave, vetor[centro].chaveBusca, 12) < 0) 
			posicaoFinal = centro - 1;
		else if (strncmp(chave, vetor[centro].chaveBusca, 12) > 0) 
			posicaoInicial = centro + 1;
	}

    //Coloca o -1 para saber qual é o ultimo elemento
    vetorByteOffset[tam_vetor-1] = -1;
    return vetorByteOffset;
}


//FUnção que dado um arquivo de indice e uma chave de busca, retorna todos os bytesOffsets que satisfazem
long int* BuscaBinariaIndiceString(char *nomeArqIndice, char *valorBuscado, long int *vetorByteOffset){
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalhoAux = CabecalhoIndiceCriar();

    //abre o arquivo de indice
    if(!AbreArquivo(&arqIndice, nomeArqIndice, "rb", NULL)) return NULL;

    LeCabecalhoDoArqIndice(cabecalhoAux, arqIndice);
    //Numero de registros do cabecalho
    int nroReg = GetNroRegArqIndice(cabecalhoAux);

    //Le arqIndice e preenche o vetor
    DADOS_STR *vetorIndices = VetorIndicesStringCriar(nroReg);
    PreencheVetorIndicesSTR(arqIndice, vetorIndices, nroReg);
 
    vetorByteOffset = buscaBinariaString(vetorIndices, 0, nroReg, valorBuscado, vetorByteOffset);

    DesalocaCabecalhoIndice(cabecalhoAux);
    fclose(arqIndice);
    free(vetorIndices);

    return vetorByteOffset;
}

//Dado um arquivo de indice, le o arq e preenche um vetor.
void PreencheVetorIndicesSTR(FILE *arqIndice, DADOS_STR *vetor, int tamanho){
    DADOS_STR *registroIndice = IndiceDadosStringCriar();
    //Preenche o vetor de indices
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i] = LerRegIndiceString(arqIndice, registroIndice);
    }
    free(registroIndice);
}


//Copia os dados de acordo com o tipoCampo pedido
void CopiaChaveEByteOffsetSTR(DADOS *registro, DADOS_STR *registroIndice, int byteoffset, int tipoCampo){
    switch(tipoCampo){
        //caso dataCrime
        case 2:{
            strncpySemBarra0(registroIndice->chaveBusca, registro->dataCrime, 12);
            registroIndice->byteOffset = byteoffset;
            break;
        }

        //caso marcaCelular
        case 3:{
            strncpySemBarra0(registroIndice->chaveBusca, registro->marcaCelular, 12);
            registroIndice->byteOffset = byteoffset;
            break;
        }

        //caso lugarCrime
        case 4:{
            strncpySem0(registroIndice->chaveBusca, registro->lugarCrime, 12);
            registroIndice->byteOffset = byteoffset;
            break;
        }

        //caso descricaoCrime
        case 5:{
            strncpySem0(registroIndice->chaveBusca, registro->descricaoCrime, 12);
            registroIndice->byteOffset = byteoffset;
            break;
        }

    }
}


void InsereVetorIndicesOrdenadoSTR(DADOS_STR *vetorIndices, DADOS_STR *registroIndice, int tamanho){
    //algoritmo usado esta disponivel em https://www.sanfoundry.com/c-program-insert-element-specified-position-array/
    int pos;

    for(int i=0; i<tamanho; i++){
        if(strncmp(registroIndice->chaveBusca, vetorIndices[i].chaveBusca, strlen(vetorIndices[i].chaveBusca)) < 0){
            pos = i;
            break;
        }

        if(strncmp(registroIndice->chaveBusca, vetorIndices[i].chaveBusca, strlen(vetorIndices[i].chaveBusca)) > 0){
            pos = tamanho;
            break;
        }
    }


    if(pos != tamanho){
        int deslocamento = tamanho - pos + 1;

        for(int i=0; i<deslocamento; i++){
            strcpy(vetorIndices[tamanho - i + 2].chaveBusca, vetorIndices[tamanho - i + 1].chaveBusca);
        }
    }

    strncpySem0(vetorIndices[pos].chaveBusca, registroIndice->chaveBusca, 12);
    vetorIndices[pos].byteOffset = registroIndice->byteOffset; 
}



void ShiftadaDoRemovidoString(DADOS_STR *vetorIndices, int tamanhoVetor, int posRemovido){
    for (int i = posRemovido; i < tamanhoVetor-1; i++)
    {
        DADOS_STR aux = vetorIndices[i];
        vetorIndices[i] = vetorIndices[i+1];
        vetorIndices[i+1] = aux;
    }
}

//Remove fisicamente um registro do arquivo de indice
void RemoveArquivoIndiceString(char *nomeArqIndice, long int byteOffset){
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalhoIndiceAux = CabecalhoIndiceCriar();
    DADOS_STR *registroIndiceAux= IndiceDadosStringCriar();

    //abre o arquivo de indice
    if(!AbreArquivo(&arqIndice, nomeArqIndice, "rb", NULL)) return;

    LeCabecalhoDoArqIndice(cabecalhoIndiceAux, arqIndice);

    int nroReg = GetNroRegArqIndice(cabecalhoIndiceAux);

    DADOS_STR *vetorIndices = VetorIndicesStringCriar(nroReg);

    int posByteOffset = -1;
    //Preenche o vetor de indices
    for (int i = 0; i < nroReg; i++)
    {
        vetorIndices[i] = LerRegIndiceString(arqIndice, registroIndiceAux);
        if(vetorIndices[i].byteOffset == byteOffset){
            posByteOffset = i;
            strcpy(vetorIndices[i].chaveBusca, "$$$$$$$$$$$"); //Indica que esta removido
        }
    }

    if(posByteOffset == -1){
        ErroArquivo();  
    } 

    fclose(arqIndice); //Fecha o arquivo para leitura


    //abre o arquivo de indice para reescrita
    if(!AbreArquivo(&arqIndice, nomeArqIndice, "wb", NULL)) return ;

    LeCabecalhoDoArqIndice(cabecalhoIndiceAux, arqIndice);

    if(posByteOffset != -1){
        ShiftadaDoRemovidoString(vetorIndices, nroReg, posByteOffset);
    }

    AtualizaNroRegArqIndice(cabecalhoIndiceAux, nroReg-1);

    EscreveCabecalhoIndice(arqIndice, cabecalhoIndiceAux);

    for (int i = 0; i < nroReg; i++)
    {
        EscreveArqIndiceString(arqIndice, vetorIndices[i], &nroReg);
    }


    free(vetorIndices);
    DesalocaCabecalhoIndice(cabecalhoIndiceAux);
    free(registroIndiceAux);
    fclose(arqIndice);
}
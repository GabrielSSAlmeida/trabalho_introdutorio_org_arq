#include "indiceInteiro.h"
#include "../../utils/utils.h"
#include "../../arquivos/arquivos.h"
#include "../../prints_e_erros/prints_e_erros.h"

//Criação das Structs
//aloca um registro de indice do tipo inteiro
DADOS_INT *IndiceDadosIntCriar(void){
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

DADOS_INT LerRegIndiceInt(FILE *arqIndice, DADOS_INT *registroIndice){
    fread(&(registroIndice->chaveBusca), sizeof(int), 1, arqIndice);
    fread(&(registroIndice->byteOffset), sizeof(long int), 1, arqIndice);

    return (*registroIndice);
}


//Escreve os indices no binario e atualiza o numero de registros para os casos de chaves nulas
void EscreveArqIndiceInt(FILE* arqIndice, DADOS_INT indice, int*nroRegistros){
    //verifica se a chave de busca eh nula
    if(indice.chaveBusca != -1){
        fwrite(&(indice.chaveBusca), sizeof(int), 1, arqIndice);
        fwrite(&(indice.byteOffset), sizeof(long int), 1, arqIndice);
    }else{
        (*nroRegistros)--;
    }
}

//insere os campos chaveBusca e byteOffset na pos do vetor
bool InsereCampoIntEmIndices(DADOS_INT *vetor, DADOS *registro_auxiliar, int posicao, int campo, int byteoffset){
    DADOS_INT *registroDados = IndiceDadosIntCriar();
    
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

//Cria um arquivo de indice que a chave de busca é do tipo inteiro
bool CriaIndiceInteiro(char arqEntrada[], char arqSaida[], char campo[]){
    FILE *arqBin;
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalhoIndice = CabecalhoIndiceCriar();

    //Aloca cabecalho e registro auxiliar para percorrer arquivo binario
    CABECALHO *cabecalhoRegistro = CabecalhoCriar();
    DADOS *registroAuxiliar = RegistroCriar();


    //abre os arquivos recebidos na entrada
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;
    if(!AbreArquivo(&arqIndice, arqSaida, "wb+", arqBin, NULL)) return false;


    //Escreve cabecalho de indices
    EscreveCabecalhoIndice(arqIndice, cabecalhoIndice);
    //fecha para atualizar em disco
    fclose(arqIndice);
    //abre novamente para uso
    if(!AbreArquivo(&arqIndice, arqSaida, "rb+", NULL)) return false;
    //ajusta o ponteiro para depois do cabecalho
    fseek(arqIndice, 5, SEEK_SET);

   
    //Le cabeçalho do arquivo binario e avança cursor
    LeCabecalhoDoArqBinario(cabecalhoRegistro, arqBin);


    //Calcula o numero de registros não removidos e cria um vetor para os indices
    int nroRegistros = GetNroRegArq(cabecalhoRegistro) - GetNroRegRem(cabecalhoRegistro);
    DADOS_INT *indices = VetorIndicesIntCriar(nroRegistros);


    //Decide qual o tipo da chave de busca 
    int tipoCampo = TipoChaveBusca(campo);

    //Percorre arquivo binario lendo os registros
    long int offsetProximoRegistro = 17; //será o offset do proximo registro durante o loop
    long int offsetInicioRegistro = 17;

    //A flag verifica se o id foi lido e serve para saber se chegou no final do arquivo
    int flag = LerRegBinario(arqBin, registroAuxiliar, &offsetProximoRegistro);

    //pos é utilizada para armazenar a posicao que o campo deve ser inserido
    int pos = 0;
    int i;
    //le registros até o final do arquivo, inserindo no vetor de indices
    for(i = 0; flag != 0; i++){
        if(GetRegistroRemovido(registroAuxiliar) == '0'){
            InsereCampoIntEmIndices(indices, registroAuxiliar, pos, tipoCampo, offsetInicioRegistro);
            pos++;
        }

        DesalocaCamposVariaveis(registroAuxiliar);

        offsetInicioRegistro = offsetProximoRegistro;

        flag = LerRegBinario(arqBin, registroAuxiliar, &offsetProximoRegistro); 
    }


    //ordenar
    mergeSortIndiceInt(indices, 0, nroRegistros-1);

    //É preciso uma cópia para não interferir no for(o nroRegistros diminui a cada chave nula)
    int copiaNroRegistro = nroRegistros;
    //escrever no binario
    for (int j = 0; j < nroRegistros; j++)
    {
        EscreveArqIndiceInt(arqIndice, indices[j], &copiaNroRegistro);
    }

    //se nao existem registros no arquivo
    if(i==0) ErroArquivo();

    
    //Atualiza cabecalho do arquivo de indices
    AtualizaStatusIndice(cabecalhoIndice, '1');
    AtualizaNroRegArqIndice(cabecalhoIndice, copiaNroRegistro);
    //Coloca o cursor no começo do arq para sobrescrever o cabecalho
    ArqIndReescreveCabecalho(arqIndice, cabecalhoIndice);

    //desaloca os auxiliares criados
    DesalocaCabecalhoIndice(cabecalhoIndice);
    DesalocaCabecalho(cabecalhoRegistro);
    DesalocaRegistro(registroAuxiliar);

    free(indices);

    //fecha para atualizar em disco
    fclose(arqBin);
    fclose(arqIndice);
    
    return true;
}

//Função auxiliar do merge sort
void intercalaInt(DADOS_INT* vetor, int inicio, int centro, int fim){
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

//ordena o vetor de indices do tipo inteiro
void mergeSortIndiceInt(DADOS_INT* vetor, int inicio, int fim){

	//caso base
	if (fim <= inicio) return;

	//processo de divis�o
	int centro = (int)((inicio+fim)/2.0);
	mergeSortIndiceInt(vetor, inicio, centro);
	mergeSortIndiceInt(vetor, centro+1, fim);

	//processo de conquista
	intercalaInt(vetor, inicio, centro, fim);
}



long int* buscaBinariaInt(DADOS_INT* vetor, int posicaoInicial, int posicaoFinal, int chave, long int *vetorByteOffset){
    int posInicialGlobal = posicaoInicial;
    int posFinalGlobal = posicaoFinal;
    int tam_vetor = 1;
	while(posicaoInicial <= posicaoFinal){
		int centro = (int)((posicaoInicial+posicaoFinal)/2);
		
		if (chave == vetor[centro].chaveBusca) {
            int centroAux = centro;    

            //volta ate o primeiro que satisfaz
            while (centroAux >= posInicialGlobal && chave == vetor[centroAux].chaveBusca )
            {
                centroAux--;
            }
            
            centroAux++;

            //ao encontrar um que satisfaz, verifica todos acima dele que sejam iguais 
            while (centroAux <= posFinalGlobal && chave == vetor[centroAux].chaveBusca )
            {
                vetorByteOffset[tam_vetor-1] = vetor[centroAux].byteOffset;
                tam_vetor++;
                vetorByteOffset = (long int *) realloc(vetorByteOffset, sizeof(long int) * tam_vetor);
                centroAux++;
            }       
            break;
        }
			
		else if (chave < vetor[centro].chaveBusca) 
			posicaoFinal = centro - 1;
		else if (chave > vetor[centro].chaveBusca) 
			posicaoInicial = centro + 1;
	}

    //Coloca o -1 para saber qual é o ultimo elemento
    vetorByteOffset[tam_vetor-1] = -1;
    return vetorByteOffset;
}


//FUnção que dado um arquivo de indice e uma chave de busca, retorna todos os bytesOffsets que satisfazem
long int* BuscaBinariaIndiceInt(char *nomeArqIndice, int valorBuscado, long int *vetorByteOffset){
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalhoIndiceAux = CabecalhoIndiceCriar();

    //abre o arquivo de indice
    if(!AbreArquivo(&arqIndice, nomeArqIndice, "rb", NULL)) return NULL;

    LeCabecalhoDoArqIndice(cabecalhoIndiceAux, arqIndice);

    //Numero de registros do cabecalho
    int nroReg = GetNroRegArqIndice(cabecalhoIndiceAux);

    //Le arqIndice e preenche o vetor
    DADOS_INT *vetorIndices = VetorIndicesIntCriar(nroReg);
    PreencheVetorIndicesINT(arqIndice, vetorIndices, nroReg);

    vetorByteOffset = buscaBinariaInt(vetorIndices, 0, nroReg, valorBuscado, vetorByteOffset);

    DesalocaCabecalhoIndice(cabecalhoIndiceAux);
    fclose(arqIndice);

    
    free(vetorIndices);
    return vetorByteOffset;
}

//Dado um arquivo de indice, le o arq e preenche um vetor.
void PreencheVetorIndicesINT(FILE *arqIndice, DADOS_INT *vetor, int tamanho){
    DADOS_INT *registroIndice = IndiceDadosIntCriar();
    //Preenche o vetor de indices
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i] = LerRegIndiceInt(arqIndice, registroIndice);
    }
    free(registroIndice);
}

void InsereVetorIndicesOrdenadoINT(DADOS_INT *vetorIndices, DADOS_INT *registroIndice, int tamanho){
    //algoritmo usado esta disponivel em https://www.sanfoundry.com/c-program-insert-element-specified-position-array/
    int pos;

    for(int i=0; i<tamanho; i++){
        if(registroIndice->chaveBusca < vetorIndices[i].chaveBusca){
            pos = i;
            break;
        }

        if(registroIndice->chaveBusca > vetorIndices[tamanho - 1].chaveBusca){
            pos = tamanho;
            break;
        }
    }


    if(pos != tamanho){
        int deslocamento = tamanho - pos + 1;

        for(int i=0; i<deslocamento; i++){
            vetorIndices[tamanho - i + 2] = vetorIndices[tamanho - i + 1];
        }
    }
    vetorIndices[pos].chaveBusca = registroIndice->chaveBusca;
    vetorIndices[pos].byteOffset = registroIndice->byteOffset; 
}

//Copia os dados de acordo com o tipoCampo pedido
void CopiaChaveEByteOffsetINT(DADOS *registro, DADOS_INT *registroIndice, int byteoffset, int tipoCampo){
    switch(tipoCampo){
        //caso idCrime
        case 0:{
            registroIndice->chaveBusca = registro->idCrime;
            registroIndice->byteOffset = byteoffset;
            break;
        }
        //caso numeroArtigo
        case 1:{
            registroIndice->chaveBusca = registro->numeroArtigo;
            registroIndice->byteOffset = byteoffset;
            break;
        }
    }
}

void ShiftadaDoRemovidoInt(DADOS_INT *vetorIndices, int tamanhoVetor, int posRemovido){
    for (int i = posRemovido; i < tamanhoVetor-1; i++)
    {
        DADOS_INT aux = vetorIndices[i];
        vetorIndices[i] = vetorIndices[i+1];
        vetorIndices[i+1] = aux;
    }
}

//Remove fisicamente um registro do arquivo de indice
void RemoveArquivoIndiceInt(char *nomeArqIndice, long int byteOffset){
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalhoAux = CabecalhoIndiceCriar();
    DADOS_INT *registroIndiceAux= IndiceDadosIntCriar();


    //abre o arquivo de indice
    if(!AbreArquivo(&arqIndice, nomeArqIndice, "rb", NULL)) return ;

    LeCabecalhoDoArqIndice(cabecalhoAux, arqIndice);

    int nroReg = GetNroRegArqIndice(cabecalhoAux);

    DADOS_INT *vetorIndices = VetorIndicesIntCriar(nroReg);

    //armazena a posição do registro que deve ser removido
    int posByteOffset = -1;
    //Preenche o vetor de indices
    for (int i = 0; i < nroReg; i++)
    {
        vetorIndices[i] = LerRegIndiceInt(arqIndice, registroIndiceAux);
        if(vetorIndices[i].byteOffset == byteOffset){
            posByteOffset = i;
            vetorIndices[i].chaveBusca = -1;
        }
    }

    if(posByteOffset == -1){
        ErroArquivo();  
    } 

    fclose(arqIndice); //Fecha o arquivo para leitura


    //abre o arquivo de indice para reescrita
    if(!AbreArquivo(&arqIndice, nomeArqIndice, "wb", NULL)) return ;

    LeCabecalhoDoArqIndice(cabecalhoAux, arqIndice);


    if(posByteOffset != -1){
        ShiftadaDoRemovidoInt(vetorIndices, nroReg, posByteOffset);
    }

    AtualizaNroRegArqIndice(cabecalhoAux, nroReg-1);

    EscreveCabecalhoIndice(arqIndice, cabecalhoAux);

    for (int i = 0; i < nroReg; i++)
    {
        EscreveArqIndiceInt(arqIndice, vetorIndices[i], &nroReg);
    }

    free(vetorIndices);
    DesalocaCabecalhoIndice(cabecalhoAux);
    free(registroIndiceAux);
    fclose(arqIndice);
    
}
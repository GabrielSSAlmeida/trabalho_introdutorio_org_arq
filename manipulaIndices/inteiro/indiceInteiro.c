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

//Preenche o vetor de indices, ou seja, insere os campos chaveBusca e byteOffset em cada pos do vetor
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
    if(!AbreArquivo(&arqIndice, arqSaida, "rb+", NULL)) return false;
    //ajusta o ponteiro para depois do cabecalho
    fseek(arqIndice, 5, SEEK_SET);

   
    //Le cabeçalho do arquivo binario e avança cursor
    LeCabecalhoDoArqBinario(cabecalho_registro, arqBin);


    //Calcula o numero de registros não removidos e cria um vetor para os indices
    int nroRegistros = GetNroRegArq(cabecalho_registro) - GetNroRegRem(cabecalho_registro);
    DADOS_INT *indices = VetorIndicesIntCriar(nroRegistros);


    //Decide qual o tipo da chave de busca 
    int tipoCampo = TipoChaveBusca(campo);

    //Percorre arquivo binario lendo os registros
    int offsetlido = 17; //será o offset do proximo registro durante o loop
    int offsetanterior = 17;
    //A flag verifica se o id foi lido e serve para saber se chegou no final do arquivo
    int flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido);
    int pos=0;
    int i;
    for(i=0; flag!=0; i++){
        if(GetRegistroRemovido(registro_auxiliar) == '0'){
            InsereCampoIntEmIndices(indices, registro_auxiliar, pos, tipoCampo, offsetanterior);
            pos++;
        }

        DesalocaCamposVariaveis(registro_auxiliar);

        offsetanterior = offsetlido;

        flag = LerRegBinario(arqBin, registro_auxiliar, &offsetlido); 
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

    
    //ATUALIZA CABECALHO INDICE
    AtualizaStatusIndice(cabecalho, '1');
    AtualizaNroRegArqIndice(cabecalho, copiaNroRegistro);
    //Coloca o cursor no começo do arq para sobrescrever o cabcecalho
    fseek(arqIndice, 0, SEEK_SET); 
    EscreveCabecalhoIndice(arqIndice, cabecalho);

    //fecha para atualizar em disco
    DesalocaCabecalhoIndice(cabecalho);


    //desaloca os auxiliares criados
    DesalocaCabecalho(cabecalho_registro);
    DesalocaRegistro(registro_auxiliar);

    free(indices);


    fclose(arqBin);
    fclose(arqIndice);

    
    binarioNaTela(arqSaida);
    
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



long int* buscaBinaria_iterativa_int(DADOS_INT* vetor, int posicaoInicial, int posicaoFinal, int chave, long int *vetorByteOffset){
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



long int* BuscaBinariaIndiceInt(char *nomeArqIndice, int valorBuscado, long int *vetorByteOffset){
    FILE *arqIndice;
    CABECALHO_INDICE *cabecalho_aux = CabecalhoIndiceCriar();
    DADOS_INT *registroIndice_aux= IndiceDadosIntCriar();


    //abre o arquivo de indice
    if(!AbreArquivo(&arqIndice, nomeArqIndice, "r", NULL)) return NULL;

    LeCabecalhoDoArqIndice(cabecalho_aux, arqIndice);

    int nroReg = GetNroRegArqIndice(cabecalho_aux);

    DADOS_INT *vetorIndices = VetorIndicesIntCriar(nroReg);

    //Preenche o vetor de indices
    for (int i = 0; i < nroReg; i++)
    {
        vetorIndices[i] = LerRegIndiceInt(arqIndice, registroIndice_aux);
    }


    vetorByteOffset = buscaBinaria_iterativa_int(vetorIndices, 0, nroReg, valorBuscado, vetorByteOffset);

    DesalocaCabecalhoIndice(cabecalho_aux);
    free(registroIndice_aux);
    fclose(arqIndice);

    
    free(vetorIndices);
    return vetorByteOffset;
}








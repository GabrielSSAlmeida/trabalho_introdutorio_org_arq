#include "funcionalidades.h"


bool funcionalidade1(){
    char arqEntrada[32], arqSaida[32];
    FILE *arqCsv;
    FILE *arqBin;

    //abertura dos arquivos para funcionalidade 1
    RecebeEntradaFunc1(arqEntrada, arqSaida);
    
    //Abre arquivos csv e binario
    //Se a abertura falhar para o código 
    if(!AbreArquivo(&arqCsv, arqEntrada, "r", NULL)) return false;
    if(!AbreArquivo(&arqBin, arqSaida, "wb+", arqCsv, NULL)) return false;

    //Cria o cabecalho que será usado no arquivo binario
    CABECALHO *cabecalho_saida = CabecalhoCriar();
    
    //Ler e ignorar a primeira linha do arquivo CSV
    IgnoraLinha(arqCsv);

    //atualiza o status do cabecalho
    AtualizaStatus(cabecalho_saida, '0');

    //escreve cabecalho no arquivo binario
    EscreveCabecalho(arqBin,cabecalho_saida);

    //fecha arquivo para salvar status
    fclose(arqBin);

    //abre o arquivo novamente
    if(!AbreArquivo(&arqBin, arqSaida, "wb+", NULL)) return false;

    //ajusta o ponteiro para dps do cabecalho
    fseek(arqBin, GetByteOffset(cabecalho_saida), SEEK_SET);

    //Le registros do csv e escreve no binario
    CsvParaBin(arqCsv, arqBin, cabecalho_saida);
    
    //atualiza o status do cabecalho
    AtualizaStatus(cabecalho_saida, '1');
    
    //atualiza o cabecalho
    fseek(arqBin, 0, SEEK_SET);
    EscreveCabecalho(arqBin,cabecalho_saida);

    //fecha arquivos
    fclose(arqCsv);
    fclose(arqBin);
    
    binarioNaTela(arqSaida);

    DesalocaCabecalho(cabecalho_saida);

    return true;
}


bool funcionalidade2(){
    char arqEntrada[32];
    FILE *arqBin;

    RecebeEntradaFunc2(arqEntrada);

    //abre arquivo binario para funcionalidade 2
    if(!AbreArquivo(&arqBin, arqEntrada, "rb", NULL)) return false;

    //imprime os registro do arquivo binario na tela
    if(!ImprimirBinario(arqBin)){
        fclose(arqBin);
        return false;
    }

    fclose(arqBin);

    return true;
}

bool funcionalidade3(){
    char arqEntrada[32];
    char arqSaida[32];
    char campo[32];
    char dado[32];

    RecebeEntradaFunc3(arqEntrada, campo, dado, arqSaida);

    //verifica se o tipo de dado é string ou int
    int dadoVerificado = VerificaDado(dado);

    switch(dadoVerificado){
        //caso int
        case 0:{
            CriaIndiceInteiro(arqEntrada, arqSaida, campo);
            break;
        }
        //caso string
        case 1:{
            CriaIndiceString(arqEntrada, arqSaida, campo);
            break;
        }
        default:
            return false;
            break;
    }

    return true;
}

bool funcionalidade4(){
    char arqEntrada[32];
    char nomeArqIndice[32];
    char campoIndexado[32];
    char dado[32];
    int qtdBuscas;

    RecebeEntradaFunc4(arqEntrada, campoIndexado, dado, nomeArqIndice, &qtdBuscas);



    //verifica se o tipo de dado é string(1) ou int(0)
    int dadoVerificado = VerificaDado(dado);


    for (int i = 0; i < qtdBuscas; i++)
    {
        int qtdPares;
        scanf("%d ", &qtdPares);

        PARES_BUSCA *paresBusca = VetorParesBuscaCriar(qtdPares);


        for (int j = 0; j < qtdPares; j++)
        {
            char *nomeCampo = GetNomeCampo(paresBusca, j);
            scanf("%s ", nomeCampo);
            int tipoCampo = TipoChaveBusca(nomeCampo);

            AtualizaTipoCampo(paresBusca, j, tipoCampo);
            if (tipoCampo == 0 || tipoCampo == 1){ //O campo é do tipo int
                int valorCampoInt;
                scanf("%d ", &valorCampoInt);
                AtualizaValorCampoInt(paresBusca, j, valorCampoInt);

            }else if(tipoCampo >= 2){
                scan_quote_string(GetValorCampoString(paresBusca, j));

            }else{//Caso seja -1
                ErroArquivo();
            }
        } 
        

        

        printf("Resposta para a busca %d\n", i+1);
        int contador = 0;
        
        //Vai indicar os byteOffset dos registros encontrados
        //-1 indica o fim do vetor
        //0 indica o byteOffset que não passou em todos os casos de busca
        long int *vetorByteOffset  = calloc(1, sizeof(long int));

        //Primeira busca eh binaria
        if(DecideOrdemBusca(paresBusca, qtdPares, campoIndexado)){

            FILE *arqBin;
            if(!AbreArquivo(&arqBin, arqEntrada, "r", NULL)) return false;

            int tipoCampo = GetTipoCampo(paresBusca, 0);

            if (tipoCampo == 0 || tipoCampo == 1){ //O campo é do tipo int
                vetorByteOffset =  BuscaBinariaIndiceInt(nomeArqIndice, GetValorCampoInt(paresBusca, 0), vetorByteOffset);

            }else if(tipoCampo >= 2){ //O campo é do tipo string
               vetorByteOffset =  BuscaBinariaIndiceString(nomeArqIndice, GetValorCampoString(paresBusca, 0), vetorByteOffset);
            }

            contador++;

            /*  A partir das buscas retornadas da busca binaria, verifica se esses campos
                satisfazem os outros criterios de busca
            */
            while (contador < qtdPares){
                int tipoCampo = GetTipoCampo(paresBusca, contador);

                for (int j = 0; vetorByteOffset[j] != -1 ; j++)
                {
                    DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);

                    switch (tipoCampo)
                    {
                        case 0:
                            if(GetRegistroIdCrime(registro_aux) != GetValorCampoInt(paresBusca, contador)){
                                vetorByteOffset[j] = 0;
                            }
                            break;
                        
                        case 1:
                            if(GetRegistroNroArtigo(registro_aux) != GetValorCampoInt(paresBusca, contador)){
                                vetorByteOffset[j] = 0;
                            }
                            break;
                        
                        case 2:
                            if(strncmp(GetRegistroDataCrime(registro_aux), 
                            GetValorCampoString(paresBusca, contador), 10) != 0){
                                vetorByteOffset[j] = 0;
                            }
                            break;

                        case 3:
                            if(strncmp(GetRegistroMarcaCelular(registro_aux), 
                            GetValorCampoString(paresBusca, contador), 12) !=0){
                                vetorByteOffset[j] = 0;
                            }
                            break;

                        case 4:
                            if(strcmpAtePipe(GetRegistroLugarCrime(registro_aux), 
                            GetValorCampoString(paresBusca, contador))!= 0){
                                vetorByteOffset[j] = 0;
                            }
                            break;
                        
                        case 5:
                            if(strcmpAtePipe(GetRegistroDescricaoCrime(registro_aux), 
                            GetValorCampoString(paresBusca, contador)) != 0){
                                vetorByteOffset[j] = 0;
                            }
                            break;
                        default:
                            break;
                    }

                    DesalocaRegistro(registro_aux);
                }
                
                contador++;
            }

            //Não encontrou o registro
            if(vetorByteOffset[0] == -1){
                ErroRegistro();
            }else{
                int passou = 0;
                int tamanho = 0;
                //Percorre vetor de byteoffset e le os registros
                for (int j = 0; vetorByteOffset[j] != -1; j++)
                {
                    if(vetorByteOffset[j] != 0){
                        passou = 1;
                        DADOS *registro_aux = LeRegistroPorByteOffset(arqBin, vetorByteOffset[j]);
                        ImprimeRegistroBinario(registro_aux);

                        DesalocaRegistro(registro_aux);
                    }
                }
                
                if(!passou){
                    ErroRegistro();
                }
            }

            
            fclose(arqBin);
        }else{
            //todos os casos são busca sequencial
            BuscaSequencialBinario(arqEntrada, paresBusca, qtdPares);
            
        }
        


        





        free(vetorByteOffset);
        DesalocaParesBusca(paresBusca);
        
    }
    
    return true;
}
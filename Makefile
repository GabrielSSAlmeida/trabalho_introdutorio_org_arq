all: utils.o prints_e_erros.o parBusca.o bEstrela.o cabecalhoBEstrela.o indiceString.o indiceInteiro.o cabecalhoIndice.o cabecalho.o arquivos.o funcionalidades.o registro.o fornecidas.o programaTrab.o
	gcc *.o -o programaTrab -g -Wall

prints_e_erros.o:
	gcc -c -g prints_e_erros/prints_e_erros.c -o prints_e_erros.o

parBusca.o:
	gcc -c -g parBusca/parBusca.c -o parBusca.o

bEstrela.o:
	gcc -c -g manipulaBEstrela/bEstrela.c -o bEstrela.o

cabecalhoBEstrela.o:
	gcc -c -g manipulaBEstrela/cabecalhoBEstrela.c -o cabecalhoBEstrela.o

indiceInteiro.o:
	gcc -c -g manipulaIndices/inteiro/indiceInteiro.c -o indiceInteiro.o

indiceString.o:
	gcc -c -g manipulaIndices/string/indiceString.c -o indiceString.o

cabecalhoIndice.o:
	gcc -c -g cabecalho/cabecalhoIndice.c -o cabecalhoIndice.o

cabecalho.o:
	gcc -c -g cabecalho/cabecalho.c -o cabecalho.o

arquivos.o:
	gcc -c -g arquivos/arquivos.c -o arquivos.o

funcionalidades.o:
	gcc -c -g funcionalidades/funcionalidades.c -o funcionalidades.o

registro.o:
	gcc -c -g manipulaRegistros/registro.c -o registro.o

fornecidas.o:
	gcc -c -g utils/funcoesFornecidas.c -o fornecidas.o

utils.o:
	gcc -c -g utils/utils.c -o utils.o

programaTrab.o:
	gcc -c -g programaTrab.c -o programaTrab.o
	 
clean:
	rm *.o programaTrab

run:
	./programaTrab
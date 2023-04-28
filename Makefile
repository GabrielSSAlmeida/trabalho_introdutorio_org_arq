all: prints_e_erros.o cabecalho.o arquivos.o funcionalidades.o registro.o fornecidas.o utils.o programaTrab.o
	gcc programaTrab.o utils.o fornecidas.o registro.o funcionalidades.o arquivos.o cabecalho.o prints_e_erros.o -o programaTrab -g -Wall

prints_e_erros.o:
	gcc -c -g prints_e_erros/prints_e_erros.c -o prints_e_erros.o

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
	valgrind --leak-check=full --track-origins=yes ./programaTrab < 6.in > saida.out
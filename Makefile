all: prints_e_erros.o registro.o fornecidas.o auxiliares.o programaTrab.o
	gcc programaTrab.o auxiliares.o fornecidas.o registro.o prints_e_erros.o -o programaTrab -g -Wall

prints_e_erros.o:
	gcc -c -g prints_e_erros/prints_e_erros.c -o prints_e_erros.o

registro.o:
	gcc -c -g manipulaRegistros/registro.c -o registro.o

fornecidas.o:
	gcc -c -g auxiliares/funcoesFornecidas.c -o fornecidas.o

auxiliares.o:
	gcc -c -g auxiliares/funcoesAuxiliares.c -o auxiliares.o

programaTrab.o:
	gcc -c -g programaTrab.c -o programaTrab.o
	 
clean:
	rm *.o programaTrab

run:
	./programaTrab
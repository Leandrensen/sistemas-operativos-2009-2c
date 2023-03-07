cache: archivos.o cadenas.o  maincache.o sendrecv.o defines.o 
	gcc -Wall -ansi -lnsl -g archivos.o cadenas.o maincache.o sendrecv.o $(LIBS) -lmemcached -o cache -lpthread 
	make clean

archivos.o: archivos.c archivos.h
	gcc -g -Wall -ansi -c archivos.c
 
cadenas.o: cadenas.c cadenas.h
	gcc -g -Wall -ansi -c cadenas.c

main.o: main.c
	gcc -g -Wall -ansi -c main.c

sendrecv.o: sendrecv.c sendrecv.h
	gcc -g -Wall -ansi -c sendrecv.c
	
defines.o: defines.h
	gcc -g -Wall -ansi -c defines.h

.PHONY : clean

clean:
	rm *.o
	rm *.gch
	

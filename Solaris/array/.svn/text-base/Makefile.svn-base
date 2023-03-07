array:	consola.o sendrecv.o cadenas.o disco.o archivos.o syscall.o sincronizacion.o protocolos.o array.o
	gcc -Wall -ansi -g  consola.o sendrecv.o cadenas.o disco.o archivos.o syscall.o sincronizacion.o array.o -o array -lthread -lsocket -lnsl
	make clean
 
consola.o: consola.c consola.h
	gcc -g -Wall -ansi -c consola.c

sendrecv.o: sendrecv.c sendrecv.h
	gcc -g -Wall -ansi -c sendrecv.c

cadenas.o: cadenas.c cadenas.h
	gcc -g -Wall -ansi -c cadenas.c

disco.o: disco.c disco.h
	gcc -g -Wall -ansi -c disco.c

archivos.o: archivos.c archivos.h
	gcc -g -Wall -ansi -c archivos.c

syscall.o: syscall.c syscall.h
	gcc -g -Wall -ansi -c syscall.c

sincronizacion.o: sincronizacion.c sincronizacion.h
	gcc -g -Wall -ansi -c sincronizacion.c

protocolos.o: protocolos.h
	gcc -g -Wall -ansi -c protocolos.h

array.o: array.c 
	gcc -g -Wall -ansi -c array.c

.PHONY : clean

clean:
	rm *.o
	rm *.gch


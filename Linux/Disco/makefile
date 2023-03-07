LDAPLIB=-lldap  -llber
INCDIR=/usr/local/include
LIBDIR=/usr/local/lib
LIBS= -L $(INCDIR) -L $(LIBDIR) $(LDAPLIB)

disco: archivos.o cache.o cadenas.o calculosYEstadisticas.o main.o primitivas.o sendrecv.o handshake.o defines.o listas.o funcionesLdap.o initBind.o
	gcc -Wall -ansi -lnsl -g archivos.o cache.o cadenas.o calculosYEstadisticas.o main.o primitivas.o sendrecv.o handshake.o listas.o funcionesLdap.o initBind.o $(LIBS) -o disco -lpthread
	make clean

archivos.o: archivos.c archivos.h
	gcc -g -Wall -ansi -c archivos.c
 
cache.o: cache.c cache.h
	gcc -g -Wall -ansi -c cache.c 

cadenas.o: cadenas.c cadenas.h
	gcc -g -Wall -ansi -c cadenas.c

calculosYEstadisticas.o: calculosYEstadisticas.c calculosYEstadisticas.h
	gcc -g -Wall -ansi -c calculosYEstadisticas.c 

main.o: main.c
	gcc -g -Wall -ansi -c main.c

primitivas.o: primitivas.c primitivas.h
	gcc -g -Wall -ansi -c primitivas.c

sendrecv.o: sendrecv.c sendrecv.h
	gcc -g -ansi -c sendrecv.c

handshake.o: handshake.c handshake.h
	gcc -g -Wall -ansi -c handshake.c
	
defines.o: defines.h
	gcc -g -Wall -ansi -c defines.h
	
listas.o: listas.c listas.h
	gcc -g -Wall -ansi -c listas.c

funcionesLdap.o: funcionesLdap.c funcionesLdap.h
	gcc -g -Wall -ansi -c funcionesLdap.c

init.o: initBind.c initBind.h
	gcc -g -Wall -ansi -c initBind.c

.PHONY : clean

clean:
	rm *.o
	rm *.gch
	

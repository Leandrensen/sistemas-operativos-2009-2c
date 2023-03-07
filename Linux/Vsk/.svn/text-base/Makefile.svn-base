LDAPLIB=-lldap  -llber
INCDIR=/usr/local/include
LIBDIR=/usr/local/lib
LIBS= -L $(INCDIR) -L $(LIBDIR) $(LDAPLIB)

vsk: 	sendrecv.o sysopenN.o sysclose.o syslist.o sysread.o syswrite.o archivos.o cadenas.o consola.o protocolo.o oft.o fd.o syscalls.o  funcionesldap.o vsk.o
	gcc -Wall -ansi -g  sendrecv.o sysopenN.o sysclose.o syslist.o sysread.o syswrite.o archivos.o cadenas.o protocolo.o oft.o fd.o syscalls.o  funcionesldap.o vsk.o consola.o $(LIBS) -o vsk -lpthread
	make clean
 
sendrecv.o: sendrecv.c sendrecv.h
	gcc -g -Wall -ansi -c sendrecv.c

sysopenN.o: sysopenN.c sysopenN.h
	gcc -g -Wall -ansi -c sysopenN.c

sysclose.o: sysclose.c sysclose.h
	gcc -g -Wall -ansi -c sysclose.c

syslist.o: syslist.c syslist.h
	gcc -g -Wall -ansi -c syslist.c

sysread.o: sysread.c sysread.h
	gcc -g -Wall -ansi -c sysread.c

syswrite.o: syswrite.c syswrite.h
	gcc -g -Wall -ansi -c syswrite.c

cadenas.o: cadenas.c cadenas.h
	gcc -g -Wall -ansi -c cadenas.c

archivos.o: archivos.c archivos.h
	gcc -g -Wall -ansi -c archivos.c

consola.o: consola.c consola.h
	gcc -g -Wall -ansi -c consola.c

protocolo.o: protocolo.c protocolo.h
	gcc -g -Wall -ansi -c protocolo.c

oft.o: oft.c oft.h
	gcc -g -Wall -ansi -c oft.c

fd.o: fd.c fd.h
	gcc -g -Wall -ansi -c fd.c

syscalls.o: syscalls.c syscalls.h
	gcc -g -Wall -ansi -c syscalls.c

funcionesldap.o: funcionesldap.c funcionesldap.h
	gcc -g -Wall -ansi -c funcionesldap.c

vsk.o: vsk.c 
	gcc -g -Wall -ansi -c vsk.c

.PHONY : clean

clean:
	rm *.o


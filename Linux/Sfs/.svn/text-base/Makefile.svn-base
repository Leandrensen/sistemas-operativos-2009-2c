LDAPLIB=-lldap  -llber
INCDIR=/user/local/include
LIBDIR=/user/local/lib
LIBS= -L $(INCDIR) -L $(LIBDIR) $(LDAPLIB)

sfs: 	cadenas.o sendrecv.o archivos.o delFile.o modFile.o addFile.o srchFile.o initBind.o define.o main.o
	gcc -ansi -lnsl -g cadenas.o sendrecv.o archivos.o delFile.o modFile.o addFile.o srchFile.o initBind.o main.o $(LIBS) -o sfs
	make clean

cadenas.o: cadenas.c cadenas.h
	gcc -g -Wall -ansi -c cadenas.c

sendrecv.o: sendrecv.c sendrecv.h
	gcc -g -Wall -ansi -c sendrecv.c

archivos.o: archivos.c archivos.h
	gcc -g -Wall -ansi -c archivos.c

modFile.o: modFile.c modFile.h
	gcc -g -ansi -c modFile.c

delFile.o: delFile.c delFile.h
	gcc -g -ansi -c delFile.c
	
addFile.o: addFile.c addFile.h
	gcc -g -ansi -c addFile.c

srchFile.o: srchFile.c srchFile.h
	gcc -g -ansi -c srchFile.c
	
initBind.o: initBind.c initBind.h
	gcc -g -ansi -c initBind.c

define.o: define.h
	gcc -g -ansi -c define.h
	
main.o: main.c 
	gcc -g -Wall -ansi -c main.c

.PHONY : clean

clean:
	rm *.o
	rm *.gch
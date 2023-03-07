LDAPLIB=-lldap  -llber
INCDIR=/usr/local/include
LIBDIR=/usr/local/lib
LIBS= -L $(INCDIR) -L $(LIBDIR) $(LDAPLIB)

sacarSectorBinario: ejemploLdapBinarios.o main.o funcionesLdap.o initBind.o cadenas.o archivos.o
	gcc -Wall -ansi -lnsl -g ejemploLdapBinarios.o main.o funcionesLdap.o initBind.o cadenas.o archivos.o $(LIBS) -o sacarSectorBinario 
	make clean

ejemploLdapBinarios.o: ejemploLdapBinarios.c ejemploLdapBinarios.h
	gcc -g -Wall -ansi -c ejemploLdapBinarios.c
	
main.o: main.c
	gcc -g -Wall -ansi -c main.c
 
funcionesLdap.o: funcionesLdap.c funcionesLdap.h
	gcc -g -Wall -ansi -c funcionesLdap.c

init.o: initBind.c initBind.h
	gcc -g -Wall -ansi -c initBind.c

cadenas.o: cadenas.c cadenas.h
	gcc -g -Wall -ansi -c cadenas.c	
	
archivos.o: archivos.c archivos.h
	gcc -g -Wall -ansi -c archivos.c	
	
.PHONY : clean

clean:
	rm *.o

	

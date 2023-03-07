#ifndef _MODEFILE_H
#define _MODEFILE_H
#include <ldap.h>

typedef struct
{
	char nombreDA[100];
	char nombreArchivo[50];
	char tamanio[50];
	char* listaSectores;
}LDAPMODIFICARARCHIVO;

void cargarModifircarArchivo(LDAPMODIFICARARCHIVO* info, char* nombreDA, char* nombreArchivo, char* listaSectores);
int ldapModificarArchivo( LDAP*ld, LDAPMODIFICARARCHIVO info);
void cargarModifircarArchivoTamanio(LDAPMODIFICARARCHIVO* info, char* nombreDA, char* nombreArchivo, char* tamanio);
int ldapModificarArchivoTamanio( LDAP*ld, LDAPMODIFICARARCHIVO info);

#endif

#ifndef _ADDFILE_H
#define _ADDFILE_H
#include <ldap.h>

typedef struct ldapagregararchivo
{
	char nombreDA[100];
	char nombreArchivo[100];
	char autor[100];
	char permisos[20];
	char tamanio[50];
	char listaSectores[100];
}LDAP_AGREGAR_ARCHIVO;

void cargarAgregarArchivo(LDAP_AGREGAR_ARCHIVO* info, char* nombreDA, char* nombreArchivo, char* autor, char* permisos, char* tamanio, char* listaSectores);
int ldapAgregarArchivo(LDAP* ld, LDAP_AGREGAR_ARCHIVO info);
int ldapFormatearDA(LDAP* ld, char* nombreDA, int iCantSectores);
int ldapQuitarDosSectoresDA(LDAP* ld, char* nombreDA, int iSector1, int iSector2 , int* listaSectores, int iCantSectoresLibres);


#endif

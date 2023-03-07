#ifndef _DELFILE_H
#define _DELFILE_H
#include <ldap.h>
#include "srchFile.h"

typedef struct ldapeliminararchivo
{
	char nombreDA[100];
	char nombreArchivo[50];
}LDAPELIMINARARCHIVO;

void cargarLDAPELIMINARARCHIVO(LDAPELIMINARARCHIVO* info, char* nombreDA, char* nombreArchivo);
int ldapEliminarArchivo(LDAP *ld, LDAPELIMINARARCHIVO info);
int ldapEliminarDA(LDAP *ld, char* nombreDA);
int ldapBuscarDAsectoresLibresDEL( LDAP *ld, LDAPBUSCARARCHIVO info, pINT* listaSectores, int* iCantSectores);

#endif
void cargarSectoresLiberados(pINT* ipNuevosSectores, pINT ipListaSectoresLibres, ptrLdapResultados* ldapResultados, int* iCantSectores);

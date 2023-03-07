#ifndef _FUNCIONESLDAP_H
#define _FUNCIONESLDAP_H

#include <ldap.h>
#include "cadenas.h"

typedef struct ldapmodificarsector
{
	char nombreDA[100];
	char idSector[50];
	char contenido[513];
}LDAPMODIFICARSECTOR;

typedef struct ldapAgregarSector
{
	char nombreDA[100];
	char idSector[50];
	char contenido[513];
}LDAPAGREGARSECTOR;

typedef struct ldapSector
{
	unsigned char utnDAName[100];
	unsigned char utnSectorID[50];
	char utnSectorContent[512];	
}LDAPSECTOR;


void cargarLDAPMODIFICARSECTOR(LDAPMODIFICARSECTOR *info, char* nombreDA, char* idSector, char* contenido);
int ldapModificarSector(LDAP *ld, LDAPMODIFICARSECTOR info);

void cargarLDAPAGREGARSECTOR(LDAPAGREGARSECTOR *info, char* nombreDA, char* idSector, char* contenido);
int ldapAgregarSector(LDAP* ld, LDAPAGREGARSECTOR info);

int ldapGetSector(LDAP *ld, LDAPSECTOR info, char* pBuffer);
void cargarSector(LDAPSECTOR *info, char* utnDAName, char* utnSectorID);


#endif

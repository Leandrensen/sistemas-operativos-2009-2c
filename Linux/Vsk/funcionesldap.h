#ifndef _FUNCIONESLDAP_H
#define _FUNCIONESLDAP_H

#include <ldap.h>

#define LDAP_INIT_FAIL	-500
#define LDAP_BIND_FAIL	-501
#define LDAP_INICIAR_OK	-502

typedef struct ldapSector
{
	unsigned char utnDAName[100];
	unsigned char utnSectorID[50];
	/* la fecha es interna */
	char utnSectorContent[512];	
	char cacheBucket[50]; /* numero de entrada de la cache */ 
}LDAPSECTOR;

typedef struct ldapInit
{
	char* ldapHost;
	int ldapPort;
	char* ldapDN;
	char* ldapPass;
	LDAP *ldapSesion;
	int iCantidadMaxima;
	int iCantidadActual;
}LDAPINIT, *pLDAPINIT;

int ldapCargarCache(LDAP *ld, LDAPSECTOR info);
void cargarSector(LDAPSECTOR *info, unsigned char* utnDAName, char* utnSectorID, char* utnSectorContent, int cacheBucket);
int ldapIniciarSesion(pLDAPINIT* info);
void ldapFinSesion(pLDAPINIT* info);
void ldapCargarSesion(pLDAPINIT* info, char* ldapHost, int ldapPort, char* ldapPass, int iTamanioCache);
int ldapModificarCache(LDAP *ld, char* modifyDN);
int ldapBuscarViejoCache(LDAP *ld, LDAPSECTOR info);
int ldapBuscarMasViejoCache(LDAP *ld, char * dnMasViejoBuscado); /* con minimo por bandera */
int ldapBuscarCache(LDAP *ld, LDAPSECTOR info, char* pBuffer);
int ldapBorrarEntrada(LDAP *ld, LDAPSECTOR info);
int ldapBuscarCacheParaInvalidar(LDAP *ld, LDAPSECTOR info);
int ldapInvalidarEntradaCache(LDAP *ld, char* modifyDN);
int ldapModificarContenidoCache (LDAP *ld, char* modifyDN, char* cpBUFutnSectorContent);


#endif


#ifndef _SRCHFILE_H
#define _SRCHFILE_H
#include <ldap.h>
#include "sendrecv.h"
#include "initBind.h"
typedef struct ldapResultadoInformacion
{
	int iSector;
	void* sgte; /* para que busque todos los archivos del DA
				escribir "*" (asterisco) paraque lo entiena LDAP */
}LDAPRESULTADOINFORMACION, *pResultadoInformacion, *pResultadoInformacionAuxiliar;

typedef struct ldapBuscarArchivo
{
	char nombreDA[100];
	char nombreArchivo[50]; /* para que busque todos los archivos del DA
				escribir "*" (asterisco) paraque lo entiena LDAP */
}LDAPBUSCARARCHIVO;

typedef struct ldapResultados
{
	char nombreDA[100];
	char nombreArchivo[50];
	char autor[100];
	char permisos[20];
	char* listaSectores;
	char tamanio[50];
}LDAPRESULTADOS;

typedef struct
{
	int* lista;
}pINT;

typedef struct nodoldapResultados
{
	LDAPRESULTADOS info;
	void* sgte;
}NODOLDAPRESULTADOS, *ptrLdapResultados;

int enviarDatosArchivo(int iSocket ,pResultadoInformacion* ldapResultadoInformacion, int iContadorDatosLdap);
void cargarInfoArchivo (ptrLdapResultados* Resultados, pResultadoInformacion* ldapResultadoInformacion, int* iContadorDatosLdap);
int recibirDatos(int i, HEADERIRCIPC cabecera, unsigned char* bfr ,char* nombreDA, char* nombreArchivo);
void cargarLDAPBUSCARARCHIVO(LDAPBUSCARARCHIVO* info, char* nombreDA, char* nombreArchivo);
int ldapBuscarArchivo( LDAP *ld, LDAPBUSCARARCHIVO info, ptrLdapResultados* Resultados);
int ldapBuscarDAs( LDAP *ld, LDAPBUSCARARCHIVO info, ptrLdapResultados* Resultados, int* longitudCadena);
void listarArchivos(int iSocket, HEADERIRCIPC cabecera, LDAPINIT ldap);
int estaElDA(char* nombreDA, ptrLdapResultados* Resultados);
int ldapBuscarDAsectoresLibres( LDAP *ld, LDAPBUSCARARCHIVO info, unsigned char* buf, pINT* listaSectores, int* iCantSectores);
int exsteArchivo( LDAP *ld, LDAPBUSCARARCHIVO info);
int ldapListarArchivos( LDAP *ld, LDAPBUSCARARCHIVO info, ptrLdapResultados* Resultados, int* longitudCadena);
#endif

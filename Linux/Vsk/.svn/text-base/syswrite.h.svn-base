#ifndef _SYSWRITE_H
#define _SYSWRITE_H
#include "sendrecv.h"
#include "oft.h"
#include "fd.h"
#include "funcionesldap.h"

#define PUTSECTORES 0x08

void sys_write (int iSocketSFS, int iSocketCliente, pNodoFD* pFD, pOft* pOFT, HEADERIRCIPC cabecera,pConexiones* L_Conexiones, int iHayCache, pLDAPINIT* ldap);
void sys_flush (int iSocketSFS, pNodoFD* pFD, pOft* pOFT, HEADERIRCIPC cabecera, pConexiones* L_Conexiones, int iHayCache, pLDAPINIT* ldap);
void pedirDosSectoresLibres (int iSocketSFS, unsigned char* ucpDA, HEADERIRCIPC cabecera);

#endif


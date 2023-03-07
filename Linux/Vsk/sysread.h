#ifndef _SYSREAD_H
#define _SYSREAD_H

#include "fd.h"
#include "sendrecv.h"
#include "protocolo.h"
#include "funcionesldap.h"
#include  <ldap.h>

#define GETSECTORES2		0x11
#define GETSECTORES1		0x12

void sys_read (int iSocketSFS, int iSocketCliente, pNodoFD* pFD, HEADERIRCIPC cabecera, pConexiones* L_Conexiones, int iHayCache, pLDAPINIT *ldap_sesion);
void copiarN (unsigned char* bufferDestino, unsigned char* bufferOrigen, int iDesdeEnDestino, int iCantidad);

#endif

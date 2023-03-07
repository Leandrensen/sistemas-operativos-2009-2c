#ifndef PRIMITIVAS_H_
#define PRIMITIVAS_H_
#include <ldap.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "sendrecv.h"
#include "archivos.h"
#include "cache.h"
#include "calculosYEstadisticas.h"
#include "defines.h"
#include "archivos.h"
#include "funcionesLdap.h"

int getSectores(int iSocket, HEADERIRCIPC cabecera, CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, int iHayCache, int iSocketCache, LDAP* ld);
int putSectores(int iSocket, HEADERIRCIPC cabecera, CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, LDAP *ld);
int getCHS(int iSocket, HEADERIRCIPC cabecera, CONFIGURACION stArchConf);
int getSector(int iIdSector, unsigned char *ucBufSector, int iHayCache, int iSocketCache, CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas,LDAP * ld);int putSector(int iIdSector, unsigned char *ucBufSector, CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, LDAP *ld);

#endif /*PRIMITIVAS_H_*/

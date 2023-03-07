#ifndef _ARCHIVOS_H
#define _ARCHIVOS_H
#include <stdio.h>

/*************            ESTRUCTURAS                     ******************/

typedef struct config
{
    char cvIP[15+2];
    int iPuerto;
	char cvIPVsk[15+2];
	int iPuertoVsk;
    char cvLdapIp[15+2];
	int iLdapPuerto;
    char cvLdapUsr[30];
	char cvLdapPass[30];	
}CONFIGURACION;

/************             CABECERAS                       ******************/

int cargarConfiguracion(CONFIGURACION *, FILE *);

#endif

#ifndef _ARCHIVOS_H
#define _ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include "cadenas.h"

#define NOMBRE_PROCESO "DISCO"

/*************            ESTRUCTURAS                     ******************/

typedef struct CHS
{
	int iCilinders;
	int iHeaders;
	int iSectors;
}typeCHS;

typedef struct config
{
    char cvNombreDisco[31];
    char cvIP[16];
    int iPuerto;
    char cvIPArrayVSK[16];
    int iPuertoArrayVSK;
    typeCHS stCHS; /* Cilinders,Headers,SectorsPerCilinder */
    int iRPM; /* Revoluciones por minuto */
    char cvCache[3]; /* si: tiene cache / no: no tiene cache */
    char cvLdapIp[16];
    int iLdapPuerto;
    char cvLdapPass[30];
    int iTiempoAccesoACache;
}CONFIGURACION;

/************             CABECERAS                       ******************/

int cargarConfiguracion(CONFIGURACION*);
int crearLog();
int escribirLog (char cTipoLog, char* cpMensaje);

#endif	

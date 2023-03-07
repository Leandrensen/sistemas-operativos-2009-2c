#ifndef ARCHIVOS_H_
#define ARCHIVOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include "cadenas.h"

#define NOMBRE_PROCESO "CACHE DEL DISCO"

/* Estructura del archivo de configuracion de la cache */

typedef struct config
{
	char cvNombreDisco[31];
    char cvIP1[16];
    int iPuerto1;
    char cvIP2[16];
    int iPuerto2;
   	char cvIPDisco[16];
    int iPuertoDisco;
}CONFIGURACION;

int cargarConfiguracion(CONFIGURACION* arch);
int crearLog();
int escribirLog(char cTipoLog, char* cpMensaje);

#endif /*ARCHIVOS_H_*/

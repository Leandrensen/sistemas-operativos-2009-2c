#ifndef _SINCRONIZACION_H
#define _SINCRONIZACION_H
#include <stdio.h>
#include <stdlib.h>
#include "disco.h"

/*************            CONSTANTES                      ******************/

#define SINCRONIZACION_NO_INICIAR			-400
#define SINCRONIZACION_INICIO_FAIL			-401
#define SINCRONIZACION_INICIO_OK			-402
#define SINCRONIZACION_SINCRONIZAR			-403
#define SINCRONIZACION_NO_ES_SINCRONIZACION	-403
#define SINCRONIZACION_SINCRONIZANDO_FAIL		-404
#define SINCRONIZACION_SINCRONIZANDO_OK		-405
#define SINCRONIZACION_FIN				-406
#define SINCRONIZACION_CONTINUAR			-407
#define SINCRONIZACION_NO_HAY_OTRO_SINCRONIZANDO -408
#define SINCRONIZACION_HAY_OTRO_SINCRONIZANDO	-409


/*************            ESTRUCTURAS                     ******************/

/************             CABECERAS                       ******************/
int iniciarSoncronizacion(SDISCOSCONECTADOS* EstructuraArray, int iPosSincDest, int iCantidadDiscos);
int sincronizarDisco(SDISCOSCONECTADOS* sDiscos, SCHS chs, SDISCOSSINCRONIZANDOS* sDiscosSincronizando);
int enSincronizacion(HEADERIRCIPC cabecera,  SDISCOSSINCRONIZANDOS* sDiscosSincronizando);
int calcularCHS(SCHS chs, int idSectorActual, int* idSector);
int hayOtroSincronizando(SDISCOSSINCRONIZANDOS* Estructura,int iCantidadDiscos);
int continuarSincronizacion(int i,SDISCOSCONECTADOS* sDiscos, SCHS chs, SDISCOSSINCRONIZANDOS* sDiscosSincronizando, int iCantidadDiscos, HEADERIRCIPC cabecera);
void enviarOtroASincronizar(SDISCOSCONECTADOS* sDiscos, SCHS chs, SDISCOSSINCRONIZANDOS* sDiscosSincronizando, int iCantidadDiscos);
void cargarDiscoASincronizar(SDISCOSSINCRONIZANDOS* Estructura,int iCantidadDiscos, SDISCOSSINCRONIZANDOS datos);

#endif

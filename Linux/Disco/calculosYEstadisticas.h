#ifndef CALCULOSYESTADISTICAS_H_
#define CALCULOSYESTADISTICAS_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "archivos.h"
#include "cadenas.h"

typedef struct typeESTADISTICAS
{
	unsigned int cantSectoresBuscados;
	unsigned int tiempoUtilizadoEnBusquedas;
	unsigned int totalBytesTransferidos;
	unsigned int tiempoUtilizadoEnTransferencias;
	unsigned int cantAccesosACache;
	unsigned int tiempoUtilizadoAccesoACache;
	unsigned int uiPosCabezal;
	unsigned int peorTiempoSector;
	unsigned int tiempoLectoEscrituraSector;
	unsigned int uiCacheHit;
	unsigned int uiCacheMiss;
	unsigned int uiTiempoAccesoACache;	
} ESTADISTICAS;
	
int generarEstadisticas(CONFIGURACION stArchConf, ESTADISTICAS stEstadisticas);
int generarEstadisticasGetSectores(CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, int iIdSector, char cEstabaEnCache);
int generarEstadisticasPutSectores(CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, int iIdSector);
void TransformarUnidad(long valorEnBytes, char* numeroReducido, char* unidad);

#endif /*CALCULOSYESTADISTICAS_H_*/

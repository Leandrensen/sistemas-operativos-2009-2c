#ifndef _ARCHIVOS_H
#define _ARCHIVOS_H
#include <stdio.h>
#include <stdlib.h>

/*************            CONSTANTES                      ******************/
#define CONFIG_OK		1
#define CONFIG_FAIL		0



/*************            ESTRUCTURAS                     ******************/
typedef struct sConfig
{
	char cvNombreArray[50];
	char cvArrayIP[25];
	int iArrayPuerto;
	char cvVSKIP[25];
	int iVSKPuerto;
	int iCantidadDiscos;
	int iCilindros;
	int iCabezas;
	int iSectores;
}CONFIGURACION;

typedef struct sDiscosEsperados
{
	char* cvNombreDisco;
	char cvArrayIP[25];
	int iPuertoArray;
	void* sgte;
}SDISCOSESPERADOS, *ptrDiscosEsperados;



/************             CABECERAS                       ******************/

int cargarConfiguracion (CONFIGURACION* arch, ptrDiscosEsperados* ListaDiscos);
int creaLog ();
int escribirLog (char cTipoLog, char* cpMensaje);
void mostrarConfiguracion (CONFIGURACION configuraciones, ptrDiscosEsperados* listaDiscos);

#endif

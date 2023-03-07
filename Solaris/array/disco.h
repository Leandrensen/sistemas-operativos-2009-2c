#ifndef _DISCO_H
#define _DISCO_H
#include <stdio.h>
#include <stdlib.h>
#include "sendrecv.h"
#include "archivos.h"
#include "syscall.h"

/* CONSTANTES ESTADO DE ESTRUCTURA ARRAY */
#define ARRAY_LUGAR_RESERVADO			100
#define ARRAY_LUGAR_DISPONIBLE			101
#define ARRAY_NO_HAY_ESPACIO			102
#define ARRAY_DISCO_ACEPTADO			103
#define ARRAY_DISCO_RECHAZADO			104
#define ARRAY_HILO_OK				105
#define ARRAY_HILO_FAIL				106

/* Constantes de ESTADO de DISCO en el ARRAY, responde a SDISCOSCONECTADOS, campo int iEstadoDisco*/
#define DISCO_ESTADO_NO_CONECTADO			119
#define DISCO_ESTADO_CONECTADO			120 /* El disco espera ser sincronizado */
#define DISCO_ESTADO_SINCRONIZANDO			121 /* Para GET y PUT hasta que termine*/
#define DISCO_ESTADO_SINCRONIZADO			122 /* Puede realizar operaciones de GET y PUT */

/* -------------- Estructura de trabajo de DISCOS que se conecten -----*/
typedef struct sDiscosConectados
{
	int iSocketHilo;
	int iSocketArray;
	int iSocketDisco;
	char* cpNombreDisco;
	char cvArrayIP[25];
	int iPuertoArray;
	int iEstadoDisco;
}SDISCOSCONECTADOS;

typedef struct sDiscos
{
	SDISCOSCONECTADOS disco;
	void* sgte;
}SDISCOS, *ptrDiscos;

/* -------------- Estructura de trabajo de CHS -----------------------*/
typedef struct sChs
{
	int iCilindros;
	int iCabezas;
	int iSectores;
}SCHS;


typedef struct discosSincronizando
{
	unsigned char cpDescriptorID[16];
	int iIdSectorActual;
	int iPosVecOrigen;
	int iPosVecDest;
}SDISCOSSINCRONIZANDOS;


/* #################### CABECERAS ##########################################*/

void respaldarLista(SDISCOSCONECTADOS* EstructuraArray, ptrDiscosEsperados* listaDiscos, int iServidor, int iCantidadDiscos, char* IP, int iPuerto);
int recibirDatosDisco(int iSocketDisco, SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos, SCHS datosCHS);
int esUnDiscoEsperado(SDISCOSCONECTADOS* EstructuraArray, int iSocket);
int asignarDisco(int iSocketDisco, SDISCOSCONECTADOS* EstructuraArray, unsigned char* nombreDisco ,int iCantidadDiscos);
int asociarSocketHilo(int iSocketHilo, SDISCOSCONECTADOS* EstructuraArray, HEADERIRCIPC cabecera, int iCantidadDiscos);
void desconectarDisco(int iSocketDisco, SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos);
void desconectarDiscoPorNombre(char* cpNombreDisco, SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos);
int subestadoFueraDeServicio(SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos);
int subestadoEnServicio(SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos);
int elegirDisco(SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos, int* iSelector1, int* iSelector2);
void repartirPutSectores(SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos,HEADERIRCIPC cabecera ,SSECTORES sSectores, SDISCOSSINCRONIZANDOS* sDiscosSincronizando, SCHS datosCHS);
void escribirEnSincronizacion(SDISCOSCONECTADOS sDisco, HEADERIRCIPC cabecera ,SSECTORES sSectores , int iIdSectorActual);
int enviarPedido_CHS(int iSocketDestino, HEADERIRCIPC cabecera, SCHS datosCHS);
int atenderDisco(SDISCOSCONECTADOS* sDisco);

#endif

#ifndef _CONSOLA_H
#define _CONSOLA_H

#include <stdlib.h>
#include <stdio.h>
#include "disco.h"
#include "archivos.h"

#define CONSOLA_ELIMINAR_NO_SINCRONIZADOS		-200
#define CONSOLA_ELIMINAR_DISCO_NO_ENCONTRADO	-201
#define CONSOLA_ELIMINAR_DISCO			-202
#define CONSOLA_SERVICIO				-203
#define CONSOLA_AGREGAR_FUERASERVICIO		-204
#define CONSOLA_AGREGAR_DISCOINEXISTENTE		-205
#define CONSOLA_AGREGAR_DISCO			-206
#define CONSOLA_DISCO_SINCRONIZADO			-207
#define CONSOLA_ESTADO				-208

#define SYSTEM_CLS					-217
#define SYSTEM_AGREGAR				-218
#define SYSTEM_ELIMINAR				-219
#define SYSTEM_INICIAR_SERVICIO			-220
#define SYSTEM_DETENER_SERVICIO			-221
#define SYSTEM_AYUDA					-222
#define SYSTEM_ERROR					-223
#define SYSTEM_SALIR					-224
#define SYSTEM_ESTADO				-225





int ejecutarComando(int iSocketConsola, SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos, int* iEstadoActual, int* iSubEstadoActual, int* iPosVector);
int eliminarDisco(SDISCOSCONECTADOS* EstructuraArray, char* cpNombreDisco, int iCantidadDiscos);
int agregarDisco(SDISCOSCONECTADOS* EstructuraArray, char* cpNombreDisco, int iCantidadDiscos);
void consola(CONFIGURACION* config);
int determinarComando(char * cvStringConsola);
void mostrarAyuda();
void mostrarEstadoSistema(SDISCOSCONECTADOS* EstructuraArray, int iEstado, int *iSubEstado, CONFIGURACION config);
#endif

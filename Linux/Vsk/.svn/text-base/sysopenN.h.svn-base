#ifndef _SYSOPEN_H
#define _SYSOPEN_H

#define ESCRITURA_APERTURA				0xA0
#define ESCRITURA_ARCHIVO_EXISTE		0xA1
#define ESCRITURA_ARCHIVO_INEXISTENTE	0xA2
#define ESCRITURA_ARCHIVO_ELIMINAR		0xA3
#define ESCRITURA_ARCHIVO_ELIMINADO		0xA4
#define ESCRITURA_ARCHIVO_CREAR			0xA5
#define ESCRITURA_ARCHIVO_CREADO		0xA6

#define LECTURA_APERTURA				0xC0
#define LECTURA_ARCHIVO_EXISTE			0xC1
#define LECTURA_ARCHIVO_INEXISTENTE		0xC2
#define LECTURA_ARCHIVO_INFORMACION		0xC3
#define LECTURA_ARCHIVO_INFOVUELTA		0xC4

#define ELIMINAR_APERTURA				0xE0
#define ELIMINAR_ARCHIVO_EXISTE			0xE1
#define ELIMINAR_ARCHIVO_INEXISTENTE	0xE2

#include <stdio.h>
#include <stdlib.h>
#include "oft.h"
#include "fd.h"
#include "sendrecv.h"

void separarParametrosOpen (unsigned char* ucpMensaje, unsigned char* ucpArchivo, unsigned char * ipFlag);
int sys_open (int iSocketSFS, int iSocketCliente, pOft* L_Oft, pNodoFD* pFD, HEADERIRCIPC cabecera);


#endif

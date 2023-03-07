#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#define SYS_OPEN		0x30
#define SYS_CLOSE		0x31
#define SYS_LIST		0x32
#define SYS_READ		0x33
#define SYS_WRITE		0x34
#define SYS_LIST_OK					0x35
#define OPEN_FILE_OK				0x36
#define OPEN_FILE_FAILED			0x37
#define CLOSE_FILE_OK				0x38
#define READ_FILE_OK				0x39
#define WRITE_FILE_OK				0x40
#define SYSCALL_OPEN		100
#define SYSCALL_CLOSE	400
#define SYSCALL_READ		200
#define SYSCALL_WRITE	300
#define SYSCALL_LIST		600

#define EXISTEARCHIVO_IDA			0x10
#define EXISTEARCHIVO_VUELTA		0x11
#define INFOARCHIVO_IDA			0x12
#define INFOARCHIVO_VUELTA			0x13
#define ELIMINARARCHIVO_IDA			0x14
#define ELIMINARARCHIVO_VUELTA		0x15
#define CREARARCHIVO_IDA			0x16
#define CREARARCHIVO_VUELTA			0x17
#define ACTUALIZARTAMANIO_IDA		0x18
#define ACTUALIZARTAMANIO_VUELTA		0x19
#define CREARTABLASECTORESLIBRES_IDA	0x20
#define CREARTABLASECTORESLIBRES_VUELTA	0x21
#define DOSSECTORESLIBRES_IDA		0x22
#define DOSSECTORESLIBRES_VUELTA		0x23
#define LIBERARSECTORES_IDA			0x24
#define LIBERARSECTORES_VUELTA		0x25
#define LISTARDIRECTORIO_IDA		0x26
#define LISTARDIRECTORIO_VUELTA		0x27
#define FORMATEAR_IDA			0x28
#define FORMATEAR_VUELTA			0x29


#include <stdlib.h>
#include "sendrecv.h"
#include "protocolo.h"
#include "fd.h"
#include "oft.h"

int enviar_getSectores(int iSocketDA, HEADERIRCIPC cabecera, unsigned char* ucpSectores);
int recibir_getSectores(int iSocket,unsigned char* buffer);
int enviar_putSectores(int iSocket, HEADERIRCIPC cabecera, unsigned char* buffer, int iSect1, int iSect2);
int recibir_putSectores(int iSocket, HEADERIRCIPC cabecera);
int enviar_getCHS(int iSocket, HEADERIRCIPC cabecera);
int recibir_getCHS(int iSocket, unsigned char* cpInfo, int iCantidad);
void limbiarBuffer (char * buffer);
void enviarASFS (int iSocketSFS, unsigned char *ucvDA, unsigned char *ucvNombreArchivo, int iProtocolo, HEADERIRCIPC cabecera);
#endif


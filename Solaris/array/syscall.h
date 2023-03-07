#ifndef _SYSCALL_H
#define _SYSCALL_H
#include <stdio.h>
#include <stdlib.h>
#include "disco.h"
#include "sendrecv.h"

/* CONSTANTES TAMAÑOS STRUCT GET_SECTORES y PUT_SECTORES */
#define SIZE_STRUCT_IDSECTOR		4
#define SIZE_STRUCT_BUFFERSECTOR		512
#define SIZE_STRUCT_CHS			12

#define SYSCALL_GETSECTORES_RECV_PEDIDO_OK		-300
#define SYSCALL_GETSECTORES_RECV_PEDIDO_FAIL		-301
#define SYSCALL_GETSECTORES_SEND_PEDIDO_OK		-302
#define SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL		-303
#define SYSCALL_GETSECTORES_RECV_RTA_OK			-304
#define SYSCALL_GETSECTORES_RECV_RTA_FAIL			-305
#define SYSCALL_GETSECTORES_SEND_RTA_OK			-306
#define SYSCALL_GETSECTORES_SEND_RTA_FAIL			-307
#define SYSCALL_PUTSECTORES_RECV_PEDIDO_OK		-308
#define SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL		-309
#define SYSCALL_PUTSECTORES_SEND_PEDIDO_OK		-310
#define SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL		-311
#define SYSCALL_CHS_SEND_FAIL				-312
#define SYSCALL_CHS_SEND_OK					-313


/*************            ESTRUCTURAS                     ******************/
/* -------------- Estructura de trabajo de GET_SECTORES --------------*/
typedef struct sSect
{
	int iIdSector1;
	unsigned char cvBufferSector1[512];
	int iIdSector2;
	unsigned char cvBufferSector2[512];
}SSECTORES;

typedef struct sPedido
{
	unsigned char cpDescriptorID[16];
	SSECTORES sectores;
	int iSocketS1;
	int iSocketS2;
	int iCant;
}SPEDIDO;

/************             CABECERAS                       ******************/

int recibirPedido_getSectores(int iSocketOrigen, HEADERIRCIPC cabecera, SSECTORES* sSectores);
int enviarPedido_getSectoresDosDiscos(int iSocketDestino, HEADERIRCIPC cabecera, SSECTORES sSectores);
int enviarPedido_getSectoresUnDisco(int iSocketDestino, HEADERIRCIPC cabecera, int iSector);
int recibirRespuesta_getSectores(int iSocketOrigen, HEADERIRCIPC cabecera, SSECTORES* sSectores);
int enviarRespuesta_getSectores(int iSocketDestino, HEADERIRCIPC cabecera, SSECTORES sSectores);
int recibirPedido_putSectores(int iSocketOrigen, HEADERIRCIPC cabecera, SSECTORES* sSectores);
int enviarPedido_putSectores(int iSocketDestino, HEADERIRCIPC cabecera, SSECTORES sSectores);
void enviar_getCHS(int iSocket,HEADERIRCIPC cabecera, CONFIGURACION config);
int recibirRespuesta_getSectoresB(int iSocket, HEADERIRCIPC cabecera, unsigned char* cpSector);
int enviarRespuesta_getSectoresB(int iSocket, HEADERIRCIPC cabecera, unsigned char* cpSector);


void limpiarPedidos(SPEDIDO* vPeidos, int MAX);
void agregarPedido(SPEDIDO* vPeidos, int MAX, HEADERIRCIPC cabecera, int iSector1, int iSector2, int iSocketS1, int iSocketS2);
int agregarRespuesta(int iSocket, HEADERIRCIPC cabecera, SPEDIDO* vPeidos, int MAX);
void eliminarPedido(SPEDIDO* vPeidos, int iPos);

#endif

#ifndef _FD_H
#define _FD_H


#include <stdio.h>

typedef struct infoFD
{
	unsigned char cvDescriptorID[16]; /* del pedido temporal */
	unsigned char ucvNombreArchivo[40];
	unsigned char ucvDA[100];
	int iSocket; /* de la conexion con el cliente del ftp*/
	void* pDescriptor; /* descriptor de la OFT */
	int iSysCall; /* codigo de syscall en la que esta trabajando */
	int iPaso; /* paso de la proxima ejecucion */
	int iPosALeer; /* posicion del vector de sectores */
	int iTotal; /* cantidad de sectores que componen el archivo */
	int iPedidos; /* indica cuantos sectores se pidieron */
	int iSector1; /* sector pedido al DA -1 es no se pidio sector*/
	int iSector2; /* sector pedido al DA -1 es no se pidio sector*/
}INFOFD;

typedef struct nodoFD
{
	INFOFD fd;
	void* sgte;
}NODOFD, *pNodoFD;

int meterFd (pNodoFD* pila, INFOFD fd);
int quitarFdPorSocket(pNodoFD* pila, int iSocket);
NODOFD* buscarFdPorSocket (pNodoFD* pila, int iSocket);
NODOFD* buscarFdPorDescriptorID (pNodoFD* pila, unsigned char* ucpDescriptorID);
void cargarInfoFD (INFOFD* fd, unsigned char* ucpDescriptorID, int iSocket, int iDescriptor, int iSysCall, int iPosALeer);

#endif

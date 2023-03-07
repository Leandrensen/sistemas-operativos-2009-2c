#include "fd.h"
#include "cadenas.h"
#include <stdlib.h>


/* 	Nombre de la Función: cargarInfoFD
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: meter en una lista un pedido de archivo
	Entrada:	ucpDescriptorID: descriptor id del pedido
			iSocket: socket del cliente
			iDescriptor: numero identificador de entrada de OFT
			iSysCall: identificador de llamada al sistema
			iPosALeer: 0
	Salida:	fd: registro con informacion del fd
*/

void cargarInfoFD (INFOFD* fd, unsigned char* ucpDescriptorID, int iSocket, int iDescriptor, int iSysCall, int iPosALeer)
{
	int i;
	for (i=0; i<16; i++)
		fd->cvDescriptorID[i] = ucpDescriptorID[i];

	fd->iSocket = iSocket;
	fd->iPaso = 0;
	fd->iSysCall = iSysCall;
	fd->pDescriptor = NULL;
	fd->iPosALeer = iPosALeer;
}

/* 	Nombre de la Función: meterFd
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: meter en una lista un pedido de archivo
	Entrada:	pila: Lista de archivos
			fd: informacion que ocontendra el nodo
	Salida:	return: 0 si no pudo pedir memoria
				 1 si metio en la pila
*/

int meterFd (pNodoFD* pila, INFOFD fd)
{
	pNodoFD pAux;

	pAux = (pNodoFD) malloc (sizeof(NODOFD));
	if (pAux == NULL) return 0;
	
	copiarU(pAux->fd.cvDescriptorID, fd.cvDescriptorID);
	pAux->fd.iSocket = fd.iSocket;
	pAux->fd.iSysCall = fd.iSysCall;
	pAux->fd.pDescriptor= fd.pDescriptor;
	pAux->fd.iPosALeer = fd.iPosALeer;

	pAux->sgte = *pila;
	*pila = pAux;
	return 1;
}

/* 	Nombre de la Función: quitarFdPorSocket
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: eliminar nodo por Socket
	Entrada:	pila: Lista de archivos
			iSocket: nombre del archivo a eliminar
	Salida:	return: 0 si no lo encontro
				 1 si lo encontro
*/
int quitarFdPorSocket(pNodoFD* pila, int iSocket)
{
	pNodoFD pActual, pAnterior;
	int iBandera = 0;

	pActual = *pila;
	pAnterior = NULL;

	while ((pActual !=NULL) && (pActual->fd.iSocket == iSocket))
	{
		pAnterior = pActual;
		pActual = pActual->sgte;
		iBandera = 1;
	}


	if (pActual !=NULL)
	{
		/* ESTA EN EL PRINCIPIO */
		if (iBandera == 0)
		{
			*pila = pActual->sgte;
		}
		else pAnterior->sgte = pActual->sgte;
		printf("Borrando: %d\n", pActual->fd.iSocket);
		free(pActual);
		return 1; /*LO ENCONTRO*/
	}

	return 0; /*NO LO ENCONTRO*/

}

/* 	Nombre de la Función: buscarFdPorSocket 
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: eliminar nodo por Socket
	Entrada:	pila: Lista de archivos
			iSocket: nombre del archivo a eliminar
	Salida:	return: puntero al fd o NULL
*/
NODOFD* buscarFdPorSocket (pNodoFD* pila, int iSocket)
{
	pNodoFD pBuscador = *pila;

	for (;(pBuscador != NULL) && (pBuscador->fd.iSocket != iSocket); pBuscador = pBuscador->sgte);

	return pBuscador;
}

/* 	Nombre de la Función: buscarFdPorDescriptorID 
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: eliminar nodo por Socket
	Entrada:	pila: Lista de archivos
			ucpDescriptorID: desciproID a buscar
	Salida:	return: puntero al fd o NULL
*/
NODOFD* buscarFdPorDescriptorID (pNodoFD* pila, unsigned char* ucpDescriptorID)
{
	pNodoFD pBuscador = *pila;
	int i;

	for (;(pBuscador != NULL); pBuscador = pBuscador->sgte)
	{
		 for (i = 0; (i<16) && (pBuscador->fd.cvDescriptorID[i] == ucpDescriptorID[i]); i++);
		 if (i == 16) return pBuscador;
	}

	return NULL;
}

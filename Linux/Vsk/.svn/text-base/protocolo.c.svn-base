#include <stdio.h>
#include <stdlib.h>
#include "protocolo.h"
#include "cadenas.h"

/* 	Nombre de la Función: meterConexion
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: Meter en una Pila informacion de una conexion
	Entrada:	pila: Puntero a la pila
			info: registro con la informacion
	Salida:	return: 1 si metio en pila
				 0 si produjo error en malloc
*/
int meterConexion(pConexiones* pila, INFOCONEXIONES info)
{
	pConexiones p;
	p = (pConexiones) malloc (sizeof(CONEXIONES));

	if (p==NULL)
		return 0;

	p->conexion.iSocket = info.iSocket;
	p->conexion.iTipo = info.iTipo;
	p->conexion.cMontado = info.cMontado;
	copiarU(p->conexion.ucvNombre, info.ucvNombre);
	/* FALTA IDENTIFICADOR */
	p->sgte = *pila;
	*pila = p;

	return 1;
}

/* 	Nombre de la Función: quitarConexionPorSocket
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: elimina un nodo de la lista segun socket
	Entrada:	pila: Puntero a la pila
			unSocket: socket a buscar
	Salida:	info: registro con la informacion del nodo
			return: 1 si lo encontro
				 0 si no lo encontro
*/
int quitarConexionPorSocket(pConexiones* pila, int unSocket, INFOCONEXIONES* info)
{
	pConexiones pActual, pAnterior;
	int iBandera = 0;

	pActual = *pila;
	pAnterior = NULL;

	while (pActual !=NULL && pActual->conexion.iSocket != unSocket)
	{
		pAnterior = pActual;
		pActual = pActual->sgte;
		iBandera = 1;
	}


	if (pActual !=NULL)
	{
		info->iTipo = pActual->conexion.iTipo;
		info->iSocket = unSocket;

		/* ESTA EN EL PRINCIPIO */
		if (iBandera == 0)
		{
			*pila = pActual->sgte;
		}
		else pAnterior->sgte = pActual->sgte;
		free(pActual);
		return 1; /*LO ENCONTRO*/
	}

	return 0; /*NO LO ENCONTRO*/

}

/* 	Nombre de la Función: buscarConexionPorSocket 
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: busca un nodo de la lista segun socket
	Entrada:	pila: Puntero a la pila
			unSocket: socket a buscar
	Salida:	return: direccion del nodo
*/
pConexiones buscarConexionPorSocket (pConexiones* pila, int unSocket)
{
	pConexiones pBuscador = NULL;

	pBuscador = *pila;

	while (pBuscador != NULL && pBuscador->conexion.iSocket != unSocket)
		pBuscador = pBuscador->sgte;

	return pBuscador;
}

/* 	Nombre de la Función: buscarConexionPorTipo 
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 21/09/2009
	modificación:  21/09/2009
	Objetivo de la Función: busca un nodo de la lista segun tipo
	Entrada:	pila: Puntero a la pila
			unSocket: socket a buscar
	Salida:	return: direccion del nodo
*/
pConexiones buscarConexionPorTipo (pConexiones* pila, int iTipo)
{
	pConexiones pBuscador = NULL;

	pBuscador = *pila;

	while (pBuscador != NULL && pBuscador->conexion.iTipo != iTipo)
		pBuscador = pBuscador->sgte;

	return pBuscador;
}

pConexiones buscarConexionPorDA (pConexiones* pila, unsigned char* ucpNombreDA)
{
	pConexiones pBuscador = NULL;

	pBuscador = *pila;

	while (pBuscador != NULL && (compararU(pBuscador->conexion.ucvNombre, ucpNombreDA)!=0))
		pBuscador = pBuscador->sgte;

	return pBuscador;
}


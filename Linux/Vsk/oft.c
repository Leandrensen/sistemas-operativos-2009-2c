#include <stdlib.h>
#include <stdio.h>
#include "cadenas.h"
#include "oft.h"
#include "sendrecv.h"

/* 	Nombre de la Función: meterOft 
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: meter en una lista un nuevo archivo
	Entrada:	pila: Lista de archivos
			fcb: informacion que ocontendra el nodo
	Salida:	return: 0 si no pudo pedir memoria
				 1 si metio en la pila
*/
int meterOft (pOft* pila, INFOOFT fcb)
{
	pOft p;

	p = (pOft) malloc (sizeof(NODOOFT));
	if (p == NULL) return 0;

	p->fcb.iDescriptor = fcb.iDescriptor;
	copiarU (p->fcb.cvNombreDA, fcb.cvNombreDA);
	copiarU (p->fcb.cvNombreArchivo, fcb.cvNombreArchivo);
	p->fcb.iModoApertura = fcb.iModoApertura;
	p->fcb.iTamanio = fcb.iTamanio;
	p->fcb.sectores = fcb.sectores;

	p->sgte = *pila;
	*pila = p;

	return 1;
}

/* 	Nombre de la Función: quitarOftPorNombreArchivo
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: eliminar nodo por NombreArchivo
	Entrada:	pila: Lista de archivos
			cpNombreArchivo: nombre del archivo a eliminar
	Salida:	return: 0 si no lo encontro
				 1 si lo encontro
*/
int quitarOftPorNombreArchivo(pOft* pila, unsigned char* cpNombreArchivo)
{
	pOft pActual, pAnterior;
	int iBandera = 0;

	pActual = *pila;
	pAnterior = NULL;

	while (pActual !=NULL && (compararU(pActual->fcb.cvNombreArchivo, cpNombreArchivo)!=0))
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
		free(pActual->fcb.sectores);
		free(pActual);
		return ENCONTRADO; /*LO ENCONTRO*/
	}

	return NO_ENCONTRADO; /*NO LO ENCONTRO*/

}

/* 	Nombre de la Función: quitarOftPorNombreDescriptor
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 04/10/2009
	modificación:  04/10/2009
	Objetivo de la Función: eliminar nodo por NombreArchivo
	Entrada:	pila: Lista de archivos
			iDescriptor: Descriptor unico
	Salida:	return: 0 si no lo encontro
				 1 si lo encontro
*/
int quitarOftPorNombreDescriptor(pOft* pila, int iDescriptor)
{
	pOft pActual, pAnterior;
	int iBandera = 0;

	pActual = *pila;
	pAnterior = NULL;

	while (pActual !=NULL && (pActual->fcb.iDescriptor != iDescriptor))
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
		free(pActual);
		return ENCONTRADO; /*LO ENCONTRO*/
	}

	return NO_ENCONTRADO; /*NO LO ENCONTRO*/

}

/* 	Nombre de la Función: crearfcbOFT 
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: crear el contenido de una entrada de OFT
	Entrada:	iDescriptor: Valor unico para esta entrada (sera el socket)
			cpNombreDA: nombre del dispositivo de almacenamiento
			cpNombreArchivo: nombre del archivo a eliminar
			iModoApertura: modo de aprtura (ESCRITURA, LECTURA)
			iTamanio: tamanio del archivo
	Salida:	descripto: registro con los datos cargados sin sectores
*/
void crearfcbOFT (INFOOFT * fcb, int iDescriptor, unsigned char* cpNombreDA, unsigned char* cpNombreArchivo, int iModoApertura, int iTamanio, int* ipListaSectores)
{

	fcb->iDescriptor = iDescriptor;
	copiarU(fcb->cvNombreDA,cpNombreDA);
	copiarU(fcb->cvNombreArchivo, cpNombreArchivo);
	fcb->iModoApertura = iModoApertura;
	fcb->iTamanio = iTamanio;
	fcb->iAperturas = 1;
	fcb->sectores = ipListaSectores;
}

/* 	Nombre de la Función: verificarArchivoPara
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: buscar y decir si el archivo esta abierto con un modo determinado
	Entrada:	pila: Lista de archivos
			unAlmacenamiento: nombre del dispositivo
			nombreArchivo: nombre del archivo
			imodoApertura: modo de aprtura (ESCRITURA, LECTURA)
	Salida:	return: 0 si no lo encontro
				 1 si lo encontro
*/
int verificarArchivoAbiertoPara(pOft* pila, unsigned char* unAlmacenamiento, unsigned char* nombreArchivo, int imodoApertura)
{
	pOft pAux = *pila;

	for (;pAux!=NULL; pAux->sgte = pAux)
	{
		if ((compararU(pAux->fcb.cvNombreDA, unAlmacenamiento)==0) && (compararU(pAux->fcb.cvNombreArchivo, nombreArchivo)==0))
			if (pAux->fcb.iModoApertura == imodoApertura)
				return ENCONTRADO;
	}
	return NO_ENCONTRADO;
}

/* 	Nombre de la Función: verificarArchivoAbierto
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 21/09/2009
	modificación:  21/09/2009
	Objetivo de la Función: buscar y decir si el archivo esta abierto con un modo determinado
	Entrada:	pila: Lista de archivos
			unAlmacenamiento: nombre del dispositivo
			nombreArchivo: nombre del archivo
	Salida:	return: 0 si no lo encontro
				 1 si lo encontro
*/
int verificarArchivoAbierto(pOft* pila, unsigned char* unAlmacenamiento, unsigned char* nombreArchivo)
{
	pOft pAux = *pila;

	for (;pAux!=NULL; pAux->sgte = pAux)
	{
		if ((compararU(pAux->fcb.cvNombreDA, unAlmacenamiento)==0) && (compararU(pAux->fcb.cvNombreArchivo, nombreArchivo)==0))
				return ENCONTRADO;
	}
	return NO_ENCONTRADO;
}

/* 	Nombre de la Función: crearListaSectores 
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: genera un vector con la cantidad de sectores que ocupa el achivo
	Entrada:	iCantidad: cantidad de entradas
	Salida:	pLista: se guarda en la direccion de comienzo de la lista
			return: 0 si no lo encontro
				 1 si lo encontro
*/
int crearListaSectores (int * pLista, int iCantidad, unsigned char* ucpLista)
{
	int i;
	/* por ejemplo: vienen en boques de 4 bytes. Tengo que convertirlos a numeros*/
	/* me muevo sobre el vector cada 4 posiciones y los guardo del final al principio */
	for (i = 0; i< iCantidad; i++)
		pLista[iCantidad - i - 1] = (int) BinToNum (ucpLista, 4*i);

	return 1;
}

/* 	Nombre de la Función: obtenerNombres 
	Versión: 0.5
	Autor/es: Villalva Emanuel
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: dado un /DA/Nombre archivo, los retorna separados
	Entrada:	cpNombre: cadena con formato /DA/Nombre
	Salida:	cpNombreDA: se guarda DA
			cpNombreArchivo: se guarda Nombre
*/
void obtenerNombres (unsigned char* ucpNombre,unsigned  char* ucpNombreDA,unsigned  char* ucpNombreArchivo)
{
	int i,j;

	for (j=0, i=1; ucpNombre[i]!='/'; i++, j++)
		ucpNombreDA[j]= ucpNombre[i];
	ucpNombreDA[j]='\0';
	i++;
	for (j=0; ucpNombre[i]!='\0'; i++, j++)
		ucpNombreArchivo[j] = ucpNombre[i];
	ucpNombreArchivo[j]='\0';
}

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syscalls.h"
#include "sysopenN.h"
#include "oft.h"
#include "fd.h"
#include "sendrecv.h"
#include "cadenas.h"

/* 	Nombre de la Función: sys_open
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 21/09/2009
	modificación:  21/09/2009
	Objetivo de la Función: iniciar sys_open
	Entrada:	iSocketSFS: socket del SFS
			iSocketCliente: socket del cliente que mando el mensaje o hay que enviarle (0 si no corresponde)
			L_Oft: OFT del sistema
			pFD: puntero al FD correspondiente
			cabecera: cabecera que recibio el mensaje en prorgama principal
	Salida:	internamente responde al cliente
*/
int sys_open (int iSocketSFS, int iSocketCliente, pOft* L_Oft, pNodoFD* pFD, HEADERIRCIPC cabecera)
{

	/* variables de OFT */
	pOft pAuxOFT = * L_Oft;
	int* pListaSectores;

	/* asignaciones locales */
	pNodoFD fdLocal = *pFD;
	unsigned char * ucpPedidos;

	INFOOFT unfcb;

	switch (cabecera.ucPayloadDescriptor)
	{
	/* casos para ESCRITURA ***************************************************/
		case ESCRITURA_APERTURA:/* inicia*/
			
			if (verificarArchivoAbiertoPara(L_Oft, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_ESCRITURA) != ENCONTRADO) /* oft.h */
			{
				enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, cabecera.ucPayloadDescriptor, cabecera);
			}
			break;
			
		case ESCRITURA_ARCHIVO_EXISTE:
			/* enviar un ELIMINARARCHIVO_IDA con sus parametros */
			enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, ESCRITURA_ARCHIVO_ELIMINAR , cabecera); /*syscall.h*/
			break;
			
		case ESCRITURA_ARCHIVO_INEXISTENTE:
			/* enviar un CREARARCHIVO_IDA con sus parametros */
			enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, ESCRITURA_ARCHIVO_CREAR , cabecera); /*syscall.h*/
			break;
			
		case ESCRITURA_ARCHIVO_ELIMINADO:
			/* enviar un CREARARCHIVO_IDA con sus parametros */
			enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, ESCRITURA_ARCHIVO_CREAR, cabecera); /*syscall.h*/
			break;
			
		case ESCRITURA_ARCHIVO_CREADO:
			/* crearEntradaOFT con parametro ESCRITURA*/
			crearfcbOFT (&unfcb, iSocketCliente,fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_ESCRITURA, 0, NULL); /* oft.h*/
			meterOft (&pAuxOFT , unfcb); /*oft.h*/
			*L_Oft = pAuxOFT ;
			fdLocal->fd.pDescriptor = pAuxOFT;	
			return 2;
			break;
	/* ************************************************************************ */
	/* casos de LECTURA ******************************************************* */
	
		case LECTURA_APERTURA:/* inicia*/
			
			if (verificarArchivoAbiertoPara(L_Oft, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_LECTURA) != ENCONTRADO) /* oft.h */
			{
				enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, cabecera.ucPayloadDescriptor, cabecera);
			}
			break;
			
		case LECTURA_ARCHIVO_EXISTE:
			/* enviar un INFOARCHIVO_IDA con sus parametros */
			enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, LECTURA_ARCHIVO_INFORMACION, cabecera); /*syscall.h*/
			break;
			
		case LECTURA_ARCHIVO_INFOVUELTA:
			/* recibir respuesta de INFOARCHIVO */
			ucpPedidos = (unsigned char*) malloc (cabecera.uiPayloadLength);
			recibirAlgo (iSocketCliente, ucpPedidos, cabecera.uiPayloadLength);
			pListaSectores = (int*) malloc(cabecera.uiPayloadLength-4);
											     /*porque los primeros 4 es tamanioArchivo*/
			crearListaSectores (pListaSectores,  (cabecera.uiPayloadLength-4)/4,  &ucpPedidos[4]); /*oft.h*/
			/* crearEntradaOFT con parametro LECTURA*/
			fdLocal->fd.iTotal = (cabecera.uiPayloadLength-4)/4;
			crearfcbOFT (&unfcb, iSocketCliente,fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_LECTURA, BinToNum (ucpPedidos,0), pListaSectores); /*oft.h*/
			meterOft (&pAuxOFT , unfcb); /*oft.h */
			*L_Oft = pAuxOFT ;
			fdLocal->fd.pDescriptor = pAuxOFT;			
			free (ucpPedidos);
			return 2;
			break;
		/* ************************************************************ */
		case ELIMINAR_APERTURA:/* inicia*/
			if (verificarArchivoAbiertoPara(L_Oft, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_LECTURA) != ENCONTRADO) /* oft.h */
			{
				enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, cabecera.ucPayloadDescriptor, cabecera);
			}
			break;
		case ELIMINAR_ARCHIVO_EXISTE:
			/* crearEntradaOFT con parametro ELIMINAR*/
			crearfcbOFT (&unfcb, iSocketCliente, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_ELIMINAR, 0, NULL); /*syscall.h*/
			meterOft (&pAuxOFT , unfcb);
			*L_Oft = pAuxOFT ;
			fdLocal->fd.pDescriptor = pAuxOFT;	
			return 2;
			break;
			
		/* *************************************************************** */
		case ELIMINAR_ARCHIVO_INEXISTENTE: case LECTURA_ARCHIVO_INEXISTENTE:
			return 3;
			break;
	}

	*L_Oft = pAuxOFT ;
	return 0;
	
}

/* 	Nombre de la Función: separarParametrosOpen 
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: separar el mensaje recibido en los parametros de open
	Entrada:	ucpMensaje: stream que contiene los datos
	Salida:	ucpArchivo: nombre de archivo a procesar
			ipFlag: tipo de apertura
			ipMode: derechos de acceso
*/

void separarParametrosOpen (unsigned char* ucpMensaje, unsigned char* ucpArchivo, unsigned char* ipFlag)
{
	char* p;
	
	p = strtok ((char*)ucpMensaje, "|");
	copiarU (ucpArchivo, (unsigned char*)p);
	p = strtok (NULL, "|");
	*ipFlag = p[0];

}



#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syscalls.h"
#include "sysopen.h"
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
	/* auxiliares datos */
	unsigned int iFlag;
	unsigned int iMode;

	/* variables de OFT */
	unsigned char ucvNombreArchivo[140];
	pOft pAuxOFT = * L_Oft;
	int* pListaSectores;

	/* asignaciones locales */
	pNodoFD fdLocal = *pFD;
	unsigned char * ucpPedidos;

	INFOOFT unfcb;

	switch (fdLocal->fd.iPaso)
	{
	/* casos para ESCRITURA */
		case 0: /* inicia*/
			ucpPedidos = (unsigned char*) malloc (cabecera.uiPayloadLength);
			recibirAlgo (iSocketCliente, ucpPedidos, cabecera.uiPayloadLength);
			separarParametrosOpen (ucpPedidos, ucvNombreArchivo, &iFlag, &iMode); /* sysopen.h */
			obtenerNombres (ucvNombreArchivo, fdLocal->fd.ucvDA,fdLocal->fd.ucvNombreArchivo); /* oft.h*/ /*saco DA y NomArch */
			if (verificarArchivoAbiertoPara(L_Oft, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_LECTURA) != ENCONTRADO) /* oft.h */
			{
				enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, EXISTEARCHIVO_IDA, cabecera);
			}
			/* casos de uiFlag: 1 LECTURA, 2 ESCRITURA, 3 ELIMINAR */
			fdLocal->fd.iPaso = iFlag % 10;			
			free (ucpPedidos);
			break;
		case 1:			
			/* enviar un INFOARCHIVO_IDA con sus parametros */
			enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, INFOARCHIVO_IDA, cabecera); /*syscall.h*/
			fdLocal->fd.iPaso = 4;

			break;
		case 2:
			/* enviar un ELIMINARARCHIVO_IDA con sus parametros */
			enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, ELIMINARARCHIVO_IDA , cabecera); /*syscall.h*/
			fdLocal->fd.iPaso = 5;

			break;
		case 3:
			/* crearEntradaOFT con parametro ELIMINAR*/
			crearfcbOFT (&unfcb, iSocketCliente, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_ELIMINAR, 0, NULL); /*syscall.h*/
			meterOft (&pAuxOFT , unfcb);
			return 2;
			break;

		case 4:
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
			free (ucpPedidos);
			return 2;
			break;

		case 5:
			/* enviar un CREARARCHIVO_IDA con sus parametros */
			enviarASFS (iSocketSFS, fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, CREARARCHIVO_IDA , cabecera); /*syscall.h*/
			fdLocal->fd.iPaso = 6;
			break;

		case 6:
			/* crearEntradaOFT con parametro ESCRITURA*/
			crearfcbOFT (&unfcb, iSocketCliente,fdLocal->fd.ucvDA, fdLocal->fd.ucvNombreArchivo, APERTURA_ESCRITURA, 0, NULL); /* oft.h*/
			meterOft (&pAuxOFT , unfcb); /*oft.h*/
			return 2;
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

void separarParametrosOpen (unsigned char* ucpMensaje, unsigned char* ucpArchivo, unsigned int* ipFlag, unsigned int* ipMode)
{
	char* p;
	
	p = strtok ((char*)ucpMensaje, "|");
	copiarU (ucpArchivo, (unsigned char*)p);
	p = strtok (NULL, "|");
	*ipFlag = miatoi(p);
	p = strtok (NULL, "|");
	*ipMode = miatoi (p);


}



#define BUFFER 		1024
#define BUFFER_EXT		1032

#include <stdlib.h>
#include "syscalls.h"
#include "sendrecv.h"
#include "cadenas.h"

/* 	Nombre de la Función: enviar_get_sectores
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: enviar pedido de getSectores
	Entrada:	iSocket: socket de donde llega el pedido
			cabecera: contiene informacion de recepcion
			ucpSectores: Cadena con los dos valores (numtobin)
	Salida:	return: 1 si fue correcto
				 0 si no fue correcto
*/
int enviar_getSectores(int iSocketDA, HEADERIRCIPC cabecera, unsigned char* ucpSectores)
{

	enviarH (iSocketDA, cabecera);
	enviarAlgo (iSocketDA, ucpSectores, cabecera.uiPayloadLength);

	return 1;
}

/* 	Nombre de la Función: recibir_getSectores
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: recubir los sectores pedidos de getSectores y
				   guardarlos en el buffer
	Entrada:	iSocket: socket de donde llega la respuesta
	Salida:	buffer: bloque donde se guarda lo pedido
			return: 1 si fue correcto
				 0 si no fue correcto
*/
int recibir_getSectores(int iSocket, unsigned char* buffer)
{
	recibirAlgo (iSocket, buffer, BUFFER);
	return 1;
}

/* 	Nombre de la Función: enviar_putSectores
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: enviar sectores a escribir
	Entrada:	iSocket: socket de donde se va a enviar
			cabecera: para indicar tipo de accion
			buffer: bloque a transferir	
			unAlmacenamiento: almacenamiento de destino
			iSect1: numero de sector del primer bloque
			iSect2: numero de sector del segundo bloque		
	Salida:	return: 1 si fue correcto
				 0 si no fue correcto
*/
int enviar_putSectores(int iSocket, HEADERIRCIPC cabecera, unsigned char* buffer, int iSect1, int iSect2)
{
	unsigned char cvSector[4];
	enviarH (iSocket, cabecera);
	NumToBin(cvSector, iSect1, 0);
	enviarAlgo (iSocket, cvSector, 4);
	enviarAlgo (iSocket, buffer, BUFFER/2);
	NumToBin(cvSector, iSect2, 0);
	enviarAlgo (iSocket, cvSector, 4);
	enviarAlgo (iSocket, &buffer[BUFFER/2], BUFFER/2);

	return 1;
}

/* 	Nombre de la Función: enviar_getCHS
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: enviar pedido de CHS
	Entrada:	iSocket: socket de donde se va a enviar pedido
			cabecera: contiene el pedido
	Salida:	return: 1 si fue correcto
				 0 si no fue correcto
*/
int enviar_getCHS(int iSocket, HEADERIRCIPC cabecera)
{
	if (enviarH(iSocket, cabecera))
		return 1;
	else return 0;
}

/* 	Nombre de la Función: recibir_getCHS
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: recibir confirmacion de sectores escritos
	Entrada:	iSocket: socket de donde se va a recibir confirmacion
			iCantidad: tamanio del mensaje
	Salida:	cpInfo: contiene la respuesta con los datos de CHS
			return: 1 si fue correcto
				 0 si no fue correcto
*/
int recibir_getCHS(int iSocket, unsigned char* cpInfo, int iCantidad)
{
	if (recibirAlgo(iSocket, cpInfo, iCantidad))
		return 1;
	else return 0;
}

/* 	Nombre de la Función: limbiarBuffer 
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: limpia el buffer con '\0'
	Entrada:	buffer: bloque que se va a limpiar
	Salida:	-
*/
void limbiarBuffer (char * buffer)
{
	int i;
	for (i=0; i < 1024; i++)
		buffer[i]='\0';
}

/* 	Nombre de la Función: enviarASFS 
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 20/09/2009
	modificación:  20/09/2009
	Objetivo de la Función: limpia el buffer con '\0'
	Entrada:	ucvDA: nombre del dispositivo
			ucvNombreArchivo: nombre del archivo
			iProtocolo: primitiva que se desea llamar
			cabecera: para saber cuando responde de quien es
	Salida:	-
*/
void enviarASFS (int iSocketSFS, unsigned char *ucvDA, unsigned char *ucvNombreArchivo, int iProtocolo, HEADERIRCIPC cabecera)
{
	unsigned char * ucpPedidos;
	
	ucpPedidos = (unsigned char*) malloc (longitudCadenaU(ucvDA) + longitudCadena ("|") + longitudCadenaU(ucvNombreArchivo) + 1);
	copiarU (ucpPedidos, ucvDA); concatenarU (ucpPedidos, (unsigned char*)"|"); concatenarU (ucpPedidos, ucvNombreArchivo);
	cabecera.ucPayloadDescriptor = iProtocolo;
	cabecera.uiPayloadLength = longitudCadenaU (ucpPedidos);
	enviarH (iSocketSFS, cabecera);
	enviarAlgo (iSocketSFS, ucpPedidos, cabecera.uiPayloadLength);
	free (ucpPedidos);

}




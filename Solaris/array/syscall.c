#include <stdio.h>
#include <stdlib.h>
#include "protocolos.h"
#include "disco.h"
#include "syscall.h"
#include "protocolos.h"

/* 	Nombre de la Función: recibirPedido_getSectores 
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 19/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Recibir un pedido de GETSECTORES
	Entrada:	int iSocketOrigen: Socket de donde vino el pedido
			HEADERIRCIPC cabecera: Para la longitud del mensaje
	Salida:	SSECTORES* sSectores: Los sectores pedidos;
			return: SYSCALL_GETSECTORES_RECV_PEDIDO_FAIL, SYSCALL_GETSECTORES_RECV_PEDIDO_OK
*/
int recibirPedido_getSectores(int iSocketOrigen, HEADERIRCIPC cabecera, SSECTORES* sSectores)
{
	int iResult;
	unsigned char buffer[SIZE_STRUCT_IDSECTOR * 2];

	iResult = recibirAlgo(iSocketOrigen, buffer, cabecera.uiPayloadLength);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_RECV_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_RECV_PEDIDO_FAIL; break;
	}

	sSectores->iIdSector1 =(int) BinToNum (buffer, 0);
	sSectores->iIdSector2 =(int) BinToNum (buffer, 4);

	return SYSCALL_GETSECTORES_RECV_PEDIDO_OK;
}

/* 	Nombre de la Función: enviarPedido_getSectores
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 19/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Enviar un pedido de GETSECTORES
	Entrada:	int iSocketDestino: Socket de donde se enviara el pedido
			HEADERIRCIPC cabecera: Debe tener cargado el cpDescriptorID
			SSECTORES* sSectores: Los sectores a enviar deben estar cargados;
	Salida:	-
			return: SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL, SYSCALL_GETSECTORES_SEND_PEDIDO_OK
*/
int enviarPedido_getSectoresDosDiscos(int iSocketDestino, HEADERIRCIPC cabecera, SSECTORES sSectores)
{
	int iResult;
	unsigned char buffer[SIZE_STRUCT_IDSECTOR * 2];

	NumToBin (buffer, sSectores.iIdSector1 , 0);
	NumToBin (buffer, sSectores.iIdSector2 , 4);

	cabecera.ucPayloadDescriptor = IPCIRC_GET_SECTORES2;
	cabecera.uiPayloadLength = sizeof(	buffer);

	iResult = enviarH(iSocketDestino, cabecera);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
	}

	iResult = enviarAlgo(iSocketDestino , buffer, cabecera.uiPayloadLength);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
	}

	return SYSCALL_GETSECTORES_SEND_PEDIDO_OK;
	
}

/* 	Nombre de la Función: enviarPedido_getSectoresDosDiscos
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 19/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Enviar un pedido de GETSECTORES
	Entrada:	int iSocketDestino: Socket de donde se enviara el pedido
			HEADERIRCIPC cabecera: Debe tener cargado el cpDescriptorID
			int iSector: El sector a enviar
	Salida:	-
			return: SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL, SYSCALL_GETSECTORES_SEND_PEDIDO_OK
*/
int enviarPedido_getSectoresUnDisco(int iSocketDestino, HEADERIRCIPC cabecera, int iSector)
{
	int iResult;
	unsigned char buffer[SIZE_STRUCT_IDSECTOR];

	NumToBin (buffer,iSector, 0);
	cabecera.ucPayloadDescriptor = IPCIRC_GET_SECTORES1;
	cabecera.uiPayloadLength = sizeof(	buffer);

	iResult = enviarH(iSocketDestino, cabecera);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
	}

	iResult = enviarAlgo(iSocketDestino , buffer, cabecera.uiPayloadLength);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
	}

	return SYSCALL_GETSECTORES_SEND_PEDIDO_OK;
	
}

void limpiarPedidos(SPEDIDO* vPedidos, int MAX)
{
	int i;
	
	for (i=0; i<MAX; i++)
	{
		vPedidos[i].cpDescriptorID[0]='\0';
		vPedidos[i].iCant = 0;
	}
}

void agregarPedido(SPEDIDO* vPedidos, int MAX, HEADERIRCIPC cabecera, int iSector1, int iSector2, int iSocketS1, int iSocketS2)
{
	int i,j;
	
	for (i=0; (i<MAX) && (vPedidos[i].cpDescriptorID[0]!='\0'); i++);

	for(j=0; j<16; vPedidos[i].cpDescriptorID[j]=cabecera.cpDescriptorID[j],j++);
	
	vPedidos[i].iCant=0;
	
	vPedidos[i].iSocketS1 = iSocketS1;
	vPedidos[i].iSocketS2 = iSocketS2;
	
	vPedidos[i].sectores.iIdSector1 = iSector1;
	vPedidos[i].sectores.iIdSector2 = iSector2;
	
	for(j=0; j<512; j++)
	{
		vPedidos[i].sectores.cvBufferSector2[j]='\0';
		vPedidos[i].sectores.cvBufferSector1[j]='\0';
	}
	
	
}

int agregarRespuesta(int iSocket, HEADERIRCIPC cabecera, SPEDIDO* vPedidos, int MAX)
{
	int i;
	
	for(i=0; (i<MAX) && (compararCabecera(cabecera.cpDescriptorID, vPedidos[i].cpDescriptorID) !=1); i++);
	
	if(iSocket == vPedidos[i].iSocketS1)
		recibirAlgo(iSocket,vPedidos[i].sectores.cvBufferSector1,cabecera.uiPayloadLength);
		
	if(iSocket == vPedidos[i].iSocketS2)
		recibirAlgo(iSocket,vPedidos[i].sectores.cvBufferSector2,cabecera.uiPayloadLength);
		
	vPedidos[i].iCant++;
	
	if (vPedidos[i].iCant == 2)
	return i;
	else return -1;
}



void eliminarPedido(SPEDIDO* vPedidos, int iPos)
{
	int i = iPos;
	vPedidos[i].cpDescriptorID[0]='\0';
	vPedidos[i].iCant = 0;
}

int recibirRespuesta_getSectoresB(int iSocket, HEADERIRCIPC cabecera, unsigned char* cpSector)
{
	int iResult;
	
	iResult = recibirAlgo(iSocket, cpSector, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_RECV_RTA_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_RECV_RTA_FAIL; break;
	}
	return 1;
}

/* esta funcion la usa el hilo */
int enviarRespuesta_getSectoresB(int iSocket, HEADERIRCIPC cabecera, unsigned char* cpSector)
{
	int iResult;
	enviarH(iSocket,cabecera);
	iResult = enviarAlgo(iSocket, cpSector, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_RECV_RTA_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_RECV_RTA_FAIL; break;
	}
	return 1;
}

/* 	Nombre de la Función: recibirRespuesta_getSectores
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 20/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Recibir una respuesta de GETSECTORES
	Entrada:	int iSocketOrigen: Socket de donde vino el pedido
			HEADERIRCIPC cabecera: Para la longitud del mensaje
	Salida:	SSECTORES* sSectores: Los buffers cargados;
			return: SYSCALL_GETSECTORES_RECV_RTA_FAIL, SYSCALL_GETSECTORES_RECV_RTA_OK
*/
int recibirRespuesta_getSectores(int iSocketOrigen, HEADERIRCIPC cabecera, SSECTORES* sSectores)
{
	int iResult;

	if (cabecera.uiPayloadLength != (SIZE_STRUCT_BUFFERSECTOR * 2))
	{
		printf("El buffer enviado para recibirRespuesta_getSectores es distindo a 1024\n");
		return  SYSCALL_GETSECTORES_RECV_RTA_FAIL;
	}

	iResult = recibirAlgo(iSocketOrigen, sSectores->cvBufferSector1, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_RECV_RTA_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_RECV_RTA_FAIL; break;
	}

	
	iResult = recibirAlgo(iSocketOrigen, sSectores->cvBufferSector2, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_RECV_RTA_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_RECV_RTA_FAIL; break;
	}

	return SYSCALL_GETSECTORES_RECV_RTA_OK;
}

/* 	Nombre de la Función: recibirPedido_getSectores 
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 20/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Recibir un pedido de GETSECTORES
	Entrada:	int iSocketOrigen: Socket de donde vino el pedido
			HEADERIRCIPC cabecera: Para la longitud del mensaje
			SSECTORES* sSectores: Los sectores cargados;
	Salida:	-
			return: SYSCALL_GETSECTORES_SEND_RTA_FAIL, SYSCALL_GETSECTORES_SEND_RTA_OK
*/
int enviarRespuesta_getSectores(int iSocketDestino, HEADERIRCIPC cabecera, SSECTORES sSectores)
{
	int iResult;

	cabecera.ucPayloadDescriptor = IPCIRC_GET_SECTORES_RTA;
	cabecera.uiPayloadLength = SIZE_STRUCT_BUFFERSECTOR * 2;

	iResult = enviarH(iSocketDestino, cabecera);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_SEND_RTA_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_SEND_RTA_FAIL; break;
	}

	iResult = enviarAlgo(iSocketDestino , sSectores.cvBufferSector1, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_SEND_RTA_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_SEND_RTA_FAIL; break;
	}

	iResult = enviarAlgo(iSocketDestino , sSectores.cvBufferSector2, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_SEND_RTA_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_SEND_RTA_FAIL; break;
	}

	return SYSCALL_GETSECTORES_SEND_RTA_OK;
	
}

/* 	Nombre de la Función: recibirPedido_putSectores 
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 20/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Recibir un pedido de GETSECTORES
	Entrada:	int iSocketOrigen: Socket de donde vino el pedido
			HEADERIRCIPC cabecera: Para la longitud del mensaje
	Salida:	SSECTORES* sSectores: Los buffers cargados;
			return: SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL, SYSCALL_PUTSECTORES_RECV_PEDIDO_OK
*/
int recibirPedido_putSectores(int iSocketOrigen, HEADERIRCIPC cabecera, SSECTORES* sSectores)
{
	int iResult;
	unsigned char buffer[SIZE_STRUCT_IDSECTOR];

	if (cabecera.uiPayloadLength != ((SIZE_STRUCT_BUFFERSECTOR * 2) + (SIZE_STRUCT_IDSECTOR * 2)))
	{
		printf("El buffer recibido para recibirPedido_putSectores es distindo a 1032\n");
		return  SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL;
	}

	/* Recibo SECTOR 1 */
	iResult = recibirAlgo(iSocketOrigen, buffer, SIZE_STRUCT_IDSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL; break;
	}

	sSectores->iIdSector1 =(int) BinToNum (buffer, 0);

	/* Recibo BUFFER 1 */
	
	iResult = recibirAlgo(iSocketOrigen, sSectores->cvBufferSector1, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL; break;
	}

	/* Recibo SECTOR 2 */

	iResult = recibirAlgo(iSocketOrigen, buffer, SIZE_STRUCT_IDSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL; break;
	}

	sSectores->iIdSector2 =(int) BinToNum (buffer, 0);

	/* Recibo BUFFER 2 */

	iResult = recibirAlgo(iSocketOrigen, sSectores->cvBufferSector2, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL; break;
	}

	return SYSCALL_PUTSECTORES_RECV_PEDIDO_OK;
}

/* 	Nombre de la Función: enviarPedido_putSectores 
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 20/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Enviar un pedido de GETSECTORES
	Entrada:	int iSocketOrigen: Socket de donde vino el pedido
			HEADERIRCIPC cabecera: Para la longitud del mensaje
			SSECTORES* sSectores: Todo cargado;
	Salida:	-
			return: SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL, SYSCALL_PUTSECTORES_SEND_PEDIDO_OK
*/
int enviarPedido_putSectores(int iSocketDestino, HEADERIRCIPC cabecera, SSECTORES sSectores)
{
	int iResult;
	unsigned char buffer[SIZE_STRUCT_IDSECTOR];

	
	cabecera.ucPayloadDescriptor = IPCIRC_PUT_SECTORES;
	cabecera.uiPayloadLength = (SIZE_STRUCT_BUFFERSECTOR * 2) + (SIZE_STRUCT_IDSECTOR * 2);

	iResult = enviarH(iSocketDestino, cabecera);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL; break;
	}

	/* Envio SECTOR 1 */
	NumToBin (buffer, sSectores.iIdSector1 , 0);

	iResult = enviarAlgo(iSocketDestino , buffer, SIZE_STRUCT_IDSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL; break;
	}

	/* Envio BUFFER 1 */

	iResult = enviarAlgo(iSocketDestino , sSectores.cvBufferSector1, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL; break;
	}



	/* Envio SECTOR 2 */
	NumToBin (buffer, sSectores.iIdSector2 , 0);

	iResult = enviarAlgo(iSocketDestino , buffer, SIZE_STRUCT_IDSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL; break;
	}

	/* Envio BUFFER 2 */

	iResult = enviarAlgo(iSocketDestino , sSectores.cvBufferSector2, SIZE_STRUCT_BUFFERSECTOR);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL; break;
	}

	return SYSCALL_PUTSECTORES_SEND_PEDIDO_OK;
	
}

void enviar_getCHS(int iSocket,HEADERIRCIPC cabecera, CONFIGURACION config)
{
	unsigned char ucvBuf[12];

	NumToBin (ucvBuf,config.iCilindros, 0);
	NumToBin (ucvBuf,config.iCabezas, 4);
	NumToBin (ucvBuf,config.iSectores, 8);
	cabecera.uiPayloadLength = 12;
	cabecera.ucPayloadDescriptor = IPCIRC_GET_CHS_RTA;
	enviarH(iSocket,cabecera);
	enviarAlgo(iSocket, ucvBuf, cabecera.uiPayloadLength);
	
}


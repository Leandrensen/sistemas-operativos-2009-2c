
#include <stdlib.h>
#include "syscalls.h"
#include "oft.h"
#include "fd.h"
#include "syslist.h"
#include "cadenas.h"

int sys_list (int iSocketSFS, int iSocketCliente, pNodoFD* pFD, pConexiones* L_Conexiones, HEADERIRCIPC cabecera)
{
	pNodoFD pAuxFD = *pFD;
	unsigned char * ucpPedido;

	/* bandera para indicar que la system call termino completamente */
	int iBandera = 0;

	switch (pAuxFD->fd.iPaso)
	{
		case 0:
			ucpPedido = (unsigned char*) malloc (cabecera.uiPayloadLength + 1);
			recibirAlgo (iSocketCliente, ucpPedido, cabecera.uiPayloadLength);
			cabecera.ucPayloadDescriptor = LISTARDIRECTORIO_IDA;
			enviarH (iSocketSFS, cabecera);
			enviarAlgo (iSocketSFS, ucpPedido, cabecera.uiPayloadLength);
			free(ucpPedido);
			pAuxFD->fd.iPaso = 1;
			break;

		case 1:
			/* enviar respuestas OK al FTP */
			ucpPedido = (unsigned char *) malloc (cabecera.uiPayloadLength+1);
			recibirAlgo (iSocketSFS, ucpPedido, cabecera.uiPayloadLength);
			cabecera.ucPayloadDescriptor = 0x35;
			enviarH (iSocketCliente, cabecera);
			enviarAlgo (iSocketCliente, ucpPedido, cabecera.uiPayloadLength);
			printf ("Enviando listado al SFTP\n");
			iBandera = 1;
			break;	
	}		
	return iBandera;
}

int tamanioAEnviar (pConexiones* L_Conexiones)
{
	int iTotal = 0;
	pConexiones pAux = *L_Conexiones;
	
	for(; pAux!=NULL; pAux = pAux->sgte)
		iTotal = longitudCadenaU(pAux->conexion.ucvNombre) + 1;
	
	return iTotal;
}


#include <stdlib.h>
#include "syscalls.h"
#include "sysclose.h"
#include "oft.h"
#include "fd.h"
#include "sendrecv.h"
#include "cadenas.h"

int sys_close(int iSocketSFS, int iSocketCliente, pNodoFD * pFD, pOft* L_OFT, HEADERIRCIPC cabecera)
{
	pNodoFD pAuxFD = *pFD;
	pOft pArchivo = pAuxFD->fd.pDescriptor;
	pOft pAuxOft = *L_OFT;

	/* bandera de finalizacion total de la system call */
	int iBandera = 0;

	switch (pAuxFD->fd.iPaso)
	{
		case 0: 
			if (pArchivo != NULL)
			switch (pArchivo->fcb.iModoApertura)
			{
				case APERTURA_LECTURA:
				/*DIRECTAMENTE HACER EL EMPALME Y FIN DE LA SYSTEM CALL*/
					quitarOftPorNombreDescriptor(&pAuxOft, pArchivo->fcb.iDescriptor);
					iBandera = 1;
					break;
				case APERTURA_ESCRITURA:
					/* enviar ACTUALIZARTAMANIO con sus parametros */
					printf("Actualizando tamanio: %d", pArchivo->fcb.iTamanio);
					actualizartamanio (iSocketSFS, pArchivo->fcb.cvNombreDA, pArchivo->fcb.cvNombreArchivo, (long)pArchivo->fcb.iTamanio, cabecera);
				break;
				case APERTURA_ELIMINAR:
					/* enviar ELIMINARARCHIVO con sus parametros*/
					enviarASFS(iSocketSFS, pArchivo->fcb.cvNombreDA, pArchivo->fcb.cvNombreArchivo, ELIMINARARCHIVO_IDA, cabecera);
				break;
			}
			pAuxFD->fd.iPaso = 1;
			break;
		case 1:
			quitarOftPorNombreDescriptor(&pAuxOft, pArchivo->fcb.iDescriptor);
			cabecera.ucPayloadDescriptor = CLOSE_FILE_OK;
			enviarH(iSocketCliente, cabecera);
			*L_OFT = pAuxOft;
			iBandera = 4;
			break;
	}
	
	return iBandera;
}

void actualizartamanio (int iSocketSFS, unsigned char * ucpDA, unsigned char* ucpNombreArchivo, long lTamanio, HEADERIRCIPC cabecera)
{
	unsigned char * ucpPedidos;
	unsigned char ucvNumero[20];
	
	ucpPedidos = (unsigned char*) malloc (longitudCadenaU(ucpDA) + longitudCadena ("|") + longitudCadenaU(ucpNombreArchivo) + longitudCadena ("|") + 20 + 1);
	copiarU (ucpPedidos, ucpDA); concatenarU (ucpPedidos, (unsigned char*)"|"); concatenarU (ucpPedidos, ucpNombreArchivo); 
	concatenarU (ucpPedidos, (unsigned char*)"|"); 
	miitoa ((char*)ucvNumero,(int) lTamanio);
	concatenarU (ucpPedidos, ucvNumero);
	cabecera.ucPayloadDescriptor = ACTUALIZARTAMANIO_IDA;
	cabecera.uiPayloadLength = longitudCadenaU (ucpPedidos);
	enviarH (iSocketSFS, cabecera);
	enviarAlgo (iSocketSFS, ucpPedidos, cabecera.uiPayloadLength);
	free (ucpPedidos);

}


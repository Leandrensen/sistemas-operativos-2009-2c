#include "handshake.h"

int realizarHandshake(int iSocket, CONFIGURACION stArchConf)
{
	HEADERIRCIPC cabecera;
	char cBufNombreDisco[31];
		
	crearCabecera (&cabecera, IPCIRC_HANDSHAKE_DISCO, 0);
	enviarH(iSocket, cabecera);
	printf("Envio la primera cabecera\n");
	
	strcpy(cBufNombreDisco , stArchConf.cvNombreDisco);	
	crearCabecera(&cabecera, IPCIRC_HANDSHAKE_DISCO, strlen(cBufNombreDisco) + 1);
	enviarH(iSocket, cabecera);	
	printf("Envio la segunda cabecera\n");
	enviarAlgo(iSocket, (unsigned char *)cBufNombreDisco, cabecera.uiPayloadLength);
	
	/* NumToBin (ucBufCHS, stArchConf.stCHS.iCilinders, 0);
	NumToBin (ucBufCHS, stArchConf.stCHS.iHeaders, 4);
	NumToBin (ucBufCHS, stArchConf.stCHS.iSectors, 8);
	crearCabecera (&cabecera, IPCIRC_HANDSHAKE_DISCO, sizeof(ucBufCHS));
	enviarH(iSocket, cabecera);	
	enviarAlgo(iSocket, ucBufCHS, cabecera.uiPayloadLength); */
	
	recibirH(iSocket, &cabecera);
	
	printf("Recibo la ultima cabecera del handshake con el Array/VSK\n");
	
	switch(cabecera.ucPayloadDescriptor)
	{
		case IPCIRC_HANDSHAKE_OK: return 1; break;
		case IPCIRC_HANDSHAKE_FAIL: return 0; break;
	}
	
	return 0;
}

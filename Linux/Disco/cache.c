#include "cache.h"

int peticionCache(int iSocketCache, unsigned char *ucBufIdSector, unsigned char *ucBufSector)
{
	HEADERIRCIPC cabecera;
	
	crearCabecera (&cabecera, IPCIRC_CACHE_GET, sizeof(ucBufIdSector));
	
	enviarH(iSocketCache, cabecera);

	printf("DISCO @ Peticion cache: Entrando a enviarAlgo: idSector: %d\n", (int)BinToNum(ucBufIdSector, 0));
	
	enviarAlgo(iSocketCache, ucBufIdSector, cabecera.uiPayloadLength);

	printf("DISCO @ Peticion cache: Saliendo de enviarAlgo\n");
	
	recibirH(iSocketCache, &cabecera);
	
	switch(cabecera.ucPayloadDescriptor)
	{
		case IPCIRC_HITCACHE: recibirAlgo(iSocketCache, ucBufSector, cabecera.uiPayloadLength); break;
		case IPCIRC_MISSCACHE: return 0; break;		
	}
	
	return 1;
}

int ponerEnCache(int iSocketCache, unsigned char *ucBufIdSector, unsigned char *ucBufSector)
{
	HEADERIRCIPC cabecera;
	unsigned char ucBuffer[516];
	int i, j;
	
	for(i = 0, j = 0; i < 4; i++, j++)
	{
		ucBuffer[i] = ucBufIdSector[j];
	}
	for(j = 0; i < 516; j++, i++)
	{
		ucBuffer[i] = ucBufSector[j];
	}
	crearCabecera(&cabecera, IPCIRC_CACHE_PUT, sizeof(ucBuffer));
	
	printf("DISCO @ ponerEnCache: Entro a enviarH\n");
	enviarH(iSocketCache, cabecera);
	printf("DISCO @ ponerEnCache: Salgo de enviarH\n");
	
	enviarAlgo(iSocketCache, ucBuffer, cabecera.uiPayloadLength);
	printf("DISCO @ ponerEnCache: Salgo de enviarAlgo\n");
	
	return 1;
}

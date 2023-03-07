#include "primitivas.h"

int getSectores(int iSocket, HEADERIRCIPC cabecera, CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, int iHayCache, int iSocketCache, LDAP* ld)
{
	int iEstaEnCache, i, j;
	unsigned char ucBufIdSectores[cabecera.uiPayloadLength], ucBufIdSector1[4], ucBufIdSector2[4], ucBufSectores[1024],  ucBufSector1[512], 
	ucBufSector2[512];
	char cBufSector1[512];
	LDAPSECTOR sectores;
	int iIdSector1, iIdSector2;
	char cvIdSector1[50], cvIdSector2[50];
	struct timeval tvInicioGetSectores, tvFinGetSectores, tvInicioAccesoCache, tvFinAccesoCache;
	
	gettimeofday(&tvInicioGetSectores, NULL);
	
	recibirAlgo(iSocket, ucBufIdSectores, cabecera.uiPayloadLength);
	
	if(cabecera.ucPayloadDescriptor == IPCIRC_GET_SECTORES_2) /* Peticion de 2 sectores */
	{
	
		for (i = 0; i < 4; i++)
			ucBufIdSector1[i] = ucBufIdSectores[i];
		iIdSector1 = (int)BinToNum(ucBufIdSector1, 0);
		
		for (j = 0; j < 4; i++, j++)
			ucBufIdSector2[j] = ucBufIdSectores[i];
		iIdSector2 = (int)BinToNum(ucBufIdSector2, 0);
			
		if(iHayCache)
		{	
			gettimeofday(&tvInicioAccesoCache, NULL);
			iEstaEnCache = peticionCache(iSocketCache, ucBufIdSector1, ucBufSector1); /* cache.h */
			gettimeofday(&tvFinAccesoCache, NULL);
			stEstadisticas->uiTiempoAccesoACache += ((tvInicioAccesoCache.tv_sec - tvFinAccesoCache.tv_sec)*1000 + (tvInicioAccesoCache.tv_usec - tvFinAccesoCache.tv_usec)/1000);
			if(!iEstaEnCache)
			{
				stEstadisticas->uiCacheMiss++;
				/* A buscar en LDAP, ojo con el sector null, y al obtener cargamos en la cache el sector obtenido */
				miitoa(cvIdSector1,iIdSector1);
				cargarSector(&sectores, stArchConf.cvNombreDisco, cvIdSector1);
				ldapGetSector(ld, sectores, (char *)ucBufSector1);
				/* ponemos en cache el sector */
				ponerEnCache(iSocketCache, ucBufIdSector1, ucBufSector1);
				generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector1,0), 0);
			}
			else
			{
				stEstadisticas->uiCacheHit++;
				generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector1,0), 1);
			}
			gettimeofday(&tvInicioAccesoCache, NULL);
			iEstaEnCache = peticionCache(iSocketCache, ucBufIdSector2, ucBufSector2); /* cache.h */
			gettimeofday(&tvFinAccesoCache, NULL);
			stEstadisticas->uiTiempoAccesoACache += ((tvInicioAccesoCache.tv_sec - tvFinAccesoCache.tv_sec)*1000 + (tvInicioAccesoCache.tv_usec - tvFinAccesoCache.tv_usec)/1000);
			if(!iEstaEnCache)
			{
				stEstadisticas->uiCacheMiss++;
				/* A buscar en LDAP, ojo con el sector null, y al obtener cargamos en la cache el sector obtenido */
				miitoa(cvIdSector2,iIdSector2);
				cargarSector(&sectores, stArchConf.cvNombreDisco,cvIdSector2);
				ldapGetSector(ld, sectores, (char *)ucBufSector2);
				/* ponemos en cache el sector */
				ponerEnCache(iSocketCache, ucBufIdSector2, ucBufSector2);
				generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector2,0), 0);
			}
			else
			{
				stEstadisticas->uiCacheHit++;
				generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector2,0), 1);
			}
		}
		else
		{
			/* Buscas los dos sectores en LDAP */
			/*Buscamos el sector1 */
			miitoa(cvIdSector1,iIdSector1);
			
			cargarSector(&sectores, stArchConf.cvNombreDisco,cvIdSector1);
			ldapGetSector(ld, sectores, (char *)ucBufSector1);
			printf("Get sector 1: %s\n", ucBufSector1);			
			
			/* Buscamos el sector 2 */
			miitoa(cvIdSector2,iIdSector2);
			cargarSector(&sectores, stArchConf.cvNombreDisco,cvIdSector2);
			ldapGetSector(ld, sectores, (char *)ucBufSector2);
			printf("Get sector 2: %s\n", ucBufSector2);
			generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector1,0), 0);
			generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector2,0), 0);
		}
			
		cabecera.uiPayloadLength = 1024; /* 1 buffer solo de 512 bytes */
		cabecera.ucPayloadDescriptor = IPCIRC_GET_SECTORES_RTA;	
		printf("DISCO @ getSectores: Buffer1: %s\n", ucBufSector1);
		printf("DISCO @ getSectores: Buffer2: %s\n", ucBufSector2);
		enviarH(iSocket, cabecera);	
		enviarAlgo(iSocket, ucBufSector1, 512);
		enviarAlgo(iSocket, ucBufSector2, 512);
		
		gettimeofday(&tvFinGetSectores, NULL);
				
		stEstadisticas->tiempoUtilizadoEnTransferencias += ((tvInicioGetSectores.tv_sec - tvFinGetSectores.tv_sec)*1000 + (tvInicioGetSectores.tv_usec - tvFinGetSectores.tv_usec)/1000); 	
	}
	if(cabecera.ucPayloadDescriptor == IPCIRC_GET_SECTORES_1) /* Peticion de 1 sector */
	{
		for (i = 0; i < 4; i++)
			ucBufIdSector1[i] = ucBufIdSectores[i];
		iIdSector1 = (int)BinToNum(ucBufIdSector1, 0);
		for (i = 0; i < 511; i++)
			ucBufSector2[i] = '\0';
		
		if(iHayCache)
		{
			gettimeofday(&tvInicioAccesoCache, NULL);
			iEstaEnCache = peticionCache(iSocketCache, ucBufIdSectores, ucBufSector1); /* cache.h */
			gettimeofday(&tvFinAccesoCache, NULL);
			stEstadisticas->uiTiempoAccesoACache += ((tvInicioAccesoCache.tv_sec - tvFinAccesoCache.tv_sec)*1000 + (tvInicioAccesoCache.tv_usec - tvFinAccesoCache.tv_usec)/1000);
			if(!iEstaEnCache)
			{
				stEstadisticas->uiCacheMiss++;
				/* A buscar en LDAP, ojo con el sector null */
				miitoa(cvIdSector1,iIdSector1);
				cargarSector(&sectores, stArchConf.cvNombreDisco,cvIdSector1);
				ldapGetSector(ld, sectores, (char *)ucBufSector1);
				/* ponemos en cache el sector */
				ponerEnCache(iSocketCache, ucBufIdSector1, ucBufSector1);
				generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector1,0), 0);
			}
			else
			{
				stEstadisticas->uiCacheHit++;
				generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector1,0), 1);
			}
						
			/*Juntas los dos buffersectores de 512 en uno de 1024 (ubBufSectores) poniendo los 512 bytes finales con '\0' */
		}
		else
		{
			/* Buscas el sector */
			miitoa(cvIdSector1,iIdSector1);
			cargarSector(&sectores, stArchConf.cvNombreDisco,cvIdSector1);
			ldapGetSector(ld, sectores, (char *)ucBufSector1);
			generarEstadisticasGetSectores(stArchConf, stEstadisticas, BinToNum(ucBufIdSector1,0), 0);
		}
			
		cabecera.uiPayloadLength = 512; /* 2 buffers de 512 bytes. El ultimo con '\0'*/
		cabecera.ucPayloadDescriptor = IPCIRC_GET_SECTORES_1_RTA;	

		enviarH(iSocket, cabecera);	
		enviarAlgo(iSocket, ucBufSector1, 512);
		/*enviarAlgo(iSocket, ucBufSector2, 512); */
		
		gettimeofday(&tvFinGetSectores, NULL);
				
		stEstadisticas->tiempoUtilizadoEnTransferencias += ((tvInicioGetSectores.tv_sec - tvFinGetSectores.tv_sec)*1000 + (tvInicioGetSectores.tv_usec - tvFinGetSectores.tv_usec)/1000); 			
	}
	if(cabecera.ucPayloadDescriptor == IPCIRC_OBTENER_SECTOR_DEPASO) /* Peticion de consola */
	{		
		if(iHayCache)
		{
			iEstaEnCache = peticionCache(iSocketCache, ucBufIdSectores, ucBufSector1); /* cache.h */
			if(!iEstaEnCache)
			{
				stEstadisticas->uiCacheMiss++;
				miitoa (cBufSector1, BinToNum(ucBufIdSectores, 0));
				cabecera.uiPayloadLength = strlen(cBufSector1) + 1;
				enviarH(iSocket, cabecera);
				enviarAlgo(iSocket, (unsigned char *)cBufSector1, cabecera.uiPayloadLength);			
			}
			else
			{
				stEstadisticas->uiCacheHit++;
				sprintf(cBufSector1, "%d - En Cache", BinToNum(ucBufIdSectores, 0));
				cabecera.uiPayloadLength = strlen(cBufSector1) + 1;
				enviarH(iSocket, cabecera);
				enviarAlgo(iSocket, (unsigned char *)cBufSector1, cabecera.uiPayloadLength);	
			}
		}
		else
		{
			sprintf(cBufSector1, "%d", BinToNum(ucBufIdSectores, 0));
			cabecera.uiPayloadLength = strlen(cBufSector1) + 1;
			enviarH(iSocket, cabecera);
			enviarAlgo(iSocket, (unsigned char *)cBufSector1, cabecera.uiPayloadLength);			
		}
	}
	if(cabecera.ucPayloadDescriptor == IPCIRC_OBTENER_SECTOR_DEFINITIVO) /* Peticion de consola */
	{	

		for (i = 0; i < 4; i++)
			ucBufIdSector1[i] = ucBufIdSectores[i];
		iIdSector1 = (int)BinToNum(ucBufIdSector1, 0);
		
		if(iHayCache)
		{
			iEstaEnCache = peticionCache(iSocketCache, ucBufIdSectores, ucBufSector1); /* cache.h */
			if(!iEstaEnCache)
			{
				stEstadisticas->uiCacheMiss++;
				/* A buscar en LDAP, ojo con el sector null */
				miitoa(cvIdSector1,iIdSector1);
				cargarSector(&sectores, stArchConf.cvNombreDisco,cvIdSector1);
				ldapGetSector(ld, sectores, (char *)ucBufSector1);

				/* ponemos en cache el sector */
				ponerEnCache(iSocketCache, ucBufIdSector1, ucBufSector1);
			}
			else
			{
				stEstadisticas->uiCacheHit++;
			}
						
		}
		else
		{
			/* Buscas el sector */
			miitoa(cvIdSector1,iIdSector1);
			cargarSector(&sectores, stArchConf.cvNombreDisco, cvIdSector1);
			ldapGetSector(ld, sectores, (char *)ucBufSector1);
			
		}	
		cabecera.uiPayloadLength = 512; /* 1 buffer solo de 512 bytes */
		cabecera.ucPayloadDescriptor = IPCIRC_GET_SECTORES_RTA;
		enviarH(iSocket, cabecera);	
		enviarAlgo(iSocket, ucBufSector1, 512);		
	}
	
	return 1;
}

int putSectores(int iSocket, HEADERIRCIPC cabecera, CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, LDAP *ld)
{
	unsigned char ucBuf[cabecera.uiPayloadLength];
	int iIdSector1, iIdSector2;
	unsigned char ucBufSector1[513], ucBufSector2[513];	
	int iRecorreBuffer, i, j;
	char cvMensajeError[100];
	char cvIdSector[50];
	LDAPMODIFICARSECTOR ldap_modificarSector;
	int iReturn;
	struct timeval tvInicioPutSectores, tvFinPutSectores;

	for(i = 0; i < 513; i++)
	{
		ucBufSector1[i] = '\0';
		ucBufSector2[i] = '\0';
	}

	gettimeofday(&tvInicioPutSectores, NULL);

	recibirAlgo(iSocket, ucBuf, cabecera.uiPayloadLength);
	
	if(cabecera.ucPayloadDescriptor == IPCIRC_PUT_SECTORES_2)
	{
		for(iRecorreBuffer=0, i=0, j=0; iRecorreBuffer < 1032; iRecorreBuffer++)
		{
			if(iRecorreBuffer == 0)
			{			
				iIdSector1 = BinToNum (ucBuf, iRecorreBuffer);			
				iRecorreBuffer = 4;
			}
			if((iRecorreBuffer >= 4) && (iRecorreBuffer < 516))
			{
				ucBufSector1[i] = ucBuf[iRecorreBuffer];
				i++;
			}			
			if(iRecorreBuffer == 516)
			{
				iIdSector2 = BinToNum (ucBuf, iRecorreBuffer);	
				iRecorreBuffer = 520;
			}
			if((iRecorreBuffer >= 520) && (iRecorreBuffer < 1032))
			{
				ucBufSector2[j] = ucBuf[iRecorreBuffer];
				j++;
			}			
		}
	
		/* Vamos a LDAP con IP, Puerto, Password, Los Ids de los sectores, y los sectores , y metemos los sectores */
		/*Meto el primer sector */
		miitoa(cvIdSector, iIdSector1);
		cargarLDAPMODIFICARSECTOR(&ldap_modificarSector, stArchConf.cvNombreDisco, cvIdSector, (char *)ucBufSector1);
		ldapModificarSector(ld, ldap_modificarSector);

		/* Meto el segundo sector */
		miitoa(cvIdSector, iIdSector2);
		cargarLDAPMODIFICARSECTOR(&ldap_modificarSector, stArchConf.cvNombreDisco, cvIdSector, (char *)ucBufSector2);
		iReturn = ldapModificarSector(ld, ldap_modificarSector);
		
		if(iReturn) 	/* Si salio todo bien */
		{
			cabecera.ucPayloadDescriptor = IPCIRC_PUT_SECTORES_RTA_OK;
			cabecera.uiPayloadLength = 0;	
			enviarH(iSocket, cabecera);
			generarEstadisticasPutSectores(stArchConf, stEstadisticas, iIdSector1);
			generarEstadisticasPutSectores(stArchConf, stEstadisticas, iIdSector2);
		}
		
		else /* Si salio algo mal */
		{
			sprintf(cvMensajeError, "Algo salió mal al intentar grabar el sector: ");
			escribirLog('I', cvMensajeError);
			cabecera.ucPayloadDescriptor = IPCIRC_PUT_SECTORES_RTA_FAIL;
			cabecera.uiPayloadLength = strlen(cvMensajeError) + 1;	
			enviarH(iSocket, cabecera);
			enviarAlgo(iSocket, (unsigned char*)cvMensajeError, cabecera.uiPayloadLength);
		}

	
		gettimeofday(&tvFinPutSectores, NULL);	
	
		stEstadisticas->tiempoUtilizadoEnTransferencias += ((tvInicioPutSectores.tv_sec - tvFinPutSectores.tv_sec)*1000 + (tvInicioPutSectores.tv_usec - tvFinPutSectores.tv_usec)/1000); 	
	}

	if(cabecera.ucPayloadDescriptor == IPCIRC_PUT_SECTORES_1)
	{
		for(iRecorreBuffer=0, i=0, j=0; iRecorreBuffer < 1032; iRecorreBuffer++)
		{
			if(iRecorreBuffer == 0)
			{			
				iIdSector1 = BinToNum (ucBuf, iRecorreBuffer);			
				iRecorreBuffer = 4;
			}
			if((iRecorreBuffer >= 4) && (iRecorreBuffer < 516))
			{
				ucBufSector1[i] = ucBuf[iRecorreBuffer];
				i++;
			}			
			if(iRecorreBuffer == 516)
			{
				iIdSector2 = BinToNum (ucBuf, iRecorreBuffer);	
				iRecorreBuffer = 520;
			}
			if((iRecorreBuffer >= 520) && (iRecorreBuffer < 1032))
			{
				ucBufSector2[j] = ucBuf[iRecorreBuffer];
				j++;
			}			
		}
	
		/* Vamos a LDAP con IP, Puerto, Password, Los Ids de los sectores, y los sectores , y metemos los sectores */
		/*Meto el primer sector */
		miitoa(cvIdSector, iIdSector1);
		cargarLDAPMODIFICARSECTOR(&ldap_modificarSector, stArchConf.cvNombreDisco, cvIdSector, (char *)ucBufSector1);
		iReturn = ldapModificarSector(ld, ldap_modificarSector);
		
		if(iReturn) 	/* Si salio todo bien */
		{
			cabecera.ucPayloadDescriptor = IPCIRC_PUT_SECTORES_RTA_OK;
			cabecera.uiPayloadLength = 0;	
			enviarH(iSocket, cabecera);
			generarEstadisticasPutSectores(stArchConf, stEstadisticas, iIdSector1);			
		}
		
		else /* Si salio algo mal */
		{
			sprintf(cvMensajeError, "Algo salio mal al intentar grabar el sector");
			escribirLog('I', cvMensajeError);
			cabecera.ucPayloadDescriptor = IPCIRC_PUT_SECTORES_RTA_FAIL;
			cabecera.uiPayloadLength = strlen(cvMensajeError) + 1;	
			enviarH(iSocket, cabecera);
			enviarAlgo(iSocket, (unsigned char*)cvMensajeError, cabecera.uiPayloadLength);
		}
	
		gettimeofday(&tvFinPutSectores, NULL);	
	
		stEstadisticas->tiempoUtilizadoEnTransferencias += ((tvInicioPutSectores.tv_sec - tvFinPutSectores.tv_sec)*1000 + (tvInicioPutSectores.tv_usec - tvFinPutSectores.tv_usec)/1000);  

	}
	
	if(cabecera.ucPayloadDescriptor == IPCIRC_ESCRIBIR_SECTOR)
	{
		for(iRecorreBuffer=0, i=0, j=0; iRecorreBuffer < 516; iRecorreBuffer++)
		{
			if(iRecorreBuffer == 0)
			{			
				iIdSector1 = BinToNum (ucBuf, iRecorreBuffer);			
				iRecorreBuffer = 4;
			}
			if((iRecorreBuffer >= 4) && (iRecorreBuffer < 516))
			{
				ucBufSector1[i] = ucBuf[iRecorreBuffer];
				i++;
			}
		}
			
		/* Vamos a LDAP con IP, Puerto, Password, el Id del sector, y el sector, y lo metemos */

		miitoa(cvIdSector, iIdSector1);
		cargarLDAPMODIFICARSECTOR(&ldap_modificarSector, stArchConf.cvNombreDisco, cvIdSector, (char *)ucBufSector1);
		ldapModificarSector(ld, ldap_modificarSector);
		
		generarEstadisticasPutSectores(stArchConf, stEstadisticas, iIdSector1);
		sprintf(cvMensajeError, "Algo salió mal al intentar grabar el sector \" \" mientras se ejecutaba el comando escribirSector");
		escribirLog('I', cvMensajeError);
	}

	return 1;
}

int getCHS(int iSocket, HEADERIRCIPC cabecera, CONFIGURACION stArchConf)
{
	unsigned char ucBufCHS[12];
	
	NumToBin (ucBufCHS, stArchConf.stCHS.iCilinders, 0);
	NumToBin (ucBufCHS, stArchConf.stCHS.iHeaders, 4);
	NumToBin (ucBufCHS, stArchConf.stCHS.iSectors, 8);	
	cabecera.ucPayloadDescriptor = IPCIRC_GET_CHS_RTA;
	cabecera.uiPayloadLength = sizeof(ucBufCHS);	
	enviarH(iSocket, cabecera);	
	enviarAlgo(iSocket, ucBufCHS, cabecera.uiPayloadLength);
		
	return 1;
}

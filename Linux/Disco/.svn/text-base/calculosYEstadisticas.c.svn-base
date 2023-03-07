#include "calculosYEstadisticas.h"

int generarEstadisticas(CONFIGURACION stArchConf, ESTADISTICAS stEstadisticas)
{
	long lCapDiscoBytes;
	int iCantSectores;
	char cvCantSectores[15];
	char cvMensajeLog[100] = "\0";
	char cvCapDisco[20];
	char cvUnidad[10];
	char cvTMedioBusquedaEntero[35];
	char cvTMedioBusquedaDecimal[3];
	char cvPeorTiempoBusSectorEntero[21];
	char cvPeorTiempoBusSectorDecimal[21];
	char cvTasaTransf[35];
	char cvTiempoMedioCacheEntero[35];
	char cvTiempoMedioCacheDecimal[3];
	unsigned int uiAuxDecimal;
	unsigned int uiAuxEntero;
	
	escribirLog('I', "***CALCULOS Y ESTADISTICAS DEL DISCO***");
	
	/* Tiempo medio y peor tiempo de busqueda de un sector */
	if(stEstadisticas.cantSectoresBuscados == 0)
	{
		escribirLog('I', "Tiempo Medio de busqueda de un sector: 0 ms");
		escribirLog('I', "Peor tiempo de busqueda de un sector: 0 ms");
	}
	else
	{	
		uiAuxEntero = stEstadisticas.tiempoUtilizadoEnBusquedas / stEstadisticas.cantSectoresBuscados;
		uiAuxDecimal = stEstadisticas.tiempoUtilizadoEnBusquedas*100;
		uiAuxDecimal = uiAuxDecimal / stEstadisticas.cantSectoresBuscados;
		uiAuxDecimal = uiAuxDecimal % 100;
				
		miitoa(cvTMedioBusquedaEntero, uiAuxEntero);
		miitoa(cvTMedioBusquedaDecimal, uiAuxDecimal);
		sprintf(cvMensajeLog, "Tiempo Medio de busqueda de un sector: %s.%s ms", cvTMedioBusquedaEntero, cvTMedioBusquedaDecimal);
		escribirLog('I', cvMensajeLog);
		uiAuxEntero = stEstadisticas.peorTiempoSector / 10;
		uiAuxDecimal = stEstadisticas.tiempoUtilizadoEnBusquedas % 10;
		miitoa(cvPeorTiempoBusSectorEntero, uiAuxEntero);
		miitoa(cvPeorTiempoBusSectorDecimal, uiAuxDecimal);		
		sprintf(cvMensajeLog, "Peor tiempo de busqueda de un sector: %s.%s ms", cvPeorTiempoBusSectorEntero, cvPeorTiempoBusSectorDecimal);
	}	
	
	/* Tasa de transferencia */
	
	if(stEstadisticas.tiempoUtilizadoEnTransferencias == 0)
		escribirLog('I', "Tasa de transferencia: 0 KB/Seg");
	else
	{	
		TransformarUnidad(((stEstadisticas.totalBytesTransferidos*1000) / stEstadisticas.tiempoUtilizadoEnTransferencias), cvTasaTransf, cvUnidad);
		sprintf(cvMensajeLog, "Tasa de transferencia: %s%s/seg", cvTasaTransf, cvUnidad);
		escribirLog('I', cvMensajeLog);
	}	
	
	/*Capacidad total del disco y Cantidad de sectores */
	
	lCapDiscoBytes = stArchConf.stCHS.iCilinders * stArchConf.stCHS.iSectors * 512;
	TransformarUnidad(lCapDiscoBytes, cvCapDisco, cvUnidad);		
	
	sprintf(cvMensajeLog,"Capacidad del disco: %s%s", cvCapDisco, cvUnidad);	
	escribirLog('I', cvMensajeLog);
	
	iCantSectores = stArchConf.stCHS.iCilinders * stArchConf.stCHS.iSectors;
	miitoa (cvCantSectores, iCantSectores);
	sprintf(cvMensajeLog, "Cantidad total de sectores: %s", cvCantSectores);
	escribirLog('I', cvMensajeLog);
	
	/* Tiempo medio de acceso a cache */
	
	if(stEstadisticas.tiempoUtilizadoAccesoACache == 0)
		escribirLog('I', "Tiempo medio de acceso a cache: 0 Seg");
	else
	{	
		uiAuxEntero = stEstadisticas.tiempoUtilizadoAccesoACache / stEstadisticas.cantAccesosACache;
		uiAuxDecimal = stEstadisticas.tiempoUtilizadoAccesoACache*100;
		uiAuxDecimal = uiAuxDecimal / stEstadisticas.cantAccesosACache;
		uiAuxDecimal = uiAuxDecimal % 100;
		
		miitoa(cvTiempoMedioCacheEntero, uiAuxEntero);
		miitoa(cvTiempoMedioCacheDecimal, uiAuxDecimal);
		sprintf(cvMensajeLog, "Tiempo medio de acceso a cache: %s.%s Seg", cvTiempoMedioCacheEntero, cvTiempoMedioCacheDecimal);
		escribirLog('I', cvMensajeLog);
	}
	
	escribirLog('I', "***FIN DE CALCULOS Y ESTADISTICAS DEL DISCO***");
	
	return 1;
}

int generarEstadisticasGetSectores(CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, int iIdSector, char cEstabaEnCache)
{
	char cvMensajeLog[256];
	unsigned int imDisco[stArchConf.stCHS.iCilinders][stArchConf.stCHS.iSectors];
	char cvSector[20];
	int iCantSectoresLeidos = 0; 
	int i, j, k;
	unsigned int uiAuxEntero, uiAuxDecimal, uiTiempoEntero, uiTiempoDecimal;
	
		
	/* Inicializamos la matriz que representa al disco */

	for(i = 0, j = 0, k = 0; (i < stArchConf.stCHS.iCilinders) && (k < (stArchConf.stCHS.iSectors * stArchConf.stCHS.iCilinders));)
	{
		if(j < (stArchConf.stCHS.iSectors - 1)) 
		{
			imDisco[i][j] = k;
			k++;
			j++;					 	
		}
		else if(j < stArchConf.stCHS.iSectors) /* Quiere decir que esta en el ultimo sector del cilindro */
			{
				imDisco[i][j] = k; /* Ponemos el numero de sector en el ultimo sector del cilindro */
				k++;
				i++; /* pasamos al siguiente cilindro */
				j = 0; /* Empezamos desde la posicion cero del siguiente cilindro */
			} else 
			  {
				printf("Te pasaste negra\n");					
			  }			 
	}
	
	/* */	
		
	if(cEstabaEnCache)
	{
		escribirLog('I', "***CALCULOS GETSECTORES***");
		sprintf(cvMensajeLog, "Sector deseado: %d", iIdSector);
		escribirLog('I', cvMensajeLog);	
		sprintf(cvMensajeLog, "Posicion Actual del cabezal: %d", stEstadisticas->uiPosCabezal);
		escribirLog('I', cvMensajeLog);	
		escribirLog('I', "Sectores que tendria que atravesar: Ninguno");
		escribirLog('I', "Tiempo que tardaria en servir dicho requerimiento un disco rigido real: tiempoCache (en ms o en ns?)");
		uiAuxEntero = stEstadisticas->uiCacheHit / (stEstadisticas->uiCacheHit+stEstadisticas->uiCacheMiss);
		uiAuxDecimal = stEstadisticas->uiCacheHit*100;
		uiAuxDecimal = uiAuxDecimal / (stEstadisticas->uiCacheHit+stEstadisticas->uiCacheMiss);
		uiAuxDecimal = uiAuxDecimal % 100;
		sprintf(cvMensajeLog, "Promedio cache hit: %d.%d", uiAuxEntero, uiAuxDecimal);
		escribirLog('I', cvMensajeLog);				
		uiAuxEntero = stEstadisticas->uiCacheMiss / (stEstadisticas->uiCacheHit+stEstadisticas->uiCacheMiss);
		uiAuxDecimal = stEstadisticas->uiCacheMiss*100;
		uiAuxDecimal = uiAuxDecimal / (stEstadisticas->uiCacheHit+stEstadisticas->uiCacheMiss);
		uiAuxDecimal = uiAuxDecimal % 100;
		sprintf(cvMensajeLog, "Promedio cache miss: %d.%d", uiAuxEntero, uiAuxDecimal);		
		escribirLog('I', cvMensajeLog);
		escribirLog('I', "***FIN DE GETSECTORES***");
	}
	else
	{		
		escribirLog('I', "***CALCULOS GETSECTORES***");	
		sprintf(cvMensajeLog, "Sector deseado: %d", iIdSector);
		escribirLog('I', cvMensajeLog);
		sprintf(cvMensajeLog, "Posicion Actual del cabezal: %d", stEstadisticas->uiPosCabezal);
		escribirLog('I', cvMensajeLog);
		cvMensajeLog[0] = '\0';
		strcat(cvMensajeLog, "Sectores que tendria que atravesar: ");	
		
		/* Te posicionas en donde estas actualmente */
		for(i=0, j=0; (i < (stArchConf.stCHS.iCilinders - 1)) && (imDisco[i][j] != stEstadisticas->uiPosCabezal);)
		{
			if(j < (stArchConf.stCHS.iSectors - 1))
				j++;
			else
			{
				i++;
				j = 0;
			}
		}
		
		/* empezas a caminar por el disco */		

		while(imDisco[i][j] != iIdSector)
		{
			if((iIdSector >= ((i+1)*stArchConf.stCHS.iSectors - stArchConf.stCHS.iSectors)) && (iIdSector <= ((i+1)*stArchConf.stCHS.iSectors - 1)))
			{							
				while(imDisco[i][j] != iIdSector)
				{
					/* grabo el sector que lei*/
					miitoa(cvSector, imDisco[i][j]);
					strcat(cvMensajeLog, cvSector);
					strcat(cvMensajeLog, ", ");	
					iCantSectoresLeidos++;							
					if(j < (stArchConf.stCHS.iSectors - 1))			
						j++;
					else
						j = 0;
				}									
			}
			else
			{
				/* Anotas el sector actual como si lo leyera */
				miitoa(cvSector, imDisco[i][j]);
				strcat(cvMensajeLog, cvSector);
				strcat(cvMensajeLog, ", ");	
				iCantSectoresLeidos++;
				if(i < (stArchConf.stCHS.iCilinders - 1))		
					i++;
				else
					i = 0;
				if(j < (stArchConf.stCHS.iSectors - 1))
					j++;
				else 
					j = 0;
			}
		}
		iCantSectoresLeidos++;
		/* Movemos un sector mas el disco porque acaba de leerlo */
		if(j < (stArchConf.stCHS.iSectors - 1))
			j++;
		else 
			j = 0;
		miitoa(cvSector, iIdSector);
		strcat(cvMensajeLog, cvSector);
		escribirLog('I',cvMensajeLog);
		uiTiempoEntero = (iCantSectoresLeidos*(stEstadisticas->tiempoLectoEscrituraSector/10));
		uiTiempoDecimal = ((iCantSectoresLeidos*stEstadisticas->tiempoLectoEscrituraSector) % 10);
		sprintf(cvMensajeLog, "Tiempo que tardaria en servir dicho requerimiento un disco rigido real: %d.%d ms", uiTiempoEntero, uiTiempoDecimal);
		
		if((stEstadisticas->uiCacheHit + stEstadisticas->uiCacheMiss) == 0)
		{
			sprintf(cvMensajeLog, "Promedio cache hit: 0");
		}
		else
		{
			uiAuxEntero = stEstadisticas->uiCacheHit / (stEstadisticas->uiCacheHit+stEstadisticas->uiCacheMiss);
			uiAuxDecimal = stEstadisticas->uiCacheHit*100;
			uiAuxDecimal = uiAuxDecimal / (stEstadisticas->uiCacheHit+stEstadisticas->uiCacheMiss);
			uiAuxDecimal = uiAuxDecimal % 100;
			sprintf(cvMensajeLog, "Promedio cache hit: %d.%d", uiAuxEntero, uiAuxDecimal);
		}
		escribirLog('I', cvMensajeLog);				
		if((stEstadisticas->uiCacheHit + stEstadisticas->uiCacheMiss) == 0)
		{
			sprintf(cvMensajeLog, "Promedio cache miss: 0");
		}
		else
		{
			uiAuxEntero = stEstadisticas->uiCacheMiss / (stEstadisticas->uiCacheHit+stEstadisticas->uiCacheMiss);
			uiAuxDecimal = stEstadisticas->uiCacheMiss*100;
			uiAuxDecimal = uiAuxDecimal / (stEstadisticas->uiCacheHit+stEstadisticas->uiCacheMiss);
			uiAuxDecimal = uiAuxDecimal % 100;
			sprintf(cvMensajeLog, "Promedio cache miss: %d.%d", uiAuxEntero, uiAuxDecimal);
		}		
		escribirLog('I', cvMensajeLog);
		escribirLog('I', "***FIN DE GETSECTORES***");
		/* Modificamos el pero Tiempo de Busqueda de un Sector si es necesario */

		if(stEstadisticas->peorTiempoSector < (iCantSectoresLeidos*stEstadisticas->tiempoLectoEscrituraSector + stEstadisticas->uiTiempoAccesoACache))
			stEstadisticas->peorTiempoSector = (iCantSectoresLeidos*stEstadisticas->tiempoLectoEscrituraSector + stEstadisticas->uiTiempoAccesoACache);

		stEstadisticas->tiempoUtilizadoEnBusquedas += (iCantSectoresLeidos*stEstadisticas->tiempoLectoEscrituraSector + stEstadisticas->uiTiempoAccesoACache);
		stEstadisticas->cantSectoresBuscados += 1;
		stEstadisticas->totalBytesTransferidos += 512;
		stEstadisticas->uiPosCabezal = imDisco[i][j]; 
		/***********************************************************************/		
	}	
	return 1;
}
int generarEstadisticasPutSectores(CONFIGURACION stArchConf, ESTADISTICAS *stEstadisticas, int iIdSector)
{
	char cvMensajeLog[200];
	unsigned int imDisco[stArchConf.stCHS.iCilinders][stArchConf.stCHS.iSectors];
	char cvSector[20];
	int iCantSectoresLeidos = 0; 
	int i, j, k;
	unsigned int uiTiempoEntero, uiTiempoDecimal;	
		
	/* Inicializamos la matriz que representa al disco */

	for(i = 0, j = 0, k = 0; (i < stArchConf.stCHS.iCilinders) && (k < (stArchConf.stCHS.iSectors * stArchConf.stCHS.iCilinders));)
	{
		if(j < (stArchConf.stCHS.iSectors - 1)) 
		{
			imDisco[i][j] = k;
			k++;
			j++;					 	
		}
		else if(j < stArchConf.stCHS.iSectors) /* Quiere decir que esta en el ultimo sector del cilindro */
			{
				imDisco[i][j] = k; /* Ponemos el numero de sector en el ultimo sector del cilindro */
				k++;
				i++; /* pasamos al siguiente cilindro */
				j = 0; /* Empezamos desde la posicion cero del siguiente cilindro */
			} else 
			  {
				printf("Te pasaste negra\n");					
			  }			 
	}
	
	/* */
	
	escribirLog('I', "***CALCULOS PUTSECTORES***");	
	sprintf(cvMensajeLog, "Sector deseado: %d", iIdSector);
	escribirLog('I', cvMensajeLog);
	sprintf(cvMensajeLog, "Posicion actual del cabezal: %d", stEstadisticas->uiPosCabezal);
	escribirLog('I', cvMensajeLog);
	cvMensajeLog[0] = '\0';
	strcat(cvMensajeLog, "Sectores que tendria que atravesar: ");
	
	/* Te posicionas en donde estas actualmente */
	for(i=0, j=0; (i < (stArchConf.stCHS.iCilinders - 1)) && (imDisco[i][j] != stEstadisticas->uiPosCabezal);)
	{
		if(j < (stArchConf.stCHS.iSectors - 1))
			j++;
		else
		{
			i++;
			j = 0;
		}
	}
	
	/* Caminas por el disco */
	while(imDisco[i][j] != iIdSector)
	{
		if((iIdSector >= ((i+1)*stArchConf.stCHS.iSectors - stArchConf.stCHS.iSectors)) && (iIdSector <= ((i+1)*stArchConf.stCHS.iSectors - 1)))
		{							
			while(imDisco[i][j] != iIdSector)
			{
				/* grabo el sector que lei*/
				miitoa(cvSector, imDisco[i][j]);
				strcat(cvMensajeLog, cvSector);
				strcat(cvMensajeLog, ", ");	
				iCantSectoresLeidos++;							
				if(j < (stArchConf.stCHS.iSectors - 1))			
					j++;
				else
					j = 0;
			}									
		}
		else
		{
			/* Anotas el sector actual como si lo leyera */
			miitoa(cvSector, imDisco[i][j]);
			strcat(cvMensajeLog, cvSector);
			strcat(cvMensajeLog, ", ");							
			iCantSectoresLeidos++;
			if(i < (stArchConf.stCHS.iCilinders - 1))		
				i++;
			else
				i = 0;
			if(j < (stArchConf.stCHS.iSectors - 1))
				j++;
			else 
				j = 0;
		}
	}	
	iCantSectoresLeidos++;
	if(j < (stArchConf.stCHS.iSectors - 1))
		j++;
	else 
		j = 0;
	miitoa(cvSector, iIdSector);
	strcat(cvMensajeLog, cvSector);
	escribirLog('I',cvMensajeLog);
	uiTiempoEntero = (iCantSectoresLeidos*(stEstadisticas->tiempoLectoEscrituraSector/10));
	uiTiempoDecimal = ((iCantSectoresLeidos*stEstadisticas->tiempoLectoEscrituraSector) % 10);
	sprintf(cvMensajeLog, "Tiempo que tardo en escribir dicho sector: %d.%d ms", uiTiempoEntero, uiTiempoDecimal);
	escribirLog('I',cvMensajeLog);
	escribirLog('I', "***FIN DE PUTSECTORES***");

	/* Hago cuentas estadisticas */
	
	stEstadisticas->totalBytesTransferidos += 512;
	stEstadisticas->uiPosCabezal = imDisco[i][j];

	/****************************/
	
	return 1;
}
void TransformarUnidad(long valorEnBytes, char* numeroReducido, char* unidad)
{
	unsigned long long valorEnBytesAux=0;
	unsigned char entero[10];
	unsigned char decimal[4];
	
	valorEnBytesAux=valorEnBytes*100;
	
	if (valorEnBytes < 1024)
	{
		sprintf(unidad,"%s", " Bytes");
		miitoa(numeroReducido, valorEnBytes);
		return;
	}

	valorEnBytes = valorEnBytes/1024;
	valorEnBytesAux = valorEnBytesAux/1024;
	if (valorEnBytes < 1024)
	{
		sprintf(unidad,"%s", " KB");
		valorEnBytesAux =valorEnBytesAux % 100;
		miitoa2 (entero, valorEnBytes);
		miitoa2 (decimal, valorEnBytesAux);
		sprintf(numeroReducido,"%s.%s",entero,decimal);
		return;
	}
	valorEnBytes = valorEnBytes/1024;
	valorEnBytesAux = valorEnBytesAux/1024;
	if (valorEnBytes < 1024)
	{
		sprintf(unidad,"%s", " MB");
		valorEnBytesAux =valorEnBytesAux % 100;
		miitoa2 (entero, valorEnBytes);
		miitoa2 (decimal, valorEnBytesAux);
		sprintf(numeroReducido,"%s.%s",entero,decimal);
		return;
	}
	valorEnBytes = valorEnBytes/1024;
	valorEnBytesAux = valorEnBytesAux/1024;
	if (valorEnBytes < 1024)
	{
		sprintf(unidad,"%s", " GB");
		valorEnBytesAux =valorEnBytesAux % 100;
		miitoa2 (entero, valorEnBytes);
		miitoa2 (decimal, valorEnBytesAux);
		sprintf(numeroReducido,"%s.%s",entero,decimal);
		return;
	}
	
}

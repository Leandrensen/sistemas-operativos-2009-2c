#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <libmemcached/memcached.h>
#include "sendrecv.h"
#include "archivos.h"
#include "cadenas.h"
#include "defines.h"

int main(void)
{
	fd_set masterFds;   	/* conjunto fd maestro para funcion select() */
	fd_set leeFds; 			/* conjunto fd temporal para funcion select() */
    struct timeval tv; 		/* estructura que define el tiempo que espera select por E/S, para q no sea bloqueante */
	int iServidor, fdMax, i, iSocket_aceptado;
	int iCliente;
	srand((unsigned) time(NULL));
	CONFIGURACION stArchConf;
	HEADERIRCIPC cabecera;
	unsigned char ucBufIdSector[4];
	char cBufIdSectorKey[256]; /* Buffer en el que vamos a crear la key para almacenar los sectores.
										   * la key va a ser el id de sector pasado a string */
	int iSocket;
	int iSectorId;
  	uint32_t flags;  
  	char *cpSector = (char *)malloc(sizeof(char)*512);
  	unsigned char ucBufSector[512];
  	int j, iRecorreBuffer;
  	unsigned char ucBuf[516];
  	
  	memcached_st *memc;
    	memcached_return rc;		
    	memcached_server_st *servers;
	size_t stTamanioSector = 512;
	size_t key_length; 

	unsigned char ucBufBasura[500];  
   	
   	/****************************** CARGAMOS ARCHIVO DE CONFIGURACION Y ABRIMOS LOG **************/
   	
	cargarConfiguracion(&stArchConf); /* archivos.h */
	crearLog(); /* archivos.h */
	
	/***************************** NOS CONECTAMOS A LOS SERVIDORES MEMCACHED *******************/
	
    memc = memcached_create(NULL);
    if(memc == NULL)
    {
   	  escribirLog('E', "Error en memcached_create()");
   	  return -1;
    }

    /* Agregamos los 2 servers a la lista de servers */	
    servers = memcached_server_list_append(NULL, stArchConf.cvIP1, stArchConf.iPuerto1, &rc);
    if(servers == NULL)
    {
    	escribirLog('E', "memcached_server_list_append");
    }  	
    servers = memcached_server_list_append(servers, stArchConf.cvIP2, stArchConf.iPuerto2, &rc);
    if(servers == NULL)
    {
    	escribirLog('E', "memcached_server_list_append");
    }	
	
    /* Metemos la lista de servers dentro de la estructura memcached_st */	
    rc = memcached_server_push(memc, servers);	

	
	/****************************** CONEXION COMO CLIENTE **************************************/
	iCliente = iniciarCliente(stArchConf.cvIPDisco, stArchConf.iPuertoDisco);
	
	switch (iCliente)
	{
		case SOCKET_INVALIDO: escribirLog ('E', "Socket invalido"); return 0; break;
		case SOCKET_ERROR_CONEXION: escribirLog ('E', "Socket error de conexion"); return 0; break;
	}

	crearCabecera(&cabecera, IPCIRC_HANDSHAKE_CACHE, 0);
 	enviarH(iCliente, cabecera);

	/*******************************************************************************************/

	printf ("Proceso cache iniciando exitosamente\n");
	escribirLog ('I', "Proceso cache iniciando exitosamente");

	FD_ZERO(&masterFds);     /* borramos los dos conjuntos */
	FD_ZERO(&leeFds);
	FD_SET(iCliente, &masterFds);
	fdMax=iCliente;
	
	while(1) 
	{
		leeFds = masterFds; /* Lo copiamos porque la funcion select lo va a modificar interiormente */
		tv.tv_sec = 0; 		/* cant segundos */
		tv.tv_usec = 1; 	/* cant microsegundos */        
	
		while(select(fdMax+1, &leeFds, NULL, NULL, &tv) == -1)
        	{
			if(errno != EINTR)
           		{
				perror("select()");
				escribirLog ('E', "Select errno != EINTR");
				return 1;
			}
        	}
        	  
   		/* busca peticiones/conexiones */ 
                 
		for(iSocket = 0; iSocket <= fdMax; iSocket++) 
       	{
			if(FD_ISSET(iSocket, &leeFds)) 
      			{ 
              		if (iSocket == iServidor) /* si es el server */
              		{
		      			iSocket_aceptado = aceptarConexion (iServidor); /*sendrecv.h*/
					if (iSocket_aceptado != SOCKET_INVALIDO)
					{
						FD_SET(iSocket_aceptado, &masterFds); /* lo mandamos al maestro */
                       			if (iSocket_aceptado > fdMax) /* actualizamos maximo */
       						fdMax = iSocket_aceptado;
					}
					else 
						escribirLog ('I', "Conexion rechazada");           			
				} /* Fin verificar si alguien envio algo */
				
				else /* Hay que ver quien entra */
				{	
					recibirH(iSocket, &cabecera);

					printf("CACHE:Recibí cabecera\n");
					printf("CACHE: DescriptorID: ");
					for(i = 0; i < 16; i++)
						printf("%c", cabecera.cpDescriptorID[i]);
					printf("\n");
					printf("CACHE: Payloadlength: %d \n", cabecera.uiPayloadLength);
										
					switch(cabecera.ucPayloadDescriptor)
					{
						case IPCIRC_CACHE_GET:
							recibirAlgo(iSocket, ucBufIdSector, cabecera.uiPayloadLength);
							/* Fetching multiple values */
	 					 	iSectorId = (int)BinToNum(ucBufIdSector, 0);							
	  						miitoa(cBufIdSectorKey, (long)iSectorId);
							strcat(cBufIdSectorKey, stArchConf.cvNombreDisco);
							printf("CACHE: Entrando a memcached_get. cBufIdSectorKey: %s\n", cBufIdSectorKey);
							key_length = strlen(cBufIdSectorKey);
							cpSector = memcached_get(memc, cBufIdSectorKey, key_length, &stTamanioSector, &flags, &rc);
							printf("Sale de memcached_get\n");
							if(cpSector == NULL) /* No se encontro el sector */
							{
								printf("CACHE: No se encontro el sector solicitado\n");
								cabecera.ucPayloadDescriptor = IPCIRC_MISSCACHE;
								cabecera.uiPayloadLength = 0;
								enviarH(iSocket, cabecera);
							}
							else /* Se encontro el sector */
							{
								printf("CACHE: Se encontro el sector solicitado. Su contenido es:%s\n", cpSector);
								cabecera.ucPayloadDescriptor = IPCIRC_HITCACHE;
								cabecera.uiPayloadLength = 512; 
								enviarH(iSocket, cabecera);
								enviarAlgo(iSocket, (unsigned char *)cpSector, cabecera.uiPayloadLength);
								free(cpSector);							
							}
							
							/*printf("CACHE: Entro a Recibir la basura\n");
							recv(iSocket, ucBufBasura, 500, 0);
							printf("CACHE: Recibi la basura:\n");
							printf("%s", ucBufBasura);
							printf("\n");	*/														
							break;
							
						case IPCIRC_CACHE_PUT:
							printf("CACHE: Entro por payloadDescriptor IPCIRC_CACHE_PUT\n"); 
							recibirAlgo(iSocket, ucBuf, cabecera.uiPayloadLength);
							
							for(iRecorreBuffer=0, i=0, j=0; iRecorreBuffer < 516; iRecorreBuffer++)
							{
								if(iRecorreBuffer == 0)
								{			
									iSectorId = BinToNum(ucBuf, iRecorreBuffer);			
									iRecorreBuffer = 4;
								}
								if((iRecorreBuffer >= 4) && (iRecorreBuffer < 516))
								{
									ucBufSector[i] = ucBuf[iRecorreBuffer];
									i++;
								}
							}
							/* Adding a value to the server */	
							miitoa(cBufIdSectorKey, (long)iSectorId);
							strcat(cBufIdSectorKey, stArchConf.cvNombreDisco);
	    						rc = memcached_set(memc, cBufIdSectorKey, strlen(cBufIdSectorKey), ucBufSector, 512, (time_t)0, (uint32_t)0);
							printf("CACHE: Saliendo de memcached_set\n");
	    						if(rc != MEMCACHED_SUCCESS)
	    						{
	    							escribirLog('E',"Error en memcached_set");
								printf("CACHE @ IPCIRC_CACHE_PUT: Error: rc != MEMCACHED_SUCCESS\n");
	    							return -1;
	    						}				
						 	break;
						 	
						default:
							printf("CACHE: Entro por cualquier payloadDescriptor\n");	
							printf("CACHE: numero de descriptor: %o\n");						 
							return 0; 
							break;		
					}					
				}
			}			
		}
	}

	memcached_server_free(servers);
 	memcached_free(memc);	
	return 1;
}

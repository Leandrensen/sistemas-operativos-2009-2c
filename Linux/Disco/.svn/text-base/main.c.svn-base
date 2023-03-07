#define _XOPEN_SOURCE 1

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
#include <sys/shm.h> 
#include <pthread.h>
#include <sys/wait.h>
#include <ldap.h>
#include "sendrecv.h"
#include "archivos.h"
#include "cadenas.h"
#include "calculosYEstadisticas.h"
#include "primitivas.h"
#include "handshake.h"
#include "defines.h"
#include "listas.h"
#include "funcionesLdap.h"
#include "initBind.h"

int terminado = 0;
void Error(int);
void controladorSenialWait(int); /* Funcion a llamar cuando se capture la seniaal SIGUSR1 */
void controladorSenialTerminado(int); /* Funcion a llamar cuando se capture la seniaal SIGUSR2 */
void controladorSenialSigInt(int); /* Funcion a llamar cuando se capture la seniaal SIGINT */
pid_t ppid; 	/* pid proceso padre */
pid_t cpid; 	/* pid proceso hijo */
void *consola(void *);
void mostrarAyuda(void);
ESTADISTICAS stEstadisticas;

unsigned int tiempoDeLectura;
CONFIGURACION stArchConf;
int iCache = 0;
int iHayCache;
int main(void)
{
	fd_set masterFds;   		/* conjunto fd maestro para funcion select() */
	fd_set leeFds; 		/* conjunto fd temporal para funcion select() */
    struct timeval tv; 		/* estructura que define el tiempo que espera select por E/S, para q no sea bloqueante */
	int iServidor = -1, fdMax, i, iSocket_aceptado;
	int iCliente;	
	int iHShake;
	int j, k;
	HEADERIRCIPC cabecera;
	pthread_t threadConsola;
	LDAPINIT ldap_sesion;
	LDAPAGREGARSECTOR ldap_agregarSector;
	char cvIdSector[50];
	int iContadorIncrementoCabezal = 0;
	
		
	srand((unsigned) time(NULL));
	
	system("clear");
	
	/****************************** SENIALES ***********************************/
	
	if(signal(SIGCHLD, controladorSenialWait) == SIG_ERR) /* se define la funcion a llamar cuando se capture la señal */
		perror ("No se puede cambiar signal");
	
	if(signal(SIGUSR2, controladorSenialTerminado) == SIG_ERR) /* se define la funcion a llamar cuando se capture la senial */
		perror ("Error de funcion signal()");
		
	if(signal(SIGINT, controladorSenialSigInt) == SIG_ERR)
		perror("Error de funcion signal()");	
	
	/****************************** ARCHIVO DE CONFIGURACION Y LOG ***********************************/
	
	cargarConfiguracion(&stArchConf); /* archivos.h */
	crearLog(); /* archivos.h */
	
	/****************************** CREO LA ESTRUCTURA DEL DISCO Y LA INICIALIZO ********************/
	
	unsigned int imDisco[stArchConf.stCHS.iCilinders][stArchConf.stCHS.iSectors];	
		
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
	
	/******************************* CALCULO DE CARACTERISTICAS DE DISCO Y ESCRITURA EN LOG *********/
	
	stEstadisticas.cantAccesosACache = 0;
	stEstadisticas.cantSectoresBuscados = 0;
	stEstadisticas.tiempoUtilizadoAccesoACache = 0;
	stEstadisticas.tiempoUtilizadoEnBusquedas = 0;
	stEstadisticas.tiempoUtilizadoEnTransferencias = 0;
	stEstadisticas.totalBytesTransferidos = 0;
	stEstadisticas.uiPosCabezal = 0; /* la posicion inicial del cabezal del disco es en el sector 0 */
	stEstadisticas.peorTiempoSector = 0;
	stEstadisticas.tiempoLectoEscrituraSector = (60*1000*10)/(stArchConf.iRPM*stArchConf.stCHS.iSectors);
	stEstadisticas.uiCacheHit = 0;	
	stEstadisticas.uiCacheMiss = 0;
	stEstadisticas.uiTiempoAccesoACache = stArchConf.iTiempoAccesoACache; /* Con un decimal de precision */
	generarEstadisticas(stArchConf, stEstadisticas); /* calculosYEstadisticas.h */

	/****************************** CONEXION COMO CLIENTE **************************************/
	FD_ZERO(&masterFds);     /* borramos los dos conjuntos */
	FD_ZERO(&leeFds);
	
	iCliente = iniciarCliente(stArchConf.cvIPArrayVSK, stArchConf.iPuertoArrayVSK); /* sendrecv.h */
	
	switch (iCliente)
	{
		case SOCKET_INVALIDO: escribirLog ('E', "Socket invalido"); return 0; break;
		case SOCKET_ERROR_CONEXION: escribirLog ('E', "Socket error de conexion"); return 0; break;
	}

	FD_SET(iCliente, &masterFds);
	fdMax=iCliente;
	
	iHShake = realizarHandshake(iCliente, stArchConf);
	
	switch(iHShake) /* handshake.h */
	{
		case 1:
			printf ("Iniciando exitosamente\n");
			escribirLog ('I', "Iniciando exitosamente");
			break; 
		case 0:
			printf ("Conexion rechazada\n");
			escribirLog ('I', "Conexion rechazada");
			break;  
	}

	/****************************** CONEXION COMO SERVIDOR *************************************/
	iServidor = iniciarServidor(stArchConf.cvIP, stArchConf.iPuerto, 20); /* sendrecv.h */
	
	switch (iServidor)
	{
		case SOCKET_INVALIDO: escribirLog ('E', "Socket invalido"); return 0; break;
		case SOCKET_ERROR_INICIALIZAR: escribirLog ('E', "No se pudo inializar el socket como servidor"); return 0; break;
	}
		
	FD_SET(iServidor, &masterFds);
	if(iServidor > iCliente)
		fdMax=iServidor;	

	/*******************************************************************************************/

	/******************************* CREAMOS UN THREAD NUEVO , EL DE LA CONSOLA ********************/
	
	pthread_create(&threadConsola, NULL, consola, NULL);

	/****************************** CONEXION CON LDAP ******************************************/

	ldapCargarSesion(&ldap_sesion, stArchConf.cvLdapIp, stArchConf.iLdapPuerto, stArchConf.cvLdapPass);

	if(LDAP_INICIAR_OK == ldapIniciarSesion(&ldap_sesion))
	{
		for(i = 0; i < stArchConf.stCHS.iCilinders*stArchConf.stCHS.iSectors; i++)
		{
			miitoa(cvIdSector, i);
			cargarLDAPAGREGARSECTOR(&ldap_agregarSector, stArchConf.cvNombreDisco, cvIdSector, "");
			ldapAgregarSector(ldap_sesion.ldapSesion, ldap_agregarSector);
		}
	}

	/*******************************************************************************************/
	if(strcmp(stArchConf.cvCache, "si") == 0)
		iHayCache = 1;
	else
		iHayCache = 0;
	
	if(iHayCache)
	{ 		
				
		/* Forkeamos un proceso cache que se va a conectar a nosotros */
		
		if((cpid = fork()) == -1)
		{
			perror("fork()");
			return -1;
		}
		else
		{
			if(cpid == 0) /* estamos dentro del hijo (esto seria de la cache) */
			{
		 		if(signal(SIGINT, SIG_IGN) == SIG_ERR) /* los hijos ignoran la SIGINT */
					perror("Error de funcion signal()");
				
				printf("CACHE: Forkie a la cache\n");										
				execv("./Cache/cache", NULL); /* Ejecutamos la cache limpiando todo el codigo del padre */
			}
			else printf("DISCO: Forkie a la cache\n"); /* estamos en el padre */ 									
		}   
	}
	
	while(!terminado) 
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
                 
		for(i = 0; i <= fdMax; i++) 
       	{
			if(FD_ISSET(i, &leeFds)) 
      			{ 
              		if (i == iServidor) /* si es el server , peticion de conexion. Lo unico que puede conectarse al Disco es la Cache */ /* Mal. La consola tambien se conecta */
              		{
			    		iSocket_aceptado = aceptarConexion(iServidor); /* sendrecv.h */
			    		
					if (iSocket_aceptado != SOCKET_INVALIDO)
					{
						FD_SET(iSocket_aceptado, &masterFds); /* lo mandamos al maestro */
                        			if (iSocket_aceptado > fdMax) /* actualizamos maximo */
        						fdMax = iSocket_aceptado;
					}
					else
					{
					 escribirLog ('I', "Conexion rechazada"); 
					}
					
				} /* Fin verificar si alguien envio algo */
				else /* Hay que ver quien entra */
				{
					/*************** ATENCION DE Array/VSK/Cache */
					recibirH(i, &cabecera);
					
					switch(cabecera.ucPayloadDescriptor)
					{
						case IPCIRC_GET_SECTORES_2:
							getSectores(i, cabecera, stArchConf, &stEstadisticas, iHayCache, iCache, ldap_sesion.ldapSesion); 
							generarEstadisticas(stArchConf, stEstadisticas); /* primitivas.h */
							break; 
						case IPCIRC_GET_SECTORES_1: 
							getSectores(i, cabecera, stArchConf, &stEstadisticas, iHayCache, iCache, ldap_sesion.ldapSesion); 
							generarEstadisticas(stArchConf, stEstadisticas); /* primitivas.h */
							break; 
						case IPCIRC_OBTENER_SECTOR_DEPASO: 
							getSectores(i, cabecera, stArchConf, &stEstadisticas, iHayCache, iCache, ldap_sesion.ldapSesion); 
							break; 
						case IPCIRC_OBTENER_SECTOR_DEFINITIVO: 
							getSectores(i, cabecera, stArchConf, &stEstadisticas, iHayCache, iCache, ldap_sesion.ldapSesion); 
							generarEstadisticas(stArchConf, stEstadisticas); /* primitivas.h */
							break; 
						case IPCIRC_PUT_SECTORES_2: 
							putSectores(i, cabecera, stArchConf, &stEstadisticas, ldap_sesion.ldapSesion); 
							generarEstadisticas(stArchConf, stEstadisticas); /* primitivas.h */
							break; 
						case IPCIRC_PUT_SECTORES_1: 
							putSectores(i, cabecera, stArchConf, &stEstadisticas, ldap_sesion.ldapSesion); 
							generarEstadisticas(stArchConf, stEstadisticas); /* primitivas.h */
							break;
						case IPCIRC_ESCRIBIR_SECTOR:
							putSectores(i, cabecera, stArchConf, &stEstadisticas, ldap_sesion.ldapSesion); 
							generarEstadisticas(stArchConf, stEstadisticas); /* primitivas.h */
							break; 
						case IPCIRC_GET_CHS: 
							getCHS(i, cabecera, stArchConf); /* primitivas.h */
							break; 
						case IPCIRC_HANDSHAKE_CACHE:
							printf("DISCO: Recibi el handshake de la cache\n");
							iCache = i;
							break;
						case IPCIRC_HANDSHAKE_OK:
							break;
						default:
							printf("payloadDescriptor invalido\n"); 
							close(i); 
							break;
					}/* Fin swith*/	
				}/* fin else */
			}/* fin fdset*/		
		}/* fin for*/
		
		if(iContadorIncrementoCabezal < 4)
			iContadorIncrementoCabezal++;
		else if(iContadorIncrementoCabezal >= 4)
			 {
				/* Te posicionas donde estas actualmente */
				for(i=0, j=0; (i < (stArchConf.stCHS.iCilinders - 1)) && (imDisco[i][j] != stEstadisticas.uiPosCabezal);)
				{
					if(j < (stArchConf.stCHS.iSectors - 1))
						j++;
					else
					{
						i++;
						j = 0;
					}
				}
				
				/* Hacemos girar al disco un sector */
				if(j < (stArchConf.stCHS.iSectors - 1))
					j++;
				else 
					j = 0;
					
				stEstadisticas.uiPosCabezal = imDisco[i][j];			
				iContadorIncrementoCabezal = 0;
			 } 
	}/* fin while*/
	ldapFinSesion(ldap_sesion);
	return 1;
}

void controladorSenialWait(int i)
{
	pid_t cpid;
	if((cpid = wait(NULL)) == -1)
		perror("wait()");
	signal(SIGCHLD, controladorSenialWait);
}

void controladorSenialTerminado(int i)
{
     terminado = 1; /* hace q salga del while "infinito" */
     kill(0, SIGKILL);
     signal(SIGUSR2, controladorSenialTerminado);
}

void controladorSenialSigInt(int i) 
{	
	generarEstadisticas(stArchConf, stEstadisticas); /* primitivas.h */
	kill(0, SIGKILL);
	exit(EXIT_SUCCESS);
	signal(SIGINT, controladorSenialSigInt);
}

void *consola(void *parametroSinUso) /* Hay que implementar semaforos aca, sacar de GNUtella */
{
	char *cp, *cp2, cvStringConsola[257], cBanderaPrint = 1, cvListaSectores[500], cvNumeroDeSector[50], cvBufferEscribeSector[512];
	unsigned int imDisco[stArchConf.stCHS.iCilinders][stArchConf.stCHS.iSectors], iIdSector, iCantSectores = 0, iMueveListaSectores = 0;
	int i, j, k, l, q, iCantSectoresLeidos, iIdSectorAux, iSocketConsola;
	unsigned char ucBufSector[512], ucBufPutSector[516], ucvIdSector[4];	
	HEADERIRCIPC cabeceraConsola;
	ESTADISTICAS stEstadisticas;
	int iContadorIncrementoCabezal = 0;
	
	/******************************* CALCULO DE CARACTERISTICAS DE DISCO Y ESCRITURA EN LOG *********/
	
	stEstadisticas.cantAccesosACache = 0;
	stEstadisticas.cantSectoresBuscados = 0;
	stEstadisticas.tiempoUtilizadoAccesoACache = 0;
	stEstadisticas.tiempoUtilizadoEnBusquedas = 0;
	stEstadisticas.tiempoUtilizadoEnTransferencias = 0;
	stEstadisticas.totalBytesTransferidos = 0;
	stEstadisticas.uiPosCabezal = 0; /* la posicion inicial del cabezal del disco es en el sector 0 */
	stEstadisticas.peorTiempoSector = 0;
	stEstadisticas.tiempoLectoEscrituraSector = (60*1000*10)/(stArchConf.iRPM*stArchConf.stCHS.iSectors);
	stEstadisticas.uiCacheHit = 0;	
	stEstadisticas.uiCacheMiss = 0;
	stEstadisticas.uiTiempoAccesoACache = stArchConf.iTiempoAccesoACache; /* Con un decimal de precision */
	generarEstadisticas(stArchConf, stEstadisticas); /* calculosYEstadisticas.h */

	/****************************** ************************* **************************************/
	
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

	iSocketConsola = iniciarCliente(stArchConf.cvIP, stArchConf.iPuerto); /* sendrecv.h */
	
	printf("Para obtener una lista de todos los comandos disponibles escriba \"ayuda\".\n\n");
	
	/* Te posicionas en donde estas actualmente */
	for(i=0, j=0; (i < (stArchConf.stCHS.iCilinders - 1)) && (imDisco[i][j] != stEstadisticas.uiPosCabezal);)
	{
		if(j < (stArchConf.stCHS.iSectors - 1))
			j++;
		else
		{
			i++;
			j = 0;
		}
	}

	while(1)
	{
		iCantSectoresLeidos = 0;
		
		fgets(cvStringConsola, 256, stdin);	
		cp = strtok(cvStringConsola, "(\n"); 
	
		if(strcmp(cvStringConsola, "posicionCabezal") == 0)
		{
			printf("\nPosicion actual del cabezal: Sector %d\n\n", stEstadisticas.uiPosCabezal);
		}
		else if(strcmp(cvStringConsola, "obtenerSectores") == 0)
			{	
				iCantSectores = 0;
				iMueveListaSectores = 0;
				printf("hacemos lo de obtenerSectores\n");
				cp = strtok(NULL, ")");
				strcpy(cvListaSectores, cp);
				for(k = 0; cvListaSectores[k] != '\0'; k++)
				{
					if(cvListaSectores[k] == ',')
					{
						cvListaSectores[k] = '\0';
						iCantSectores++;
					}
				}
				iCantSectores++;
				for(q = 0; q < iCantSectores; q++)
				{	
					cBanderaPrint = 1;
					iCantSectoresLeidos = 0;				
					/* calcular sectores leidos */
					printf("\nPosicion actual: %d\n", stEstadisticas.uiPosCabezal);
					printf("Sectores Leidos: ");			
					
					/* Si el sector deseado esta en el clilindro actual */
					iIdSector = miatoi(&cvListaSectores[iMueveListaSectores]);	
					while(imDisco[i][j] != iIdSector)
					{
						if((iIdSector >= ((i+1)*stArchConf.stCHS.iSectors - stArchConf.stCHS.iSectors)) && (iIdSector <= ((i+1)*stArchConf.stCHS.iSectors - 1)))
						{							
							while(imDisco[i][j] != iIdSector)
							{
								NumToBin (ucvIdSector, imDisco[i][j], 0);
								crearCabecera(&cabeceraConsola, IPCIRC_OBTENER_SECTOR_DEPASO, sizeof(ucvIdSector));
								enviarH(iSocketConsola, cabeceraConsola);
								enviarAlgo(iSocketConsola, ucvIdSector, cabeceraConsola.uiPayloadLength);								
								recibirH(iSocketConsola, &cabeceraConsola);
								recibirAlgo(iSocketConsola, ucBufSector, cabeceraConsola.uiPayloadLength);
								if(cBanderaPrint == 1)
								{
									printf("%s", ucBufSector);
									cBanderaPrint = 0;
								}
								else
									printf(", %s", ucBufSector);
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
							NumToBin(ucvIdSector, imDisco[i][j], 0);
							crearCabecera(&cabeceraConsola, IPCIRC_OBTENER_SECTOR_DEPASO, sizeof(ucvIdSector));
							enviarH(iSocketConsola, cabeceraConsola);
							enviarAlgo(iSocketConsola, ucvIdSector, cabeceraConsola.uiPayloadLength);								
							recibirH(iSocketConsola, &cabeceraConsola);
							recibirAlgo(iSocketConsola, ucBufSector, cabeceraConsola.uiPayloadLength);
							if(cBanderaPrint == 1)
							{
								printf("%s", ucBufSector);
								cBanderaPrint = 0;
							}
							else
								printf(", %s", ucBufSector);

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
					printf("\nTiempo Consumido: ");
					printf("%d.%d\n", (stEstadisticas.tiempoLectoEscrituraSector / 10)*iCantSectoresLeidos, (stEstadisticas.tiempoLectoEscrituraSector % 10)*iCantSectoresLeidos);
					
					/* Limpiamos el buffer con '\0' antes de recibir */
					for(k = 0; k < 512; k++)
						ucBufSector[k] = '\0';
					
					iIdSectorAux = miatoi(&cvListaSectores[iMueveListaSectores]);
					NumToBin (ucvIdSector, iIdSectorAux, 0);
					crearCabecera(&cabeceraConsola, IPCIRC_OBTENER_SECTOR_DEFINITIVO, sizeof(ucvIdSector));
					enviarH(iSocketConsola, cabeceraConsola);
					enviarAlgo(iSocketConsola, ucvIdSector, cabeceraConsola.uiPayloadLength);			
					recibirH(iSocketConsola, &cabeceraConsola);
					recibirAlgo(iSocketConsola, ucBufSector, cabeceraConsola.uiPayloadLength);
					iCantSectoresLeidos++;
					/* Movemos un sector mas el disco porque acaba de leerlo */
					if(j < (stArchConf.stCHS.iSectors - 1))
						j++;
					else 
						j = 0;
					/********************************************************/	
					printf("Sector %d: \"", iIdSectorAux);
					for(k = 0; k < 10; k++)
						printf("%c", ucBufSector[k]);
					printf("\"...\"");
					for(k = 501; k < 512; k++)
						printf("%c", ucBufSector[k]);
					printf("\"\n\n");
									
					stEstadisticas.uiPosCabezal	= imDisco[i][j];
					for(; cvListaSectores[iMueveListaSectores] != '\0'; iMueveListaSectores++);
					iMueveListaSectores++; 													
				}
			}else if(strcmp(cvStringConsola, "escribirSector") == 0)
				{	
					cBanderaPrint = 1;
					iCantSectoresLeidos = 0;
					printf("hacemos lo de escribirSector\n");
					printf("\nPosicion actual: %d\n", stEstadisticas.uiPosCabezal);
					cp = strtok(NULL, ",");
					strcpy(cvNumeroDeSector, cp);
					cp = strtok(NULL, ")\n");
					strcpy(cvBufferEscribeSector, cp);					

					iIdSector = miatoi(cvNumeroDeSector);
																				
					/* calcular sectores leidos */					
					printf("Sectores Leidos: ");
					
					/* Te posicionas en donde estas actualmente */
					for(i=0, j=0; (i < (stArchConf.stCHS.iCilinders - 1)) && (imDisco[i][j] != stEstadisticas.uiPosCabezal); j++)
					{
						if(j < (stArchConf.stCHS.iSectors - 1))
							j++;
						else
						{
							i++;
							j = 0;
						}
					}
					
					/* Si el sector deseado esta en el clilindro actual */
					while(imDisco[i][j] != iIdSector)
					{
						if((iIdSector >= ((i+1)*stArchConf.stCHS.iSectors - stArchConf.stCHS.iSectors)) && (iIdSector <= ((i+1)*stArchConf.stCHS.iSectors - 1)))
						{							
							while(imDisco[i][j] != iIdSector)
							{
								NumToBin (ucvIdSector, imDisco[i][j], 0);
								crearCabecera(&cabeceraConsola, IPCIRC_OBTENER_SECTOR_DEPASO, sizeof(ucvIdSector));
								enviarH(iSocketConsola, cabeceraConsola);
								enviarAlgo(iSocketConsola, ucvIdSector, cabeceraConsola.uiPayloadLength);								
								recibirH(iSocketConsola, &cabeceraConsola);
								recibirAlgo(iSocketConsola, ucBufSector, cabeceraConsola.uiPayloadLength);
								if(cBanderaPrint == 1)
								{
									printf("%s", ucBufSector);
									cBanderaPrint = 0;
								}
								else
									printf(", %s", ucBufSector);
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
							NumToBin(ucvIdSector, imDisco[i][j], 0);
							crearCabecera(&cabeceraConsola, IPCIRC_OBTENER_SECTOR_DEPASO, sizeof(ucvIdSector));
							enviarH(iSocketConsola, cabeceraConsola);
							enviarAlgo(iSocketConsola, ucvIdSector, cabeceraConsola.uiPayloadLength);								
							recibirH(iSocketConsola, &cabeceraConsola);
							recibirAlgo(iSocketConsola, ucBufSector, cabeceraConsola.uiPayloadLength);
							if(cBanderaPrint == 1)
							{
								printf("%s", ucBufSector);
								cBanderaPrint = 0;
							}
							else
								printf(", %s", ucBufSector);

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
					printf("\nTiempo Consumido: ");
					printf("%d.%d", (stEstadisticas.tiempoLectoEscrituraSector / 10)*iCantSectoresLeidos, (stEstadisticas.tiempoLectoEscrituraSector % 10)*iCantSectoresLeidos);
					
					NumToBin (ucBufPutSector, iIdSector, 0);
										
					printf("sector %d: \"", iIdSector);
					for(k = 0; k < 10; k++)
						printf("%c", cvBufferEscribeSector[k]);
					printf("\"...\"");
					for(k = 500; k < 512; k++)
						printf("%c", cvBufferEscribeSector[k]);
					printf("\"\n\n");
					
					for(l = 0, k = 4; k < 516; k++, l++)
						ucBufPutSector[k] = cvBufferEscribeSector[l];
					
					crearCabecera(&cabeceraConsola, IPCIRC_ESCRIBIR_SECTOR, sizeof(ucBufPutSector));
					enviarH(iSocketConsola, cabeceraConsola);
					enviarAlgo(iSocketConsola, ucBufPutSector, cabeceraConsola.uiPayloadLength);
					
					/* Movemos un sector mas el cabezal, porque lo acaba de escribir */
					if(j < (stArchConf.stCHS.iSectors - 1))
						j++;
					else 
						j = 0;
					/*****************************************************************/

					stEstadisticas.uiPosCabezal	= imDisco[i][j];
										
				}else if(strcmp(cvStringConsola, "ayuda") == 0)
				  	{
				  		mostrarAyuda();
				  	}else  printf("Comando incorrecto. Para obtener ayuda escriba \"ayuda\".\n");
		sleep(1);

		if(iContadorIncrementoCabezal < 4)
			iContadorIncrementoCabezal++;
		else if(iContadorIncrementoCabezal >= 4)
			 {
				
				/* Hacemos girar al disco un sector */
				if(j < (stArchConf.stCHS.iSectors - 1))
					j++;
				else 
					j = 0;
					
				stEstadisticas.uiPosCabezal = imDisco[i][j];			
				iContadorIncrementoCabezal = 0;
				printf("CONSOLA: Gire un sector el disco\n");
			 } 
	}
}

void mostrarAyuda(void)
{
	printf("================================================================================\n");
	printf("\t\t\t\tAYUDA DE CONSOLA\n");
	printf("Comandos permitidos:\n\n");
	printf("posicionCabezal()\n>>>Imprime en pantalla la posicion actual del cabezal del disco\n\n");
	printf("obtenerSectores([lista sectores])\n>>>recibe como parametro una lista de no mas de 20 sectores y muestra en\npantalla " 
		   "por cada sector la informacion del comportamiento del disco al realizar\nla tarea "
		   "de busqueda de los mismos, junto con los primero 10 y los ultimos 10\n"
		   "caracteres de dicho sector\nEjemplo: ObtenerSectores(100)\n\tPosicion actual: 0\n\tSectores leidos:21, 32, 43, 54, 65, 76, 87, "
		   "98, 99, 100\n\tTiempo consumido: 16ms\n\tSector 100: \"Informacion\"...\"del final.\".\n\n");
	printf("escribirSector([numero de sector], [buffer de hasta 512 caracteres])\n>>>Imprimira en pantalla la misma informacion que el comando "
	       "obtenerSectores()\nsolo que en este caso escribira el sector pasado como parametro en el buffer.\n");
	printf("================================================================================\n\n");
	
}

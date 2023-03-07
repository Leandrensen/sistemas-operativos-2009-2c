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
#include "sendrecv.h"
#include "archivos.h"
#include "cadenas.h"
#include "delFile.h"
#include "addFile.h"
#include "srchFile.h"
#include "modFile.h"
#include "initBind.h"
#include "define.h"


int main (void)
{
	fd_set masterFds;   		/* conjunto fd maestro para funcion select() */
	fd_set leeFds; 		/* conjunto fd temporal para funcion select() */
    struct timeval tv; 		/* estructura que define el tiempo que espera select por E/S, para q no sea bloqueante */
	int fdMax, i, iSector1, iSector2;
	srand((unsigned) time(NULL));
	
	int iVSKSock;
	
	/*VAR LDAP*/
    LDAPINIT ldap;
	
    /* ----- */
    
	/* VAR ARCHCONF */
	CONFIGURACION stArchConf;
	
	/* VAR PARA BUSQUEDAS */
	unsigned char* bfr;
	unsigned char ucBuf[1000],ucBuf2[1000];

	LDAPBUSCARARCHIVO LdapBuscarArchivo;
	LDAPELIMINARARCHIVO LdapEliminarArchivo;
	LDAP_AGREGAR_ARCHIVO LdapAgregarArchivo;
	LDAPMODIFICARARCHIVO LdapModificarArchivo;
	char nombreDAinfo[100];
	char nombreArchivoinfo[100];
	char* nombreDA, nombreDAS[100],nombreArchivoDA[100];
	char* nombreArchivo;							
	char autor[20];
	char permisos[20];
	char cvtamanio[20];
	char * tamanio;
	char listaSectores[20];
	char* cCantSectores;

	ptrLdapResultados ldapResultados = NULL;
	pResultadoInformacion ldapResultadoInformacion = NULL;
	int iContadorDatosLdap=0;
	int iCantidad;
	pINT iplistaSectoresLibres, ipNuevosSectores;
	int iCantidadSectoresLibres;
	
	pthread_t threadConsola;

	/* datos cabecera */
	HEADERIRCIPC header;
	
    /* VAR ARCHLOG */
    FILE *archLog;

    if((archLog = fopen("./SFSlog.txt", "a")) == NULL)
    {
        perror("Error en apertura del archivo log");
        return -1;
    }

   /* INICIO PROGRAMA */

    /* Obtener datos del archivo de configuracion */
    if(cargarConfiguracion(&stArchConf, archLog) == 0)
    {
        return -1;
    }

	printf ("Iniciando exitosamente\n");
    iVSKSock = iniciarCliente(stArchConf.cvIPVsk, stArchConf.iPuertoVsk);
	
	/* Realizo HandShake con el VSK */
	if(realizarHandshake(iVSKSock, archLog) == -1)
    {
        printf("No se ha podido realizar el handshake con el VSK\n");
        return -1;
    }
	ldapCargarSesion(&ldap, stArchConf.cvLdapIp,stArchConf.iLdapPuerto, stArchConf.cvLdapPass);
	if (ldapIniciarSesion(&ldap) != LDAP_INICIAR_OK)
	{
		return 1;
	}
	
	pthread_create(&threadConsola, NULL, consola, NULL);

	/***********************************************************************************/

	FD_ZERO(&masterFds);     /* borramos los dos conjuntos */
	FD_ZERO(&leeFds);
	FD_SET(iVSKSock, &masterFds);
	fdMax=iVSKSock;
	
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
				/*creaLog(archLog, "ERROR", "Error en el Select().", 0, NULL);*/
				return 1;
				}
        	}
  
   	/* busca peticiones/conexiones */ 
                 
		for(i = 0; i <= fdMax; i++) 
       	{
			if(FD_ISSET(i, &leeFds)) 
      		{ 
              	/*if (i == iVSKSock)*/ /* si es el server */
              	/*{
			       	iSocket_aceptado = aceptarConexion (iVSKSock);*/ /*sendrecv.h*/

					/*if (iSocket_aceptado != SOCKET_INVALIDO)
					{*/
						/*FD_SET(iSocket_aceptado, &masterFds);*/ /* lo mandamos al maestro */
                           		       /*if (iSocket_aceptado > fdMax)*/ /* actualizamos maximo */
        						/*fdMax = iSocket_aceptado;*/
				/*	}
					break;    */                                   			
					
				/*} *//* Fin verificar si alguien envio algo */
			
				/*else *//* Hay que ver quien entra */
				/*{*/
					if(recibirH(i, &header) < 1) /*de donde recibo?*/
					{
						close(i);
						FD_CLR(i,&masterFds);
						return 1;
					}
					else						
					switch(header.ucPayloadDescriptor)
					{
						case IPCIRC_EXISTEARCHIVO_W: case IPCIRC_EXISTEARCHIVO_R: case IPCIRC_EXISTEARCHIVO_D:
						/* ExisteArchivo() ********************************************TESTEADO */
							
							recibirDatos(i, header, bfr , nombreDAinfo, nombreArchivoinfo);
							
							cargarLDAPBUSCARARCHIVO(&LdapBuscarArchivo, nombreDAinfo, nombreArchivoinfo);
							
							if (exsteArchivo( ldap.ldapSesion, LdapBuscarArchivo) == 1)
							{
								printf("EXISTEARCHIVO: Se encuentra exitosamente %s - %s:\n", nombreDAinfo, nombreArchivoinfo);
								switch(header.ucPayloadDescriptor)
								{
									case IPCIRC_EXISTEARCHIVO_R: header.ucPayloadDescriptor = IPCIRC_EXISTEARCHIVO_RSI; break;
									case IPCIRC_EXISTEARCHIVO_W: header.ucPayloadDescriptor = IPCIRC_EXISTEARCHIVO_WSI; break;
									case IPCIRC_EXISTEARCHIVO_D: header.ucPayloadDescriptor = IPCIRC_EXISTEARCHIVO_DSI; break;
								}
								

							}
							else
							{
								/* No existe el archivo, mando ERROR */
								switch(header.ucPayloadDescriptor)
								{
									case IPCIRC_EXISTEARCHIVO_R: header.ucPayloadDescriptor = IPCIRC_EXISTEARCHIVO_RNO; break;
									case IPCIRC_EXISTEARCHIVO_W: header.ucPayloadDescriptor = IPCIRC_EXISTEARCHIVO_WNO; break;
									case IPCIRC_EXISTEARCHIVO_D: header.ucPayloadDescriptor = IPCIRC_EXISTEARCHIVO_DNO; break;
								}
						
							}	
							header.uiPayloadLength = 0;
							enviarH(i, header);
							/*free(bfr);*/
							break;
							
						case IPCIRC_INFOARCHIVO: /* InfoArchivo() */
							
							recibirDatos(i, header, bfr ,nombreDAinfo, nombreArchivo);
							
							cargarLDAPBUSCARARCHIVO(&LdapBuscarArchivo, nombreDAinfo, nombreArchivoinfo);
							
							if (ldapBuscarArchivo(ldap.ldapSesion, LdapBuscarArchivo, &ldapResultados) != -1)
							{
								if (ldapResultados != NULL)
								{	printf("INFOARCHIVO: Se encuentra exitosamente %s - %s:\n", nombreDAinfo, nombreArchivoinfo);
									cargarInfoArchivo (&ldapResultados, &ldapResultadoInformacion, &iContadorDatosLdap);
									
									header.ucPayloadDescriptor = IPCIRC_INFOARCHIVO_RTA;
									header.uiPayloadLength = iContadorDatosLdap*(sizeof (int));
									free(ldapResultados);
									ldapResultados=NULL;
									if(enviarH(i, header) != -1)
									{
										if (enviarDatosArchivo(i ,&ldapResultadoInformacion, iContadorDatosLdap) == -1)
										{
											close(i);
											FD_CLR(i, &masterFds);
										}
									}
									else
									{
										close(i);
										FD_CLR(i, &masterFds);
									}
									iCantidad = 0;
								}
								else
								{
									/* No existe el archivo, mando ERROR */
									header.ucPayloadDescriptor = IPCIRC_ERROR;								
									header.uiPayloadLength = strlen("Archivo inexistente");
									copiarU(ucBuf,(unsigned char *)"Archivo inexistente");
									if(enviarAlgo(i, ucBuf, header.uiPayloadLength) == -1)
									{
										close(i);
										FD_CLR(i, &masterFds);
									}
								}
							}
							iCantidad =0;
							
							/*free(bfr);*/
							break;							
						case IPCIRC_ELIMINAARCHIVO: case IPCIRC_ELIMINARDELETE:/* EliminaArchivo() */
						
							recibirDatos(i, header, bfr ,nombreDAinfo, nombreArchivoinfo);
							cargarLDAPBUSCARARCHIVO(&LdapBuscarArchivo, nombreDAinfo, nombreArchivoinfo);
							ldapBuscarArchivo(ldap.ldapSesion, LdapBuscarArchivo, &ldapResultados);
							ldapBuscarDAsectoresLibresDEL( ldap.ldapSesion, LdapBuscarArchivo, &iplistaSectoresLibres, &iCantidadSectoresLibres);
							cargarSectoresLiberados(&ipNuevosSectores,iplistaSectoresLibres,&ldapResultados,&iCantidadSectoresLibres);
							cargarLDAPELIMINARARCHIVO(&LdapEliminarArchivo, nombreDAinfo, nombreArchivoinfo);
							ldapQuitarDosSectoresDA(ldap.ldapSesion, LdapBuscarArchivo.nombreDA,0, 0, ipNuevosSectores.lista,iCantidadSectoresLibres);

							if (ldapEliminarArchivo(ldap.ldapSesion, LdapEliminarArchivo) != 0)
							{
								/* Error en el borrado */
								header.ucPayloadDescriptor = IPCIRC_ERROR;								
								header.uiPayloadLength = sizeof("Error en el borrado");
								copiarU(ucBuf,(unsigned char *)"Error en el borrado");
								iCantidad = sizeof("Error en el borrado");
							}
							else
							{
								/* Borrado con exito */
								if (header.ucPayloadDescriptor == IPCIRC_ELIMINAARCHIVO ) header.ucPayloadDescriptor = IPCIRC_ELIMINAARCHIVO_RTA;
								else header.ucPayloadDescriptor = IPCIRC_ELIMINARDELETE_RTA;
								header.uiPayloadLength = 0;								
							}
							
							enviarH(i, header);
							free(bfr);
							break;
							
						case IPCIRC_CREARARCHIVO: /* CrearArchivo() */
						
							bfr = (unsigned char*)malloc(header.uiPayloadLength +1);
							if (recibirAlgo(i, bfr, header.uiPayloadLength) != SOCKET_OK_SENDRECV)
							{
								close(i);
								FD_CLR(i, &masterFds);
							}
							
							bfr[header.uiPayloadLength] = '\0';
							
							nombreDA = strtok((char*)bfr,"|");
							nombreArchivo = strtok(NULL,"|");
							copiar(autor ,"root");
							copiar(permisos , "rwxrwxrwx");
							copiar(listaSectores , "a");
							copiar(cvtamanio,"a");

							cargarAgregarArchivo(&LdapAgregarArchivo, nombreDA, nombreArchivo, autor, permisos, cvtamanio, listaSectores);
							
							if (ldapAgregarArchivo(ldap.ldapSesion, LdapAgregarArchivo) != 0)
							{
								/* Error en el agregado */
								header.ucPayloadDescriptor = IPCIRC_ERROR;								
								header.uiPayloadLength = sizeof("Error en el agregado");
								copiarU(ucBuf,(unsigned char *)"Error en el agregado");
								iCantidad = sizeof("Error en el agregado");
							}
							else
							{
								/* agregado con exito */
								header.ucPayloadDescriptor = IPCIRC_CREARARCHIVO_RTA;								
								header.uiPayloadLength = 0;								
								iCantidad = 0;
							}
							
							enviarH(i, header);

							free(bfr);
							break;
							
						case IPCIRC_ARCTUALIZARTAMANIO: /* ActualizarTamanio() */
							bfr = (unsigned char*)malloc(header.uiPayloadLength +1);
							if (recibirAlgo(i, bfr, header.uiPayloadLength) != SOCKET_OK_SENDRECV)
							{
								close(i);
								FD_CLR(i, &masterFds);
							}
							
							bfr[header.uiPayloadLength] = '\0';
							
							nombreDA = strtok((char*)bfr,"|");
							nombreArchivo = strtok(NULL,"|");
							tamanio = strtok(NULL,"|");

							cargarModifircarArchivoTamanio(&LdapModificarArchivo, nombreDA, nombreArchivo, tamanio);
							
							if (ldapModificarArchivoTamanio(ldap.ldapSesion, LdapModificarArchivo) != 0)
							{
								/* Error en el agregado */
								header.ucPayloadDescriptor = 0x97;								
								header.uiPayloadLength = sizeof("Error en el modificado del tamanio");
								copiarU(ucBuf,(unsigned char *)"Error en el modificado del tamanio");
								iCantidad = sizeof("Error en el modificado del tamanio");
							}
							else
							{
								/* agregado con exito */
								header.ucPayloadDescriptor = 0x19;
								header.uiPayloadLength = 0;								
								iCantidad = 0;
							}
							
							enviarH(i, header);

							free(bfr);
							break;
							
						case IPCIRC_CREARSECTORESLIBRES: /* CrearTablaSectoresLibres() */ /************revisar* ***************/
							bfr = (unsigned char*)malloc(header.uiPayloadLength +1);
							if (recibirAlgo(i, bfr, header.uiPayloadLength) != SOCKET_OK_SENDRECV)
							{
								close(i);
								FD_CLR(i, &masterFds);
							}
							
							bfr[header.uiPayloadLength] = '\0';
							
							nombreDA = strtok((char*)bfr,"|");
							cCantSectores = strtok(NULL,"|");
							/*iCantSectores = BinToNum(cCantSectores, 0);*/

							cargarModifircarArchivoTamanio(&LdapModificarArchivo, nombreDA, nombreArchivo, tamanio);
							
							if (ldapModificarArchivoTamanio(ldap.ldapSesion, LdapModificarArchivo) != 0)
							{
								/* Error en el agregado */
								header.ucPayloadDescriptor = 0x97;								
								header.uiPayloadLength = strlen("Error en el modificado del tamanio");
								copiarU(ucBuf,(unsigned char *)"Error en el modificado del tamanio");
								iCantidad = sizeof("Error en el modificado del tamanio");
							}
							else
							{
								/* agregado con exito */
								header.ucPayloadDescriptor = 0x21;
								header.uiPayloadLength = 0;								
								iCantidad = 0;
							}
							
							if(enviarH(i, header) != -1)
							{
								if(enviarAlgo(i, ucBuf, iCantidad) == -1)
								{
									close(i);
									FD_CLR(i, &masterFds);
								}
							}
							else
							{
								close(i);
								FD_CLR(i, &masterFds);
							}
							free(bfr);
							break;
							
						case IPCIRC_DOSSECTORESLIBRES: /* DarSectoresLibres() */
							bfr = (unsigned char*)malloc(header.uiPayloadLength +1);
							if (recibirAlgo(i, bfr, header.uiPayloadLength) != SOCKET_OK_SENDRECV)
							{
								close(i);
								FD_CLR(i, &masterFds);
							}
							/* Recibo el nombre del DA */
							bfr[header.uiPayloadLength] = '\0';
							cargarLDAPBUSCARARCHIVO(&LdapBuscarArchivo, (char*)bfr, "*");
							if (ldapBuscarDAsectoresLibres(ldap.ldapSesion,LdapBuscarArchivo,ucBuf,&iplistaSectoresLibres,&iCantidadSectoresLibres) != 0)
							{
								header.ucPayloadDescriptor = IPCIRC_DOSSECTORESLIBRES_RTA;
								header.uiPayloadLength = 8;
								if(enviarH(i, header) != -1)
								{
									if(enviarAlgo(i, ucBuf, header.uiPayloadLength) == -1)
									{
										close(i);
										FD_CLR(i, &masterFds);
									}
									ldapQuitarDosSectoresDA(ldap.ldapSesion, LdapBuscarArchivo.nombreDA, BinToNum(ucBuf,0), BinToNum(ucBuf,4), iplistaSectoresLibres.lista,iCantidadSectoresLibres);
									free (iplistaSectoresLibres.lista);
								}
								else
								{
									close(i);
									FD_CLR(i, &masterFds);
								}
							}
							else
							{
								header.ucPayloadDescriptor = IPCIRC_ERROR;								
								header.uiPayloadLength = strlen("Error en el modificado del tamanio");
								copiarU(ucBuf,(unsigned char *)"Error en el modificado del tamanio");
								enviarH(i,header);
								enviarAlgo(i,ucBuf,header.uiPayloadLength);
							}
							break;
							
						case IPCIRC_LIBRERARSECTORES: /* LiberarSectores() */
							header.ucPayloadDescriptor = 0x25;
							/* Respuesta LiberarSectores() */
							header.uiPayloadLength = sizeof("OK\n\n");
							if(enviarH(i, header) != -1)
							{
								copiarU(ucBuf,(unsigned char *)"OK\n\n");

								iCantidad = sizeof("OK\n\n");

								if(enviarAlgo(i, ucBuf, iCantidad) == -1)
								{
									close(i);
									FD_CLR(i, &masterFds);
								}
							}
							else
							{
								close(i);
								FD_CLR(i, &masterFds);
							}
							break;
							
						case IPCIRC_LISTARDIRECTORIO: /* ListarDirectorio() ********** srchFile.h * *** TESTEADO *******/
						
							listarArchivos(i, header, ldap);
							break;
							
						case IPCIRC_FORMATEAR: /* Formatear() */
						
							recibirAlgo(i,ucBuf,header.uiPayloadLength);
							ucBuf[header.uiPayloadLength]='\0';
							
							recibirAlgo(i,ucBuf2,12);
							iCantidad = BinToNum(ucBuf2,0) * BinToNum(ucBuf2,4) * BinToNum(ucBuf2,8);
							ldapFormatearDA(ldap.ldapSesion, (char*)ucBuf, iCantidad);
							
							header.ucPayloadDescriptor = IPCIRC_FORMATEAR_RTA;
							/* Respuesta Formatear() */
							if(enviarH(i, header) < 1)
							{
								close(i);
								FD_CLR(i, &masterFds);
							}
							break;
							
						case IPCIRC_AGEGARSECTORES:
							
							recibirAlgo(i,(unsigned char*) nombreDAS, header.uiPayloadLength);
							nombreDAS[header.uiPayloadLength] = '\0';

							recibirH(i,&header);
							recibirAlgo(i,(unsigned char*) nombreArchivoDA, header.uiPayloadLength);
							nombreArchivoDA[header.uiPayloadLength] = '\0';
							
							recibirAlgo(i,ucBuf,8);
							iSector1 = BinToNum(ucBuf,0);
							iSector2 = BinToNum(ucBuf,4);
							
							miitoa((char*)ucBuf,(long)iSector1);
							copiarU(ucBuf2, ucBuf);
							
							concatenar((char*)ucBuf2," ");
							miitoa((char*)ucBuf,(long)iSector2);
							concatenarU(ucBuf2, ucBuf);
							cargarLDAPBUSCARARCHIVO(&LdapBuscarArchivo, nombreDAS, nombreArchivoDA);
							ldapResultados = NULL;
							if (ldapBuscarArchivo(ldap.ldapSesion, LdapBuscarArchivo, &ldapResultados) != -1)
							{
								if (ldapResultados != NULL)
								{
									if (strcmp("a",ldapResultados->info.listaSectores) != 0)
									{
										copiar((char*)ucBuf,ldapResultados->info.listaSectores);
										concatenar((char*)ucBuf," ");
									}
									else copiar((char*)ucBuf,"");
									
									concatenarU(ucBuf,ucBuf2);
									
									cargarModifircarArchivo(&LdapModificarArchivo, nombreDAS, nombreArchivoDA, (char*)ucBuf);
									ldapModificarArchivo(ldap.ldapSesion, LdapModificarArchivo);
								}
							}
							break;
						
						/* Fin del Switch */
					}
				/*}*/
			}
		}
	}
	/*creaLog(archLog, "INFO", "Fin del Simplified File System.", 0, NULL);`*/
	return 1;
}

void *consola(void *parametroSinUso) /* Hay que implementar semaforos aca, sacar de GNUtella */
{
	char *cp, *cp2;
	char cvStringConsola[257], cvDirectorio[1000], cvNombreArchivo[1000];
	int i, j, k, l, q, iCantSectoresLeidos, iIdSectorAux, iSocketConsola;
	HEADERIRCIPC cabeceraConsola;
	ptrLdapResultados ldapResultados = NULL;
	pResultadoInformacion ldapResultadoInformacion = NULL;
	int iContadorDatosLdap=0;
	
	iSocketConsola = iniciarCliente(stArchConf.cvIP, stArchConf.iPuerto); /* sendrecv.h */
	
	mostrarMenu();	

	while(1)
	{
		fgets(cvStringConsola, 256, stdin);	
		cp = strtok(cvStringConsola, "(\n"); 
	
		if(strcmp(cvStringConsola, "listarDirectorio") == 0)
		{
			cp = strtok(NULL, ")");
			strcpy(cvDirectorio, cp);
			printf("\ListarDirectorio: \n\n");
			/* Usar función "listarArchivos(i, header, ldap);" */
		}
		else if(strcmp(cvStringConsola, "infoArchivo") == 0)
		{	
			printf("Info del directorio\n");
			cp = strtok(NULL, ",");
			strcpy(cvDirectorio, cp);
			cp = strtok(NULL, ")");
			strcpy(cvNombreArchivo, cp);
			
			cargarLDAPBUSCARARCHIVO(&LdapBuscarArchivo, cvDirectorio, cvNombreArchivo);
							
			if (ldapBuscarArchivo(ldap.ldapSesion, LdapBuscarArchivo, &ldapResultados) != -1)
			{
				if (ldapResultados != NULL)
				{	
					printf("INFOARCHIVO: Se encuentra exitosamente %s - %s:\n", cvDirectorio, cvNombreArchivo);
					cargarInfoArchivo (&ldapResultados, &ldapResultadoInformacion, &iContadorDatosLdap);
					/* Imprimir el resultado */
					free(ldapResultados);
					ldapResultados=NULL;
				}
				else
				{
					printf("INFOARCHIVO: No se encuentra %s - %s:\n", cvDirectorio, cvNombreArchivo);
				}
			}
		}			
		sleep(1);
	}
}

void mostrarMenu(void)
{
	printf("================================================================================\n");
	printf("\t\t\t\tMENU\n");
	printf("listarDirectorio([DA])\n>>>Imprimirá en pantalla el listado de archivos de dicho DA.\n\n");
	printf("infoArchivo([DA], [nombre de archivo])\n>>>Imprimirá en pantalla la información correspondiente al archivo pasado como parámetro.\n\n");
	printf("================================================================================\n\n");
}

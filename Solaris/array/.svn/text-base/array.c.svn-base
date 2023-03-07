/*#define _REENTRANT*/

#define MAX_PEDIDOS 100
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
#include <thread.h>
#include <time.h>
#include <pthread.h>
#include <thread.h>
#include "sendrecv.h"
#include "archivos.h"
#include "cadenas.h"
#include "protocolos.h"
#include "disco.h"
#include "consola.h"
#include "sincronizacion.h"
#include "syscall.h"



int terminado = 0;
void* hiloDisco(SDISCOSCONECTADOS *);
void* hiloConsola(CONFIGURACION*);

int main (void)
{
	fd_set masterFds;   		/* conjunto fd maestro para funcion select() */
	fd_set leeFds; 		/* conjunto fd temporal para funcion select() */
       struct timeval tv; 		/* estructura que define el tiempo que espera select por E/S, para q no sea bloqueante */
	int iServidor, fdMax, i, iSocket_aceptado;
	int iCliente = FALLA_SISTEMA;
	int ESTADO_ACTUAL = FALLA_SISTEMA, ESTADO_SUBACTUAL = FALLA_SISTEMA;
	int iResult;

	/* Cabecera: IPCIRC general
	   CabeceraSincronizacion: Para controlar el IDdescriptor de sincronizacion */
	HEADERIRCIPC cabecera;
	ptrDiscosEsperados listaDiscos;
	CONFIGURACION config;
	SCHS datosCHS;
	SSECTORES sSectores;
	SPEDIDO sPedidosSectores[100];

	/* Variables para sincronizacion */
	int iPosSincOrigen = FALLA_SISTEMA, iPosSincDest = FALLA_SISTEMA;

	/* Variable de selector de disco para GETSECTORES */
	int SELECTOR_DISCO1 = -1, SELECTOR_DISCO2 = -1;
	thread_t threadID;	
	
	srand((unsigned) time(NULL));
	system("clear");


	/****************************** CARGO CONFIGURACIONES **************************************/
	/* archivos.h */
	if (cargarConfiguracion (&config, &listaDiscos) == CONFIG_FAIL)
	{
		printf("Error al leer el archivo de configuracion\n");
		return 1;
	}

	
	/****************************** CONEXION COMO SERVIDOR *************************************/
	iServidor = iniciarServidor (config.cvArrayIP,config.iArrayPuerto, 20); /* sendrecv.h */
	
	switch (iServidor)
	{
		case SOCKET_INVALIDO: escribirLog ('E', "Socket invalido"); return 0; break;
		case SOCKET_ERROR_INICIALIZAR: escribirLog ('E', "No se pudo inializar el socket como servidor"); return 0; break;
	}

	/*******************************************************************************************/

	printf ("Iniciando exitosamente\n");
	escribirLog ('I', "Iniciando exitosamente");

	FD_ZERO(&masterFds);     /* borramos los dos conjuntos */
	FD_ZERO(&leeFds);
	FD_SET(iServidor, &masterFds);
	fdMax=iServidor;

	SDISCOSCONECTADOS EstructuraArray[config.iCantidadDiscos];
	SDISCOSSINCRONIZANDOS EstructuraSincronizando[config.iCantidadDiscos];
	SDISCOSSINCRONIZANDOS datosParaSincronizar;

	/* Inicializo estructuras */
	for(i = 0; i<config.iCantidadDiscos; EstructuraSincronizando[i].cpDescriptorID[0]='\0', i++);
	limpiarPedidos(sPedidosSectores,MAX_PEDIDOS);

	/* disco.h */
	respaldarLista(EstructuraArray, &listaDiscos, iServidor, config.iCantidadDiscos, config.cvArrayIP,config.iArrayPuerto);
	datosCHS.iCilindros = config.iCilindros;
	datosCHS.iCabezas= config.iCabezas;
	datosCHS.iSectores= config.iSectores;
	cabecera.ucPayloadDescriptor = 0x00;
	/****************************** CARGO VALOR INICIAL DE ESTADO DEL ARRAY ************************/
	ESTADO_ACTUAL = ESTADO_FUERASERVICIO;

	if (listaDiscos != NULL)
	{
		ESTADO_SUBACTUAL = ESTADO_FUERASERVICIO_INCOMPLETO;
		escribirLog ('I', "ESTADO ARRAY: Fuera de Servicio - Incompleto");
	}
	else
	{ 
		ESTADO_SUBACTUAL = ESTADO_FUERASERVICIO_CORRUPTO;
		escribirLog ('I', "ESTADO ARRAY: Fuera de Servicio - Corrupto");
	}

	/******************************	ESTADO DEL SISTEMA   ****************************************/
	/* consola.h */
	mostrarEstadoSistema(EstructuraArray, ESTADO_ACTUAL, &ESTADO_SUBACTUAL, config);
	

	/*****************************   CONSOLA    ****************************************************/	
	iResult = thr_create(NULL, 0, (void*)hiloConsola,&config ,THR_DETACHED|THR_DAEMON|THR_NEW_LWP, &threadID);
	if  (iResult != 0) escribirLog ('E', "El hiloConsola no se pudo crear");

	/****************************** CICLO PRINCIPAL DE TRABAJO *************************************/

	while( terminado == 0) 
	{
		leeFds = masterFds; /* Lo copiamos porque la funcion select lo va a modificar interiormente */
		tv.tv_sec = 0; 		/* cant segundos */
		tv.tv_usec = 1; 	/* cant microsegundos */        
	
		while(select(fdMax+1, &leeFds, NULL, NULL, &tv) == -1)
        	{
			if(errno != EINTR)
              	{
				perror("MAIN select()");
				escribirLog ('E', "Select errno != EINTR");
				return 1;
			}
        	}
  
   		/* busca peticiones/conexiones */ 
                 
		for(i = 0; i <= fdMax; i++) 
		{
			if(FD_ISSET(i, &leeFds)) 
      			{ 
              		if (i == iServidor) /* si es el server */
              		{
			       	iSocket_aceptado = aceptarConexion (iServidor); /*sendrecv.h*/

					if (iSocket_aceptado != SOCKET_INVALIDO)
					{
						FD_SET(iSocket_aceptado, &masterFds); /* lo mandamos al maestro */
                           		       if (iSocket_aceptado > fdMax) /* actualizamos maximo */
        						fdMax = iSocket_aceptado;
					}
					else escribirLog ('I', "Conexion rechazada");                                        			
					
				
				}
				else /* Hay que ver quien entra */
				{
				/*************** ATENCION DEL CLIENTE *************/
					iResult = recibirH(i, &cabecera);
					if (iResult <= 0)
					{
						close(i);
       	                          	FD_CLR(i,&masterFds);
						if (iCliente == i) 
						{
							printf("Se desconectó el VSK/ARRAY, Finalizando\n");
							terminado = 1;
						}
						else
						{
							desconectarDisco(i,EstructuraArray, config.iCantidadDiscos);
							mostrarEstadoSistema(EstructuraArray, ESTADO_ACTUAL, &ESTADO_SUBACTUAL, config);
						}

						break;
					}
					else
					{
						switch (cabecera.ucPayloadDescriptor)
						{
							case HANDSHAKE_DISCO:
										/* disco.h */
								iResult = recibirDatosDisco(i, EstructuraArray, config.iCantidadDiscos, datosCHS);
								switch(iResult)
								{
									case SOCKET_ERROR_DESCONEXION: case SOCKET_ERROR_SENDRECV:
										escribirLog ('E', "Error de conexion con DISCO/ARRAY");
										close(i);
                                 						FD_CLR(i,&masterFds);
										break;
	
									case ARRAY_NO_HAY_ESPACIO:
										escribirLog ('I', "No hay espacio en ARRAY");
										close(i);
                           	      						FD_CLR(i,&masterFds);
										break;
									
									case ARRAY_DISCO_RECHAZADO:
										escribirLog ('I', "El DISCO/ARRAY no cumple con CHS");
										close(i);
              	                   					FD_CLR(i,&masterFds);
										break;
									default:
										iResult = thr_create(NULL, 0, (void*)hiloDisco,&EstructuraArray[iResult] ,0, &threadID);
										FD_CLR(i,&masterFds);
										if  (iResult != 0) escribirLog ('I', "Hilo iniciado");
										break;
	
								}
								break;

							case IPCIRC_HILO:
									/* disco.h */ 
								asociarSocketHilo(i, EstructuraArray, cabecera,config.iCantidadDiscos);
								mostrarEstadoSistema(EstructuraArray, ESTADO_ACTUAL, &ESTADO_SUBACTUAL, config);
								break;
	
							case IPCIRC_CONSOLA:
									/* consola.h */
								iResult = ejecutarComando(i,EstructuraArray, config.iCantidadDiscos, &ESTADO_ACTUAL, &ESTADO_SUBACTUAL,&iPosSincDest);
								switch(iResult)
								{
									case SOCKET_ERROR_DESCONEXION:
										escribirLog ('I', "Se ha desctonectado la CONSOLA"); 
										close(i);
       	                          					FD_CLR(i,&masterFds);
										break;
	
									case CONSOLA_ELIMINAR_NO_SINCRONIZADOS: case CONSOLA_SERVICIO: case CONSOLA_ELIMINAR_DISCO_NO_ENCONTRADO:
											break;

									case CONSOLA_ESTADO:
										mostrarEstadoSistema(EstructuraArray, ESTADO_ACTUAL, &ESTADO_SUBACTUAL, config);
										break;

									case CONSOLA_ELIMINAR_DISCO: /* Elimino la conexion del maestro del hilo */
										close(EstructuraArray[iPosSincDest].iSocketHilo);
										if(EstructuraArray[iPosSincDest].iEstadoDisco != DISCO_ESTADO_NO_CONECTADO)
                     	            						FD_CLR(EstructuraArray[iPosSincDest].iSocketHilo,&masterFds);
										/* disco.h */
										desconectarDiscoPorNombre(EstructuraArray[iPosSincDest].cpNombreDisco, EstructuraArray, config.iCantidadDiscos);
										iPosSincDest = FALLA_SISTEMA;
										/* consola.h */
										mostrarEstadoSistema(EstructuraArray, ESTADO_ACTUAL, &ESTADO_SUBACTUAL, config);
										break;

									case CONSOLA_AGREGAR_DISCO:
										/* Inicia proceso de sincronizacion 
										la posicion en el EstructuraArray es iPosSincDest*/
										switch(iPosSincDest)
										{
											case CONSOLA_AGREGAR_DISCOINEXISTENTE: case CONSOLA_DISCO_SINCRONIZADO:
												iPosSincDest = FALLA_SISTEMA;
												break;
											default: /* hay que sincronizar , iPosSincDest contiene la posicion del disco a sincronizar*/
														/* sincronizacion.h */
												iPosSincOrigen = iniciarSoncronizacion(EstructuraArray, iPosSincDest, config.iCantidadDiscos);
												switch(iPosSincOrigen)
												{
													case SINCRONIZACION_NO_INICIAR:
														mostrarEstadoSistema(EstructuraArray, ESTADO_ACTUAL, &ESTADO_SUBACTUAL, config);
														break;
	
													default:  /* sendrecv.h */
															copiarCabecera(datosParaSincronizar.cpDescriptorID, cabecera.cpDescriptorID);
															datosParaSincronizar.iIdSectorActual = 0;
															datosParaSincronizar.iPosVecOrigen = iPosSincOrigen;
															datosParaSincronizar.iPosVecDest = iPosSincDest;

														if (hayOtroSincronizando(EstructuraSincronizando,config.iCantidadDiscos) == SINCRONIZACION_NO_HAY_OTRO_SINCRONIZANDO)
														{	/* sincronizacion.h */
															
															cargarDiscoASincronizar(EstructuraSincronizando,config.iCantidadDiscos, datosParaSincronizar);
															/* sincronizacion.h */
															sincronizarDisco(EstructuraArray, datosCHS, EstructuraSincronizando);
															/* consola.h */
															mostrarEstadoSistema(EstructuraArray, ESTADO_ACTUAL, &ESTADO_SUBACTUAL, config);
															
														} else cargarDiscoASincronizar(EstructuraSincronizando,config.iCantidadDiscos, datosParaSincronizar);
												}
												break;
										}
										
										break;

								}
								break;
	
							case IPCIRC_GET_SECTORES1: case IPCIRC_GET_SECTORES2: case IPCIRC_PUT_SECTORES: case IPCIRC_GET_CHS:
								switch(cabecera.ucPayloadDescriptor)
								{
									case IPCIRC_GET_SECTORES1: case IPCIRC_GET_SECTORES2: 
										recibirPedido_getSectores(i,cabecera,&sSectores);
										
										if (ESTADO_ACTUAL == ESTADO_FUERASERVICIO)
										{
											cabecera.ucPayloadDescriptor = IPCIRC_FUERA_SERVICIO;
											cabecera.uiPayloadLength = strlen("FUERA SERVICIO");
											enviarH(i, cabecera);
											enviarAlgo(i,(unsigned char*)"FUERA SERVICIO",cabecera.uiPayloadLength);
										}
										else
										{			/* disco.h */
											iResult = elegirDisco(EstructuraArray, config.iCantidadDiscos, &SELECTOR_DISCO1, &SELECTOR_DISCO2);
											switch(iResult)
											{
												case FALLA_SISTEMA: 
													cabecera.ucPayloadDescriptor = IPCIRC_FALLA;
													cabecera.uiPayloadLength = longitudCadena("No hay disco disponible");	
													enviarH(i,cabecera);
													enviarAlgo(i,(unsigned char*)"No hay disco disponible",cabecera.uiPayloadLength);
													break;
												default:
														/* syscall.h */
													if(SELECTOR_DISCO1 == SELECTOR_DISCO2) 
													{	
														enviarPedido_getSectoresDosDiscos(EstructuraArray[SELECTOR_DISCO1].iSocketHilo, cabecera, sSectores);
													}
													else
													{
														enviarPedido_getSectoresUnDisco(EstructuraArray[SELECTOR_DISCO1].iSocketHilo,cabecera,sSectores.iIdSector1);
														enviarPedido_getSectoresUnDisco(EstructuraArray[SELECTOR_DISCO2].iSocketHilo,cabecera,sSectores.iIdSector2);
														
													/* No hay problema si se pasa del limite del array porque elegirDisco lo valida */
													}
													
													agregarPedido(sPedidosSectores, MAX_PEDIDOS, cabecera, sSectores.iIdSector1, sSectores.iIdSector2, EstructuraArray[SELECTOR_DISCO1].iSocketHilo, EstructuraArray[SELECTOR_DISCO2].iSocketHilo);
													break;
													
											}
											
										}		
										
									break; 

									case IPCIRC_GET_CHS:
										enviar_getCHS(i,cabecera, config);
										break;
									case IPCIRC_PUT_SECTORES:
										 /* revisar si se manda a todos, que me parece que si */
										recibirPedido_putSectores(i, cabecera,&sSectores); /* syscal.h disco.h */
										repartirPutSectores(EstructuraArray, config.iCantidadDiscos,cabecera ,sSectores, EstructuraSincronizando, datosCHS);
											
										break;

								}
								
	
								break;	

							
							case IPCIRC_GET_SECTORES_RTA:
											/* sincronizacion.h */
								iResult = enSincronizacion(cabecera, EstructuraSincronizando);
								switch(iResult)
								{
									case SINCRONIZACION_NO_ES_SINCRONIZACION:
										recibirRespuesta_getSectores(i, cabecera, &sSectores);
										enviarRespuesta_getSectores(iCliente, cabecera, sSectores);
									break;
									case SINCRONIZACION_FIN:case SINCRONIZACION_CONTINUAR:
										/* debo continuar la sincronizacion */ /* sincronizacion.h */
										iResult = continuarSincronizacion(i,EstructuraArray, datosCHS, EstructuraSincronizando, config.iCantidadDiscos, cabecera);
										if (iResult == SINCRONIZACION_FIN) mostrarEstadoSistema(EstructuraArray, ESTADO_ACTUAL, &ESTADO_SUBACTUAL, config);
										break;
								}
								break;
								
							case  IPCIRC_GET_SECTORES_RTAb:
								
								iResult = agregarRespuesta(i, cabecera, sPedidosSectores, MAX_PEDIDOS);
								if (iResult>(-1))
								{
									enviarRespuesta_getSectores(iCliente, cabecera, sPedidosSectores[iResult].sectores);
									eliminarPedido(sPedidosSectores, iResult);
								}

								break;
							case IPCIRC_PUT_SECTORES_OK: 
								enviarH(iCliente,cabecera);
								break;
							case IPCIRC_FINALIZAR_SISTEMA:
								terminado = 1;
								break;

							case IPCIRC_FALLA:

							case HANDSHAKE_OK:
								printf("ACEPTADO\n");
								escribirLog ('I', "ACEPTADO");
								break;
							
							default: /* EL PROTOCOLO ES INCORRECO */
								printf("OVNI: NOS ATACAN LOS VIRUS!!!!!!!!, protocolo: %d\n", cabecera.ucPayloadDescriptor);
								escribirLog ('I', "ERROR DE PROTOCOLO");
								close(i);
       	                    	      			FD_CLR(i,&masterFds);
								break;
	
						} 
					}

				} /* Fin de ATENDER CLIENTE */
			} /* Fin del FD_ISSET */
	
			switch (ESTADO_ACTUAL)
			{
				case ESTADO_FUERASERVICIO:
				/****************************** CONEXION COMO CLIENTE AL VSK - ARRAY **************************************/
					if (((ESTADO_SUBACTUAL == ESTADO_INICIALIZADO_SINCRONIZADO) || (ESTADO_SUBACTUAL == ESTADO_FUERASERVICIO_SINCRONIZADO))&& (iCliente == FALLA_SISTEMA))
					{
						iCliente = iniciarCliente (config.cvVSKIP, config.iVSKPuerto); /* sendrecv.h */
	
		
						switch (iCliente )
						{
							case SOCKET_INVALIDO: escribirLog ('E', "Socket invalido"); return 0; break;
							case SOCKET_ERROR_CONEXION: escribirLog ('E', "Socket error de conexion"); return 0; break;
						}
	
						printf ("Conexion exitosa al VSK-ARRAY\n");
						escribirLog ('I', "Conexion exitosa al VSK-ARRAY");
						/*ESTADO_ACTUAL = ESTADO_FUNCIONAL;*/
						crearCabecera (&cabecera,HANDSHAKE_VSK, 0);
						enviarH(iCliente,cabecera);

						cabecera.uiPayloadLength = longitudCadena(config.cvNombreArray);
						enviarH(iCliente,cabecera);
						enviarAlgo(iCliente, (unsigned char*)config.cvNombreArray,cabecera.uiPayloadLength);
						FD_SET(iCliente, &masterFds);
						if (iCliente > fdMax) /* actualizamos maximo */
        						fdMax = iCliente;
					}
				

				break;
			} /* Fin del switch */
		
		enviarOtroASincronizar(EstructuraArray, datosCHS, EstructuraSincronizando, config.iCantidadDiscos);
		} /* Fin del for */
	} /* Fin del WHILE de trabajo principal */
	return 1;
}

void* hiloDisco(SDISCOSCONECTADOS* sDisco)
{
	/* disco.h */
	atenderDisco(sDisco);
	printf("Se desconecta DISCO: %s\n",sDisco->cpNombreDisco);
	return (NULL);
}
void* hiloConsola(CONFIGURACION* config)
{
	/* consola.h */
	consola(config);
	return (NULL);
}


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
#include <ldap.h>
#include <pthread.h>
#include <fcntl.h>
#include "sendrecv.h"
#include "archivos.h"
#include "cadenas.h"
#include "consola.h"
#include "protocolo.h"
#include "syscalls.h"
#include "oft.h"
#include "fd.h"
#include "sysopenN.h"
#include "sysclose.h"
#include "syslist.h"
#include "sysread.h"
#include "syswrite.h"
#include "funcionesldap.h"

CONFIGURACION datos;
void desconexion (int quien);

int main (void)
{
	fd_set masterFds;   		/* conjunto fd maestro para funcion select() */
	fd_set leeFds; 		/* conjunto fd temporal para funcion select() */
       struct timeval tv; 		/* estructura que define el tiempo que espera select por E/S, para q no sea bloqueante */
	int iServidor, fdMax, i, iSocket_aceptado;
	srand((unsigned) time(NULL));
	HEADERIRCIPC cabecera;
	int enProceso = 1;
	int fd;

	/* variables de error y mensajes */
	char cvMensaje[1000], cvMensaje2[1000];
	unsigned char ucvMensaje[2000];
	unsigned char* ucpPedidos;
	int iC, iH, iS;

	/* variables de conexiones */
	pConexiones L_Conexiones = NULL;
	pConexiones Aux_Conexiones = NULL;
	INFOCONEXIONES infoConexion;

	/* variables de OFT */
	pOft L_OFT = NULL;
	pOft Aux_OFT = NULL;
	unsigned char ucvNombreArchivo[140];

	/* variables FD */
	INFOFD unfd;
	pNodoFD L_FD = NULL;
	pNodoFD Aux_FD = NULL;
	unsigned char* ucpMensajes;

	/* bandera para terminacion de una SYSTEM CALL*/
	int iTerminado = 0;

	/* variables LDAP */
	pLDAPINIT sesionLdap;

		/*LDAPSECTOR unSector; char dnViejo[1000]; char bufferP[513]; int iBloque1;*/ /**************/
	system("clear");

	if (cargarConfiguracion (&datos) != 1)
	{
		escribirLog ('E', "No se cargo el archivo de configuración");
	}
	mostrarConfiguracion (datos);
	/* cargo datos de ldap e inicio la sesion si esta habilitada la cache */
	if (datos.iCache == 1)
	{
		ldapCargarSesion(&sesionLdap, datos.cvIPLdap, datos.iPuertoLdap, datos.cvContraseniaLdap, datos.iTamanioCache);
		ldapIniciarSesion (&sesionLdap);
		/*ldapBuscarMasViejoCache(sesionLdap->ldapSesion, dnMinimo); */
		/*printf("DN minimo: %s\n", dnMinimo);*/

	}


	/*cargarSector(&unSector, "DA1", "7", "", 0);*/
	/*ldapBuscarCacheParaInvalidar(sesionLdap->ldapSesion, unSector);*/
	/*iBloque1 = ldapBuscarCache(sesionLdap->ldapSesion, unSector, bufferP);
	printf ("Hubo %d, contenido: %s\n", iBloque1, bufferP);*/
	/*ldapCargarCache(sesionLdap->ldapSesion, unSector);*/
	/*ldapBuscarMasViejoCache(sesionLdap->ldapSesion, dnViejo);
	ldapModificarContenidoCache (sesionLdap->ldapSesion, dnViejo, "actualizacion de 5");
	ldapModificarCache(sesionLdap->ldapSesion, dnViejo);*/
/*return 1;*/
	/****************************** CONEXION COMO SERVIDOR *************************************/
	
	iServidor = iniciarServidor (datos.cvIP,datos.iPuerto, 30); /*sendrecv.h*/
	
	switch (iServidor)
	{
		case SOCKET_INVALIDO: escribirLog ('E', "Socket invalido"); return 0; break;
		case SOCKET_ERROR_INICIALIZAR: escribirLog ('E', "No se pudo inializar el socket como servidor"); return 0; break;
	}

	/********************************************************************************/

	printf ("Iniciando exitosamente\n");
	escribirLog ('I', "Iniciando exitosamente");
	
	/* ************ INICIANDO CONSOLA ************************************ */

	FD_ZERO(&masterFds);     /* borramos los dos conjuntos */
	FD_ZERO(&leeFds);
	FD_SET(iServidor, &masterFds);
	fdMax=iServidor;
	
	/* ***********AGREGAMOS EL TECLADO************************************** */
	fd = fcntl(STDIN_FILENO,  F_DUPFD, 0);
	FD_SET(fd, &masterFds);
	if (fd > fdMax) fdMax = fd;

	/*pthread_create(&hilo0, NULL, (void*)&mandarConsola, NULL);*/


	while(enProceso) 
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
					
				} /* Fin verificar si alguien envio algo */
				/**************** PARA EL TECLADO **********************/
				else if (i == fd)
				{
					switch(consola(&cabecera, ucvMensaje))
					{
							case COMANDO_GET_SECTORES:
								Aux_Conexiones = buscarConexionPorTipo ( &L_Conexiones, ARRAY);
								if (Aux_Conexiones == NULL) 
								{
									printf ("No hay dispositivo disponible\n");
									break;
								}
								cabecera.ucPayloadDescriptor = GETSECTORES2;	/*sysread.h*/						
								enviarH(Aux_Conexiones->conexion.iSocket, cabecera);
								enviarAlgo(Aux_Conexiones->conexion.iSocket,ucvMensaje, cabecera.uiPayloadLength);
								break;

							case COMANDO_PUT_SECTORES:
								Aux_Conexiones = buscarConexionPorTipo ( &L_Conexiones, ARRAY);
								if (Aux_Conexiones == NULL) 
								{
									printf ("No hay dispositivo disponible\n");
									break;
								}
								cabecera.ucPayloadDescriptor = PUTSECTORES; /*syswrite.h*/							
								enviarH(Aux_Conexiones->conexion.iSocket, cabecera);
								enviarAlgo(Aux_Conexiones->conexion.iSocket,ucvMensaje, cabecera.uiPayloadLength);
								break;

							case COMANDO_GET_CHS:
								/* consola.h */
								cabecera.ucPayloadDescriptor = GETCHS;
								Aux_Conexiones = buscarConexionPorTipo ( &L_Conexiones, ARRAY);
								if (Aux_Conexiones == NULL) 
								{
									printf ("No hay dispositivo disponible\n");
									break;
								}
								enviarH(Aux_Conexiones->conexion.iSocket, cabecera);
								break;
								
							case COMANDO_MONTAR:
								Aux_Conexiones = buscarConexionPorDA (&L_Conexiones, ucvMensaje);
								if (Aux_Conexiones != NULL)
								{
									Aux_Conexiones->conexion.cMontado = 1;
									printf ("Dispositivo Montado\n");
								}
								else printf ("Dispositivo inexistente\n");
								break;
								
							case COMANDO_DESMONTAR:
								Aux_Conexiones = buscarConexionPorDA (&L_Conexiones, ucvMensaje);
								if (Aux_Conexiones != NULL)
								{
									Aux_Conexiones->conexion.cMontado = 0;
									printf ("Dispositivo Desmontado\n");
								}
								else printf ("Dispositivo inexistente\n");
								
								break;
								
							case COMANDO_FORMATEAR:
								Aux_Conexiones = buscarConexionPorDA (&L_Conexiones, ucvMensaje);
								if ( (Aux_Conexiones != NULL) && (Aux_Conexiones->conexion.cMontado == 1))
								{
									copiar (cvMensaje2, (char*)Aux_Conexiones->conexion.ucvNombre);
									/* pido caracteristicas del CHS */
									cabecera.ucPayloadDescriptor = GETCHS;
									enviarH (Aux_Conexiones->conexion.iSocket, cabecera);
									recibirH (Aux_Conexiones->conexion.iSocket, &cabecera);
									recibirAlgo (Aux_Conexiones->conexion.iSocket, ucvMensaje, cabecera.uiPayloadLength);
									/* busco el SFS */
									Aux_Conexiones = buscarConexionPorTipo ( &L_Conexiones, SFS);
									cabecera.ucPayloadDescriptor = FORMATEAR_IDA;
									cabecera.uiPayloadLength = strlen (cvMensaje2);
									/* envio al SFS el DA */
									enviarH (Aux_Conexiones->conexion.iSocket, cabecera);
									enviarAlgo (Aux_Conexiones->conexion.iSocket, (unsigned char *)cvMensaje2, cabecera.uiPayloadLength);
									/* enviar CHS */
									enviarAlgo (Aux_Conexiones->conexion.iSocket, ucvMensaje, 12);
									recibirH (Aux_Conexiones->conexion.iSocket, &cabecera);
									if (cabecera.ucPayloadDescriptor == FORMATEAR_VUELTA)
									{
										printf("Formateo exitoso \n");
									}
								}
								else
									printf("El dispositivo no se ha montado o no existe\n");
								break;
							case COMANDO_SALIR:
								enProceso = 0;
								break;
					}
								
							
				}
				else /* Hay que ver quien entra */
				{
					/*************** ATENCION DEL CLIENTE*/
					if (recibirH(i, &cabecera) != SOCKET_OK_SENDRECV)
					{	
						/*DETECTAR CAIDA*/
						quitarConexionPorSocket(&L_Conexiones, i, &infoConexion); /*protocolo.h*/
						desconexion(infoConexion.iTipo);
						FD_CLR(i, &masterFds);
						close (i);
						sprintf (cvMensaje, "Error al recibir. Desconexion %x", infoConexion.iTipo);
						escribirLog ('I', cvMensaje);
					}
					else
						switch(cabecera.ucPayloadDescriptor)
						{
							case CONSOLA:
								printf("CONSOLA\n");
								infoConexion.iTipo = cabecera.ucPayloadDescriptor;
								infoConexion.iSocket = i;
								meterConexion(&L_Conexiones, infoConexion);
								break;
							/*##################################################*/
							case ARRAY : /*igual disco */ case SFS : case FTP : case CLIENTE: /*en protocolo.h */
								/* COINCIDEN TIPO CON UCPAYLOADDESCRIPTOR */
								infoConexion.iTipo = cabecera.ucPayloadDescriptor;
								infoConexion.iSocket = i;
								infoConexion.cMontado = 0;
								/* recibir nombre de dispositivo */
								recibirH(i, &cabecera);
								recibirAlgo (i, infoConexion.ucvNombre, cabecera.uiPayloadLength);
								infoConexion.ucvNombre[cabecera.uiPayloadLength]='\0';
								if (meterConexion(&L_Conexiones, infoConexion)) /*protocolo.h*/
								{
									sprintf(cvMensaje, "Conexion aceptada de %s", infoConexion.ucvNombre);
									escribirLog('I', cvMensaje);
									cabecera.ucPayloadDescriptor = HANDSHAKE_OK;
									enviarH (i, cabecera);
									printf("HANDSHAKE OK\n");
								}
								else
								{
									cabecera.ucPayloadDescriptor = HANDSHAKE_FAIL;
									enviarH (i, cabecera);
									printf("HANDSHAKE FAIL\n");

								}
								break;
							/*##################################################*/
							case SYS_OPEN: /*en protocolo.h*/
								/* cargo un fd */
								cargarInfoFD (&unfd, cabecera.cpDescriptorID, i, -1, SYSCALL_OPEN, 0);
								/* lo meto en la lista de fd y es el primero */
								meterFd (&L_FD, unfd);
								/* busco el socket del SFS */
								Aux_Conexiones = buscarConexionPorTipo (&L_Conexiones, SFS);
								/* recibo la informacion que indica si es lectura, escritura o eliminacion */
								ucpPedidos = (unsigned char*) malloc (cabecera.uiPayloadLength+1);
								ucpPedidos[cabecera.uiPayloadLength] = '\0';
								recibirAlgo (i, ucpPedidos, cabecera.uiPayloadLength);
								separarParametrosOpen (ucpPedidos, ucvNombreArchivo, &cabecera.ucPayloadDescriptor); /* sysopen.h */
								obtenerNombres (ucvNombreArchivo, L_FD->fd.ucvDA, L_FD->fd.ucvNombreArchivo); /* oft.h*/ /*saco DA y NomArch */
								/* llamada al sistema */
								sys_open (Aux_Conexiones->conexion.iSocket, i, &L_OFT, &L_FD, cabecera);
								free (ucpPedidos);
								break;

							case SYS_CLOSE:
								/* busco el fd del socket puesto que esta operacion requiere que ya
								haya hecho un sys_open por lo que su fd sigue estando */
								printf("En sysclose\n");
								Aux_FD = buscarFdPorSocket (&L_FD, i);
								/* le actualizo ahora que system call utiliza */
								Aux_FD->fd.iSysCall = SYSCALL_CLOSE;
								Aux_FD->fd.iPaso = 0;
								/* busco el socket del SFS */
								Aux_Conexiones = buscarConexionPorTipo (&L_Conexiones, SFS);
								/* llamada al sistema */
								if (sys_close(Aux_Conexiones->conexion.iSocket, i, &Aux_FD, &L_OFT, cabecera) == 1)
								{
									cabecera.ucPayloadDescriptor = CLOSE_FILE_OK;									
									enviarH(Aux_FD->fd.iSocket,cabecera);
									FD_CLR(Aux_FD->fd.iSocket, &masterFds);
									close (Aux_FD->fd.iSocket);
									quitarFdPorSocket(&L_FD, Aux_FD->fd.iSocket);
								}
								break;

							case SYS_LIST:
								/* cargo un fd */
								cargarInfoFD (&unfd, cabecera.cpDescriptorID, i, -1, SYSCALL_LIST, -1);
								/* lo meto en la lista de fd y es el primero */
								meterFd (&L_FD, unfd);
								/* busco el socket del SFS */
								Aux_Conexiones = buscarConexionPorTipo (&L_Conexiones, SFS);
								/* llamada al sistema */
								sys_list (Aux_Conexiones->conexion.iSocket, i, &L_FD, &L_Conexiones, cabecera);
								break;

							case SYS_READ:
								/* busco el fd del socket puesto que esta operacion requiere que ya
								haya hecho un sys_open por lo que su fd sigue estando */
								Aux_FD = buscarFdPorSocket (&L_FD, i);
								/* le actualizo ahora que system call utiliza */
								Aux_FD->fd.iSysCall = SYSCALL_READ;
								Aux_FD->fd.iPaso = 0;
								/* llamada al sistema */
								sys_read (Aux_Conexiones->conexion.iSocket, i, &Aux_FD, cabecera, &L_Conexiones, datos.iCache, &sesionLdap);
								break;

							case SYS_WRITE:
								/* busco el fd del socket puesto que esta operacion requiere que ya
								haya hecho un sys_open por lo que su fd sigue estando */
								Aux_FD = buscarFdPorSocket (&L_FD, i);
								/* le actualizo ahora que system call utiliza */
								Aux_FD->fd.iSysCall = SYSCALL_WRITE;
								Aux_FD->fd.iPaso = 0;
								/* llamada al sistema */
								sys_write (Aux_Conexiones->conexion.iSocket, i, &Aux_FD, &L_OFT, cabecera, &L_Conexiones, datos.iCache, &sesionLdap);
								break;

							case ESCRITURA_ARCHIVO_EXISTE: 
							case ESCRITURA_ARCHIVO_INEXISTENTE:
							case ESCRITURA_ARCHIVO_ELIMINADO:
							case ESCRITURA_ARCHIVO_CREADO:	
							case LECTURA_ARCHIVO_EXISTE:
							case LECTURA_ARCHIVO_INEXISTENTE:
							case LECTURA_ARCHIVO_INFOVUELTA:
							case ELIMINAR_ARCHIVO_EXISTE:
							case ELIMINAR_ARCHIVO_INEXISTENTE:
							case ACTUALIZARTAMANIO_VUELTA: case DOSSECTORESLIBRES_VUELTA: 
							case COMANDO_GET_SECTORES_RTA: case COMANDO_PUT_SECTORES_RTA: /*en consola.h*/ 
							case COMANDO_GET_CHS_RTA: case LISTARDIRECTORIO_VUELTA:
							case ELIMINARARCHIVO_VUELTA:
							/* ############################# verificar constantes*/
								/* se verifica que descriptor id lo manda y se busca en L_FD */
								Aux_FD = buscarFdPorDescriptorID (&L_FD, cabecera.cpDescriptorID);
								if (Aux_FD == NULL )
								{
									/* imprimir por pantalla */
									if (cabecera.ucPayloadDescriptor == COMANDO_GET_SECTORES_RTA)
									{	
										recibirAlgo(i,ucvMensaje, cabecera.uiPayloadLength/2);
										ucvMensaje[512]='\0';
										recibirAlgo(i,&ucvMensaje[513], cabecera.uiPayloadLength/2);
										ucvMensaje[1026]='\0';
										printf("Primer sector: %s\n", ucvMensaje);
										printf("Segundo sector: %s\n", &ucvMensaje[513]);
										
									}
									else if (cabecera.ucPayloadDescriptor == COMANDO_GET_CHS_RTA)
									{
										recibirAlgo(i,ucvMensaje, cabecera.uiPayloadLength);
										iC = (int) BinToNum (ucvMensaje, 0);
										iH = (int) BinToNum (ucvMensaje, 4);
										iS = (int) BinToNum (ucvMensaje, 8);
										printf("Infomacion C:%d, H:%d, S:%d\n", iC, iH, iS);
									}
									else if (cabecera.ucPayloadDescriptor == COMANDO_PUT_SECTORES_RTA)
										printf("Escritura exitosa\n");
									
									break;
								}
								/* busco el socket del SFS */
								Aux_Conexiones = buscarConexionPorTipo ( &L_Conexiones, SFS);
								Aux_FD =  buscarFdPorDescriptorID (&L_FD, cabecera.cpDescriptorID);
								/* llamada al sistema segun la llamada al sistema que figura en Aux_FD */
								switch (Aux_FD->fd.iSysCall )
								{
									/* cada uno tiene su .h correspondiente */
									case SYSCALL_OPEN:  iTerminado = sys_open (Aux_Conexiones->conexion.iSocket, i, &L_OFT, &Aux_FD, cabecera); break;
									case SYSCALL_CLOSE: iTerminado = sys_close(Aux_Conexiones->conexion.iSocket, Aux_FD->fd.iSocket, &Aux_FD, &L_OFT, cabecera); break;
									case SYSCALL_LIST: iTerminado = sys_list (Aux_Conexiones->conexion.iSocket, Aux_FD->fd.iSocket, &Aux_FD, &L_Conexiones, cabecera); break;
									case SYSCALL_READ: sys_read (Aux_FD->fd.iSocket, i, &Aux_FD, cabecera, &L_Conexiones, datos.iCache, &sesionLdap); break;
									case SYSCALL_WRITE: sys_write (Aux_Conexiones->conexion.iSocket, Aux_FD->fd.iSocket, &Aux_FD, &L_OFT, cabecera, &L_Conexiones, datos.iCache, &sesionLdap); break;

								}
								switch (iTerminado)
								{
									case 1:
										/* hay que eliminar el FD correspondiente al cliente porque
										ya no va a ser usado */
										cabecera.ucPayloadDescriptor = 0x92;									
										enviarH(Aux_FD->fd.iSocket,cabecera);
										quitarFdPorSocket(&L_FD, Aux_FD->fd.iSocket);
										FD_CLR(Aux_FD->fd.iSocket, &masterFds);
										close (Aux_FD->fd.iSocket);
										iTerminado = 0;
									break;
									
									case 2: /*fin SYSOPEN correcto */
										cabecera.ucPayloadDescriptor = OPEN_FILE_OK;									
										enviarH(Aux_FD->fd.iSocket,cabecera);
										iTerminado = 0;
									break;
									case 3: /*fin SYSOPEN fallido */
										cabecera.ucPayloadDescriptor = OPEN_FILE_FAILED;									
										enviarH(Aux_FD->fd.iSocket,cabecera);
										iTerminado = 0;
										break;
									case 4:
										cabecera.ucPayloadDescriptor = CLOSE_FILE_OK;									
										enviarH(Aux_FD->fd.iSocket,cabecera);
										FD_CLR(Aux_FD->fd.iSocket, &masterFds);
										close (Aux_FD->fd.iSocket);
										quitarFdPorSocket(&L_FD, Aux_FD->fd.iSocket);
										
										iTerminado = 0;
								}
								
								break;

							
							case ERROR:
								ucpMensajes = (unsigned char*) malloc (cabecera.uiPayloadLength +1);
								recibirAlgo (i, ucpMensajes, cabecera.uiPayloadLength);
								ucpMensajes[cabecera.uiPayloadLength]='\0';
								/* enviar a quien corresponda y como corresponda */
								Aux_FD=buscarFdPorDescriptorID (&L_FD, cabecera.cpDescriptorID);
								if ( Aux_FD != NULL)
								{
									Aux_OFT = Aux_FD->fd.pDescriptor;									
									if (Aux_OFT != NULL)
									{
										quitarOftPorNombreDescriptor(&L_OFT, Aux_OFT->fcb.iDescriptor);
									}
									enviarH(Aux_FD->fd.iSocket, cabecera);
									enviarAlgo(Aux_FD->fd.iSocket, ucpMensajes ,cabecera.uiPayloadLength);
									FD_CLR(Aux_FD->fd.iSocket, &masterFds);
									close (Aux_FD->fd.iSocket);

								}
								else
								printf("Mensaje: %s\n", ucpMensajes);

								free (ucpMensajes);
								break;
							default:
								/* CONEXION INCORRECTA */
								printf("OVNI: NOS ATACAN LOS VIRUS!!!!!!!\n");
								sprintf(cvMensaje, "Error de protocolo: %d\n",cabecera.ucPayloadDescriptor);
								escribirLog('I', cvMensaje);
								close (i);
								FD_CLR(i, &masterFds);
								break;
								
						} /* fin del switch */
					
				} /* fin del hay que ver quien entra */
			} /* fin del fdisset */
		} /* fin del for */
	} /* fin del while */

	if (datos.iCache == 1) ldapFinSesion(&sesionLdap);
	return 1;
}

void desconexion (int quien)
{
	switch(quien)
	{
		case ARRAY: printf("Desconexion de un DA\n"); break;
		case SFS: printf("Desconexion del SFS\n"); break;
		case CLIENTE: printf("Desconexion de un Cliente\n"); break;
		case FTP: printf("Desconexion del SFTP\n"); break;
	}

}


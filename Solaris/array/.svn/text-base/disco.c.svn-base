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
#include "archivos.h"
#include "disco.h"
#include "cadenas.h"
#include "sendrecv.h"
#include "protocolos.h"
#include "syscall.h"

/* 	Nombre de la Función: respaldarLista
	Versión: 1.0
	Autor/es: Villalva Emiliano
	Fechas de creación: 14/09/2009
	modificación: 30/09/2009
	Objetivo de la Función: En EstructuraArray cargar los nombres de los discos esperados
				   e inicializar el resto de los espacios vacios
	Entrada:	ptrDiscosEsperados* listaDiscos: Lista de nombre de los discos
			int iServidor: Socket del proceso PADRE
			int iCantidadDiscos: Cantidad de elementos que posee el arreglo, es decir, tamaño del array
	Salida:	SDISCOSCONECTADOS* EstructuraArray: Datos cargados
			return: -
*/
void respaldarLista(SDISCOSCONECTADOS* EstructuraArray, ptrDiscosEsperados* listaDiscos, int iServidor, int iCantidadDiscos, char* IP, int iPuerto)
{
	ptrDiscosEsperados lista;
	int i;
	
	lista = *listaDiscos; 

	for(i=0; lista != NULL;  lista=lista->sgte, i++)
	{
		EstructuraArray[i].cpNombreDisco = lista->cvNombreDisco;
		copiar(EstructuraArray[i].cvArrayIP ,IP);
		EstructuraArray[i].iSocketDisco = ARRAY_LUGAR_RESERVADO;
		EstructuraArray[i].iSocketArray = ARRAY_LUGAR_RESERVADO;
		EstructuraArray[i].iSocketHilo = ARRAY_LUGAR_RESERVADO;
		EstructuraArray[i].iEstadoDisco = /*DISCO_ESTADO_SINCRONIZADO;*/DISCO_ESTADO_NO_CONECTADO;
		EstructuraArray[i].iPuertoArray = iPuerto;
	}

	for(; i < iCantidadDiscos; i++)
	{
		copiar(EstructuraArray[i].cvArrayIP ,IP);
		EstructuraArray[i].cpNombreDisco = NULL;
		EstructuraArray[i].iSocketDisco = ARRAY_LUGAR_DISPONIBLE;
		EstructuraArray[i].iSocketArray = ARRAY_LUGAR_DISPONIBLE;
		EstructuraArray[i].iSocketHilo = ARRAY_LUGAR_DISPONIBLE;
		EstructuraArray[i].iEstadoDisco = /*DISCO_ESTADO_SINCRONIZADO;*/DISCO_ESTADO_NO_CONECTADO;
		EstructuraArray[i].iPuertoArray = iPuerto;
	}

}

/* 	Nombre de la Función: recibirDatosDisco
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 17/09/2009
	modificación: 17/09/2009
	Objetivo de la Función: Recibir los datos de NOMBRE y CHS, comprobar que cumpla los requisitos
				   de CHS y asignar el lugar en el ARRAY
	Entrada:	int iSocketDisco: Socket de conexion del disco
			SDISCOSCONECTADOS* EstructuraArray: Contiene los discos conectados
			int iCantidadDiscos: Cantidad de elementos que posee el arreglo, es decir, tamaño del array
			SCHS datosCHS: Datos de CHS que debe cumplir el disco para ser aceptado
	Salida:	SDISCOSCONECTADOS* EstructuraArray: Datos cargados
			
			return: SOCKET_ERROR_DESCONEXION, SOCKET_ERROR_SENDRECV, 
			ARRAY_NO_HAY_ESPACIO, ARRAY_DISCO_RECHAZADO, iResult: la posicion del nuevo disco
*/

int recibirDatosDisco(int iSocketDisco, SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos, SCHS datosCHS)
{
	int iResult, iCilindros, iCabezas, iSectores;
	HEADERIRCIPC cabecera;
	unsigned char* nombreDisco;
	unsigned char CHS[12];

	
	/* Recibo el NOMBRE del disco */
	iResult = recibirH(iSocketDisco, &cabecera);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SOCKET_ERROR_DESCONEXION; break;
		case SOCKET_ERROR_SENDRECV	: return SOCKET_ERROR_SENDRECV; break;
	}

	nombreDisco= (unsigned char*) malloc(sizeof(cabecera.uiPayloadLength + 1));

	iResult = recibirAlgo(iSocketDisco, nombreDisco, cabecera.uiPayloadLength);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: free(nombreDisco); return SOCKET_ERROR_DESCONEXION; break;
		case SOCKET_ERROR_SENDRECV	: free(nombreDisco); return SOCKET_ERROR_SENDRECV; break;
	}

	nombreDisco[cabecera.uiPayloadLength] = '\0';

	cabecera.ucPayloadDescriptor = HANDSHAKE_OK;
	enviarH(iSocketDisco, cabecera);

	cabecera.ucPayloadDescriptor = IPCIRC_GET_CHS;
	enviarH(iSocketDisco, cabecera);

	/* Recibo los datos de CHS */
	iResult = recibirH(iSocketDisco, &cabecera);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: free(nombreDisco); return SOCKET_ERROR_DESCONEXION; break;
		case SOCKET_ERROR_SENDRECV	: free(nombreDisco); return SOCKET_ERROR_SENDRECV; break;
	}

	iResult = recibirAlgo(iSocketDisco, CHS, cabecera.uiPayloadLength);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SOCKET_ERROR_DESCONEXION; break;
		case SOCKET_ERROR_SENDRECV	: return SOCKET_ERROR_SENDRECV; break;
	}


	iCilindros = (int) BinToNum (CHS,0);
	iCabezas = (int) BinToNum (CHS,4);
	iSectores= (int) BinToNum (CHS,8);
	
	if ((iCilindros == datosCHS.iCilindros) && (iCabezas == datosCHS.iCabezas) && (iSectores == datosCHS.iSectores))
	{
		/* disco.h */
		iResult = asignarDisco(iSocketDisco, EstructuraArray, nombreDisco, iCantidadDiscos);

		switch(iResult)
		{
			case ARRAY_NO_HAY_ESPACIO:
				cabecera.ucPayloadDescriptor =  HANDSHAKE_FAIL;
				enviarH(iSocketDisco, cabecera);
				return ARRAY_NO_HAY_ESPACIO;
				break;

			default:
				return iResult ;
				break;

		}
							
	}

	cabecera.ucPayloadDescriptor =  HANDSHAKE_FAIL;
	enviarH(iSocketDisco, cabecera);
	return ARRAY_DISCO_RECHAZADO;
	
}

/* 	Nombre de la Función: asignarDisco
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 14/09/2009
	modificación: 16/09/2009
	Objetivo de la Función: En EstructuraArray cargar los nombres de los discos esperados
				   e inicializar el resto de los espacios vacios
	Entrada:	ptrDiscosEsperados* listaDiscos: Lista de nombre de los discos
			int iServidor: Socket del proceso PADRE
			int iCantidadDiscos: Cantidad de elementos que posee el arreglo, es decir, tamaño del array
	Salida:	SDISCOSCONECTADOS* EstructuraArray: Datos cargados
			return: ARRAY_NO_HAY_ESPACIO, i: posicion del vector en donde se encuentra el nuevo disco
*/
int asignarDisco(int iSocketDisco, SDISCOSCONECTADOS* EstructuraArray, unsigned char* nombreDisco ,int iCantidadDiscos)
{
	int i;
	
	/* Reviso que sea un Disco conocido */
	for(i = 0 ; i < iCantidadDiscos; i++)
	{
		/* Si son iguales los nombres es un disco esperado */
		if ( (EstructuraArray[i].cpNombreDisco != NULL) && (comparar(EstructuraArray[i].cpNombreDisco, (char*)nombreDisco) == 0))
		{
			EstructuraArray[i].iSocketDisco = iSocketDisco;
			EstructuraArray[i].iEstadoDisco = DISCO_ESTADO_CONECTADO;
			free(nombreDisco);
			return i;
		}

	}

	/* Como no lo conoce busco un lugar libre */
	for(i = 0 ; i < iCantidadDiscos; i++)
	{
	
		if (EstructuraArray[i].iSocketDisco == ARRAY_LUGAR_DISPONIBLE)
		{
			EstructuraArray[i].cpNombreDisco = (char*) nombreDisco;
			EstructuraArray[i].iSocketDisco = iSocketDisco;
			EstructuraArray[i].iEstadoDisco = DISCO_ESTADO_CONECTADO;

			return i;
		}

	}

	return ARRAY_NO_HAY_ESPACIO; 
}

/* 	Nombre de la Función: asociarSocketHilo
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 14/09/2009
	modificación: 26/09/2009
	Objetivo de la Función: Busca el iSocketDisco del disco en EstructuraArray para indicar el Socket de comunicacion
	Entrada:	int iSocketHilo: Socket del proceso HILO
			SDISCOSCONECTADOS* EstructuraArray: Estructura que contiene los discos conectados
			int iSocketDisco: Socket origen del HANDSHAKE
			int iCantidadDiscos: Cantidad de elementos que posee el arreglo, es decir, tamaño del array
	Salida:	SDISCOSCONECTADOS* EstructuraArray: Datos cargados
			return: ARRAY_HILO_OK, ARRAY_HILO_FAIL
*/
int asociarSocketHilo(int iSocketHilo, SDISCOSCONECTADOS* EstructuraArray, HEADERIRCIPC cabecera, int iCantidadDiscos)
{
	int i;
	unsigned char* nombreDisco;

	nombreDisco= (unsigned char*) malloc(sizeof(cabecera.uiPayloadLength + 1));

	recibirAlgo(iSocketHilo, nombreDisco, cabecera.uiPayloadLength);
	nombreDisco[cabecera.uiPayloadLength] = '\0';

	for(i = 0; i < iCantidadDiscos; i++)
	{
		if ((EstructuraArray[i].cpNombreDisco != NULL) && (comparar(EstructuraArray[i].cpNombreDisco, (char*)nombreDisco) == 0))
		{
			EstructuraArray[i].iSocketHilo = iSocketHilo;
			EstructuraArray[i].iEstadoDisco = DISCO_ESTADO_CONECTADO;
			free(nombreDisco);
			return ARRAY_HILO_OK;
		}

	}
	free(nombreDisco);
	return ARRAY_HILO_FAIL;
}


/* 	Nombre de la Función: desconectarDisco
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 18/09/2009
	modificación: 18/09/2009
	Objetivo de la Función: Limpiar la entrada en EstructuraArray del disco que se desconectó
	Entrada:	int iSocketHilo: Socket del proceso HILO
			SDISCOSCONECTADOS* EstructuraArray: Estructura que contiene los discos conectados
			int iCantidadDiscos: Cantidad de elementos que posee el arreglo, es decir, tamaño del array
	Salida:	SDISCOSCONECTADOS* EstructuraArray: La entrada limpia
			return: -
*/
void desconectarDisco(int iSocketHilo, SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos)
{
	int i;

	for (i=0; i < iCantidadDiscos; i++)
	{
		if (iSocketHilo == EstructuraArray[i].iSocketHilo)
		{
			free(EstructuraArray[i].cpNombreDisco);
			EstructuraArray[i].cpNombreDisco = NULL;
			EstructuraArray[i].iSocketDisco = ARRAY_LUGAR_DISPONIBLE;
			EstructuraArray[i].iSocketArray = ARRAY_LUGAR_DISPONIBLE;
			EstructuraArray[i].iSocketHilo = ARRAY_LUGAR_DISPONIBLE;
			EstructuraArray[i].iEstadoDisco = DISCO_ESTADO_NO_CONECTADO;
			return;
		}
	}
}

/* 	Nombre de la Función: desconectarDiscoPorNombre
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 29/09/2009
	modificación: 30/09/2009
	Objetivo de la Función: Limpiar la entrada en EstructuraArray del disco que se pidio por consola
	Entrada:	char* cpNombreDisco: Nombre del disco a eliminar
			SDISCOSCONECTADOS* EstructuraArray: Estructura que contiene los discos conectados
			int iCantidadDiscos: Cantidad de elementos que posee el arreglo, es decir, tamaño del array
	Salida:	SDISCOSCONECTADOS* EstructuraArray: La entrada limpia
			return: -
*/
void desconectarDiscoPorNombre(char* cpNombreDisco, SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos)
{
	int i;

	for (i=0; i < iCantidadDiscos; i++)
	{
		if ((EstructuraArray[i].cpNombreDisco != NULL) && (comparar(EstructuraArray[i].cpNombreDisco,cpNombreDisco) == 0))
		{
			free(EstructuraArray[i].cpNombreDisco);
			EstructuraArray[i].cpNombreDisco = NULL;
			EstructuraArray[i].iSocketDisco = ARRAY_LUGAR_DISPONIBLE;
			EstructuraArray[i].iSocketArray = ARRAY_LUGAR_DISPONIBLE;
			EstructuraArray[i].iSocketHilo = ARRAY_LUGAR_DISPONIBLE;
			EstructuraArray[i].iEstadoDisco = DISCO_ESTADO_NO_CONECTADO;
			return;
		}
	}
}


/* 	Nombre de la Función: subestadoFueraDeServicio
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 19/09/2009
	modificación: 19/09/2009
	Objetivo de la Función: Indica si todos los discos estan sincronizados
	Entrada:	SDISCOSCONECTADOS* sDisco: Informacion del Disco
			int iCantidadDiscos = Cantidad de elementos que tiene la estructura
	Salida:	
			return: ESTADO_FUERASERVICIO_INCOMPLETO, ESTADO_FUERASERVICIO_CORRUPTO, ESTADO_FUERASERVICIO_SINCRONIZADO
*/
int subestadoFueraDeServicio(SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos)
{
	int i;

	for (i=0; i < iCantidadDiscos; i++)
	{
		if ((EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_CONECTADO) || (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZANDO))
		{
			return ESTADO_FUERASERVICIO_CORRUPTO;
		}
	}

	for (i=0; i < iCantidadDiscos; i++)
	{
		if ((EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_NO_CONECTADO) && (EstructuraArray[i].cpNombreDisco != NULL))
		return ESTADO_FUERASERVICIO_INCOMPLETO;
	}
	

	return ESTADO_FUERASERVICIO_SINCRONIZADO;
}

/* 	Nombre de la Función: subestadoEnServicio
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 24/09/2009
	modificación: 24/09/2009
	Objetivo de la Función: Indica si todos los discos estan sincronizados
	Entrada:	SDISCOSCONECTADOS* sDisco: Informacion del Disco
			int iCantidadDiscos = Cantidad de elementos que tiene la estructura
	Salida:	
			return: DISCO_ESTADO_SINCRONIZANDO, DISCO_ESTADO_SINCRONIZADO
*/
int subestadoEnServicio(SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos)
{
	int i;
	
	for (i=0; i < iCantidadDiscos; i++)
	{
		if((EstructuraArray[i].cpNombreDisco != NULL) && (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZANDO))
			return ESTADO_INICIALIZADO_SINCRONIZANDO;
	}
	
	return ESTADO_INICIALIZADO_SINCRONIZADO;
}

/* 	Nombre de la Función: elegirDisco
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 24/09/2009
	modificación: 24/09/2009
	Objetivo de la Función: Obtiene el disco que debe utilizarse
	Entrada:	SDISCOSCONECTADOS* sDisco: Informacion del Disco
			int iCantidadDiscos = Cantidad de elementos que tiene la estructura
			int iPosVecAnterior: Posicion del vector que se uso anteriormente
			int* iSelector1: Posicion del vector del disco1
			int* iSelector2: Posicion del vector del disco2
	Salida:	int* iSelector1: Nueva Posicion del vector del disco1
			int* iSelector2: Nueva Posicion del vector del disco2
			return: FALLA_SISTEMA
*/
int elegirDisco(SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos, int* iSelector1, int* iSelector2)
{
	int i;

	/* Para el iSelector1 ------------------------------------------------------------------------*/
	for(i = (*iSelector1 + 1); (i < iCantidadDiscos) && (*iSelector1 == -1); i++)
	{
		if((EstructuraArray[i].cpNombreDisco != NULL) && (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZADO))
			*iSelector1 = i;
	}

	if ((i == iCantidadDiscos) && (*iSelector1 == -1))
	{
		for(i = 0 ; i < iCantidadDiscos; i++)
		{
			if((EstructuraArray[i].cpNombreDisco != NULL) && (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZADO))
			*iSelector1 = i;
		}
	}

	/* Para el iSelector2 incio desde donde quedo el iSelector1 --------------------------------*/
	for(i = (*iSelector1 + 1);(i < iCantidadDiscos) && (*iSelector2 == -1); i++)
	{
		if((EstructuraArray[i].cpNombreDisco != NULL) && (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZADO))
		*iSelector2 = i;
	}

	if ((i == iCantidadDiscos) && (*iSelector2 == -1))
	{
		for(i = 0 ; i < iCantidadDiscos; i++)
		{
			if((EstructuraArray[i].cpNombreDisco != NULL) && (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZADO))
			*iSelector2 = i;
		}
	}
	
	if (*iSelector2 < 0)
	return FALLA_SISTEMA;
	else return 1;
}

/* 	Nombre de la Función: repartirPutSectores
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 24/09/2009
	modificación: 03/10/2009
	Objetivo de la Función: Al venir un pedido de PUTSECTORES se envia a todos los discos y al que esta en
				   sincronizacion se valida si ya se paso por ese sector
	Entrada:	SDISCOSCONECTADOS* sDisco: Informacion del Disco
			int iCantidadDiscos = Cantidad de elementos que tiene la estructura
			HEADERIRCIPC cabecera: cabecera
			SSECTORES sSectores: Los sectores que se van a enviar
			int iPosVecDestSinc: La posicion del vector que esta en sincronizacion
			SCHS datosCHS: Si hay un disco en sincronizacion se debe verificar por que sector va
	Salida:	-
			return:-
*/
void repartirPutSectores(SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos,HEADERIRCIPC cabecera ,SSECTORES sSectores, SDISCOSSINCRONIZANDOS* sDiscosSincronizando, SCHS datosCHS)
{
	int i;

	for (i = 0; i < iCantidadDiscos; i++)
	{
		if ((i == sDiscosSincronizando[0].iPosVecDest) && (sDiscosSincronizando[0].cpDescriptorID[0]!='\0'))
			escribirEnSincronizacion(EstructuraArray[sDiscosSincronizando[0].iPosVecDest], cabecera ,sSectores , sDiscosSincronizando[0].iIdSectorActual );
		else
		{
			if((EstructuraArray[i].cpNombreDisco != NULL) && (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZADO))
			printf("DISCO a enviar: %s\n",EstructuraArray[i].cpNombreDisco); 
			enviarPedido_putSectores(EstructuraArray[i].iSocketHilo, cabecera,sSectores);
		}	
	}
}

void escribirEnSincronizacion(SDISCOSCONECTADOS sDisco, HEADERIRCIPC cabecera ,SSECTORES sSectores , int iIdSectorActual)
{
	if (  sSectores.iIdSector1 < iIdSectorActual)
		enviarPedido_putSectores(sDisco.iSocketHilo, cabecera, sSectores);
}

/* 	Nombre de la Función: enviarPedido_CHS 
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 24/09/2009
	modificación: 24/09/2009
	Objetivo de la Función: Enviar respuesta de pedido de CHS
	Entrada:	int iSocketOrigen: Socket de donde vino el pedido
			HEADERIRCIPC cabecera: Para la longitud del mensaje
			SCHS datosCHS: Todo cargado;
	Salida:	-
			return: SYSCALL_CHS_SEND_FAIL, SYSCALL_CHS_SEND_OK
*/
int enviarPedido_CHS(int iSocketDestino, HEADERIRCIPC cabecera, SCHS datosCHS)
{
	unsigned char buffer[SIZE_STRUCT_CHS];
	int iResult;

	cabecera.ucPayloadDescriptor = IPCIRC_GET_CHS_RTA;
	cabecera.uiPayloadLength = SIZE_STRUCT_CHS;
	NumToBin (buffer,datosCHS.iCilindros, 0);
	NumToBin (buffer,datosCHS.iCabezas, 4);
	NumToBin (buffer,datosCHS.iSectores, 8);

	iResult = enviarH(iSocketDestino, cabecera);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_CHS_SEND_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_CHS_SEND_FAIL; break;
	}

	iResult = enviarAlgo(iSocketDestino, buffer, SIZE_STRUCT_CHS);
	switch (iResult)
	{
		case SOCKET_ERROR_DESCONEXION: return SYSCALL_CHS_SEND_FAIL; break;
		case SOCKET_ERROR_SENDRECV	: return SYSCALL_CHS_SEND_FAIL; break;
	}

	return SYSCALL_CHS_SEND_OK;
}

/* 	Nombre de la Función: atenderDisco
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 14/09/2009
	modificación: 16/09/2009
	Objetivo de la Función: Select principal de atencion que sera de puente entre el PADRE y DISCO/ARRAY
	Entrada:	SDISCOSCONECTADOS* sDisco: Informacion del Disco
	Salida:	
			return: HILO_DESCONEXION 
*/
int atenderDisco(SDISCOSCONECTADOS* sDisco)
{
	fd_set masterFds;   		/* conjunto fd maestro para funcion select() */
	fd_set leeFds; 		/* conjunto fd temporal para funcion select() */
 	struct timeval tv; 		/* estructura que define el tiempo que espera select por E/S, para q no sea bloqueante */

	int fdMax, i, iResult, iPuertoTemporal=3000, iSocketArray, iSocketHilo ;
	HEADERIRCIPC cabecera;
	SSECTORES sSectores;

	char* cpMensaje;
	unsigned char buffer[512];

	iSocketHilo = generarSocketHilo(sDisco->cvArrayIP, iPuertoTemporal);
	iSocketArray = iniciarCliente(sDisco->cvArrayIP, sDisco->iPuertoArray);

	crearCabecera (&cabecera, IPCIRC_HILO,longitudCadena(sDisco->cpNombreDisco));
	enviarH(iSocketArray , cabecera);
	enviarAlgo(iSocketArray , (unsigned char*)sDisco->cpNombreDisco,cabecera.uiPayloadLength);

	FD_ZERO(&masterFds);     
	FD_ZERO(&leeFds);

	FD_SET(iSocketArray,&masterFds);
	FD_SET(sDisco->iSocketDisco, &masterFds);
	FD_SET(iSocketHilo,&masterFds);

	if (sDisco->iSocketDisco > iSocketArray)
	{
		
		if (sDisco->iSocketDisco>iSocketHilo)
			fdMax=sDisco->iSocketDisco;
		else fdMax  = iSocketHilo;
	}
	else
	{
		if (iSocketArray>iSocketHilo)
			fdMax=iSocketArray;
		else fdMax  = iSocketHilo;

	}

	while(1) 
	{
		leeFds = masterFds; /* Lo copiamos porque la funcion select lo va a modificar interiormente */
		tv.tv_sec = 0; 		/* cant segundos */
		tv.tv_usec = 1; 	/* cant microsegundos */        

		while(select(fdMax+1, &leeFds, NULL, NULL, &tv) == -1)
        	{
			if(errno != EINTR)
              	{
				perror("HILO select()");
				return 1;
			}
        	}
  
   		/* busca peticiones/conexiones */ 
                 
		for(i = 0; i <= fdMax; i++) 
       	{
			
			/*************** ATENCION LOS ENVIOS QUE HACE EL ARRAY AL DISCO  *************/
			if(FD_ISSET(i,&leeFds))
			{
				iResult = recibirH(i, &cabecera);
				if (iResult <= 0)
				{
					switch(iResult)
					{
						case SOCKET_ERROR_DESCONEXION: 
							close(i);
	                             		FD_CLR(i,&masterFds);
							close(iSocketArray);
							FD_CLR(iSocketArray,&masterFds);

							return HILO_DESCONEXION;
							break;
	
						case SOCKET_ERROR_SENDRECV: 
							close(i);
              	                		FD_CLR(i,&masterFds);
							close(iSocketArray);
							FD_CLR(iSocketArray,&masterFds);
							return HILO_DESCONEXION;
							break;
					}
				}

				else
				{
					switch (cabecera.ucPayloadDescriptor)
					{	
						case IPCIRC_GET_SECTORES1: case IPCIRC_GET_SECTORES2:
							iResult = recibirPedido_getSectores(i, cabecera, &sSectores);
							switch(iResult)
							{
								case SYSCALL_GETSECTORES_RECV_PEDIDO_FAIL:
									escribirLog ('E', "Se ha producido un ERROR en proceso PADRE"); 
									close(i);
              	                				FD_CLR(i,&masterFds);
									return HILO_DESCONEXION;
									break;
	
								case SYSCALL_GETSECTORES_RECV_PEDIDO_OK:
									switch (cabecera.ucPayloadDescriptor)
									{
										case IPCIRC_GET_SECTORES1: /* syscall.h */
											iResult = enviarPedido_getSectoresUnDisco(sDisco->iSocketDisco, cabecera, sSectores.iIdSector1);
											switch (iResult)
											{
												case SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL:
													return HILO_DESCONEXION;
													break;
												case SYSCALL_GETSECTORES_SEND_PEDIDO_OK:
													break;
											}
											break;
										case IPCIRC_GET_SECTORES2: /* syscall.h */
											iResult = enviarPedido_getSectoresDosDiscos(sDisco->iSocketDisco, cabecera, sSectores);
											switch (iResult)
											{
												case SYSCALL_GETSECTORES_SEND_PEDIDO_FAIL:
												return HILO_DESCONEXION;
												break;
												case SYSCALL_GETSECTORES_SEND_PEDIDO_OK:
												break;
											}
										break;
	
									}
									break;
							}
							break;
	
						case IPCIRC_GET_SECTORES_RTA:
							
							iResult = recibirRespuesta_getSectores(i, cabecera, &sSectores);
							switch(iResult)
							{
								case SYSCALL_GETSECTORES_RECV_RTA_FAIL:
									close(i);
                              					FD_CLR(i,&masterFds);
									return HILO_DESCONEXION;
									break;
	
								case SYSCALL_GETSECTORES_RECV_RTA_OK:
									iResult = enviarRespuesta_getSectores(iSocketArray, cabecera, sSectores);
									switch(iResult)
									{
										case SYSCALL_GETSECTORES_SEND_RTA_FAIL:
											return HILO_DESCONEXION;
											break;
	
											break;
										case SYSCALL_GETSECTORES_SEND_RTA_OK:
											break;
	
									}
	
									break;
	
							}
	
							break;
						case IPCIRC_GET_SECTORES_RTAb:
							recibirRespuesta_getSectoresB(i, cabecera, buffer);
							enviarRespuesta_getSectoresB(iSocketArray, cabecera, buffer);
							break;
						case IPCIRC_PUT_SECTORES:
							iResult = recibirPedido_putSectores(i, cabecera, &sSectores);
							switch(iResult)
							{
								case SYSCALL_PUTSECTORES_RECV_PEDIDO_FAIL:
									close(i);
	                              				FD_CLR(i,&masterFds);
									return HILO_DESCONEXION;
									break;
	
								case SYSCALL_PUTSECTORES_RECV_PEDIDO_OK:
									iResult = enviarPedido_putSectores(sDisco->iSocketDisco, cabecera, sSectores);
									switch(iResult)
									{
										case SYSCALL_PUTSECTORES_SEND_PEDIDO_FAIL:
											return HILO_DESCONEXION;
											break;
	
										case SYSCALL_PUTSECTORES_SEND_PEDIDO_OK:
											break;
	
									}
									break;
	
							}
	
							break;
	
						case IPCIRC_PUT_SECTORES_OK: 
							enviarH(iSocketArray, cabecera);
							break;
						case IPCIRC_FALLA:
							cpMensaje = (char*) malloc(cabecera.uiPayloadLength);
							recibirAlgo(i,(unsigned char*)cpMensaje,cabecera.uiPayloadLength);
							enviarH(iSocketArray, cabecera);
							enviarAlgo(iSocketArray,(unsigned char*)cpMensaje,cabecera.uiPayloadLength);
							free (cpMensaje);
							break;
					} /* Fin de switch (cabecera.ucPayloadDescriptor) */
				} /* Fin del ELSE de nueva conexion */
	
			}
		}/* Fin del FOR */
	}/* Fin del WHILE */
	return 1;

}

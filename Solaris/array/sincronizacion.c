#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cadenas.h"
#include "archivos.h"
#include "sendrecv.h"
#include "disco.h"
#include "sincronizacion.h"
#include "syscall.h"
#include "protocolos.h"


/* 	Nombre de la Función: iniciarSoncronizacion
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 21/09/2009
	modificación: 21/09/2009
	Objetivo de la Función: Busca un disco sincronizado para iniciar la sincronizacion, en caso
				   de que no haya se da por sincronizado
	Entrada:	SDISCOSCONECTADOS* EstructuraArray: Estructura de trabajo
			int iPosSincDest: Posicion del arreglo donde esta el disco a sincronizar
			int iCantidadDiscos: Tamaño de la estructura
	Salida:	-
			return: SINCRONIZACION_NO_INICIAR, i: posicion del arreglo donde se encuentra el disco origen
*/
int iniciarSoncronizacion(SDISCOSCONECTADOS* EstructuraArray, int iPosSincDest, int iCantidadDiscos)
{
	int i;

	/* Buscamos algun disco sincronizado */
	
	for (i=0; (i < iCantidadDiscos) && (EstructuraArray[i].iEstadoDisco != DISCO_ESTADO_SINCRONIZADO); i++);

	if (i == iCantidadDiscos) /* No hay discos sincronizados */
	{
		EstructuraArray[iPosSincDest].iEstadoDisco = DISCO_ESTADO_SINCRONIZADO;
		return SINCRONIZACION_NO_INICIAR;
	}

	/* Se encontro un disco sincronizado */
	return i; /* Retorno la posicion del disco que dara los datos de sincronizacion */
}

/* 	Nombre de la Función: sincronizarDisco
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 21/09/2009
	modificación: 03/10/2009
	Objetivo de la Función: Inicia el primer pedido de sectores
	Entrada:	SDISCOSCONECTADOS* EstructuraArray: Estructura de trabajo
			int iPosSincOrigen: Posicion del arreglo donde esta el disco a sincronizar
			int iCantidadDiscos: Tamaño de la estructura
	Salida:	SCHS* chs: deben estar inicializados en 0 los i...Actuales
			HEADERIRCIPC* cabecera: IDdescriptor de control para saber que es de sincronizacion
			return: IPCIRC_GET_SECTORES1, IPCIRC_GET_SECTORES2
*/
int sincronizarDisco(SDISCOSCONECTADOS* sDiscos, SCHS chs, SDISCOSSINCRONIZANDOS* sDiscosSincronizando)
{
	int iResult, iSocketHilo;
	SSECTORES sSectores;
	HEADERIRCIPC cabecera;
	/* tomo el primero de los discos que hay que sincronizar porque es una cola */

	iSocketHilo = sDiscos[sDiscosSincronizando[0].iPosVecOrigen].iSocketHilo;
	sDiscos[sDiscosSincronizando[0].iPosVecDest].iEstadoDisco = DISCO_ESTADO_SINCRONIZANDO;
	copiarCabecera(cabecera.cpDescriptorID,sDiscosSincronizando[0].cpDescriptorID);
	calcularCHS(chs, 0, &sSectores.iIdSector1);
	calcularCHS(chs, 1, &sSectores.iIdSector2);
	sDiscosSincronizando[0].iIdSectorActual= 1; /* guardo el proximo que hay que pedir */
	enviarPedido_getSectoresDosDiscos(iSocketHilo, cabecera, sSectores);

	return iResult;
}

/* 	Nombre de la Función: enSincronizacion
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 21/09/2009
	modificación: 23/10/2009
	Objetivo de la Función: Controla que el la respuesta de GETSECTORES sea de sicronizacion
	Entrada:	HEADERIRCIPC cabecera,  SDISCOSSINCRONIZANDOS* sDiscosSincronizando
	Salida:	-
			return: SINCRONIZACION_NO_ES_SINCRONIZACION,SINCRONIZACION_FIN,SINCRONIZACION_CONTINUAR
*/
int enSincronizacion(HEADERIRCIPC cabecera,  SDISCOSSINCRONIZANDOS* sDiscosSincronizando)
{
	int i;
	for (i=0 ; (i < 16) && (cabecera.cpDescriptorID[i] == sDiscosSincronizando[0].cpDescriptorID[i]) ; i++);
	
	if (i==16) return SINCRONIZACION_CONTINUAR;

	return SINCRONIZACION_NO_ES_SINCRONIZACION;
}

int continuarSincronizacion(int iSock,SDISCOSCONECTADOS* sDiscos, SCHS chs, SDISCOSSINCRONIZANDOS* sDiscosSincronizando, int iCantidadDiscos, HEADERIRCIPC cabecera)
{	
	int iResult, iResult2;
	SSECTORES sSectores;

	sSectores.iIdSector1 = sDiscosSincronizando[0].iIdSectorActual -1;
	sSectores.iIdSector2 = sDiscosSincronizando[0].iIdSectorActual;

	recibirRespuesta_getSectores(iSock, cabecera , &sSectores);
	enviarPedido_putSectores(sDiscos[sDiscosSincronizando[0].iPosVecDest].iSocketHilo, cabecera, sSectores);

	/* Calcular nuevo CHS actual, me quedo con el segundo resultado*/
	sDiscosSincronizando[0].iIdSectorActual++;
	iResult = calcularCHS(chs, sDiscosSincronizando[0].iIdSectorActual, &sSectores.iIdSector1);
	sDiscosSincronizando[0].iIdSectorActual++;
	iResult2 = calcularCHS(chs, sDiscosSincronizando[0].iIdSectorActual, &sSectores.iIdSector2);
	
	/* Si el primero no tiene mas por calcular finaliza el proceso de sincronizacion */
	switch(iResult)
	{
		case SINCRONIZACION_FIN:
			sDiscosSincronizando[0].cpDescriptorID[0] = '\0';
			printf("FIN %s\n",sDiscos[sDiscosSincronizando[0].iPosVecDest].cpNombreDisco);
			sDiscos[sDiscosSincronizando[0].iPosVecDest].iEstadoDisco = DISCO_ESTADO_SINCRONIZADO;
			hayOtroSincronizando(sDiscosSincronizando, iCantidadDiscos);		
			return SINCRONIZACION_FIN;
			break;
		case SINCRONIZACION_CONTINUAR:
			printf("Enviando Pedido hacia: %s - para sincronizar %s - ID1: %d - ID2: %d, de un total de %d\n",sDiscos[sDiscosSincronizando[0].iPosVecOrigen].cpNombreDisco,sDiscos[sDiscosSincronizando[0].iPosVecDest].cpNombreDisco,sSectores.iIdSector1,sSectores.iIdSector2,((chs.iCilindros) * (chs.iSectores)) -1 );
			enviarPedido_getSectoresDosDiscos(sDiscos[sDiscosSincronizando[0].iPosVecOrigen].iSocketHilo, cabecera, sSectores);
			return 0;
			break;
	}
	/* Si el primero no tiene mas por calcular finaliza el proceso de sincronizacion */
	return SINCRONIZACION_FIN;
}

void enviarOtroASincronizar(SDISCOSCONECTADOS* sDiscos, SCHS chs, SDISCOSSINCRONIZANDOS* sDiscosSincronizando, int iCantidadDiscos)
{
	int iResult;

	iResult = hayOtroSincronizando(sDiscosSincronizando,iCantidadDiscos);
	switch(iResult)
	{
		case SINCRONIZACION_HAY_OTRO_SINCRONIZANDO:
			/* revisemos si termino de sincronizar */
			if(((chs.iCilindros * chs.iSectores) - 1 ) > sDiscosSincronizando[1].iIdSectorActual)
			{
				return; /* sigue sincronizando, le faltan sectores */
			}
			
			break;
		case SINCRONIZACION_NO_HAY_OTRO_SINCRONIZANDO:
			if (sDiscosSincronizando[0].cpDescriptorID[0] == '\0') 
			{
				return; /* no hay disco en la cola */
			}
			break;
	}

	/* hay que iniciar otro */
	sincronizarDisco(sDiscos, chs, sDiscosSincronizando);
}

/* 	Nombre de la Función: calcularCHS
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 22/09/2009
	modificación: 03/10/2009
	Objetivo de la Función: Incrementa los valores de CHS si es necesario, si no no permite 
					continuar la sincronizacion
	Entrada:	-
	Salida:	SCHS* chs: actualiza los Actuales
			int* idSector1: Da el valor del sector
			int* idSector2: Da el valor del sector
				return: SINCRONIZACION_FIN, SINCRONIZACION_CONTINUAR
*/
int calcularCHS(SCHS chs, int idSectorActual, int* idSector)
{
	if(idSectorActual > ((chs.iCilindros * chs.iSectores) - 1))
	{
		/* Llego al final */
		return SINCRONIZACION_FIN;
	}
	*idSector = idSectorActual;

	return SINCRONIZACION_CONTINUAR;
}

/* 	Nombre de la Función: inicializarSincronizando
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 03/10/2009
	modificación: 03/10/2009
	Objetivo de la Función: Limpia el vector
	Entrada:	SDISCOSSINCRONIZANDOS* Estructura: Contiene la cola de discos a sincronizar 
			int iCantidadDiscos: cantidad del array
	Salida:	SDISCOSSINCRONIZANDOS* Estructura: Contiene inicializada la estructura
				return: -
*/
void inicializarSincronizando(SDISCOSSINCRONIZANDOS* Estructura, int iCantidadDiscos)
{
	int i,j;

	for(i = 0; i < iCantidadDiscos; i++)
	{
		for(j = 0; j < 16; Estructura[i].cpDescriptorID[j] = '\0', j++);
	}

}

/* 	Nombre de la Función: cargarDiscoASincronizar
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 03/10/2009
	modificación: 03/10/2009
	Objetivo de la Función: Limpia el vector
	Entrada:	SDISCOSSINCRONIZANDOS* Estructura: Contiene la cola de discos a sincronizar 
			int iCantidadDiscos: cantidad del array
			SDISCOSSINCRONIZANDOS datos: Informacion a cargar en el vector
	Salida:	SDISCOSSINCRONIZANDOS* Estructura: Contiene con el nuevo socket a sincronizar la estructura
				return: -
*/
void cargarDiscoASincronizar(SDISCOSSINCRONIZANDOS* Estructura,int iCantidadDiscos, SDISCOSSINCRONIZANDOS datos)
{
	int i,j;

	for (i = 0; (i < iCantidadDiscos) && (Estructura[i].cpDescriptorID[0]!='\0'); i++);
	
	for (j = 0; j < 16 ; Estructura[i].cpDescriptorID[j] = datos.cpDescriptorID[j], j++);

	Estructura[i].iIdSectorActual = datos.iIdSectorActual;
	Estructura[i].iPosVecOrigen = datos.iPosVecOrigen;
	Estructura[i].iPosVecDest = datos.iPosVecDest;

}

/* 	Nombre de la Función: hayOtroSincronizando
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 03/10/2009
	modificación: 03/10/2009
	Objetivo de la Función: Determina si el disco a sincronizar inicia o no porque ya hay otro sincronizando
	Entrada:	SDISCOSSINCRONIZANDOS* Estructura: Contiene la cola de discos a sincronizar 
			int iCantidadDiscos: cantidad del array
			
	Salida:	-
				return: -
*/
int hayOtroSincronizando(SDISCOSSINCRONIZANDOS* Estructura,int iCantidadDiscos)
{
	int i,j;
	for(i = 0; (i < iCantidadDiscos) && (Estructura[i].cpDescriptorID[0]!= '\0'); i++);

	if (i==0)
	{
		for(i = 1; i < iCantidadDiscos; i++)
		{
			for (j = 0; j < 16 ; Estructura[i-1].cpDescriptorID[j] = Estructura[i].cpDescriptorID[j], j++);

			Estructura[i-1].iIdSectorActual = Estructura[i].iIdSectorActual;
			Estructura[i-1].iPosVecOrigen = Estructura[i].iPosVecOrigen;
			Estructura[i-1].iPosVecDest = Estructura[i].iPosVecDest;

		}

		for (j = 0; j < 16 ; Estructura[iCantidadDiscos].cpDescriptorID[j] = '\0', j++);

		return SINCRONIZACION_NO_HAY_OTRO_SINCRONIZANDO;
	}

	return SINCRONIZACION_HAY_OTRO_SINCRONIZANDO;
}


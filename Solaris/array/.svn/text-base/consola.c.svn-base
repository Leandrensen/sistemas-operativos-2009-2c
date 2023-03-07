#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h>
#include "consola.h"
#include "sendrecv.h"
#include "protocolos.h"
#include "archivos.h"
#include "cadenas.h"
#include "disco.h"

/* 	Nombre de la Función: ejecutarComando
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 19/09/2009
	modificación: 19/09/2009
	Objetivo de la Función:
	Entrada:	int iSocketConsola: Socket de la consola pra recibir el comando
			SDISCOSCONECTADOS* EstructuraArray: Estructura de trabajo de los discos
			int iCantidadDiscos: Cantidad de elementos que posee el arreglo, es decir, tamaño del array
	Salida:	int* iEstadoActual: Cambia el estado si el comando refiere al servicio del ARRAY
			return: SOCKET_ERROR_DESCONEXION, CONSOLA_ELIMINAR_NO_SINCRONIZADOS, CONSOLA_ELIMINAR_NO_SINCRONIZADOS,  
					CONSOLA_ELIMINAR_DISCO, CONSOLA_SERVICIO, CONSOLA_AGREGAR_DISCO

*/
int ejecutarComando(int iSocketConsola, SDISCOSCONECTADOS* EstructuraArray, int iCantidadDiscos, int* iEstadoActual, int* iSubEstadoActual, int* iPosVector)
{
	int iResult;
	HEADERIRCIPC cabecera;
	unsigned char* nombreDisco;

	iResult = recibirH(iSocketConsola , &cabecera);
	if (iResult <= 0)
		return SOCKET_ERROR_DESCONEXION;

	switch (cabecera.ucPayloadDescriptor)
	{
		case IPCIRC_CONSOLA_AGREDARDA:
			nombreDisco= (unsigned char*) malloc(sizeof(cabecera.uiPayloadLength + 1));
			
			iResult = recibirAlgo(iSocketConsola, nombreDisco, cabecera.uiPayloadLength);
			if (iResult <= 0)
				return SOCKET_ERROR_DESCONEXION;
			
			nombreDisco[cabecera.uiPayloadLength]= '\0';
					/* consola.h */
			*iPosVector = agregarDisco(EstructuraArray, (char*) nombreDisco, iCantidadDiscos);
			return CONSOLA_AGREGAR_DISCO;
			break;

		case IPCIRC_CONSOLA_ELIMINARDA:
			nombreDisco= (unsigned char*) malloc(sizeof(cabecera.uiPayloadLength + 1));

			iResult = recibirAlgo(iSocketConsola, nombreDisco, cabecera.uiPayloadLength);
			if (iResult <= 0)
				return SOCKET_ERROR_DESCONEXION;
			nombreDisco[cabecera.uiPayloadLength]= '\0';
					/* consola.h */
			iResult = eliminarDisco(EstructuraArray, (char*)nombreDisco, iCantidadDiscos);
		
			switch (iResult)
			{
				case CONSOLA_ELIMINAR_NO_SINCRONIZADOS: case CONSOLA_ELIMINAR_DISCO_NO_ENCONTRADO:
					free(nombreDisco);
					escribirLog ('I', "CONSOLA: no se puede eliminar el disco, discos NO SINCRONIZADOS/NO ENCONTRADO");
					return CONSOLA_ELIMINAR_NO_SINCRONIZADOS;
					break;
				default: 
					/* iResult contiene el socket a limpiar del masterFds */
					free(nombreDisco);
					escribirLog ('I', "Disco eliminado con exito");
					*iPosVector = iResult;
					return CONSOLA_ELIMINAR_DISCO;
					break;
			}
			
			break;

		case IPCIRC_CONSOLA_INICIARSERVICIO:
			if (*iEstadoActual == ESTADO_FUNCIONAL)
				printf("El ARRAY ya esta en Servicio\n");
			if (*iEstadoActual == ESTADO_FUERASERVICIO)
			{
				
				*iSubEstadoActual = subestadoFueraDeServicio(EstructuraArray, iCantidadDiscos);
				if (*iSubEstadoActual == ESTADO_FUERASERVICIO_SINCRONIZADO)
				{ 
					*iEstadoActual = ESTADO_FUNCIONAL;
					printf("El ARRAY inicia el Servicio\n");
					*iSubEstadoActual = ESTADO_INICIALIZADO_SINCRONIZADO;
				}
				else printf("Imposible iniciar servicio, no se cumplen los requisitos\n");
			}
			break;
			
		case IPCIRC_CONSOLA_FUERASERVICIO:
			if (*iEstadoActual == ESTADO_FUERASERVICIO)
				printf("El ARRAY ya esta fuera de Servicio\n");
			if (*iEstadoActual == ESTADO_FUNCIONAL)
			{
				printf("El ARRAY pasa a estado INACTIVO\n");
				*iEstadoActual = ESTADO_FUERASERVICIO;
				*iSubEstadoActual = subestadoFueraDeServicio(EstructuraArray, iCantidadDiscos);
			}	
			break;
		case  IPCIRC_CONSOLA_ESTADO:
			return CONSOLA_ESTADO;
	}
	
	return CONSOLA_SERVICIO;

}

/* 	Nombre de la Función: eliminarDisco
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 19/09/2009
	modificación: 19/09/2009
	Objetivo de la Función: Limpiar la entrada de Disco de la estructura
	Entrada:	SDISCOSCONECTADOS* EstructuraArray: Estructura de trabajo
			char* cpNombreDisco: A eliminar
			int iCantidadDiscos: Tamaño de la estructura
	Salida:	SDISCOSCONECTADOS* EstructuraArray: Datos limpiados
			return: CONSOLA_ELIMINAR_NO_SINCRONIZADOS, CONSOLA_ELIMINAR_DISCO_NO_ENCONTRADO, i: posicion del vector
*/
int eliminarDisco(SDISCOSCONECTADOS* EstructuraArray, char* cpNombreDisco, int iCantidadDiscos)
{
	int i, iSocketHiloACerrar, encontrado = FALLA_SISTEMA;

	/* Reviso que esten sincronizados */
	for (i=0 ; i < iCantidadDiscos; i++)
	{
		if ((EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_CONECTADO) || (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZANDO))
		return CONSOLA_ELIMINAR_NO_SINCRONIZADOS;
	}

	/* Si llego hasta aca es porque estan sincronizados */
	/* Buscamos el Disco a eliminar */

	for (i = 0; i < iCantidadDiscos; i++)
	{
		if (EstructuraArray[i].cpNombreDisco!=NULL)
 			if(comparar(EstructuraArray[i].cpNombreDisco, cpNombreDisco) == 0) encontrado = i;
	}

	if ((i == iCantidadDiscos) && (encontrado ==FALLA_SISTEMA)) /* No lo encontro */
		return CONSOLA_ELIMINAR_DISCO_NO_ENCONTRADO;

	iSocketHiloACerrar = EstructuraArray[i].iSocketHilo;

	/* Encontro el Disco, desconecto el HILO, quien al recibir la desconexion se finaliza */
		
	return encontrado;
}

/* 	Nombre de la Función: agregarDisco
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 19/09/2009
	modificación: 19/09/2009
	Objetivo de la Función: Cambia el estado del disco a SINCRONIZANDO
	Entrada:	SDISCOSCONECTADOS* EstructuraArray: Estructura de trabajo
			char* cpNombreDisco: A eliminar
			int iCantidadDiscos: Tamaño de la estructura
	Salida:	SDISCOSCONECTADOS* EstructuraArray: Estado cambiado
			return: CONSOLA_AGREGAR_DISCOINEXISTENTE, CONSOLA_DISCO_SINCRONIZADO, i: posicion en el vector
*/
int agregarDisco(SDISCOSCONECTADOS* EstructuraArray, char* cpNombreDisco, int iCantidadDiscos)
{
	int i, encontrado = FALLA_SISTEMA;
	
	for (i = 0; i < iCantidadDiscos; i++)
	{
		if (EstructuraArray[i].cpNombreDisco!=NULL)
 			if(comparar(EstructuraArray[i].cpNombreDisco, cpNombreDisco) == 0) encontrado = i;
	}

	if ((i == iCantidadDiscos) && (encontrado == FALLA_SISTEMA))
		return CONSOLA_AGREGAR_DISCOINEXISTENTE;

	if (EstructuraArray[encontrado].iEstadoDisco == DISCO_ESTADO_SINCRONIZADO)
		return CONSOLA_DISCO_SINCRONIZADO;

	/* Encontro el Disco buscado */
	EstructuraArray[encontrado].iEstadoDisco = DISCO_ESTADO_SINCRONIZANDO;

	return encontrado;

}

/* 	Nombre de la Función: consola
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 20/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Cambia el estado del disco a SINCRONIZANDO
	Entrada:	CONFIGURACION* config: datos de configuracion
	Salida:	-
			return: -
*/
void consola(CONFIGURACION* config)
{
	int terminado = 0, iResult, ayuda = 0, IPCIRC, iSocketArray;
	char cvStringConsola[257];
	char* cp;
	HEADERIRCIPC cabecera;
	
	iSocketArray =iniciarCliente(config->cvArrayIP, config->iArrayPuerto); /* sendrecv.h */

	printf ("CONSOLA INICIADA CON EXITO\n");
	escribirLog ('I', "Consola iniciada");

	while (terminado == 0) 
	{	
		printf ("ARRAY ?>");
		
		fgets(cvStringConsola,256,stdin);
		fflush(stdout);
		cp = strtok(cvStringConsola, " \n\r");
	
		iResult = determinarComando(cvStringConsola);
		
		switch(iResult)
		{
			case SYSTEM_CLS:
				system("clear");
				break;

			case SYSTEM_AGREGAR: case SYSTEM_ELIMINAR: 
				if (iResult == SYSTEM_AGREGAR)
					IPCIRC = IPCIRC_CONSOLA_AGREDARDA;
				else IPCIRC = IPCIRC_CONSOLA_ELIMINARDA;

				cp = strtok(NULL, " \n\r");

				if (cp == NULL) ayuda = 1; /* Si el argumento de Nombre de DA no lo escribio */
				else
				{
					crearCabecera (&cabecera, IPCIRC_CONSOLA, 0);
					enviarH(iSocketArray, cabecera);
					
					cabecera.ucPayloadDescriptor = IPCIRC;
					cabecera.uiPayloadLength = longitudCadena(cp);
					enviarH(iSocketArray, cabecera);
					
					enviarAlgo(iSocketArray, (unsigned char*)cp, cabecera.uiPayloadLength);
					
				}
				break;

			case SYSTEM_INICIAR_SERVICIO: case SYSTEM_DETENER_SERVICIO:
				if (iResult == SYSTEM_INICIAR_SERVICIO)
					IPCIRC = IPCIRC_CONSOLA_INICIARSERVICIO;
				else IPCIRC = IPCIRC_CONSOLA_FUERASERVICIO;

				crearCabecera (&cabecera, IPCIRC_CONSOLA, 0);
				enviarH(iSocketArray, cabecera);
								
				cabecera.ucPayloadDescriptor = IPCIRC;
				enviarH(iSocketArray, cabecera);
				
				break;
		
			case SYSTEM_ESTADO:
				crearCabecera (&cabecera, IPCIRC_CONSOLA, 0);
				enviarH(iSocketArray, cabecera);
				cabecera.ucPayloadDescriptor = IPCIRC_CONSOLA_ESTADO;
				enviarH(iSocketArray, cabecera);

				break;

			case SYSTEM_AYUDA:
				/* consola.h */
				mostrarAyuda();
				break;

			case SYSTEM_SALIR:
				terminado = 1;
				crearCabecera (&cabecera, IPCIRC_FINALIZAR_SISTEMA, 0);
				enviarH(iSocketArray, cabecera);
				break;
		}
sleep(1);
	}
	
}

/* 	Nombre de la Función: determinarComando
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 20/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Cambia el estado del disco a SINCRONIZANDO
	Entrada:	char * cvStringConsola: cadena que contien  el comando
	Salida:	-
			return: SYSTEM_CLS,SYSTEM_AGREGAR,SYSTEM_ELIMINAR,SYSTEM_REANUDAR_SERVICIO,
			SYSTEM_DETENER_SERVICIO,SYSTEM_SALIR,SYSTEM_AYUDA
*/
int determinarComando(char * cvStringConsola)
{
	if (comparar(cvStringConsola, "cls") == 0)
		return SYSTEM_CLS;

	if (comparar(cvStringConsola,"agregar") == 0)
		return SYSTEM_AGREGAR;

	if (comparar(cvStringConsola,"eliminar") == 0)
		return SYSTEM_ELIMINAR;

	if (comparar(cvStringConsola,"iniciar") == 0)
		return SYSTEM_INICIAR_SERVICIO;

	if (comparar(cvStringConsola,"detener") == 0)
		return SYSTEM_DETENER_SERVICIO;

	if (comparar(cvStringConsola,"salir") == 0)
		return SYSTEM_SALIR;

	if (comparar(cvStringConsola,"ayuda") == 0)
		return SYSTEM_AYUDA;

	if (comparar(cvStringConsola,"estado") == 0)
		return SYSTEM_ESTADO;


	return -1;

}

/* 	Nombre de la Función: mostrarAyuda
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 20/09/2009
	modificación: 20/09/2009
	Objetivo de la Función: Muestra por pantalla los comandos aceptados
	Entrada:	-
	Salida:	-
			return: -
*/

void mostrarAyuda()
{
	printf("\n***************************************************************************\n\n");
	printf("CONSOLA ARRAY\n");
	printf("sintaxis: comando argumentos\n");
	printf("	comando:	cls		argumentos: no tiene\n");
	printf("	descripcion: Limpia la pantalla\n\n");
	printf("	comando:	agregar	argumentos: nombre del dispositivo\n");
	printf("	descripcion: Indica al sistema poner en sincronizacion al\n");
	printf("	             dispositivo que se pasa como argumento\n\n");
	printf("	comando:	eliminar	argumentos: nombre del dispositivo\n");
	printf("	descripcion: Indica al sistema quitar del sistema al\n");
	printf("	             dispositivo que se pasa como argumento\n\n");
	printf("	comando:	iniciar	argumentos: no tiene\n");
	printf("	descripcion: Indica al sistema permitir recibir peticiones de\n");
	printf("	             lectura y escritura de datos\n\n");
	printf("	comando:	detener	argumentos: no tiene\n");
	printf("	descripcion: Indica al sistema impedir recibir peticiones de\n");
	printf("	             lectura y escritura de datos\n\n");
	printf("	comando:	ayuda		argumentos: no tiene\n");
	printf("	descripcion: Muestra por pantalla el ayuda\n\n");
	printf("	comando:	salir		argumentos: no tiene\n");
	printf("	descripcion: Finaliza la aplicacion\n\n");
	printf("***************************************************************************\n\n");
}

/* 	Nombre de la Función: mostrarEstadoSistema
	Versión: 0.5
	Autor/es: Villalva Emiliano
	Fechas de creación: 24/09/2009
	modificación: 24/09/2009
	Objetivo de la Función: Muestra por pantalla los estados de la estructura array y del proceso
	Entrada:	SDISCOSCONECTADOS* EstructuraArray: Estructura de datos
			int iEstado: Estado del sistema
			int *iSubEstado: Subestados que puede modificarse
			CONFIGURACION config: datos de configuracion
	Salida:	int *iSubEstado: nuevo subestado del sistema
			return: -
*/
void mostrarEstadoSistema(SDISCOSCONECTADOS* EstructuraArray, int iEstado, int *iSubEstado, CONFIGURACION config)
{
	int iConectados = 0,	iLibres = 0, iEsperando = 0, i;
	char cvEstado[30];

	printf("NOMBRE DEL ARRAY: %s\n", config.cvNombreArray);
	printf("CAPACIDAD DEL ARRAY: %d\n\n", config.iCantidadDiscos);
	
	for (i=0; i< config.iCantidadDiscos; i++)
	{
		
		if(EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_NO_CONECTADO)
			copiar(cvEstado,"Disco No Conectado");

		if(EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_CONECTADO)
		{
			copiar(cvEstado,"Disco Conectado");
			iConectados++;
		}
		if(EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_SINCRONIZANDO)
			copiar(cvEstado,"Disco Sincronizando");
		if(EstructuraArray[i].iEstadoDisco ==  DISCO_ESTADO_SINCRONIZADO)
		{
			copiar(cvEstado,"Disco Sincronizado");
			iConectados++;
		}

		if ((EstructuraArray[i].cpNombreDisco != NULL) && (EstructuraArray[i].iEstadoDisco == DISCO_ESTADO_NO_CONECTADO))
		{
				iEsperando++;
		}
		if (EstructuraArray[i].cpNombreDisco != NULL)
		{
			printf(" - Disco: %s, Estado: %s\n", EstructuraArray[i].cpNombreDisco , cvEstado);
		}
		if (EstructuraArray[i].cpNombreDisco == NULL)
		{
			iLibres++;
			printf(" - Disco: (?), Estado: %s\n", cvEstado);
		}
			
	}

	printf("\nDiscos conectados: %d, Esperando %d discos a conectar, Espacios libres: %d\n",  iConectados, iEsperando, iLibres ); 

	/* Revision de sub estados de acuerdo al estado del sistema */

	if (iEstado == ESTADO_FUERASERVICIO)
	{
		/* Obtengo el subestado */
		/* disco.h */
		*iSubEstado = subestadoFueraDeServicio(EstructuraArray, config.iCantidadDiscos);

		copiar(cvEstado, "FUERA DE SERVICIO");
		printf("ESTADO DEL ARRAY: %s\n", cvEstado);
		if (*iSubEstado== ESTADO_FUERASERVICIO_INCOMPLETO)
			copiar(cvEstado, "INCOMPLETO");
		if (*iSubEstado== ESTADO_FUERASERVICIO_SINCRONIZADO)
			copiar(cvEstado, "SINCRONIZADO");
		if (*iSubEstado== ESTADO_FUERASERVICIO_CORRUPTO)
			copiar(cvEstado, "CORRUPTO");

		printf("SUBESTADO: %s\n\n", cvEstado);

	}
	if (iEstado == ESTADO_FUNCIONAL)
	{
		*iSubEstado = subestadoEnServicio(EstructuraArray, config.iCantidadDiscos);
		copiar(cvEstado, "EN SERVICIO");
		printf("ESTADO DEL ARRAY: %s\n", cvEstado);
		if (*iSubEstado== ESTADO_INICIALIZADO_SINCRONIZADO)
			copiar(cvEstado, "SINCRONIZADO");
		if (*iSubEstado==  ESTADO_INICIALIZADO_SINCRONIZANDO)
			copiar(cvEstado, "SINCRONIZANDO");
		
		printf("SUBESTADO: %s\n\n", cvEstado);
	}
	
}

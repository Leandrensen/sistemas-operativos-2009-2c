#define NOMBRE_PROCESO "ARRAY"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include "cadenas.h"
#include "archivos.h"


/* 	Nombre de la Función: cargarConfiguracion 
	Versión: 1.0
	Autor/es: Villalva Emiliano
	Fechas de creación: 14/09/2009
	modificación: 16/09/2009
	Objetivo de la Función: Cargar en CONFIGURACION, los parámetros de configuracion
				    Crear una lista de los NOMBRES de los discos esperados
	Entrada:	-
	Salida:	CONFIGURACION* arch: Se cargan los datos de configuracion
			ptrDiscosEsperados* ListaDiscos: se carga la lista de nombres
			return: CONFIG_FAIL - CONFIG_OK
*/
int cargarConfiguracion (CONFIGURACION* arch, ptrDiscosEsperados* ListaDiscos)
{
	char *cp;
   	char buffer[30];
	ptrDiscosEsperados Lista = NULL, nuevoDisco = NULL;
	
	int iPosVector= 0;

  	FILE* fichero;

	fichero = fopen("config.txt","r");

   	if(fichero!=NULL) printf("Fichero CONFIG.TXT Abierto con exito\n" );
   	else
   	{
   		printf("Fichero CONFIG.TXT no abierto, posiblemente no exista\n" );
   		return CONFIG_FAIL;
   	}

   	fgets(arch->cvNombreArray, sizeof(arch->cvNombreArray), fichero);
	cp = strtok (arch->cvNombreArray," \n");

	fgets(arch->cvArrayIP, sizeof(arch->cvArrayIP), fichero);
	cp = strtok (arch->cvArrayIP," \n");

   	fgets(buffer, 8, fichero);
	cp = strtok (buffer," \n");
       arch->iArrayPuerto = atoi(buffer);

	fgets(arch->cvVSKIP, sizeof(arch->cvVSKIP), fichero);
	cp = strtok (arch->cvVSKIP," \n");

   	fgets(buffer, 8, fichero);
	cp = strtok (buffer," \n");
       arch->iVSKPuerto = atoi(buffer);

	fgets(buffer, 8, fichero);
	cp = strtok (buffer," \n");
       arch->iCantidadDiscos = atoi(buffer);

	fgets(buffer, 8, fichero);
	cp = strtok (buffer," \n");
       arch->iCilindros = atoi(buffer);

	fgets(buffer, 8, fichero);
	cp = strtok (buffer," \n");
       arch->iCabezas = atoi(buffer);

	fgets(buffer, 8, fichero);
	cp = strtok (buffer," \n");
       arch->iSectores = atoi(buffer);

	
	/* Cargo todos los nombres de los Discos si hay para cargar*/
	
	do
	{	
		/* Leo la linea del archivo */
		fgets(buffer,sizeof(buffer), fichero);
		if (buffer[0] != '-')
		{
			cp = strtok (buffer," \n");
			nuevoDisco = (ptrDiscosEsperados) malloc(sizeof(SDISCOSESPERADOS));
			
			nuevoDisco->cvNombreDisco = (char*) malloc (strlen(cp) + 1);
			copiar(nuevoDisco->cvNombreDisco,buffer);
			copiar(nuevoDisco->cvArrayIP,arch->cvArrayIP);

			nuevoDisco->sgte = Lista;
			Lista = nuevoDisco;
			iPosVector++;
		}

	}while(buffer[0] != '-');

		
	*ListaDiscos = Lista;
	mostrarConfiguracion (*arch, &Lista);

	/* Obtengo la lista de Discos que se esperan para conectar */
	

	if(!fclose(fichero)) printf( "Fichero CONFIG.TXT cerrado con exito\n\n" );
    	else
       {
            	printf("El Fichero CONFIG.TXT no pudo cerrarse\n" );
   		return CONFIG_FAIL;
       }

   	return CONFIG_OK;
}

/* 	Nombre de la Función: mostrarConfiguracion 
	Versión: 1.0
	Autor/es: Villalva Emanuel - Villalva Emiliano
	Fechas de creación: 13/09/2009
	modificación: 16/09/2009
	Objetivo de la Función: Mostrar por pantalla los datos obtenidos de la configuracion
	Entrada:	CONFIGURACION configuraciones: Se cargan los datos de configuracion
			ptrDiscosEsperados* listaDiscos: se carga la lista de nombres
	Salida:	-
			return: -
*/
void mostrarConfiguracion (CONFIGURACION configuraciones, ptrDiscosEsperados* listaDiscos)
{
	ptrDiscosEsperados lista = NULL, ptrDisco = NULL;

	lista = * listaDiscos;

	/* Mostrar cada campo del registro */
	printf("***** PROCESO ARRAY ************************************\n");
	printf("	NOMBRE ARRAY: %s\n", configuraciones.cvNombreArray);
       printf("   	IP: %s\n", configuraciones.cvArrayIP);
	printf("   	PUERTO: %d\n", configuraciones.iArrayPuerto);
	printf("   	IP DESTINO - VSK o ARRAY: %s\n", configuraciones.cvVSKIP);
	printf("   	PUERTO DESTINO - VSK o ARRAY: %d\n", configuraciones.iVSKPuerto);
	printf("	CANTIDAD DE DISCOS: %d\n", configuraciones.iCantidadDiscos);
	printf("	CHS: %d , %d , %d\n",configuraciones.iCilindros,configuraciones.iCabezas,configuraciones.iSectores);
	printf("	Listado de los discos en espera:\n");
	
	if (lista == NULL) printf ("NO HAY DISCOS EN ESPERA\n");
	
	for(ptrDisco = lista; ptrDisco != NULL; ptrDisco = ptrDisco->sgte)
	{
		printf("	- %s\n",ptrDisco->cvNombreDisco);
	}
	
	
	printf("********************************************************\n");

	return;
}

/* 	Nombre de la Función: mostrarConfiguracion 
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 13/09/2009
	modificación: 13/09/2009
	Objetivo de la Función: Crear el archivo de LOG
	Entrada:	-
	Salida:	-
			return: 0 Error - 1 OK
*/
int crearLog ()
{
	FILE* archLog;
	if ((archLog = fopen ("log.txt","w")) == NULL)
	{
            printf("El archivo crearLog no se pudo crear\n");
            return 0;
	}
	fclose (archLog);
	return 1;
}

/* 	Nombre de la Función: escribirLog
	Versión: 1.0
	Autor/es: Villalva Emanuel - Mendez Leandro
	Fechas de creación: 13/09/2009
	modificación: 13/09/2009
	Objetivo de la Función: Escribir en el archivo LOG
	Entrada:	char cTipoLog: Tipo de Informacion. I : INFO, E : ERROR, W : WARNING, D : DEBUG
 			char* cpMensaje: Mensaje a escribir
	Salida:	-
			return: 1 OK
*/
int escribirLog(char cTipoLog, char* cpMensaje)
{
    char *cpFecha, *p;
    char cvProceso[]=NOMBRE_PROCESO;
    time_t t;
    pid_t miPID;
    char cmiPIDc[6];
    FILE* archLog;

/*	mutex_lock(&archLog_mutex);*/
    archLog = fopen ("log.txt", "a");

    miPID = getpid();

    time(&t);
    cpFecha = ctime(&t);

    p = strtok(cpFecha," ");
    p = strtok(NULL," ");
    p = strtok(NULL," ");
    p = strtok(NULL," ");

    fputs(p, archLog);
    fputs(" ", archLog);
    fputs(cvProceso, archLog);
    fputs(" ", archLog);
    fputs("[", archLog);

    miitoa(cmiPIDc, (long)miPID);
    fputs(cmiPIDc, archLog);
    fputs("]:",archLog);

    switch(cTipoLog)
    {
             case('I'): fputs("INFO: ", archLog);
                       break;

	     case('E'): fputs("ERROR: ", archLog);
                       break;

             case('W'): fputs("WARNING ", archLog);
                       break;

             case('D'): fputs("DEBUG ", archLog);
                       break;

             case('C'):fputs("INFO:", archLog);
                      fputs("Proceso terminado.\n", archLog);
                      break;
    }

	if (cTipoLog != 'C')
	{
		fputs(cpMensaje, archLog);
		fputs(".\n",archLog);
	}

    fclose (archLog);

/*	mutex_unlock(&archLog_mutex);*/
    return 1;
}

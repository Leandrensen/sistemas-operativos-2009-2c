#define NOMBRE_PROCESO "PROCESO"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include "archivos.h"
#include "cadenas.h"


/* 	Nombre de la Función: cargarConfiguracion
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: cargar en registro datos iniciales
	Entrada:	-
	Salida:	arch: registro con datos iniciales
			return: 1 si fue correcto
				 0 si no fue correcto
*/
int cargarConfiguracion (CONFIGURACION* arch)
{
	char *cp;
   	char cvPUERTO[8];
	char cvCache[3];

  	FILE* fichero;

	fichero = fopen("config.txt","r");

   	if(fichero!=NULL) printf("Fichero CONFIG.TXT Abierto con exito\n" );
   	else
   	{
   		printf("Fichero CONFIG.TXT no abierto, posiblemente no exista\n" );
   		return 0;
   	}

   	fgets(arch->cvIP, 20, fichero);
	cp = strtok (arch->cvIP," \n");

   	fgets(cvPUERTO, 8, fichero);
	cp = strtok (cvPUERTO," \n");
        arch->iPuerto = atoi(cvPUERTO);

   	fgets(arch->cvIPLdap, 20, fichero);
	cp = strtok (arch->cvIPLdap," \n");

   	fgets(cvPUERTO, 8, fichero);
	cp = strtok (cvPUERTO," \n");
        arch->iPuertoLdap = atoi(cvPUERTO);

   	fgets(arch->cvUsuarioLdap, 20, fichero);
	cp = strtok (arch->cvUsuarioLdap," \n");

   	fgets(arch->cvContraseniaLdap, 20, fichero);
	cp = strtok (arch->cvContraseniaLdap," \n");

	fgets(cvCache, 20, fichero);
	cp = strtok (cvCache," \n");
	arch->iCache = atoi (cvCache);
	
	fgets(cvCache, 20, fichero);
	cp = strtok (cvCache," \n");
	arch->iTamanioCache = atoi (cvCache);


	if(!fclose(fichero)) printf( "Fichero CONFIG.TXT cerrado con exito\n\n" );
    	else
        {
            printf("El Fichero CONFIG.TXT no pudo cerrarse\n" );
   		return 0;
        }

   	return 1;
}

/* 	Nombre de la Función: cargarConfiguracion
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 19/09/2009
	modificación:  19/09/2009
	Objetivo de la Función: mostrar por pantalla datos iniciales
	Entrada:	arch: registro con datos iniciales
	Salida:	-
*/
void mostrarConfiguracion (CONFIGURACION configuraciones)
{

	/* Mostrar cada campo del registro */
	printf ("************ PROCESO VSK ***************\n");
       printf("La IP es: %s\n", configuraciones.cvIP);
	printf("El Puerto es: %d\n", configuraciones.iPuerto);
	printf("El IPLDAP es: %s\n", configuraciones.cvIPLdap);
	printf("El PuertoLDAP es: %d\n", configuraciones.iPuertoLdap);
	printf("El Usuario LDAP es: %s\n", configuraciones.cvUsuarioLdap);
	printf("La Contraseña LDAP es: %s\n", configuraciones.cvContraseniaLdap);
	printf("Utiliza Cache: %d (1 SI, 0 NO)\n",configuraciones.iCache);
	printf("Tamanio Cache (valido si Utiliza Cache = 1):%d\n", configuraciones.iTamanioCache);
	printf ("************ PROCESO VSK ***************\n\n");


	return;
}

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

int escribirLog(char cTipoLog, char* cpMensaje)
{
    char *cpFecha, *p;
    char cvProceso[]=NOMBRE_PROCESO;
    time_t t;
    pid_t miPID;
    char cmiPIDc[6];
    FILE* archLog;
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

    return 1;
}

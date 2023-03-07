#ifndef ARCHIVOS_C_
#define ARCHIVOS_C_

#include "archivos.h"

int cargarConfiguracion(CONFIGURACION* arch)
{
	char *cp;
   	char cvPUERTO[8];
   	
   	FILE* fichero;

	fichero = fopen("./Cache/config.txt","r");

   	if(fichero != NULL) 
   		printf("Fichero CONFIG.TXT Abierto con exito\n" );
   	else
   	{
   		printf("Fichero CONFIG.TXT no abierto, posiblemente no exista\n" );
   		return 0;
   	}
   	
   	fgets(arch->cvNombreDisco, 31, fichero);
	cp = strtok (arch->cvNombreDisco,"\n");
       
   	fgets(arch->cvIP1, 20, fichero);
	cp = strtok (arch->cvIP1," \n");

   	fgets(cvPUERTO, 8, fichero);
	cp = strtok (cvPUERTO," \n");
    arch->iPuerto1 = atoi(cp);
        
    fgets(arch->cvIP2, 20, fichero);
	cp = strtok (arch->cvIP2," \n");

   	fgets(cvPUERTO, 8, fichero);
	cp = strtok (cvPUERTO," \n");
    arch->iPuerto2 = atoi(cp);
    
    fgets(arch->cvIPDisco, 20, fichero);
	cp = strtok (arch->cvIP2," \n");

   	fgets(cvPUERTO, 8, fichero);
	cp = strtok (cvPUERTO," \n");
    arch->iPuertoDisco = atoi(cp);
    
    if(!fclose(fichero)) 
    	printf("Fichero CONFIG.TXT cerrado con exito\n\n" );
    
    else
    {
    	printf("El Fichero CONFIG.TXT no pudo cerrarse\n" );
   		return 0;
    }

   	return 1;
}

int crearLog()
{
	FILE* archLog;
	if ((archLog = fopen ("./Cache/log.txt","w")) == NULL)
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
    archLog = fopen("./Cache/log.txt", "a");

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

#endif /*ARCHIVOS_C_*/

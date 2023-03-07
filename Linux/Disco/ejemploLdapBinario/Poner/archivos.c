#include "archivos.h"


int cargarConfiguracion (CONFIGURACION* arch)
{
	char *cp, *cp2;
   	char cvPUERTO[8];
   	char cvCHS[20];
   	char cvRPM[20];
	char cvTiempoCache[22];
	int iAux;
   	

  	FILE* fichero;

	fichero = fopen("config.txt","r");

   	if(fichero!=NULL) printf("Fichero CONFIG.TXT Abierto con exito\n" );
   	else
   	{
   		printf("Fichero CONFIG.TXT no abierto, posiblemente no exista\n" );
   		return 0;
   	}
   	
   	fgets(arch->cvNombreDisco, 31, fichero);
	cp = strtok (arch->cvNombreDisco,"\n");
       
   	fgets(arch->cvIP, 20, fichero);
	cp = strtok (arch->cvIP," \n");

   	fgets(cvPUERTO, 8, fichero);
	cp = strtok (cvPUERTO," \n");
       arch->iPuerto = atoi(cp);
        
       fgets(arch->cvIPArrayVSK, 20, fichero);
	cp = strtok (arch->cvIPArrayVSK," \n");

   	fgets(cvPUERTO, 8, fichero);
	cp = strtok (cvPUERTO," \n");
       arch->iPuertoArrayVSK = atoi(cp);
        
    fgets(cvCHS, 20, fichero);
	cp = strtok (cvCHS,"- \n");
    arch->stCHS.iCilinders = atoi(cp);
    cp = strtok (NULL,"- \n");
    arch->stCHS.iHeaders = atoi(cp);
    cp = strtok (NULL,"- \n");
    arch->stCHS.iSectors = atoi(cp);
        
    fgets(cvRPM, 20, fichero);
    cp = strtok (cvRPM," \n");
    arch->iRPM = atoi(cvRPM);
    
    fgets(arch->cvCache, 10, fichero);
	cp = strtok (arch->cvCache," \n");
	
	fgets(arch->cvLdapIp, 50, fichero);
	cp = strtok(arch->cvLdapIp," \n");
	printf("%s\n", arch->cvLdapIp);

	fgets(cvPUERTO, 50, fichero);
	cp = strtok(cvPUERTO," \n");
	arch->iLdapPuerto = atoi(cp);
	printf("%d\n", arch->iLdapPuerto);	

	fgets(arch->cvLdapPass, 50, fichero);
	cp = strtok(arch->cvLdapPass," \n");
	printf("%s\n", arch->cvLdapPass);

	fgets(cvTiempoCache, 22, fichero);
    	cp = strtok (cvTiempoCache," \n");
	cp2 = strtok(cp, ".");
	cp = strtok(NULL, "\0");	
	iAux = atoi(cp2);
    	arch->iTiempoAccesoACache = iAux*10 + atoi(cp);
		   
	if(!fclose(fichero)) printf("Fichero CONFIG.TXT cerrado con exito\n\n" );
    
    else
    {
    	printf("El Fichero CONFIG.TXT no pudo cerrarse\n" );
   		return 0;
    }

   	return 1;
}

void mostrarConfiguracion (CONFIGURACION configuraciones)
{

	/* Mostrar cada campo del registro */
        printf("La IP es: %s\n", configuraciones.cvIP);

	return;
}

int crearLog()
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
    archLog = fopen("log.txt", "a");

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

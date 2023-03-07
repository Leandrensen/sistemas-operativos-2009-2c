#define NOMBRE_PROCESO "PROCESO"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include "archivos.h"
#include "cadenas.h"

int cargarConfiguracion (CONFIGURACION* arch, FILE *archLog)
{
	char *cp;
	char *cvIP = NULL;
	char *cvPuerto = NULL;
	char *cvIPVsk = NULL;
	char *cvPuertoVsk = NULL;
	char *cvIPLDAP = NULL;
	char *cvPuertoLDAP = NULL;
	char *cvPassword = NULL;
	char *cvUsrLDAP = NULL;

  	FILE* fichero;

	cvIP = (char *)malloc(sizeof(char)*20);
    cvPuerto = (char *)malloc(sizeof(char)*8);
	cvIPVsk = (char *)malloc(sizeof(char)*20);
    cvPuertoVsk = (char *)malloc(sizeof(char)*8);
	cvIPLDAP = (char *)malloc(sizeof(char)*20);
    cvPuertoLDAP = (char *)malloc(sizeof(char)*8);
	cvPassword = (char *)malloc(sizeof(char)*30);
	cvUsrLDAP = (char *)malloc(sizeof(char)*30);
 
	fichero = fopen("config.txt","r");

   	if(fichero!=NULL) 
	{
		printf("Fichero CONFIG.TXT del SFS Abierto con exito\n" );
	}	
   	else
   	{
   		printf("Fichero CONFIG.TXT no abierto, posiblemente no exista\n" );
		free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
   	}
		
	fgets(cvIP, 20, fichero);
    cp = strtok(cvIP," \n");
    if (cvIP[0] != '\0')
    {
        strcpy(arch->cvIP, cvIP);
        printf("El IP del SFS es: %s\n", arch->cvIP);
    }
    else
    {
        printf("Error al obtener IP del SFS\n");
        free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
    }
	
    fgets(cvPuerto, 8, fichero);
    cp = strtok(cvPuerto," \n");
    if(cvPuerto[0] != '\0')
    {
        arch->iPuerto = miatoi(cvPuerto);
        printf("El Puerto del SFS es: %d\n", arch->iPuerto);
    }
    else
    {
        printf("Error al obtener Puerto del SFS\n");
        free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
    }
    
	fgets(cvIPVsk, 20, fichero);
    cp = strtok(cvIPVsk," \n");
    if (cvIPVsk[0] != '\0')
    {
        strcpy(arch->cvIPVsk, cvIPVsk);
        printf("El IP del VSK es: %s\n", arch->cvIPVsk);
    }
    else
    {
        printf("Error al obtener IP del VSK\n");
        free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
    }
	
    fgets(cvPuertoVsk, 8, fichero);
    cp = strtok(cvPuertoVsk," \n");
    if(cvPuertoVsk[0] != '\0')
    {
        arch->iPuertoVsk = miatoi(cvPuertoVsk);
        printf("El Puerto del VSK es: %d\n", arch->iPuertoVsk);
    }
    else
    {
        printf("Error al obtener Puerto del VSK\n");
        free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
    }
    
	fgets(cvIPLDAP, 20, fichero);
    cp = strtok(cvIPLDAP," \n\r");
    if (cvIPLDAP[0] != '\0')
    {
        strcpy(arch->cvLdapIp, cvIPLDAP);
        printf("La IP del host del servidor LDAP es: %s\n", arch->cvLdapIp);
    }
    else
    {
        printf("Error al obtener IP del host del servidor LDAP\n");
        free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
    }
	
    fgets(cvPuertoLDAP, 8, fichero);
    cp = strtok(cvPuertoLDAP," \n\r");
    if(cvPuertoLDAP[0] != '\0')
    {
        arch->iLdapPuerto = miatoi(cvPuertoLDAP);
        printf("El Puerto del servidor LDAP es: %d\n", arch->iLdapPuerto);
    }
    else
    {
        printf("Error al obtener Puerto del servidor LDAP\n");
        free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
    }
	
	fgets(cvPassword, 30, fichero);
    cp = strtok(cvPassword," \n\r");
    if (cvPassword[0] != '\0')
    {
        strcpy(arch->cvLdapPass, cvPassword);
        printf("El password del servidor LDAP es: %s\n", arch->cvLdapPass);
    }
    else
    {
        printf("Error al obtener el password del servidor LDAP\n");
        free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
    }
    
	fgets(cvUsrLDAP, 30, fichero);
    cp = strtok(cvUsrLDAP," \n");
    if (cvUsrLDAP[0] != '\0')
    {
        strcpy(arch->cvLdapUsr, cvUsrLDAP);
        printf("El usuario del servidor LDAP es: %s\n", arch->cvLdapUsr);
    }
    else
    {
        printf("Error al obtener el usuario del servidor LDAP\n");
        free(cvIP);
        free(cvPuerto);
        free(cvIPVsk);
        free(cvPuertoVsk);
        free(cvIPLDAP);
        free(cvPuertoLDAP);
        free(cvPassword);
        free(cvUsrLDAP);
   		return 0;
    }	

    free(cvIP);
	free(cvPuerto);
	free(cvIPVsk);
	free(cvPuertoVsk);
	free(cvIPLDAP);
	free(cvPuertoLDAP);
	free(cvPassword);
	free(cvUsrLDAP);

	if(!fclose(fichero)) printf( "Fichero CONFIG.TXT del SFS cerrado con exito\n\n" );
    	else
        {
            printf("El Fichero CONFIG.TXT no pudo cerrarse\n" );
			return 0;
        }

   	return 1;
}

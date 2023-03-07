#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ejemploLdapBinarios.h"
#include "initBind.h"
#include "funcionesLdap.h"
#include "archivos.h"

int main (void)
{ 
	
	LDAPINIT ldap_sesion;
	LDAPAGREGARSECTOR ldap_agregarSector;
	unsigned char ucBufSector[512];
	CONFIGURACION stArchConf;
	int i, iReturn;
	char cvIdSector[21];
	LDAPMODIFICARSECTOR ldap_modificarSector;
	LDAPSECTOR ldap_getsector;
	int fd;
	long int sizeWrite = 0;
	
	
	/****************************** ARCHIVO DE CONFIGURACION Y LOG ***********************************/
	
	cargarConfiguracion(&stArchConf); /* archivos.h */
	
	/****************************** CONEXION CON LDAP ******************************************/

	ldapCargarSesion(&ldap_sesion, stArchConf.cvLdapIp, stArchConf.iLdapPuerto, stArchConf.cvLdapPass);

	if(LDAP_INICIAR_OK == ldapIniciarSesion(&ldap_sesion))
	{
		for(i = 0; i < stArchConf.stCHS.iCilinders*stArchConf.stCHS.iSectors; i++)
		{
			miitoa(cvIdSector, i);
			cargarLDAPAGREGARSECTOR(&ldap_agregarSector, stArchConf.cvNombreDisco, cvIdSector, "");
			ldapAgregarSector(ldap_sesion.ldapSesion, ldap_agregarSector);
		}
	}
	
	/*******************************************************************************************/
	if ((fd = open("salida.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP )) == -1) 
	{
		printf("Cannot open output file\n"); 
		return -1;
    } 
	
	printf("Obtenemos el archivo de la base LDAP\n");
	
	for(i = 0; i < 2; i++)
	{	
		miitoa(cvIdSector, i);
		cargarSector(&ldap_getsector, stArchConf.cvNombreDisco, cvIdSector);
		iReturn = ldapGetSectorBin(ldap_sesion.ldapSesion, ldap_getsector, (char *)ucBufSector);
		sizeWrite = write(fd, ucBufSector, iReturn);
        printf ("Bytes write %ld\n", (long)sizeWrite);                          	
	}
	
	close(fd);
	printf("Terminamos de obtener el archivo de la base de LDAP\n");
	
	ldapFinSesion(ldap_sesion);	
	return 1;
}

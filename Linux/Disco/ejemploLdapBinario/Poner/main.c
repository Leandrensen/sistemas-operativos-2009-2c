#include <stdio.h>
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
	
	printf("Modificamos en la base LDAP\n");
	/***** aca sacamos 512 bytes del archivo binario *****/
	cargarLDAPMODIFICARSECTOR(&ldap_modificarSector, stArchConf.cvNombreDisco, cvIdSector, (char *)ucBufSector);
	iReturn = ldapModificarSectorBin(ldap_sesion.ldapSesion, ldap_modificarSector);
	printf("Terminamos de modificar en la base LDAP\n");	
	
	ldapFinSesion(ldap_sesion);
	
	return 1;
}

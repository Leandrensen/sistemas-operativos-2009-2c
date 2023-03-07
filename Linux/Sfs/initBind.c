#include <stdio.h>
#include <stdlib.h>
#include <ldap.h>
#include <string.h>
#include "initBind.h"

void ldapCargarSesion(LDAPINIT* ldap_sesion, char* ldapHost, int ldapPort, char* ldapPass)
{
	/* por configuracion */

	ldap_sesion->ldapHost = (char*)malloc(strlen(ldapHost) + 1);
	strcpy(ldap_sesion->ldapHost,ldapHost);
	
	/* por configuracion */
	ldap_sesion->ldapPort = ldapPort;

	ldap_sesion->ldapDN = (char*)malloc(sizeof("cn=Directory Manager") + 1);
	strcpy(ldap_sesion->ldapDN,"cn=Directory Manager");
	
	/* por configuracion */
	ldap_sesion->ldapPass= (char*)malloc(strlen(ldapPass) + 1);
	strcpy(ldap_sesion->ldapPass,ldapPass);

}

int ldapIniciarSesion(LDAPINIT* info)
{
	int rc, version;
    	version = LDAP_VERSION3;

    	ldap_set_option( NULL, LDAP_OPT_PROTOCOL_VERSION, &version);

	if (( info->ldapSesion = ldap_init( info->ldapHost,info->ldapPort)) == NULL)
    	{
        	printf ( "\n\tLDAP session initialization failed\n");
        	return LDAP_INIT_FAIL;
    	}
    	printf ( "LDAP: Sesion iniciada\n");

	
 	rc = ldap_simple_bind_s( info->ldapSesion, info->ldapDN, info->ldapPass);
    	if (rc != LDAP_SUCCESS )
    	{
        	printf("ldap_simple_bind_s: %s\n", ldap_err2string( rc ));
        	ldap_unbind_s ( info->ldapSesion);
        	return LDAP_BIND_FAIL;
    	}
   	printf("LDAP: Bind correcto\n");
	
	return LDAP_INICIAR_OK;
}

void ldapFinSesion(LDAPINIT info)
{
	ldap_unbind_s(info.ldapSesion);
	printf("LDAP: sesion finalizada\n");	
}


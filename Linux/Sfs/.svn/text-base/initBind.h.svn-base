#ifndef _INITBIND_H
#define _INITBIND_H

#define LDAP_INIT_FAIL	-500
#define LDAP_BIND_FAIL	-501
#define LDAP_INICIAR_OK	-502

typedef struct ldapInit
{
	char* ldapHost;
	int ldapPort;
	char* ldapDN;
	char* ldapPass;
	LDAP *ldapSesion; 
}LDAPINIT;

int ldapIniciarSesion(LDAPINIT* info);
void ldapFinSesion(LDAPINIT info);
void ldapCargarSesion(LDAPINIT* info, char* ldapHost, int ldapPort, char* ldapPass);

#endif

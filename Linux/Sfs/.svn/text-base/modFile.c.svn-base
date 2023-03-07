#include <stdio.h>
#include <stdlib.h>
#include <ldap.h>
#include <string.h>
#include "modFile.h"

void cargarModifircarArchivo(LDAPMODIFICARARCHIVO* info, char* nombreDA, char* nombreArchivo, char* listaSectores)
{
	strcpy(info->nombreDA, nombreDA );
	strcpy(info->nombreArchivo,  nombreArchivo);
	info->listaSectores = listaSectores;
}

void cargarModifircarArchivoTamanio(LDAPMODIFICARARCHIVO* info, char* nombreDA, char* nombreArchivo, char* tamanio)
{
	strcpy(info->nombreDA, nombreDA );
	strcpy(info->nombreArchivo,  nombreArchivo);
	strcpy(info->tamanio,  tamanio);
}

int ldapModificarArchivo( LDAP*ld, LDAPMODIFICARARCHIVO info)
{ 

    int          rc; 
    char         *modifyDN;
  
  /* Los attributos que elegimos modificar para el ejemplo */ 
    char        *vutnSectorList[2];    


    LDAPMod     *modify[4];
    LDAPMod     modutnSectorList;

    /*
      Una modificacion implica localizar la entrada para modificar, 
      y aplicar la  modificacion. Por eso se provee el dn.
      El dn (distinguish name) es el identificador univoco para una entrada, 
      es el que permite ubicarla, por eso se provee.    
     */
    modifyDN =(char*) malloc (strlen("utnFileName=")+ strlen(info.nombreArchivo)+ strlen(",")
				+ strlen("utnDAName=") + strlen(info.nombreDA) 
				+ strlen(",ou=sadico,dc=utn,dc=edu")+ 5);


	sprintf(modifyDN, "utnFileName=%s,utnDAName=%s,ou=sadico,dc=utn,dc=edu",info.nombreArchivo,info.nombreDA);


    /* 
     * To modify the attributes of an entry
     *    1  Specify the modify actions    
     *    2. Specify the attribute name to be modified 
     *    3. Specify the value of the attribute
     *    4. Add to an array of LDAPMod structures
     *    5. Call ldap_modify_ext_s
     */


    /* 
       Cambiamos la lista de sectores que componen el archivo
     * LDAP_MOD_REPLACE succeeds whether the value already exists or not.
     * 
     */
    modutnSectorList.mod_op     = LDAP_MOD_REPLACE; /* Se remplaza algo ya existente (o no)*/
    modutnSectorList.mod_type   = "utnSectorList";
    vutnSectorList[0]           = info.listaSectores;
    vutnSectorList[1]           = NULL;
    modutnSectorList.mod_values = vutnSectorList;

    modify[0] = &modutnSectorList;
    modify[1] = NULL;

    /*
      modifyDN, para identificar la entrada en la base.
      modify, que describe los cambios. 
      Modify the attributes 
    */
    rc= ldap_modify_ext_s( ld,         /* LDAP session handle */
                           modifyDN,   /* the object to modify */
                           modify,     /* array of LDAPMod structures */
                           NULL,       /* server controls */
                           NULL);      /* client controls */

    if ( rc != LDAP_SUCCESS )
    {
        printf("ldap_modify_ext_s: %s\n", ldap_err2string( rc ));
        return(1);
    }
    printf("\n\t%s modified successfully.\n", modifyDN );
    return( 0 );
}

int ldapModificarArchivoTamanio( LDAP*ld, LDAPMODIFICARARCHIVO info)
{ 

    int          rc; 
    char         *modifyDN;
  
  /* Los attributos que elegimos modificar para el ejemplo */ 
    char        *vmodutnSize[2];    


    LDAPMod     *modify[4];
    LDAPMod     modutnSize;

    /*
      Una modificacion implica localizar la entrada para modificar, 
      y aplicar la  modificacion. Por eso se provee el dn.
      El dn (distinguish name) es el identificador univoco para una entrada, 
      es el que permite ubicarla, por eso se provee.    
     */
    modifyDN =(char*) malloc (strlen("utnFileName=")+ strlen(info.nombreArchivo)+ strlen(",")
				+ strlen("utnDAName=") + strlen(info.nombreDA) 
				+ strlen(",ou=sadico,dc=utn,dc=edu")+ 5);


	sprintf(modifyDN, "utnFileName=%s,utnDAName=%s,ou=sadico,dc=utn,dc=edu",info.nombreArchivo,info.nombreDA);


    /* 
     * To modify the attributes of an entry
     *    1  Specify the modify actions    
     *    2. Specify the attribute name to be modified 
     *    3. Specify the value of the attribute
     *    4. Add to an array of LDAPMod structures
     *    5. Call ldap_modify_ext_s
     */


    /* 
       Cambiamos la lista de sectores que componen el archivo
     * LDAP_MOD_REPLACE succeeds whether the value already exists or not.
     * 
     */
	
	modutnSize.mod_op     = LDAP_MOD_REPLACE;   /* Idem  */
    modutnSize.mod_type   = "utnSize";
    vmodutnSize[0]           = info.tamanio;
    vmodutnSize[1]           = NULL;
    modutnSize.mod_values = vmodutnSize;

    modify[0] = &modutnSize;
    modify[1] = NULL;

    /*
      modifyDN, para identificar la entrada en la base.
      modify, que describe los cambios. 
      Modify the attributes 
    */
    rc= ldap_modify_ext_s( ld,         /* LDAP session handle */
                           modifyDN,   /* the object to modify */
                           modify,     /* array of LDAPMod structures */
                           NULL,       /* server controls */
                           NULL);      /* client controls */

    if ( rc != LDAP_SUCCESS )
    {
        printf("ldap_modify_ext_s: %s\n", ldap_err2string( rc ));
        return(1);
    }
    printf("\n\t%s modified successfully.\n", modifyDN );
    return( 0 );
}

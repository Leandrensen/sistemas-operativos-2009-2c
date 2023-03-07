#include "ejemploLdapBinarios.h"

int ldapModificarSectorBin(LDAP *ld, LDAPMODIFICARSECTOR info)
{
    int         rc, i; 
    char        modifyDN[500];
  
  /* Los attributos que elegimos modificar para el ejemplo */ 
    /*char        *vutnSectorContent[4];*/
       
    LDAPMod     *modify[4];
    LDAPMod     modutnSectorContent;
	FILE *fp;
	char *binary_data;
	struct berval binary_berval;
	struct berval *binary_values[2];
	size_t cantBytesLeidos = 0;
	
	/* Open the audio file and read it into memory. */
	if ( ( fp = fopen( "caca.txt", "rb" ) ) == NULL ) 
	{
		perror( "fopen" );
		return( 1 );
	}
	
	if((binary_data = (char *)malloc(512 * sizeof(char)))  == NULL)
	{
		perror( "malloc" );
		return( 1 );
	}

	for(i = 0; (cantBytesLeidos = fread(binary_data, 1, 512, fp)) != 0; i++) 
	{
		/*
		Una modificacion implica localizar la entrada para modificar, 
		y aplicar la  modificacion. Por eso se provee el dn.
		El dn (distinguish name) es el identificador univoco para una entrada, 
		es el que permite ubicarla, por eso se provee.    
		*/
		
		sprintf(modifyDN,"utnSectorID=%d+utnDAName=%s,ou=disks,dc=utn,dc=edu",i,info.nombreDA); 

		/* 
		* To modify the attributes of an entry
		*    1  Specify the modify actions    
		*    2. Specify the attribute name to be modified 
		*    3. Specify the value of the attribute
		*    4. Add to an array of LDAPMod structures
		*    5. Call ldap_modify_ext_s
		*/


		/* 
		* LDAP_MOD_ADD gives an error if the value already exists.
		* Here I will try to add other keyword 
		*/
		modutnSectorContent.mod_op = (LDAP_MOD_REPLACE | LDAP_MOD_BVALUES);        /* Aca quiero remplazar. */
		modutnSectorContent.mod_type = "utnSectorContent";  
		/* vutnSectorContent[0] = info.contenido;
		vutnSectorContent[1] = NULL; */
		binary_berval.bv_len = cantBytesLeidos;
		binary_berval.bv_val = binary_data;
		binary_values[0] = &binary_berval;
		binary_values[1] = NULL;
		modutnSectorContent.mod_values = binary_values;
	
		modify[0] = &modutnSectorContent;
		modify[1] = NULL;

		/*
		modifyDN, para identificar la entrada en la base.
		modify, que describe los cambios. 
		Modify the attributes 
		*/
		rc = ldap_modify_ext_s( ld,         /* LDAP session handle */
							modifyDN,   /* the object to modify */
							modify,     /* array of LDAPMod structures */
							NULL,       /* server controls */
							NULL);      /* client controls */

		if (rc != LDAP_SUCCESS)
		{
			printf("ldap_modify_ext_s: %s\n", ldap_err2string( rc ));
			return 0;
		}
		printf("\n\t%s modified successfully.\n", modifyDN);
	}
	
	fclose( fp );
    
    return 1;
}

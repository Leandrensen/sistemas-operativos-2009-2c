#include "ejemploLdapBinarios.h"

int ldapGetSectorBin(LDAP *ld, LDAPSECTOR info, char* pBuffer)
{   
    int         version,rc, entryCount, ifor;
    char        searchBase[]="ou=disks,dc=utn,dc=edu";   
    char        *attribute, *dn, **values;       
    BerElement  *ber;
    LDAPMessage *searchResult, *entry;

    char filter[64];
    int i;
	struct berval content;     
    struct berval ** contentList;
    
    /* Agregado por Leandro Mendez: Se sabe que el tamanio de pBuffer es 
     *	siempre 512 por ser siempre un sector. Entonces lo inicializo con 
     *	'\0' para que no mande basura y para que no se repitan los datos.  
     *	Muy necesario. Cualquier cosa preguntar */

    for(i = 0; i < 512; i++)
    	pBuffer[i] = '\0';

    /********************************************************************/	
    /* Set LDAP version to 3 and set connection timeout. */
    version = LDAP_VERSION3;
 
   /* Aca armo el filtro */

   sprintf(filter, "(&(utnDAName=%s)(utnSectorID=%s))", info.utnDAName, info.utnSectorID);

   /* Search the directory */
   rc = ldap_search_ext_s(  
                    ld,                    /* LDAP session handle */
                    searchBase,            /* container to search */
                    LDAP_SCOPE_SUBTREE,    /* search scope */
                    filter,                /* search filter */
                    NULL,                  /* return all attributes */
                    0,                     /* return attributes and values */
                    NULL,                  /* server controls */
                    NULL,                  /* client controls */
                    LDAP_NO_LIMIT,         /* search timeout */
                    LDAP_NO_LIMIT,         /* no size limit */
                    &searchResult );       /* returned results */
    
    if ( rc != LDAP_SUCCESS ) 
    {
        printf("ldap_search_ext_s: %s\n", ldap_err2string( rc ));
        ldap_msgfree( searchResult );
        ldap_unbind_s( ld );
        return ( 1 );
    }   
     

    /* 
      Ya se hizo la busqueda, ya se recibio la respuesta, 
      en la estructura searchResult.
      Ahora hay que recorrer esa estructura para ver los resultados.
      
      Hay tres for anidados:
         FOR 1 -> numero de entradas devueltas (recorre cada entrada)
           FOR 1 -> numero de atributos en esta entrada (recorre atributos de una entrada)
                    En ldap los atributos permitidos, pueden existir o no, 
                    en cada una de las entrdas independientemente 
                    de la entrada.                       
             FOR 1 -> numero de valores para el atributo corriente.
                      En ldap los atributos pueden tener mas de un valor.
                       
    
      Go through the search results by checking entries 
    */
    
    /* Este es el for que itera por entradas */
    for (   entry   =   ldap_first_entry( ld, searchResult ); 
            entry   !=  NULL; 
            entry   =   ldap_next_entry( ld, entry ) ) 
    {
        /* Se toma dn de la entrada corriente */
        if (( dn = ldap_get_dn( ld, entry )) != NULL )            
        {
            printf("\n   dn: %s\n", dn );
            
        }
            
        /* Este es el for que itera por los attributos por una entrada dada 
          Para la entrada corriente se recorren los atributos
        */

        for (   attribute = ldap_first_attribute( ld, entry, &ber );
                attribute != NULL; 
                attribute = ldap_next_attribute( ld, entry, ber ) ) 
        {   
            /* 
              Se toma valor por  cada atributo. 
              Get values and print.  Assumes all values are strings. 
            */ 
            if (( values = ldap_get_values( ld, entry, attribute)) != NULL ) 
            {
               /* Este es el for para los atributos multivaluados;
                */
                for ( i = 0; values[i] != NULL; i++ ) 
				{
                
                  	if ( 0 == strcmp ("utnSectorContent", attribute))
					{
						/*for (ifor = 0; ifor<strlen(values[i]) + 1; pBuffer[ifor]=values[i][ifor], ifor++);*/
						
						if ( ( contentList = ldap_get_values_len( ld, entry, attribute ) ) != NULL ) 
						{
							content = *contentList[0];
								
							for(i = 0; i < content.bv_len; i++)	
								pBuffer[i] = content.bv_val[i];													      
						}
					}                                       
                }
                                
                /* Se liberan las estructuras creadas por la ldap API */
                ldap_value_free( values );
            }
            ldap_memfree( attribute );
        }
       ldap_memfree( dn );
        ber_free(ber, 0);

   /* Para saber el numero de entrdas devueltas por la busqueda */
    entryCount = ldap_count_entries( ld, searchResult );
   
    printf("\n    Search completed successfully.\n    Entries  returned: %d\n",
             entryCount);

   
	/* Me da error no se porque??    ldap_msgfree( searchResult ); */
  }
    return(content.bv_len);
}

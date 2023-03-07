#include <stdio.h>
#include <stdlib.h>
#include <ldap.h>
#include "delFile.h"

void cargarLDAPELIMINARARCHIVO(LDAPELIMINARARCHIVO* info, char* nombreDA, char* nombreArchivo)
{
	strcpy(info->nombreDA, nombreDA);
	strcpy(info->nombreArchivo, nombreArchivo);
}

int ldapEliminarArchivo(LDAP *ld, LDAPELIMINARARCHIVO info)
{ 

    int         rc; 

    char deleteDN[500];
  
   /* 
      Aca se manda la operacion de borrado.
      El parametro principal es el DN; o sea lo que identificamente 
      univocamente una entrada de la base de datos.
      Perform the delete operation. 
    */
    sprintf(deleteDN, "utnFileName=%s,utnDAName=%s,ou=sadico,dc=utn,dc=edu",  info.nombreArchivo, info.nombreDA);
    rc = ldap_delete_ext_s( ld,         /* LDAP session handle */
                            deleteDN,   /* dn of the object to delete */
                            NULL,       /* server controls */
                            NULL );     /* client controls */

    if ( rc != LDAP_SUCCESS )
    {
        printf("\n\tldap_delete_ext_s: %s\n", ldap_err2string( rc ));
        return(1);
    }
    printf("\n\tEntry deleted successfully: %s\n", deleteDN );
    /* Finalmente se cierra la conexion */

    return( 0 );
}

int ldapEliminarDA(LDAP *ld, char* nombreDA)
{ 

    int         rc; 

    char deleteDN[500];

	
    sprintf(deleteDN, "utnDAName=%s,ou=sadico,dc=utn,dc=edu",  nombreDA);
    rc = ldap_delete_ext_s( ld,         /* LDAP session handle */
                            deleteDN,   /* dn of the object to delete */
                            NULL,       /* server controls */
                            NULL );     /* client controls */

    if ( rc != LDAP_SUCCESS )
    {
        printf("\n\tldap_delete_ext_s: %s\n", ldap_err2string( rc ));
        return(1);
    }

    return( 0 );
}


/**************************************************************************************/
int ldapBuscarDAsectoresLibresDEL( LDAP *ld, LDAPBUSCARARCHIVO info, pINT* listaSectores, int* iCantSectores)
{
	 int         rc, entryCount, cantSectores = 0;
    char        *searchBase, *utnDAName, *utnFileName;   
    char        *attribute, *dn, **values,  *sortAttribute = "utnDAName";       
    BerElement  *ber;
	ptrLdapResultados AuxResultados = NULL;

    LDAPMessage *searchResult, *entry;

    char filter[64];
    int i, b=0;
	
	*iCantSectores =0;
	int* listaSectoresLocal = NULL;

   
    /* Esta variable indica donde (en que rama) se va a efectuar la busqueda */
    searchBase       = "ou=sadico,dc=utn,dc=edu";    

    /* Y esto es para crear el filtro */
    utnDAName        = info.nombreDA;
    utnFileName      = info.nombreArchivo;



   sprintf(filter, "(&(utnDAName=%s))", utnDAName);

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
        return ( 1 );
    }   

    ldap_sort_entries( ld, &searchResult, sortAttribute, strcmp ); 


    
    /* Este es el for que itera por entradas */
    for (   entry   =   ldap_first_entry( ld, searchResult ); 
            entry   !=  NULL; 
            entry   =   ldap_next_entry( ld, entry ) ) 
    {
        /* Se toma dn de la entrada corriente */
        if (( dn = ldap_get_dn( ld, entry )) != NULL )            
        {
            /*printf("\n   dn: %s\n", dn );*/
            ldap_memfree( dn );
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


						if (0 == strcmp("utnFreeSectors",attribute))
						{
							listaSectoresLocal = (int*)realloc(listaSectoresLocal, sizeof(int)+ (*iCantSectores)*sizeof(int));
							listaSectoresLocal [*iCantSectores]=atoi(values[i]);
							(*iCantSectores)++;

					}

                }
                               
                /* Se liberan las estructuras creadas por la ldap API */
                ldap_value_free( values );
            }
            ldap_memfree( attribute );
        }
       
        ber_free(ber, 0);

   /* Para saber el numero de entrdas devueltas por la busqueda */
    entryCount = ldap_count_entries( ld, searchResult );
   
    /*printf("\n    Search completed successfully.\n    Entries  returned: %d\n",
             entryCount);*/

   
    }    
	listaSectores->lista = listaSectoresLocal;
    return(cantSectores);
}


void cargarSectoresLiberados(pINT* ipNuevosSectores, pINT ipListaSectoresLibres, ptrLdapResultados* ldapResultados, int* iCantSectores)
{
	int i=0, j=0, num;
	int cantNuevo= 0;
	ptrLdapResultados aux = *ldapResultados;
	char* p;
	int *vec=NULL;
	
	p = strtok (aux->info.listaSectores," ");
	while(p!= NULL)
	{
		num = miatoi(p);
		vec = (int*) realloc (vec,sizeof(int) + i*sizeof(int) );
		vec[i]=num;
		i++;
		p = strtok (NULL," ");
		cantNuevo++;
	}
	
	for(;j<*iCantSectores;j++,i++)
	{
		vec = (int*) realloc (vec,sizeof(int) + i*sizeof(int) );
		vec[i]=ipListaSectoresLibres.lista[j];
		cantNuevo++;
	}
	*iCantSectores = cantNuevo;
	ipNuevosSectores->lista = vec;
}

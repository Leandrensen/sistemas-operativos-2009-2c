#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ldap.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cadenas.h"
#include "srchFile.h"
#include "sendrecv.h"

int recibirDatos(int i, HEADERIRCIPC cabecera, unsigned char* bfr ,char* nombreDA, char* nombreArchivo)
{
	char* p;
	bfr = (unsigned char*)malloc(cabecera.uiPayloadLength +1);
	if (recibirAlgo(i, bfr, cabecera.uiPayloadLength) != SOCKET_OK_SENDRECV)
	{
		return -1;
	}

	bfr[cabecera.uiPayloadLength] = '\0';

	
	p = strtok((char*)bfr,"|");
	strcpy(nombreDA,p);
	p = strtok(NULL,"|");
	strcpy(nombreArchivo,p);
	return 1;
}

void cargarInfoArchivo (ptrLdapResultados* Resultados, pResultadoInformacion* ldapResultadoInformacion, int* iContadorDatosLdap)
{
	pResultadoInformacion ldapResultadoInformacionAuxiliar = NULL;
	char* cpSector;
	ptrLdapResultados datos = *Resultados;
	*ldapResultadoInformacion = NULL;
	
	
	*iContadorDatosLdap = 0;
	cpSector = strtok(datos->info.listaSectores," ");
	
	while (cpSector != NULL)
	{
		ldapResultadoInformacionAuxiliar = (pResultadoInformacion)malloc(sizeof(LDAPRESULTADOINFORMACION));
		ldapResultadoInformacionAuxiliar->iSector = miatoi(cpSector);
		ldapResultadoInformacionAuxiliar->sgte = *ldapResultadoInformacion;
		*ldapResultadoInformacion = ldapResultadoInformacionAuxiliar;
		cpSector = strtok(NULL," ");
		(*iContadorDatosLdap)++;
	}
	
	ldapResultadoInformacionAuxiliar = (pResultadoInformacion)malloc(sizeof(LDAPRESULTADOINFORMACION));
	ldapResultadoInformacionAuxiliar->iSector = miatoi(datos->info.tamanio);
	ldapResultadoInformacionAuxiliar->sgte = *ldapResultadoInformacion;
	*ldapResultadoInformacion = ldapResultadoInformacionAuxiliar;
	(*iContadorDatosLdap)++;
}

int enviarDatosArchivo(int iSocket ,pResultadoInformacion* ldapResultadoInformacion, int iContadorDatosLdap)
{
	unsigned char ucBuf[4*iContadorDatosLdap];
	int i = -1;
	pResultadoInformacion ldapResultadoInformacionAuxiliar = NULL, datos = *ldapResultadoInformacion;
	
	while(datos != NULL)
	{
		i++;
		NumToBin(ucBuf, datos->iSector, i*4);
		
		ldapResultadoInformacionAuxiliar = datos;
		datos = datos->sgte;
		free(ldapResultadoInformacionAuxiliar);
	}
	if(enviarAlgo(iSocket, ucBuf, 4*iContadorDatosLdap) < 0)
	{
		return -1;
	}
	*ldapResultadoInformacion = NULL;
	
	return 1;
}

void cargarLDAPBUSCARARCHIVO(LDAPBUSCARARCHIVO* info, char* nombreDA, char* nombreArchivo)
{
	
	strcpy(info->nombreDA,nombreDA);
	strcpy(info->nombreArchivo,nombreArchivo);
}

void listarArchivos(int iSocket, HEADERIRCIPC cabecera, LDAPINIT ldap)
{
	unsigned char* ucpBuffer;
	char* cpLista;
	LDAPBUSCARARCHIVO info;
	ptrLdapResultados Resultados = NULL, aux=NULL;
	int i, longitudCadena = 0;
	
	ucpBuffer = (unsigned char*) malloc (cabecera.uiPayloadLength + 1);
	recibirAlgo(iSocket,ucpBuffer,cabecera.uiPayloadLength);
	ucpBuffer[cabecera.uiPayloadLength] = '\0';
	
	if (strcmp(ucpBuffer,"/") == 0) /* Si es el directorio ROOT */
	{
		cargarLDAPBUSCARARCHIVO(&info, "*", "*");
		ldapBuscarDAs(ldap.ldapSesion, info, &Resultados,&longitudCadena);
	}
	else /* Es un DA específico */
	{
		for (i = 0; i < cabecera.uiPayloadLength ; i++)
			if (ucpBuffer[i]=='/') ucpBuffer[i] = '\0';
		cargarLDAPBUSCARARCHIVO(&info, &ucpBuffer[1], "*");
		ldapListarArchivos( ldap.ldapSesion, info, &Resultados,&longitudCadena);
	}

	if(longitudCadena==0)
		cabecera.uiPayloadLength = 0;
	else
	{
		cpLista = (unsigned char*) malloc (longitudCadena + 10);
		cpLista[0]='\0';
		for(;Resultados != NULL;aux = Resultados, Resultados = Resultados->sgte, free(aux))
		{
			
			concatenar(cpLista,Resultados->info.nombreDA);
			if (strcmp(ucpBuffer,"/") == 0)
				concatenar(cpLista,"/");
			concatenar(cpLista,"\n");
			concatenar(cpLista,"|");
			concatenar(cpLista,Resultados->info.tamanio);
			concatenar(cpLista,"\n");
			concatenar(cpLista,"|");
		}
		cabecera.uiPayloadLength = strlen(cpLista);
			
	}
	cabecera.ucPayloadDescriptor = 0x27;
	enviarH(iSocket,cabecera);
	enviarAlgo(iSocket,cpLista,cabecera.uiPayloadLength);
	if(longitudCadena!=0)
		free(cpLista);
	free(ucpBuffer);

}

int ldapBuscarDAs( LDAP *ld, LDAPBUSCARARCHIVO info, ptrLdapResultados* Resultados, int* longitudCadena)
{
	 int         rc, entryCount;
    char        *searchBase, *utnDAName, *utnFileName;   
    char        *attribute, *dn, **values,  *sortAttribute = "utnDAName";       
    BerElement  *ber;
	ptrLdapResultados AuxResultados = NULL;

    LDAPMessage *searchResult, *entry;

    char filter[64];
    int i;
    char* listaSectores;

	*Resultados = NULL;
    
    /* Esta variable indica donde (en que rama) se va a efectuar la busqueda */
    searchBase       = "ou=sadico,dc=utn,dc=edu";    

    /* Y esto es para crear el filtro */
    utnDAName        = info.nombreDA;
    utnFileName      = info.nombreArchivo;

    /*
      Aca se establece la conexion con el servidor.
      Como en una conexion tcp, los unicos datos que se necesitan son el 
      host y el puerto de conexion.
      Initialize the LDAP session 
    */
    

   /* Aca armo el filtro diciendole que quiero el archivo utnFileName 
      que esta en el DA utnDAName; o sea (notacion polaca)
      ( AND( (utnDAName=algunDA)(utnFileName=algunFile) ) )
   */

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
     
    /* 
       Aca les pongo un ejemplo de ordenamiento de los  resultados. 
       La ejecucion del ordenamiento se hace del lado del cliente.
       Hay otra funcion que hace lo mismo del lado del servidor.
       
       Fijense mas arriba la asignacion de sortAttribute.
       O sea que se van a ordenar la lista de resultados por 
       los valores del attributo del device name.
    
      client-sort 
    */ 
    ldap_sort_entries( ld, &searchResult, sortAttribute, strcmp ); 

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

					if ((0 == strcmp("utnDAName",attribute)))
					{
						if (estaElDA(values[i] , Resultados) == 0)
						{
							AuxResultados =(ptrLdapResultados) malloc(sizeof(NODOLDAPRESULTADOS));
							strcpy(AuxResultados->info.nombreDA, values[i] );
							printf("DA: %s\n",AuxResultados->info.nombreDA);
							strcpy(AuxResultados->info.tamanio, "0");
							AuxResultados->sgte = *Resultados;
							*Resultados = AuxResultados;
							*longitudCadena = *longitudCadena + strlen(AuxResultados->info.nombreDA) + strlen(AuxResultados->info.tamanio) + 10;
						}
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
   
    return(entryCount);
}

int ldapBuscarArchivo( LDAP *ld, LDAPBUSCARARCHIVO info, ptrLdapResultados* Resultados)
{   
     int         rc, entryCount, resultado=1;
    char        *searchBase, *utnDAName, *utnFileName;   
    char        *attribute, *dn, **values,  *sortAttribute = "utnDAName";       
    BerElement  *ber;
	ptrLdapResultados AuxResultados = NULL;

    LDAPMessage *searchResult, *entry;

    char filter[1000];
    int i;
    char* listaSectores;

	*Resultados = NULL;
    
    /* Esta variable indica donde (en que rama) se va a efectuar la busqueda */
    searchBase       = "ou=sadico,dc=utn,dc=edu";    

    /* Y esto es para crear el filtro */
    utnDAName        = info.nombreDA;
    utnFileName      = info.nombreArchivo;

    /*
      Aca se establece la conexion con el servidor.
      Como en una conexion tcp, los unicos datos que se necesitan son el 
      host y el puerto de conexion.
      Initialize the LDAP session 
    */
    

   /* Aca armo el filtro diciendole que quiero el archivo utnFileName 
      que esta en el DA utnDAName; o sea (notacion polaca)
      ( AND( (utnDAName=algunDA)(utnFileName=algunFile) ) )
   */

   sprintf(filter, "(&(utnDAName=%s)(utnFileName=%s))", utnDAName, utnFileName);

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
     
    /* 
       Aca les pongo un ejemplo de ordenamiento de los  resultados. 
       La ejecucion del ordenamiento se hace del lado del cliente.
       Hay otra funcion que hace lo mismo del lado del servidor.
       
       Fijense mas arriba la asignacion de sortAttribute.
       O sea que se van a ordenar la lista de resultados por 
       los valores del attributo del device name.
    
      client-sort 
    */ 
    ldap_sort_entries( ld, &searchResult, sortAttribute, strcmp ); 

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
    entryCount = ldap_count_entries( ld, searchResult );
    /* Este es el for que itera por entradas */
    for (   entry   =   ldap_first_entry( ld, searchResult ); 
            entry   !=  NULL; 
            entry   =   ldap_next_entry( ld, entry ) ) 
    {
        /* Se toma dn de la entrada corriente */
        if (( dn = ldap_get_dn( ld, entry )) != NULL )            
        {
            printf("\n   dn: %s\n", dn );
            ldap_memfree( dn );
        }
            
        /* Este es el for que itera por los attributos por una entrada dada 
          Para la entrada corriente se recorren los atributos
        */
		AuxResultados =(ptrLdapResultados) malloc(sizeof(NODOLDAPRESULTADOS));
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

						if ((0 == strcmp("utnDAName",attribute)))
						{
							strcpy(AuxResultados->info.nombreDA, values[i] );
						}
					
						if ((0 == strcmp("utnFileName",attribute)))
						{
							strcpy(AuxResultados->info.nombreArchivo, values[i] );
						}

						if ((0 == strcmp("utnFileOwner",attribute)))
						{
							strcpy(AuxResultados->info.autor, values[i] );
						}

						if ((0 == strcmp("utnSize",attribute)))
						{
							strcpy(AuxResultados->info.tamanio, values[i] );
						} 

						if ((0 == strcmp("utnSectorList",attribute)))
						{
							AuxResultados->info.listaSectores = (char*) malloc(strlen(values[i]) + 1);
							strcpy(AuxResultados->info.listaSectores, values[i] );
						} 
						
						if ((0 == strcmp("utnFilePermissions",attribute)))
						{
							strcpy(AuxResultados->info.permisos, values[i] );
						}

					}
			
				*Resultados = AuxResultados;
                                
                /* Se liberan las estructuras creadas por la ldap API */
                ldap_value_free( values );
            }
            ldap_memfree( attribute );
        }
       
        ber_free(ber, 0);

   /* Para saber el numero de entrdas devueltas por la busqueda */
		entryCount = ldap_count_entries( ld, searchResult );
   
		printf("\n    Search completed successfully.\n    Entries  returned: %d\n",
             entryCount);

 
	}    
   
    return(entryCount);
}

int estaElDA(char* nombreDA, ptrLdapResultados* Resultados)
{
	ptrLdapResultados Aux=*Resultados;
	
	for(;Aux!=NULL;Aux=Aux->sgte)
	{
		if(strcmp(Aux->info.nombreDA,nombreDA)==0)
			return 1;
	}
	return 0;
}

int ldapBuscarDAsectoresLibres( LDAP *ld, LDAPBUSCARARCHIVO info, unsigned char* buf, pINT* listaSectores, int* iCantSectores)
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

    /*
      Aca se establece la conexion con el servidor.
      Como en una conexion tcp, los unicos datos que se necesitan son el 
      host y el puerto de conexion.
      Initialize the LDAP session 
    */
    

   /* Aca armo el filtro diciendole que quiero el archivo utnFileName 
      que esta en el DA utnDAName; o sea (notacion polaca)
      ( AND( (utnDAName=algunDA)(utnFileName=algunFile) ) )
   */

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
     
    /* 
       Aca les pongo un ejemplo de ordenamiento de los  resultados. 
       La ejecucion del ordenamiento se hace del lado del cliente.
       Hay otra funcion que hace lo mismo del lado del servidor.
       
       Fijense mas arriba la asignacion de sortAttribute.
       O sea que se van a ordenar la lista de resultados por 
       los valores del attributo del device name.
    
      client-sort 
    */ 
    ldap_sort_entries( ld, &searchResult, sortAttribute, strcmp ); 

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

					if (cantSectores == 2)
					{
						if (0 == strcmp("utnFreeSectors",attribute))
						{
							listaSectoresLocal = (int*)realloc(listaSectoresLocal, sizeof(int)+ (*iCantSectores)*sizeof(int));
							listaSectoresLocal [*iCantSectores]=atoi(values[i]);
							(*iCantSectores)++;
						}
					}
					
					if ((0 == strcmp("utnFreeSectors",attribute)) && (cantSectores==1))
					{
						cantSectores++;
						NumToBin(buf,atoi(values[i]), 4);
					}
					
					if ((0 == strcmp("utnFreeSectors",attribute)) && (cantSectores==0))
					{
						cantSectores++;
						NumToBin(buf,atoi(values[i]), 0);
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

int exsteArchivo( LDAP *ld, LDAPBUSCARARCHIVO info)
{   
    int         rc, entryCount;
    char        *searchBase, *utnDAName, *utnFileName;   
    char        *attribute, *dn, **values,  *sortAttribute = "utnDAName";       
    BerElement  *ber;
	ptrLdapResultados AuxResultados = NULL;

    LDAPMessage *searchResult, *entry;

    char filter[64];
    int i;
    char* listaSectores;


    /* Esta variable indica donde (en que rama) se va a efectuar la busqueda */
    searchBase       = "ou=sadico,dc=utn,dc=edu";    

    /* Y esto es para crear el filtro */
    utnDAName        = info.nombreDA;
    utnFileName      = info.nombreArchivo;

    /*
      Aca se establece la conexion con el servidor.
      Como en una conexion tcp, los unicos datos que se necesitan son el 
      host y el puerto de conexion.
      Initialize the LDAP session 
    */
    

   /* Aca armo el filtro diciendole que quiero el archivo utnFileName 
      que esta en el DA utnDAName; o sea (notacion polaca)
      ( AND( (utnDAName=algunDA)(utnFileName=algunFile) ) )
   */

   sprintf(filter, "(&(utnDAName=%s)(utnFileName=%s))", utnDAName, utnFileName);

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
     
    /* 
       Aca les pongo un ejemplo de ordenamiento de los  resultados. 
       La ejecucion del ordenamiento se hace del lado del cliente.
       Hay otra funcion que hace lo mismo del lado del servidor.
       
       Fijense mas arriba la asignacion de sortAttribute.
       O sea que se van a ordenar la lista de resultados por 
       los valores del attributo del device name.
    
      client-sort 
    */ 
   /* ldap_sort_entries( ld, &searchResult, sortAttribute, strcmp ); */

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
	  entryCount = ldap_count_entries( ld, searchResult );
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
					
                }

		
                                
                /* Se liberan las estructuras creadas por la ldap API */
                ldap_value_free( values );
            }
            ldap_memfree( attribute );
        }
       
        ber_free(ber, 0);

   /* Para saber el numero de entrdas devueltas por la busqueda */
  
   
    /*printf("\n    Search completed successfully.\n    Entries  returned: %d\n",
             entryCount);*/

   
    }    
    return(entryCount);
}

int ldapListarArchivos( LDAP *ld, LDAPBUSCARARCHIVO info, ptrLdapResultados* Resultados, int* longitudCadena)
{   
    int         rc, entryCount, resultado=1;
    char        *searchBase, *utnDAName, *utnFileName;   
    char        *attribute, *dn, **values,  *sortAttribute = "utnDAName";       
    BerElement  *ber;
	ptrLdapResultados AuxResultados = NULL;

    LDAPMessage *searchResult, *entry;

    char filter[64];
    int i;
    char* listaSectores;

	*Resultados = NULL;
    
    /* Esta variable indica donde (en que rama) se va a efectuar la busqueda */
    searchBase       = "ou=sadico,dc=utn,dc=edu";    

    /* Y esto es para crear el filtro */
    utnDAName        = info.nombreDA;
    utnFileName      = info.nombreArchivo;

    /*
      Aca se establece la conexion con el servidor.
      Como en una conexion tcp, los unicos datos que se necesitan son el 
      host y el puerto de conexion.
      Initialize the LDAP session 
    */
    

   /* Aca armo el filtro diciendole que quiero el archivo utnFileName 
      que esta en el DA utnDAName; o sea (notacion polaca)
      ( AND( (utnDAName=algunDA)(utnFileName=algunFile) ) )
   */

   sprintf(filter, "(&(utnDAName=%s)(utnFileName=%s))", utnDAName, utnFileName);

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
     
    /* 
       Aca les pongo un ejemplo de ordenamiento de los  resultados. 
       La ejecucion del ordenamiento se hace del lado del cliente.
       Hay otra funcion que hace lo mismo del lado del servidor.
       
       Fijense mas arriba la asignacion de sortAttribute.
       O sea que se van a ordenar la lista de resultados por 
       los valores del attributo del device name.
    
      client-sort 
    */ 
    ldap_sort_entries( ld, &searchResult, sortAttribute, strcmp ); 

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
    entryCount = ldap_count_entries( ld, searchResult );
    /* Este es el for que itera por entradas */
    for (   entry   =   ldap_first_entry( ld, searchResult ); 
            entry   !=  NULL; 
            entry   =   ldap_next_entry( ld, entry ) ) 
    {
        /* Se toma dn de la entrada corriente */
        if (( dn = ldap_get_dn( ld, entry )) != NULL )            
        {
            printf("\n   dn: %s\n", dn );
            ldap_memfree( dn );
        }
            
        /* Este es el for que itera por los attributos por una entrada dada 
          Para la entrada corriente se recorren los atributos
        */
		AuxResultados =(ptrLdapResultados) malloc(sizeof(NODOLDAPRESULTADOS));
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

						if ((0 == strcmp("utnFileName",attribute)))
						{
							strcpy(AuxResultados->info.nombreDA, values[i] );
						}
						
						if ((0 == strcmp("utnSize",attribute)))
						{
							strcpy(AuxResultados->info.tamanio, values[i] );
						}
					}

                                
                /* Se liberan las estructuras creadas por la ldap API */
                ldap_value_free( values );
            }
            ldap_memfree( attribute );
        }
		
		*longitudCadena = *longitudCadena + strlen(AuxResultados->info.nombreArchivo) + strlen(AuxResultados->info.tamanio) + 10;
				/* Lo cargo como PILA */
		AuxResultados->sgte = *Resultados;
		*Resultados = AuxResultados;
       
        ber_free(ber, 0);

	}    
   
    return(entryCount);
}

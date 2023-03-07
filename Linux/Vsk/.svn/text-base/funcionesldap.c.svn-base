#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ldap.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include "cadenas.h"
#include "funcionesldap.h"


/************************************************************************************************************/
void ldapCargarSesion(pLDAPINIT* ldap_sesion, char* ldapHost, int ldapPort, char* ldapPass, int iTamanioCache)
{
	/* por configuracion */
	pLDAPINIT ldapPedido;
	ldapPedido = (pLDAPINIT) malloc (sizeof(LDAPINIT));
	ldapPedido->ldapHost = (char*)malloc(strlen(ldapHost) + 1);
	strcpy(ldapPedido->ldapHost,ldapHost);
	
	/* por configuracion */
	ldapPedido->ldapPort = ldapPort;

	ldapPedido->ldapDN = (char*)malloc(sizeof("cn=Directory Manager") + 1);
	strcpy(ldapPedido->ldapDN,"cn=Directory Manager");
	
	/* por configuracion */
	ldapPedido->ldapPass= (char*)malloc(strlen(ldapPass) + 1);
	strcpy(ldapPedido->ldapPass,ldapPass);

	ldapPedido->iCantidadMaxima= iTamanioCache;
	ldapPedido->iCantidadActual = 0;
	*ldap_sesion = ldapPedido;
}

/************************************************************************************************************/
int ldapIniciarSesion(pLDAPINIT* infofuera)
{
	pLDAPINIT info = *infofuera;
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

/************************************************************************************************************/
void ldapFinSesion(pLDAPINIT* infofuera)
{
	pLDAPINIT info = *infofuera;
	ldap_unbind_s(info->ldapSesion);
	printf("LDAP: sesion finalizada\n");	
}

/************************************************************************************************************/
void cargarSector(LDAPSECTOR *info, unsigned char* utnDAName, char* utnSectorID, char* utnSectorContent, int cacheBucket)
{
	char numero[50];
	int i;
	copiarU(info->utnDAName,utnDAName);
	copiar((char*)info->utnSectorID,utnSectorID);
	for (i=0; i<512; i++)
		info->utnSectorContent[i]=utnSectorContent[i];
	miitoa(numero,cacheBucket);
	copiar(info->cacheBucket,numero);
}

/************************************************************************************************************/
int ldapCargarCache(LDAP *ld, LDAPSECTOR info)
{

/* 
  Declaracion de variables para dar de alta un registro con estos attributos:
  utnDAName $ utnSectorID $ utnSectorLastAccessTS $ utnSectorContent
  Corresponde al add de un device; o sea que esta en la rama 
  utnDAName=xyxyxyxy,ou=sadico,dc=utn,dc=edu.
*/

      int         rc; 
    char        /**containerName, *utnDAName, *utnSectorID, */*dn = NULL;     

    char        *vclass[3], *vutnDAName[2], *vutnSectorID[2], *vutnSectorLastAccessTS[2], 
                *vutnSectorContent[2];
    LDAPMod     modClass, modutnDAName, modutnSectorID, modutnSectorLastAccessTS, modutnSectorContent;
    LDAPMod     *mods[6];
    
   /* int fd, fd2;*/
 /*   struct stat statBuf;*/
    
    time_t ahora;
    char strTime[32];
    
    /*
     * No hay mas comentario que el que sigue:
     * To add an entry to the directory:
     *   1. Create the array of LDAPMod structures representing the
     *      attributes of the new entry 
     *   2. Specify the DN of the entry to create <--- Recuerden que tiene que ser unico
     *   3. Call ldap_add_ext_s to add the entry to the directory
     */
     
    /* Se agregan estos objetos */
    modClass.mod_op       =   LDAP_MOD_ADD;      /* El tipo de operacion que se va a hacer*/
    modClass.mod_type     =   "objectclass";     /* Que objecto es el que se va a crear*/
    vclass[0]             =   "top";             /* Idem*/
    vclass[1]             =   "utnCachedSector"; /* Idem. */
    vclass[2]             =   NULL;              /* Fin de declaraciones de objetos*/
    modClass.mod_values   =   vclass;

    /* En los objetos se agregan los atributos que siguen */
    modutnDAName.mod_op      =   LDAP_MOD_ADD;   /* Se va agregar*/
    modutnDAName.mod_type    =   "utnDAName";     /* Este atributo */
    vutnDAName[0]           =   (char*)info.utnDAName;          /* Con este valor*/
    vutnDAName[1]           =   NULL;           /* Fin de agregar valores para este atributo*/
    modutnDAName.mod_values =   vutnDAName;      /* Se hace apuntar los datos a la estructura*/

    modutnSectorID.mod_op     =   LDAP_MOD_ADD;   /* Se va agregar*/
    modutnSectorID.mod_type   =   "utnSectorID";  /* Este atributo */
    vutnSectorID[0]           =   (char*)info.utnSectorID;    /* Con este valor*/
    vutnSectorID[1]           =   NULL;           /* Fin de agregar valores para este atributo*/
    modutnSectorID.mod_values =   vutnSectorID;   /* Se hace apuntar los datos a la estructura*/

    ahora = time(NULL);
  	strftime ( strTime , sizeof ( strTime ) , "%Y%m%d%H%M%SZ" , localtime ( &ahora ) ) ;
	  strTime [31] = '\0' ;

    modutnSectorLastAccessTS.mod_op     =   LDAP_MOD_ADD;   /* Idem*/
    modutnSectorLastAccessTS.mod_type   =   "utnSectorLastAccessTS";  
    vutnSectorLastAccessTS[0]           =   strTime;    
    vutnSectorLastAccessTS[1]           =   NULL;          
    modutnSectorLastAccessTS.mod_values =   vutnSectorLastAccessTS; 

    modutnSectorContent.mod_op     =   LDAP_MOD_ADD;   /* Idem*/
    modutnSectorContent.mod_type   =   "utnSectorContent";  
    vutnSectorContent[0]           =  info.utnSectorContent;    
    vutnSectorContent[1]           =   NULL;          
    modutnSectorContent.mod_values =   vutnSectorContent; 

    /* mods es la estructura que se va a pasar al comando ldap.
       aca se hacen apuntar todas las estructuras que se vienen de
       rellenar para definir los agregados que se van a hacer a la entrada nueva, 
       a la estructura mnods.
     */
     
     
    mods[0] = &modClass; 
    mods[1] = &modutnDAName;
    mods[2] = &modutnSectorID;
    mods[3] = &modutnSectorLastAccessTS;
    mods[4] = &modutnSectorContent;
    mods[5] = NULL;                    /* Se marca fin de los agregados  */

    /*
       Aca se construlle el dn que tiene que ser unico. Si se trata de agregar un dn 
       que ya existe simplemente devuelve un error. De la misma manera que si se trata 
       de crear un archivo en un lugar donde ya existe un archivo que se llama igual.
       Construct the dn 
         "utnFileName=miArch.pdf,utnDAName=xyxyxyxy,ou=sadico,dc=utn,dc=edu" with the container 
       name from the command line
     */
    dn=(char*)malloc(strlen(info.cacheBucket) + strlen("utnCacheBucket=,") + strlen("ou=cachedsectors,dc=utn,dc=edu") + 1 );
    sprintf( dn, "utnCacheBucket=%s,ou=cachedsectors,dc=utn,dc=edu", info.cacheBucket);
    printf("DS: [%s]\n", dn);

    /*
       Finalmente se llama a la funcion para agregar la nueva entrada 
       a la base de datos. Los parametros principales son el dn, y todos 
       los datos que componen la entrada nueva, que se encuentran en mods.
     * Now add the object
     */
    rc = ldap_add_ext_s( ld,     /* LDAP session handle */ 
                         dn,     /* dn of the object to be created */
                         mods,   /* array of add options and values */
                         NULL,   /* server control */
                         NULL ); /* client control */

   
    if (rc == LDAP_SUCCESS)    
   	printf("\n\tEntry successfully added: %s\n", dn );
    else printf("\n\tEntry non successfully added: %s\n", dn );

    free( dn );
    return( 1 );
}

/************************************************************************************************************/
int ldapModificarContenidoCache (LDAP *ld, char* modifyDN, char* cpBUFutnSectorContent)
{
    int         rc; 
    char        *vutnSectorContent[2];
  
  /* Los attributos que elegimos modificar para el ejemplo */ 
   /* char        *vutnSectorList[2];   */ 

	time_t ahora;
    char strTime[32];

    LDAPMod     *modify[4];
    LDAPMod     modutnSectorContent;
/*    struct timeval timeOut = {10,0};    10 second connection timeout */

    
      /*
      Una modificacion implica localizar la entrada para modificar, 
      y aplicar la  modificacion. Por eso se provee el dn.
      El dn (distinguish name) es el identificador univoco para una entrada, 
      es el que permite ubicarla, por eso se provee.    
     */
 
    /* 
     * To modify the attributes of an entry
     *    1  Specify the modify actions    
     *    2. Specify the attribute name to be modified 
     *    3. Specify the value of the attribute
     *    4. Add to an array of LDAPMod structures
     *    5. Call ldap_modify_ext_s
     */


    /* 
     * LDAP_MOD_REPLACE succeeds whether the value already exists or not.
     * Here I try to change the url title
     */

   ahora = time(NULL);
  	strftime ( strTime , sizeof ( strTime ) , "%Y%m%d%H%M%SZ" , localtime ( &ahora ) ) ;
	  strTime [31] = '\0' ;

    modutnSectorContent.mod_op     = LDAP_MOD_REPLACE; /* Se remplaza algo ya existente (o no)*/ /* Idem*/
    modutnSectorContent.mod_type   =   "utnSectorContent";  
    vutnSectorContent[0]           =  cpBUFutnSectorContent;    
    vutnSectorContent[1]           =   NULL;          
    modutnSectorContent.mod_values =   vutnSectorContent; 

    modify[0] = &modutnSectorContent;
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
        ldap_unbind_s( ld );
        return(1);
    }
    printf("\n\t%s modified successfully.\n", modifyDN );

    return( 0 );
}

/**************************************************************************************************************/
/* modifica la fecha de acceso al sector */
int ldapModificarCache(LDAP *ld, char* modifyDN)
{ 

    int         rc; 
    char        *vutnSectorLastAccessTS[2];
  
  /* Los attributos que elegimos modificar para el ejemplo */ 
   /* char        *vutnSectorList[2];   */ 

	time_t ahora;
    char strTime[32];

    LDAPMod     *modify[4];
    LDAPMod     modutnSectorLastAccessTS;
/*    struct timeval timeOut = {10,0};    10 second connection timeout */

    
      /*
      Una modificacion implica localizar la entrada para modificar, 
      y aplicar la  modificacion. Por eso se provee el dn.
      El dn (distinguish name) es el identificador univoco para una entrada, 
      es el que permite ubicarla, por eso se provee.    
     */
 
    /* 
     * To modify the attributes of an entry
     *    1  Specify the modify actions    
     *    2. Specify the attribute name to be modified 
     *    3. Specify the value of the attribute
     *    4. Add to an array of LDAPMod structures
     *    5. Call ldap_modify_ext_s
     */


    /* 
     * LDAP_MOD_REPLACE succeeds whether the value already exists or not.
     * Here I try to change the url title
     */

   ahora = time(NULL);
  	strftime ( strTime , sizeof ( strTime ) , "%Y%m%d%H%M%SZ" , localtime ( &ahora ) ) ;
	  strTime [31] = '\0' ;

    modutnSectorLastAccessTS.mod_op     = LDAP_MOD_REPLACE; /* Se remplaza algo ya existente (o no)*/
    modutnSectorLastAccessTS.mod_type   = "utnSectorLastAccessTS";
    vutnSectorLastAccessTS[0]           = strTime;
    vutnSectorLastAccessTS[1]           = NULL;
    modutnSectorLastAccessTS.mod_values = vutnSectorLastAccessTS;

    modify[0] = &modutnSectorLastAccessTS;
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
        ldap_unbind_s( ld );
        return(1);
    }
    printf("\n\t%s modified successfully.\n", modifyDN );

    return( 0 );
}

/************************************************************************************************************/
int ldapBuscarCache(LDAP *ld, LDAPSECTOR info, char* pBuffer)
{   
    int         version,rc, entryCount, ifor, bandera = 0;
    char        searchBase[]="ou=cachedsectors,dc=utn,dc=edu";   
    char        *attribute, *dn, **values;       
    BerElement  *ber;
    LDAPMessage *searchResult, *entry;

    char filter[64];
    int i;    
 
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
                
			if (0 == strcmp ("utnSectorLastAccessTS", attribute))
			{
				if (0 == strcmp (values[i],"0"))
				bandera = 1;
				else
				bandera = 2;
			}


			if ( (0 == strcmp ("utnSectorContent", attribute)) && bandera == 2)
			{
				for (ifor = 0; ifor<strlen(values[i]); pBuffer[ifor]=values[i][ifor], ifor++);
				ldapModificarCache(ld, dn);
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
	if (bandera == 1 || bandera == 0) return 0;
	if (bandera == 2) return 1;
}
/* #################################################################################################################### */
/* #################################################################################################################### */
int ldapBuscarMasViejoCache(LDAP *ld, char * dnMasViejoBuscado)
{   
    int         version, rc, entryCount;
    char        searchBase[]="ou=cachedsectors,dc=utn,dc=edu";   
    char        *attribute, *dn, **values;       
    BerElement  *ber;
    LDAPMessage *searchResult, *entry;
	int iBandera = 0;
	char tiempoMinimo[32];

    char filter[64];
    int i;    
 
    /* Esta variable indica donde (en que rama) se va a efectuar la busqueda */

    /* Y esto es para crear el filtro */
   /* utnDAName        = info.utnDAName;
    utnSectorID      = info.utnSectorID;*/

    /* Set LDAP version to 3 and set connection timeout. */
    version = LDAP_VERSION3;
 
   /* Aca armo el filtro */

   /*sprintf(filter, "(&(utnDAName=%s)(utnSectorID=%s))", utnDAName, utnSectorID);*/
	sprintf(filter, "(&(utnDAName=*)(utnSectorID=*))");

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
                for ( i = 0; values[i] != NULL; i++ ) {
 
			/* BUSCO MINIMO */
			
			if ((0 == strcmp ("utnSectorLastAccessTS", attribute)) && (strcmp(values[i], "0") != 0))
			{
				if ((iBandera == 0) || (strcmp(values[i], tiempoMinimo)<0))
				{
					iBandera = 1;
					copiar(tiempoMinimo,values[i]);
					copiar (dnMasViejoBuscado, dn);
					ldap_memfree( dn );
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
   

   
/* Me da error no se porque??    ldap_msgfree( searchResult ); */
  }    
   entryCount = ldap_count_entries( ld, searchResult );
   
    printf("\n    Search completed successfully.\n    Entries  returned: %d\n",
             entryCount);

    return( 0 );
}

/**************************************************************************************************/
/*PARA SYSCALL SYS_WRITE*/
/* ============================================================================================= */
int ldapBuscarCacheParaInvalidar(LDAP *ld, LDAPSECTOR info)
{
    int         version,rc, entryCount;
    char        searchBase[]="ou=cachedsectors,dc=utn,dc=edu";   
    char        *attribute, *dn, **values;       
    BerElement  *ber;
    LDAPMessage *searchResult, *entry;

    char filter[64];
    int i, iBandera =0;    
 
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
                for ( i = 0; values[i] != NULL; i++ ) {
                
                  	if ( 0 == strcmp ("utnSectorLastAccessTS", attribute))
			{
				if (0 != strcmp (values[i],"0")) iBandera = 1;
					
			}
                                       
                }
                                
                /* Se liberan las estructuras creadas por la ldap API */
                ldap_value_free( values );
            }
            ldap_memfree( attribute );
        }
	if (iBandera == 1) ldapInvalidarEntradaCache(ld, dn);
       ldap_memfree( dn );
        ber_free(ber, 0);

   /* Para saber el numero de entrdas devueltas por la busqueda */
    entryCount = ldap_count_entries( ld, searchResult );
   
    printf("\n    Search completed successfully.\n    Entries  returned: %d\n",
             entryCount);

   
/* Me da error no se porque??    ldap_msgfree( searchResult ); */
  }    

    return( entryCount );
}



int ldapInvalidarEntradaCache(LDAP *ld, char* modifyDN)
{ 

    int         rc; 
    char        *vutnSectorLastAccessTS[2];
  
  /* Los attributos que elegimos modificar para el ejemplo */ 
   /* char        *vutnSectorList[2];   */ 

    char strTime[5];

    LDAPMod     *modify[4];
    LDAPMod     modutnSectorLastAccessTS;
/*    struct timeval timeOut = {10,0};    10 second connection timeout */

    
      /*
      Una modificacion implica localizar la entrada para modificar, 
      y aplicar la  modificacion. Por eso se provee el dn.
      El dn (distinguish name) es el identificador univoco para una entrada, 
      es el que permite ubicarla, por eso se provee.    
     */
 
    /* 
     * To modify the attributes of an entry
     *    1  Specify the modify actions    
     *    2. Specify the attribute name to be modified 
     *    3. Specify the value of the attribute
     *    4. Add to an array of LDAPMod structures
     *    5. Call ldap_modify_ext_s
     */


    /* 
     * LDAP_MOD_REPLACE succeeds whether the value already exists or not.
     * Here I try to change the url title
     */

  	  strTime[0] = '0';
	  strTime [1] = '\0' ;

    modutnSectorLastAccessTS.mod_op     = LDAP_MOD_REPLACE; /* Se remplaza algo ya existente (o no)*/
    modutnSectorLastAccessTS.mod_type   = "utnSectorLastAccessTS";
    vutnSectorLastAccessTS[0]           = strTime;
    vutnSectorLastAccessTS[1]           = NULL;
    modutnSectorLastAccessTS.mod_values = vutnSectorLastAccessTS;

    modify[0] = &modutnSectorLastAccessTS;
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
        ldap_unbind_s( ld );
        return(1);
    }
    printf("\n\t%s modified successfully.\n", modifyDN );

    return( 0 );
}



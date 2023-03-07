#include <stdio.h>
#include <stdlib.h>
#include <ldap.h>
#include <string.h>
#include "initBind.h"
#include "funcionesLdap.h"

void cargarLDAPMODIFICARSECTOR(LDAPMODIFICARSECTOR *info, char* nombreDA, char* idSector,char* contenido)
{
	strcpy(info->nombreDA,nombreDA);
	strcpy(info->idSector,idSector);
	strcpy(info->contenido,contenido);
	return;
}

int ldapModificarSector(LDAP *ld, LDAPMODIFICARSECTOR info)
{ 

    int         rc; 
    char        modifyDN[500];
  
  /* Los attributos que elegimos modificar para el ejemplo */ 
    char        *vutnSectorContent[4];    
       
    LDAPMod     *modify[4];
    LDAPMod     modutnSectorContent;

    /*
      Una modificacion implica localizar la entrada para modificar, 
      y aplicar la  modificacion. Por eso se provee el dn.
      El dn (distinguish name) es el identificador univoco para una entrada, 
      es el que permite ubicarla, por eso se provee.    
     */
    sprintf(modifyDN,"utnSectorID=%s+utnDAName=%s,ou=disks,dc=utn,dc=edu",info.idSector,info.nombreDA); 

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
    modutnSectorContent.mod_op = LDAP_MOD_REPLACE;        /* Aca quiero remplazar. */
    modutnSectorContent.mod_type = "utnSectorContent";  
    vutnSectorContent[0] = info.contenido;
    vutnSectorContent[1] = NULL;
    modutnSectorContent.mod_values = vutnSectorContent;

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

    if (rc != LDAP_SUCCESS)
    {
        printf("ldap_modify_ext_s: %s\n", ldap_err2string( rc ));
        return 0;
    }
    printf("\n\t%s modified successfully.\n", modifyDN);
    return 1;
}

void cargarLDAPAGREGARSECTOR(LDAPAGREGARSECTOR *info, char* nombreDA, char* idSector, char* contenido)
{
	strcpy(info->nombreDA,nombreDA);
	strcpy(info->idSector,idSector);
	strcpy(info->contenido,contenido);
	info->contenido[512]='\0';
}

int ldapAgregarSector(LDAP* ld, LDAPAGREGARSECTOR info)
{
    int         rc; 
    char        *utnDAName, *utnSectorID, *dn = NULL;     

    char        *vclass[3], *vutnDAName[2], *vutnSectorID[2], *vutnSectorContent[2];
    LDAPMod     modClass, modutnDAName, modutnSectorID, modutnSectorContent;
    LDAPMod     *mods[6];
  
    utnDAName   =   info.nombreDA;  
    utnSectorID =   info.idSector;  
   
    /*
     * No hay mas comentario que el que sigue:
     * To add an entry to the directory:
     *   1. Create the array of LDAPMod structures representing the
     *      attributes of the new entry 
     *   2. Specify the DN of the entry to create <--- Recuerden que tiene que ser unico
     *   3. Call ldap_add_ext_s to add the entry to the directory
     */
     
    /* Se agregan estos objetos */
    modClass.mod_op       =   LDAP_MOD_ADD;      /* El tipo de operacion que se va a hacer */
    modClass.mod_type     =   "objectclass";     /* Que objecto es el que se va a crear */
    vclass[0]             =   "top";             /* Idem */
    vclass[1]             =   "utnDiskSector"; /* Idem */ 
    vclass[2]             =   NULL;              /* Fin de declaraciones de objetos */
    modClass.mod_values   =   vclass;

    /* En los objetos se agregan los atributos que siguen */
    modutnDAName.mod_op      =   LDAP_MOD_ADD;   /* Se va agregar */
    modutnDAName.mod_type    =   "utnDAName";     /* Este atributo */
    vutnDAName[0]           =   utnDAName;          /* Con este valor */
    vutnDAName[1]           =   NULL;           /* Fin de agregar valores para este atributo */
    modutnDAName.mod_values =   vutnDAName;      /* Se hace apuntar los datos a la estructura */

    modutnSectorID.mod_op     =   LDAP_MOD_ADD;   /* Se va agregar */
    modutnSectorID.mod_type   =   "utnSectorID";  /* Este atributo */
    vutnSectorID[0]           =   utnSectorID;    /* Con este valor */
    vutnSectorID[1]           =   NULL;           /* Fin de agregar valores para este atributo */
    modutnSectorID.mod_values =   vutnSectorID;   /* Se hace apuntar los datos a la estructura */

    modutnSectorContent.mod_op     =   LDAP_MOD_ADD;   /* Idem */
    modutnSectorContent.mod_type   =   "utnSectorContent";  
    vutnSectorContent[0]           =   info.contenido;    
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
    mods[3] = &modutnSectorContent;
    mods[4] = NULL;                    /* Se marca fin de los agregados */ 

    /*
       Aca se construlle el dn que tiene que ser unico. Si se trata de agregar un dn 
       que ya existe simplemente devuelve un error. De la misma manera que si se trata 
       de crear un archivo en un lugar donde ya existe un archivo que se llama igual.
       Construct the dn 
         "utnFileName=miArch.pdf,utnDAName=xyxyxyxy,ou=sadico,dc=utn,dc=edu" with the container 
       name from the command line
     */
    dn=(char*)malloc(strlen("utnDAName=,") + strlen(utnDAName) + strlen("utnSectorID=,") + strlen(utnSectorID) + 2 + 
	strlen("ou=disks,dc=utn,dc=edu"));
    sprintf(dn, "utnSectorID=%s+utnDAName=%s,%s", utnSectorID, utnDAName, "ou=disks,dc=utn,dc=edu");
    printf("DN: [%s]\n", dn);

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

    if ( rc != LDAP_SUCCESS )
    {
       /* printf("\n\tldap_add_ext_s: %s\n", ldap_err2string( rc ));*/
        free( dn );
        return(1);
    }
    
    /*printf("\n\tEntry successfully added: %s\n", dn );*/

    free( dn );
    return( 0 );
}

int ldapGetSector(LDAP *ld, LDAPSECTOR info, char* pBuffer)
{   
    int         version,rc, entryCount, ifor;
    char        searchBase[]="ou=disks,dc=utn,dc=edu";   
    char        *attribute, *dn, **values;       
    BerElement  *ber;
    LDAPMessage *searchResult, *entry;

    char filter[64];
    int i;    
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
                for ( i = 0; values[i] != NULL; i++ ) {
                
                  	if ( 0 == strcmp ("utnSectorContent", attribute))
			{
				for (ifor = 0; ifor<strlen(values[i]) + 1; pBuffer[ifor]=values[i][ifor], ifor++);
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

    return( entryCount );
}

void cargarSector(LDAPSECTOR *info, char* utnDAName, char* utnSectorID)
{
	char numero[50];
	int i; /* <---- que hace esto aca ? xD */
	copiarU(info->utnDAName,utnDAName);
	copiar((char*)info->utnSectorID,utnSectorID);
}



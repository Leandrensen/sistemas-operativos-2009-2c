#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ldap.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cadenas.h"
#include "addFile.h"

void cargarAgregarArchivo(LDAP_AGREGAR_ARCHIVO* info, char* nombreDA, char* nombreArchivo, char* autor, char* permisos, char* tamanio, char* listaSectores)
{
	copiar(info->nombreDA,nombreDA);
	copiar(info->nombreArchivo,nombreArchivo);
	copiar(info->autor,autor);
	copiar(info->permisos,permisos);
	copiar(info->tamanio,tamanio);
	copiar(info->listaSectores,listaSectores);
}

int ldapAgregarArchivo(LDAP* ld, LDAP_AGREGAR_ARCHIVO info)
{

    int         rc; 
    char       *containerName, *utnDAName, *utnFileName, *dn = NULL;     

    char        *vclass[3], *vutnDAName[2], *vutnFileName[2], *vutnFileOwner[2], *vutnFilePermissions[2], 
                *vutnSize[2], *vutnSectorList[2];
    LDAPMod     modClass, modutnDAName, modutnFileName, modutnFileOwner, modutnFilePermissions,
                modutnSize, modutnSectorList;
    LDAPMod     *mods[8];

	utnDAName = info.nombreDA;
	utnFileName = info.nombreArchivo;
    
/*    struct timeval timeOut = {10,0};    /* 10 second connection timeout */

    /* La combinacion utnDAName y utnFileName tiene que ser unico. 
       El DN es unico por entrada. 
       En este caso el DN esta armado con  utnDAName y utnFileName; por ejemplo:
       utnFileName=miArchivo.txt,utnDAName=DA3,ou=sadico,dc=utn,dc=edu
     */
    

    /*
     * No hay mas comentario que el que sigue:
     * To add an entry to the directory:
     *   1. Create the array of LDAPMod structures representing the
     *      attributes of the new entry 
     *   2. Specify the DN of the entry to create <--- Recuerden que tiene que ser unico
     *   3. Call ldap_add_ext_s to add the entry to the directory
     */
     
    /* Se agregan estos objetos */
    modClass.mod_op       =   LDAP_MOD_ADD;    /* El tipo de operacion que se va a hacer*/
    modClass.mod_type     =   "objectclass";  /* Que objecto es el que se va a crear*/
    vclass[0]             =   "top";           /* Idem*/
    vclass[1]             =   "utnFile";       /* Idem. */
    vclass[2]             =   NULL;           /*Fin de declaraciones de objetos*/
    modClass.mod_values   =   vclass;

    /* En los objetos se agregan los atributos que siguen */
    modutnDAName.mod_op      =   LDAP_MOD_ADD;   /* Se va agregar*/
    modutnDAName.mod_type    =   "utnDAName";     /*Este atributo */
    vutnDAName[0]           =   utnDAName;          /* Con este valor*/
    vutnDAName[1]           =   NULL;           /* Fin de agregar valores para este atributo*/
    modutnDAName.mod_values =   vutnDAName;      /* Se hace apuntar los datos a la estructura*/

    modutnFileName.mod_op     =   LDAP_MOD_ADD;   /* Se va agregar*/
    modutnFileName.mod_type   =   "utnFileName";  /* Este atributo */
    vutnFileName[0]           =   utnFileName;    /* Con este valor*/
    vutnFileName[1]           =   NULL;           /* Fin de agregar valores para este atributo*/
    modutnFileName.mod_values =   vutnFileName;   /* Se hace apuntar los datos a la estructura*/

    modutnFileOwner.mod_op     =   LDAP_MOD_ADD;   /* Idem*/
    modutnFileOwner.mod_type   =   "utnFileOwner";  
    vutnFileOwner[0]           =   info.autor;    
    vutnFileOwner[1]           =   NULL;          
    modutnFileOwner.mod_values =   vutnFileOwner; 

    modutnFilePermissions.mod_op     =   LDAP_MOD_ADD;   /* Idem*/
    modutnFilePermissions.mod_type   =   "utnFilePermissions";  
    vutnFilePermissions[0]           =   info.permisos;    
    vutnFilePermissions[1]           =   NULL;          
    modutnFilePermissions.mod_values =   vutnFilePermissions; 

    modutnSize.mod_op     = LDAP_MOD_ADD;   /* Idem  */
    modutnSize.mod_type   = "utnSize";
    vutnSize[0]           = info.tamanio;
    vutnSize[1]           = NULL;
    modutnSize.mod_values = vutnSize;

    modutnSectorList.mod_op     = LDAP_MOD_ADD;   /* Idem  */
    modutnSectorList.mod_type   = "utnSectorList";
    vutnSectorList[0]           = info.listaSectores;
    vutnSectorList[1]           = NULL;
    modutnSectorList.mod_values = vutnSectorList;

    /* mods es la estructura que se va a pasar al comando ldap.
       aca se hacen apuntar todas las estructuras que se vienen de
       rellenar para definir los agregados que se van a hacer a la entrada nueva, 
       a la estructura mnods.
     */
    mods[0] = &modClass; 
    mods[1] = &modutnDAName;
    mods[2] = &modutnFileName;
    mods[3] = &modutnFileOwner;
    mods[4] = &modutnFilePermissions;
    mods[5] = &modutnSize;
    mods[6] = &modutnSectorList;
    mods[7] = NULL;                    /* Se marca fin de los agregados  */

    /*
       Aca se construlle el dn que tiene que ser unico. Si se trata de agregar un dn 
       que ya existe simplemente devuelve un error. De la misma manera que si se trata 
       de crear un archivo en un lugar donde ya existe un archivo que se llama igual.
       Construct the dn 
         "utnFileName=miArch.pdf,utnDAName=xyxyxyxy,ou=sadico,dc=utn,dc=edu" with the container 
       name from the command line
     */
    dn=(char*)malloc(strlen(utnFileName) + strlen("utnFileName=,") + strlen(utnDAName) +   strlen("utnDAName=,") + 100);
    sprintf( dn, "utnFileName=%s,utnDAName=%s,ou=sadico,dc=utn,dc=edu",  utnFileName, utnDAName);
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

    if ( rc != LDAP_SUCCESS )
    {
        printf("\n\tldap_add_ext_s: %s\n", ldap_err2string( rc ));
        free( dn );
        ldap_unbind_s ( ld );
        return(1);
    }
    
    printf("\n\tEntry successfully added: %s\n", dn );

    free( dn );
    return( 0 );
}

int ldapFormatearDA(LDAP* ld, char* nombreDA, int iCantSectores)
{

    int         rc,i; 
    char       *containerName, *utnDAName, *dn = NULL, cadena[100],*cadena2;     

    char        *vclass[3], *vutnDAName[2], 
                *vutnFreeSectors[iCantSectores+1], *vutnSize[2];
    LDAPMod     modClass, modutnDAName, modutnSize, modutnFreeSectors;
    LDAPMod     *mods[8];

	    
/*    struct timeval timeOut = {10,0};    /* 10 second connection timeout */

    /* La combinacion utnDAName y utnFileName tiene que ser unico. 
       El DN es unico por entrada. 
       En este caso el DN esta armado con  utnDAName y utnFileName; por ejemplo:
       utnFileName=miArchivo.txt,utnDAName=DA3,ou=sadico,dc=utn,dc=edu
     */
    

    /*
     * No hay mas comentario que el que sigue:
     * To add an entry to the directory:
     *   1. Create the array of LDAPMod structures representing the
     *      attributes of the new entry 
     *   2. Specify the DN of the entry to create <--- Recuerden que tiene que ser unico
     *   3. Call ldap_add_ext_s to add the entry to the directory
     */
     
    /* Se agregan estos objetos */
    modClass.mod_op       =   LDAP_MOD_ADD;    /* El tipo de operacion que se va a hacer*/
    modClass.mod_type     =   "objectclass";  /* Que objecto es el que se va a crear*/
    vclass[0]             =   "top";           /* Idem*/
    vclass[1]             =   "utnDA";       /* Idem. */
    vclass[2]             =   NULL;           /*Fin de declaraciones de objetos*/
    modClass.mod_values   =   vclass;

    /* En los objetos se agregan los atributos que siguen */
    modutnDAName.mod_op      =   LDAP_MOD_ADD;   /* Se va agregar*/
    modutnDAName.mod_type    =   "utnDAName";     /*Este atributo */
    vutnDAName[0]           =   nombreDA;          /* Con este valor*/
    vutnDAName[1]           =   NULL;           /* Fin de agregar valores para este atributo*/
    modutnDAName.mod_values =   vutnDAName;      /* Se hace apuntar los datos a la estructura*/

    modutnSize.mod_op     = LDAP_MOD_ADD;   /* Idem  */
    modutnSize.mod_type   = "utnSize";
    miitoa(cadena,512*iCantSectores);
	vutnSize[0]           = cadena;
    vutnSize[1]           = NULL;
    modutnSize.mod_values = vutnSize;

    modutnFreeSectors.mod_op     = LDAP_MOD_ADD;   /* Idem  */
    modutnFreeSectors.mod_type   = "utnFreeSectors";
	for(i=0; i < iCantSectores; i++)
	{
		cadena2 = (char*) malloc (20);
		miitoa(cadena2,i);
		vutnFreeSectors[i]           = cadena2;
	}
    vutnFreeSectors[iCantSectores]           = NULL;
    modutnFreeSectors.mod_values = vutnFreeSectors;

    /* mods es la estructura que se va a pasar al comando ldap.
       aca se hacen apuntar todas las estructuras que se vienen de
       rellenar para definir los agregados que se van a hacer a la entrada nueva, 
       a la estructura mnods.
     */
    mods[0] = &modClass; 
    mods[1] = &modutnDAName;
    mods[2] = &modutnSize;
    mods[3] = &modutnFreeSectors;
    mods[4] = NULL;                    /* Se marca fin de los agregados  */

    /*
       Aca se construlle el dn que tiene que ser unico. Si se trata de agregar un dn 
       que ya existe simplemente devuelve un error. De la misma manera que si se trata 
       de crear un archivo en un lugar donde ya existe un archivo que se llama igual.
       Construct the dn 
         "utnFileName=miArch.pdf,utnDAName=xyxyxyxy,ou=sadico,dc=utn,dc=edu" with the container 
       name from the command line
     */
    dn=(char*)malloc(strlen(nombreDA) +   strlen("utnDAName=,") + 30);
    sprintf( dn, "utnDAName=%s,ou=sadico,dc=utn,dc=edu",  nombreDA);
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

    if ( rc != LDAP_SUCCESS )
    {
        printf("\n\tldap_add_ext_s: %s\n", ldap_err2string( rc ));
        free( dn );
        ldap_unbind_s ( ld );
        return(1);
    }
    
    printf("\n\tEntry successfully added: %s\n", dn );
	for(i=0; i < iCantSectores; i++)
	{
		free(vutnFreeSectors[i]);
	}
    free( dn );
    return( 0 );
}

int ldapQuitarDosSectoresDA(LDAP* ld, char* nombreDA, int iSector1, int iSector2, int* listaSectores, int iCantSectores)
{

    int         rc,i,j; 
    char       *containerName, *utnDAName, *dn = NULL, cadena[100],*cadena2;     

    char        *vclass[3], *vutnDAName[2], 
                *vutnFreeSectors[iCantSectores+1], *vutnSize[2], modifyDN[100];
    LDAPMod     modClass, modutnDAName, modutnSize, modutnFreeSectors;
    LDAPMod     *mods[8];

	    
/*    struct timeval timeOut = {10,0};    /* 10 second connection timeout */

    /* La combinacion utnDAName y utnFileName tiene que ser unico. 
       El DN es unico por entrada. 
       En este caso el DN esta armado con  utnDAName y utnFileName; por ejemplo:
       utnFileName=miArchivo.txt,utnDAName=DA3,ou=sadico,dc=utn,dc=edu
     */
    

    /*
     * No hay mas comentario que el que sigue:
     * To add an entry to the directory:
     *   1. Create the array of LDAPMod structures representing the
     *      attributes of the new entry 
     *   2. Specify the DN of the entry to create <--- Recuerden que tiene que ser unico
     *   3. Call ldap_add_ext_s to add the entry to the directory
     */
     sprintf(modifyDN, "utnDAName=%s,ou=sadico,dc=utn,dc=edu", nombreDA);
  
    modutnFreeSectors.mod_op     = LDAP_MOD_REPLACE;   /* Idem  */
    modutnFreeSectors.mod_type   = "utnFreeSectors";
	for(i=0; i < iCantSectores; i++)
	{
		cadena2 = (char*) malloc (20);
		
		miitoa(cadena2,listaSectores[i]);
		vutnFreeSectors[i]           = cadena2;
	}
    vutnFreeSectors[i]           = NULL;
    modutnFreeSectors.mod_values = vutnFreeSectors;


    /* mods es la estructura que se va a pasar al comando ldap.
       aca se hacen apuntar todas las estructuras que se vienen de
       rellenar para definir los agregados que se van a hacer a la entrada nueva, 
       a la estructura mnods.
     */
    mods[0] = &modutnFreeSectors;
    mods[1] = NULL;                    /* Se marca fin de los agregados  */

    /*
       Aca se construlle el dn que tiene que ser unico. Si se trata de agregar un dn 
       que ya existe simplemente devuelve un error. De la misma manera que si se trata 
       de crear un archivo en un lugar donde ya existe un archivo que se llama igual.
       Construct the dn 
         "utnFileName=miArch.pdf,utnDAName=xyxyxyxy,ou=sadico,dc=utn,dc=edu" with the container 
       name from the command line
     */

    /*
       Finalmente se llama a la funcion para agregar la nueva entrada 
       a la base de datos. Los parametros principales son el dn, y todos 
       los datos que componen la entrada nueva, que se encuentran en mods.
     * Now add the object
     */
     rc= ldap_modify_ext_s( ld,         /* LDAP session handle */
                           modifyDN,   /* the object to modify */
                           mods,     /* array of LDAPMod structures */
                           NULL,       /* server controls */
                           NULL);      /* client controls */


    if ( rc != LDAP_SUCCESS )
    {
        printf("\n\tldap_modify_ext_s: %s\n", ldap_err2string( rc ));
        free( dn );
        ldap_unbind_s ( ld );
        return(1);
    }
    
	for(i=0; i < iCantSectores; i++)
	{
		free(vutnFreeSectors[i]);
	}

    return( 0 );
}




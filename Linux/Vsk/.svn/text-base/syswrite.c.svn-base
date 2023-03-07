/* este incluye SYSFLUSH */
#include <stdlib.h>
#include "syscalls.h"
#include "syswrite.h"
#include "sendrecv.h"
#include "cadenas.h"
#include "oft.h"
#include "fd.h"
#include "funcionesldap.h"

void sys_write (int iSocketSFS, int iSocketCliente, pNodoFD* pFD, pOft* pOFT, HEADERIRCIPC cabecera, pConexiones* L_Conexiones, int iHayCache, pLDAPINIT* ldapSesion)
{
	pNodoFD pAuxFD = *pFD;
	pOft pArchivo = pAuxFD->fd.pDescriptor;
	pConexiones pAux_Conexiones =*L_Conexiones;
	pLDAPINIT ldapLocal = *ldapSesion;
	int i;
	
	switch (pAuxFD->fd.iPaso)
	{
		case 0:
			if (pArchivo->fcb.iModoApertura == APERTURA_ESCRITURA)
			{
				for (i=0 ; i<1024; pArchivo->fcb.ucvBuffer[i]='\0', i++);
				recibirAlgo (iSocketCliente, pArchivo->fcb.ucvBuffer, cabecera.uiPayloadLength);
				pAuxFD->fd.iPaso = 1;
				pArchivo->fcb.iTamanio = pArchivo->fcb.iTamanio + cabecera.uiPayloadLength;
				sys_flush (iSocketSFS, &pAuxFD , &pArchivo, cabecera, &pAux_Conexiones,iHayCache, &ldapLocal );
				
			}
			break;

		case 3: 
			printf ("OK escritura\n");
			cabecera.ucPayloadDescriptor = 0x40;
			enviarH(iSocketCliente, cabecera);/* actualizar Tamanio */
			break;

		default: 
			sys_flush (iSocketSFS, &pAuxFD , &pArchivo, cabecera, &pAux_Conexiones,iHayCache, &ldapLocal );
			/*if (iHayCache == 1)
			{
				
			}*/
			break;
	}
}

void sys_flush (int iSocketSFS, pNodoFD* pFD , pOft* pOFT, HEADERIRCIPC cabecera, pConexiones* L_Conexiones, int iHayCache, pLDAPINIT* ldapSesion)
{
	pNodoFD pAuxFD = *pFD;
	pOft pArchivo = pAuxFD->fd.pDescriptor;
	unsigned char* ucpSectores;
	int iSector1, iSector2;
	pConexiones pAux_Conexiones =*L_Conexiones;
	pConexiones pAux_buscador;
	LDAPSECTOR infoSector;
	char cvSector[50];
	pLDAPINIT ldapLocal = *ldapSesion;

	switch (pAuxFD->fd.iPaso)
	{
		case 1:
			if (pArchivo->fcb.iModoApertura == APERTURA_ESCRITURA)
			{
				pedirDosSectoresLibres (iSocketSFS, pArchivo->fcb.cvNombreDA, cabecera);
				pAuxFD->fd.iPaso = 2;
			}

			break;

		case 2:
			ucpSectores = (unsigned char*) malloc (cabecera.uiPayloadLength);
			recibirAlgo (iSocketSFS, ucpSectores, cabecera.uiPayloadLength);
			iSector1 = BinToNum(ucpSectores, 0);
			iSector2 = BinToNum(ucpSectores, 4);
			cabecera.ucPayloadDescriptor = 0xFF;
			cabecera.uiPayloadLength = longitudCadenaU (pArchivo->fcb.cvNombreDA);
			enviarH(iSocketSFS, cabecera);
			enviarAlgo (iSocketSFS, pArchivo->fcb.cvNombreDA, cabecera.uiPayloadLength);
			cabecera.uiPayloadLength = longitudCadenaU (pArchivo->fcb.cvNombreArchivo);
			enviarH(iSocketSFS, cabecera);
			enviarAlgo (iSocketSFS, pArchivo->fcb.cvNombreArchivo, cabecera.uiPayloadLength);
			cabecera.uiPayloadLength = 8;
			enviarAlgo (iSocketSFS, ucpSectores, cabecera.uiPayloadLength);
			if (iHayCache)
			{
				miitoa(cvSector, iSector1 );
				cargarSector(&infoSector, pArchivo->fcb.cvNombreDA, cvSector, "", 0); 
				ldapBuscarCacheParaInvalidar(ldapLocal->ldapSesion, infoSector);
				miitoa(cvSector, iSector2 );
				cargarSector(&infoSector, pArchivo->fcb.cvNombreDA, cvSector, "", 0); 
				ldapBuscarCacheParaInvalidar(ldapLocal->ldapSesion, infoSector);
			}
			pAux_buscador = buscarConexionPorDA (&pAux_Conexiones, pAuxFD->fd.ucvDA);
			cabecera.ucPayloadDescriptor = PUTSECTORES;
			cabecera.uiPayloadLength = 1032;
			enviar_putSectores(pAux_buscador->conexion.iSocket, cabecera, pArchivo->fcb.ucvBuffer, iSector1, iSector2);
			pAuxFD->fd.iPaso = 3;
			break;
	}

}

void pedirDosSectoresLibres (int iSocketSFS, unsigned char* ucpDA, HEADERIRCIPC cabecera)
{
	
	cabecera.ucPayloadDescriptor = DOSSECTORESLIBRES_IDA;
	cabecera.uiPayloadLength = longitudCadenaU (ucpDA);
	enviarH (iSocketSFS, cabecera);
	enviarAlgo (iSocketSFS, ucpDA, cabecera.uiPayloadLength);

}


#include <stdlib.h>
#include <ldap.h>
#include "archivos.h"
#include "funcionesldap.h"
#include "sysread.h"
#include "oft.h"
#include "fd.h"
#include "sendrecv.h"
#include "syscalls.h"
#include "cadenas.h"



void sys_read (int iSocketSFS, int iSocketCliente, pNodoFD* pFD, HEADERIRCIPC cabecera, pConexiones* L_Conexiones, int iHayCache, pLDAPINIT *ldap_sesion)
{
	pNodoFD pAuxFD = *pFD;
	pOft pArchivo = pAuxFD->fd.pDescriptor;
	int iCantidadALeer, iBloque1, iBloque2;
	pLDAPINIT ldapLocal = *ldap_sesion;
	int* ipSectores;
	unsigned char ucpMensaje[1024];
	pConexiones pAux_buscador;
	pConexiones pAux_Conexiones = *L_Conexiones;
	LDAPSECTOR infoSector1, infoSector2;
	char cvSector1[100], cvSector2[100], dnViejo[100];
	ipSectores = (int *)pArchivo->fcb.sectores;
	unsigned char ucvCache[513];
	char cvLog[1000];
	

	switch (pAuxFD->fd.iPaso)
	{
		case 0:
			if (pArchivo->fcb.iModoApertura == APERTURA_LECTURA)
			{
				iCantidadALeer = pAuxFD->fd.iTotal - pAuxFD->fd.iPosALeer;
				if (iCantidadALeer >= 2)
				{ /* leo 2 */
					/* si hay cache verifico si esta, en caso de NO, sigo */
					if (iHayCache == 1)
					{
						miitoa(cvSector1,ipSectores[pAuxFD->fd.iPosALeer]);
						miitoa(cvSector2, ipSectores[pAuxFD->fd.iPosALeer+1]);
						cargarSector(&infoSector1, pAuxFD->fd.ucvDA, cvSector1, "", 0); 
						cargarSector(&infoSector2, pAuxFD->fd.ucvDA, cvSector2, "", 0); 
						/* pues donde se usara no preciso los ultimos 2 datos, por eso "" y 0 */
						iBloque1 = ldapBuscarCache(ldapLocal->ldapSesion, infoSector1, (char*)pArchivo->fcb.ucvBuffer);
						iBloque2 = ldapBuscarCache(ldapLocal->ldapSesion, infoSector2, (char*)&(pArchivo->fcb.ucvBuffer[512]));
						/**********************************************************************/
						if (iBloque1 + iBloque2 == 2)
						{
							sprintf (cvLog, "CACHE HIT - DA:%s, Sector:%s",pAuxFD->fd.ucvDA,cvSector1);
							escribirLog ('I', cvLog);
							sprintf (cvLog, "CACHE HIT - DA:%s, Sector:%s",pAuxFD->fd.ucvDA,cvSector2);
							escribirLog ('I', cvLog);
							pAuxFD->fd.iPosALeer+=2;
							pAuxFD->fd.iPaso = 0;
							cabecera.ucPayloadDescriptor = READ_FILE_OK;
							/* el tamanio a enviar ya viene en la cabecera */
							/* iSocketSFS en el segundo llamado de sysread es el cliente FTP*/ 
							/* fuezo tamanio para prueba */
							cabecera.uiPayloadLength = longitudCadena (pArchivo->fcb.ucvBuffer);
							enviarH (iSocketCliente, cabecera);
							enviarAlgo (iSocketCliente, pArchivo->fcb.ucvBuffer, cabecera.uiPayloadLength);

						}
						/********************************************************************/
						if (iBloque1 + iBloque2 == 1)
						{
							pAuxFD->fd.iPedidos = 1;
							copiarCabecera(&cabecera, GETSECTORES1, 8);
							ipSectores = (int *)pArchivo->fcb.sectores;
							if (iBloque1 == 0) 
							{
								sprintf (cvLog, "CACHE HIT - DA:%s, Sector:%s",pAuxFD->fd.ucvDA,cvSector2);
								escribirLog ('I', cvLog);
								sprintf (cvLog, "CACHE MISS - DA:%s, Sector:%s",pAuxFD->fd.ucvDA,cvSector1);
								escribirLog ('I', cvLog);
								NumToBin (ucpMensaje, ipSectores[pAuxFD->fd.iPosALeer], 0);
								pAuxFD->fd.iSector1 = ipSectores[pAuxFD->fd.iPosALeer];
								pAuxFD->fd.iSector2 = -1;

							}
							else 
							{
								sprintf (cvLog, "CACHE HIT - DA:%s, Sector:%s",pAuxFD->fd.ucvDA,cvSector1);
								escribirLog ('I', cvLog);
								sprintf (cvLog, "CACHE MISS - DA:%s, Sector:%s",pAuxFD->fd.ucvDA,cvSector2);
								escribirLog ('I', cvLog);
								NumToBin (ucpMensaje, ipSectores[pAuxFD->fd.iPosALeer+1], 0);
								pAuxFD->fd.iSector2 = ipSectores[pAuxFD->fd.iPosALeer+1];
								pAuxFD->fd.iSector1 = -1;

							}
						}
						if (iBloque1 + iBloque2 == 0)
						{
							copiarCabecera(&cabecera, GETSECTORES2, 8);
							NumToBin (ucpMensaje, ipSectores[pAuxFD->fd.iPosALeer], 0);
							NumToBin (ucpMensaje, ipSectores[pAuxFD->fd.iPosALeer+1], 4);
							pAux_buscador = buscarConexionPorDA (&pAux_Conexiones, pAuxFD->fd.ucvDA);
							enviar_getSectores(pAux_buscador->conexion.iSocket, cabecera, ucpMensaje);
							pAuxFD->fd.iSector1 = ipSectores[pAuxFD->fd.iPosALeer];
							pAuxFD->fd.iSector2 = ipSectores[pAuxFD->fd.iPosALeer+1];
							pAuxFD->fd.iPosALeer+=2;
							pAuxFD->fd.iPaso = 1;
							pAuxFD->fd.iPedidos = 2;
						}
	
					}
					else /* no hay cache */
					{
						copiarCabecera(&cabecera, GETSECTORES2, 8);
						NumToBin (ucpMensaje, ipSectores[pAuxFD->fd.iPosALeer], 0);
						NumToBin (ucpMensaje, ipSectores[pAuxFD->fd.iPosALeer+1], 4);
						pAux_buscador = buscarConexionPorDA (&pAux_Conexiones, pAuxFD->fd.ucvDA);
						enviar_getSectores(pAux_buscador->conexion.iSocket, cabecera, ucpMensaje);
						pAuxFD->fd.iSector1 = ipSectores[pAuxFD->fd.iPosALeer];
						pAuxFD->fd.iSector2 = ipSectores[pAuxFD->fd.iPosALeer+1];
						pAuxFD->fd.iPosALeer+=2;
						pAuxFD->fd.iPaso = 1;
						pAuxFD->fd.iPedidos = 2;

					}

				}
				else if (iCantidadALeer == 1) /* leo 1 */
				{
					iBloque1 = ldapBuscarCache(ldapLocal->ldapSesion, infoSector1, (char*)pArchivo->fcb.ucvBuffer);
					if (iBloque1 == 0)
					{
						copiarCabecera(&cabecera, GETSECTORES1, 8); /* constante en sysread.h*/
						ipSectores = (int *)pArchivo->fcb.sectores;
						NumToBin (ucpMensaje, ipSectores[pAuxFD->fd.iPosALeer], 0);
						pAuxFD->fd.iPosALeer+=1;
						pAux_buscador = buscarConexionPorDA (&pAux_Conexiones, pAuxFD->fd.ucvDA);
						enviar_getSectores(pAux_buscador->conexion.iSocket, cabecera, ucpMensaje);
						pAuxFD->fd.iPaso= 1;
						pAuxFD->fd.iSector1 = ipSectores[pAuxFD->fd.iPosALeer];
						pAuxFD->fd.iSector2 = -1;

					}
					else
					{
						printf("enviar al cliente\n");
						/* enviar al cliente con el tamaño restante del archivo */
					}

				}
				else 
				{/* no leo mas */
					pAuxFD->fd.iPaso = 2;
				}
			}
			else
			{
				printf("que hago si no es para lectura\n");

			}
		break;
	
		case 1:
			if (pAuxFD->fd.iPedidos == 2)
			{
				recibir_getSectores(iSocketCliente, ucpMensaje);
				copiarN (pArchivo->fcb.ucvBuffer, ucpMensaje, 0, 1024);
			}
			else
			{
				recibir_getSectores(iSocketCliente, ucpMensaje);
				if (pAuxFD->fd.iSector2 == -1)					
					copiarN (pArchivo->fcb.ucvBuffer, ucpMensaje, 0, 512);
				else
					copiarN (pArchivo->fcb.ucvBuffer, ucpMensaje, 512, 512);

			}
			pAuxFD->fd.iPaso = 0;
			/* se debe actualizar la cache si hay */
			if (iHayCache == 1)
			{
				if (pAuxFD->fd.iSector1 > -1)
				{
					miitoa(cvSector1,pAuxFD->fd.iSector1);
					copiarN (ucvCache, ucpMensaje, 0, 512);
					ucvCache[512]='\0';
					if (ldapLocal->iCantidadMaxima> ldapLocal->iCantidadActual)
					{
						/* mandar a cargar */
						cargarSector(&infoSector1 , pAuxFD->fd.ucvDA, cvSector1, (char*)ucvCache, ldapLocal->iCantidadActual + 1);
						/* uno mas en cache */
						ldapCargarCache(ldapLocal->ldapSesion, infoSector1);
						ldapLocal->iCantidadActual+= 1;
					}
					else
					{	
						/* buscar seleccionando victima, modificar contenido y tiempo acceso */
						ldapBuscarMasViejoCache(ldapLocal->ldapSesion, dnViejo);
						ldapModificarContenidoCache (ldapLocal->ldapSesion, dnViejo, (char*)ucvCache);
						ldapModificarCache(ldapLocal->ldapSesion, dnViejo);
						
					}
				}
				if (pAuxFD->fd.iSector2 > -1)
				{
					miitoa(cvSector2,pAuxFD->fd.iSector2);
					copiarN (ucvCache, &ucpMensaje[512], 0, 512);
					ucvCache[512]='\0';
					if (ldapLocal->iCantidadMaxima> ldapLocal->iCantidadActual)
					{
						/* mandar a cargar */
						cargarSector(&infoSector2, pAuxFD->fd.ucvDA, cvSector2, (char*)ucvCache, ldapLocal->iCantidadActual + 1);
						/* cargar en cache */
						ldapCargarCache(ldapLocal->ldapSesion, infoSector2);
						ldapLocal->iCantidadActual+= 1;
					}
					else
					{
						/* buscar seleccionando victima, modificar contenido y tiempo acceso */
						ldapBuscarMasViejoCache(ldapLocal->ldapSesion, dnViejo);
						ldapModificarContenidoCache (ldapLocal->ldapSesion, dnViejo, (char*)ucvCache);
						ldapModificarCache(ldapLocal->ldapSesion, dnViejo);


					}
				}

			}
			/* enviar al cliente */

			cabecera.ucPayloadDescriptor = READ_FILE_OK;
			/* el tamanio a enviar ya viene en la cabecera */
			/* iSocketSFS en el segundo llamado de sysread es el cliente FTP*/
			cabecera.uiPayloadLength= longitudCadena (pArchivo->fcb.ucvBuffer);
			enviarH (iSocketSFS, cabecera);
			enviarAlgo (iSocketSFS, pArchivo->fcb.ucvBuffer, cabecera.uiPayloadLength);
		break;
	}
	if (pAuxFD->fd.iPaso == 2 )
	{
		/* enviar al cliente que no hay mas para leer */
		cabecera.ucPayloadDescriptor = READ_FILE_OK;
		cabecera.uiPayloadLength = 0;
		enviarH (iSocketCliente, cabecera);
	}
	
}

void copiarN (unsigned char* bufferDestino, unsigned char* bufferOrigen, int iDesdeEnDestino, int iCantidad)
{
	int i;
	for (i = 0; i < iCantidad; bufferDestino[iDesdeEnDestino + i] = bufferOrigen[i], i++);	
}

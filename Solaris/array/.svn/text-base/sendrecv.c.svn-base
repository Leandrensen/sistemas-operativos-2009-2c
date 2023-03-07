#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "sendrecv.h"

/********************************************************************************************/
int enviarH(int iSock, HEADERIRCIPC cabecera)
{
	unsigned char ucBuf[21];
	int i;

	for(i = 0; i < 16; i++)
		ucBuf[i] = cabecera.cpDescriptorID[i];
	ucBuf[16] = cabecera.ucPayloadDescriptor;
	NumToBin(ucBuf, cabecera.uiPayloadLength, 17);

       return enviarAlgo(iSock, ucBuf, 21);
}

/********************************************************************************************/
int recibirH(int iSocket, HEADERIRCIPC *cabecera)
{
	unsigned char ucBuf[21];
	int i;
	int iResult;
	
       iResult = recibirAlgo(iSocket, ucBuf, 21);

	switch (iResult)
	{
		case 0: return SOCKET_ERROR_DESCONEXION; break;
		case -1: return SOCKET_ERROR_SENDRECV; break;
	} 
 
       for(i = 0; i < 16; i++)
		cabecera->cpDescriptorID[i] = ucBuf[i];
	cabecera->ucPayloadDescriptor = ucBuf[16];
	cabecera->uiPayloadLength = BinToNum (ucBuf, 17);

	return SOCKET_OK_SENDRECV;

}

/********************************************************************************************/
int recibirAlgo(int iSocket, unsigned char *ucBuf, int iCantidad)
{
    int iRecibido = 0;
    int iTotalRecibido = 0;
    int iTotalARecibir = 0;

    iTotalARecibir = iCantidad;

    while(iTotalRecibido < iTotalARecibir)
    {
       iRecibido = recv(iSocket, ucBuf+iTotalRecibido, iTotalARecibir-iTotalRecibido, 0);
	switch(iRecibido)
	{
 		case 0: return SOCKET_ERROR_DESCONEXION; break;
		case -1: return SOCKET_ERROR_SENDRECV; break;
		default: iTotalRecibido +=iRecibido ; break;
       }
    }
    return SOCKET_OK_SENDRECV;
}

/********************************************************************************************/
int enviarAlgo(int iSocket, unsigned char *ucBuf, int iCantidad)
{
    int iEnviados = 0;
    int iTotalEnviados = 0;
    int iTotalAEnviar = 0;

    iTotalAEnviar = iCantidad;
    
    while (iTotalEnviados < iTotalAEnviar)
    {
	iEnviados = send(iSocket, ucBuf+iTotalEnviados, iTotalAEnviar-iTotalEnviados,0);
	switch (iEnviados)
	{
		case 0: return SOCKET_ERROR_DESCONEXION; break;
		case -1: return SOCKET_ERROR_SENDRECV; break;
		default: iTotalEnviados+=iEnviados; break;
	}
	
    }

    return SOCKET_OK_SENDRECV;
}

/********************************************************************************************/
int iniciarServidor (char* IPlocal, int PUERTOLocal, int conexionesSimultaneas)
{
	int iSocket;
	struct sockaddr_in sin;
	int yes = 1;

	iSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (iSocket == -1)
		return SOCKET_INVALIDO; 

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(IPlocal);
	sin.sin_port = htons(PUERTOLocal);
	memset(&(sin.sin_zero),'\0',8);
	
	/* socket reusable */
	
	if(setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror ("Error en setsockopt: ");
    		return SOCKET_ERROR_INICIALIZAR;
	}  

	if(bind(iSocket, (struct sockaddr *)&sin, sizeof(sin)) == -1) 	
	{
		perror ("Error en bind: ");
    		return SOCKET_ERROR_INICIALIZAR;
	}
		
	if(listen(iSocket, conexionesSimultaneas) == -1) 	
	{
		perror ("Error en listen: ");
    		return SOCKET_ERROR_INICIALIZAR;
	}

	return iSocket;
}
    	
/********************************************************************************************/
int iniciarCliente(char* IPdestino, int PUERTODestino)
{
	struct sockaddr_in serv;
	int iSocket;
	iSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (iSocket == -1)
		return SOCKET_INVALIDO;

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr= inet_addr((char*)IPdestino);
	serv.sin_port = htons(PUERTODestino);
	memset(serv.sin_zero,'\0',sizeof (serv.sin_zero));

	if (connect(iSocket, (struct sockaddr *)&serv, sizeof(struct sockaddr)) == -1)
    		return SOCKET_ERROR_CONEXION;

	return iSocket;
}


/********************************************************************************************/
unsigned int BinToNum (unsigned char buf[], int iDesde)
{
	unsigned iTotal;

	iTotal = buf[iDesde+0] + 256*buf[iDesde+1] + 65536*buf[iDesde+2] + 16777216*buf[iDesde+3];

	return iTotal;
}

/********************************************************************************************/
void NumToBin (unsigned char ucvBuf[], int iTotal, int iDesde)
{

	if (iTotal>16777216-1)
	{
		ucvBuf[iDesde+3]=iTotal/16777216;
		iTotal-=16777216;
	}
	else
	ucvBuf[iDesde+3]=0;

	if (iTotal>65536-1)
	{
		ucvBuf[iDesde+2]=iTotal/65536;
		iTotal-=65536;
	}
	else
	ucvBuf[iDesde+2]=0;

	if (iTotal>256-1)
	{
		ucvBuf[iDesde+1]=iTotal/256;
		iTotal-=256;
	}
	else
	ucvBuf[iDesde+1]=0;

	if (iTotal>0)
	{
		ucvBuf[iDesde+0]=iTotal;
		iTotal-=0;
	}
	else
	ucvBuf[iDesde+0]=0;
}

/********************************************************************************************/
int aceptarConexion (int iSocket)
{
	int iSocket_aceptado;
	struct sockaddr_in pin;
	unsigned int 	address_size = sizeof(pin);

	iSocket_aceptado= accept(iSocket, (struct sockaddr *)&pin, &address_size);
	
	if (iSocket_aceptado == -1) 
	{
		perror ("Error de accept()\n");
		return SOCKET_INVALIDO;
	}

       printf("Servidor ARRAY: nueva conexion de %s en socket %d\nARRAY ?>", inet_ntoa(pin.sin_addr), iSocket_aceptado);	                    
	return iSocket_aceptado;

	
}

/********************************************************************************************/
void crearCabecera (HEADERIRCIPC *cabecera, unsigned char ucPayloadDescriptor, unsigned int uiPayloadLength)
{
	generarID (cabecera->cpDescriptorID);
	cabecera->ucPayloadDescriptor = ucPayloadDescriptor;
	cabecera->uiPayloadLength = uiPayloadLength;

}

/********************************************************************************************/
void generarID(unsigned char* ucpID)
{
	int i,k;
	for (i=0; i<16;i++)
	{
		k= rand()%(2-0+1)+0;
		if (k==0) ucpID[i]=rand()%('z'-'a'+1)+'a';
		else if(k==1) ucpID[i]=rand()%('9'-'0'+1)+'0';
		else ucpID[i]=rand()%('Z'-'A'+1)+'A';
	}
}

/********************************************************************************************/
int generarSocketHilo(char* IPdestino, int iPuertoTemporal)
{
	int iSocketHiloTemporal;
	do
	{
		iPuertoTemporal++;
		iSocketHiloTemporal = iniciarServidor (IPdestino,iPuertoTemporal, 20); /* sendrecv.h */
	}while ((iSocketHiloTemporal == SOCKET_INVALIDO) || (iSocketHiloTemporal == SOCKET_ERROR_INICIALIZAR));
	return iSocketHiloTemporal;			
}

/*********************************************************************************************/
int compararCabecera(unsigned char* cpDescriptorID1, unsigned char* cpDescriptorID2)
{
	int i;

	for(i = 0; (i < 16) && (cpDescriptorID1[i] == cpDescriptorID2[i]); i++) ;
	if (i == 16) return 1;

	return 0;
}

/**********************************************************************************************/
void copiarCabecera(unsigned char* cpDescriptorID1, unsigned char* cpDescriptorID2)
{
	int j;

	for (j = 0; j < 16 ; cpDescriptorID1[j] = cpDescriptorID2[j], j++);
}

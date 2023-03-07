
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "sendrecv.h"
#include "cadenas.h"

int enviarH(SOCKET sock, HEADERIRCIPC cabecera)
{
	unsigned char ucBuf[21];
	int i;

	for(i = 0; i < 16; i++)
		ucBuf[i] = cabecera.cpDescriptorID[i];

	ucBuf[16] = cabecera.ucPayloadDescriptor;
	NumToBin(ucBuf, cabecera.uiPayloadLength, 17);

	return enviarAlgo(sock, ucBuf, 21);
}

/**************************************************************************************/
int recibirH(SOCKET sock, HEADERIRCIPC * cabecera)
{
	unsigned char ucBuf[21];
    int i;
	int iResult;
	
	iResult = recibirAlgo(sock, ucBuf, 21);
	
	switch(iResult)
	{
			case 0: return SOCKET_ERROR_DESCONEXION;	break;
			case -1: return SOCKET_ERROR_SENDRECV;		break;
	}

    for(i = 0; i < 16; i++)
		cabecera->cpDescriptorID[i] = ucBuf[i];

	cabecera->ucPayloadDescriptor = ucBuf[16];
	cabecera->uiPayloadLength = BinToNum (ucBuf, 17);

	return SOCKET_OK_SENDRECV;
}

/**************************************************************************************/
int recibirAlgo(SOCKET sock, unsigned char *ucBuf, int iCantidad)
{
    int iRecibido = 0;
    int iTotalRecibido = 0;
    int iTotalARecibir = 0;

    iTotalARecibir = iCantidad;

    while(iTotalRecibido < iTotalARecibir)
    {
        iRecibido = recv(sock, ucBuf+iTotalRecibido, iTotalARecibir-iTotalRecibido, 0);
       
        switch(iRecibido)
        {
			case 0: return SOCKET_ERROR_DESCONEXION;	break;
			case -1: return SOCKET_ERROR_SENDRECV;		break;
			default: iTotalRecibido+=iRecibido;			break;
        }
    }

    return SOCKET_OK_SENDRECV;
}

/**************************************************************************************/
int enviarAlgo(SOCKET sock, unsigned char *ucBuf, int iCantidad)
{
    int iEnviados = 0;
    int iTotalEnviados = 0;
    int iTotalAEnviar = 0;

    iTotalAEnviar = iCantidad;
    
    while (iTotalEnviados < iTotalAEnviar)
    {
		iEnviados = send(sock, ucBuf+iTotalEnviados, iTotalAEnviar-iTotalEnviados,0);
	
		switch(iEnviados)
		{
			case 0: return SOCKET_ERROR_DESCONEXION;	break;
			case -1: return SOCKET_ERROR_SENDRECV;		break;
			default: iTotalEnviados+=iEnviados;			break;
        }
             		
	}

	return SOCKET_OK_SENDRECV;
}

/**************************************************************************************/
unsigned int BinToNum (unsigned char * buffer, int desde)
{
	unsigned total;

	total = buffer[desde+0] + 256*buffer[desde+1] + 65536*buffer[desde+2] + 16777216*buffer[desde+3];

	return total;
}

/**************************************************************************************/
void NumToBin (unsigned char * buffer, int total, int desde)
{

	if (total>16777216-1)
	{
		buffer[desde+3]=total/16777216;
		total-=16777216;
	}
	else
	buffer[desde+3]=0;

	if (total>65536-1)
	{
		buffer[desde+2]=total/65536;
		total-=65536;
	}
	else
	buffer[desde+2]=0;

	if (total>256-1)
	{
		buffer[desde+1]=total/256;
		total-=256;
	}
	else
	buffer[desde+1]=0;

	if (total>0)
	{
		buffer[desde+0]=total;
		total-=0;
	}
	else
	buffer[desde+0]=0;
}

/**************************************************************************************/
SOCKET iniciarCliente(char* IPdestino, int PUERTOdestino)
{
	struct sockaddr_in Server;
	SOCKET cliente;

	Server.sin_family = AF_INET;
	Server.sin_addr.s_addr = inet_addr(IPdestino);
	Server.sin_port = htons(PUERTOdestino);
	memset(&(Server.sin_zero),'\0',8);
		
	
	cliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (cliente == INVALID_SOCKET) 
		return SOCKET_INVALIDO;

	
	if ( connect( cliente, (struct sockaddr*) &Server, sizeof(Server) ) == SOCKET_ERROR) 
		return SOCKET_ERROR_CONEXION;

	return cliente;

}

/**************************************************************************************/
SOCKET iniciarServidor( char* IP,  char* Puerto, int conexionesSimultaneas)
{
	struct addrinfo *result = NULL,
                    hints;
	SOCKET ListenSocket;
	int iResult;

	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(IP, Puerto, &hints, &result);
    if ( iResult != 0 ) 
	{
		printf("Getaddrinfo fallido: %ld\n", WSAGetLastError());
        WSACleanup();
        return SOCKET_ERROR_INICIALIZAR;
    }

    /*Creación del Socket*/
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) 
	{
		printf("Socket fallido: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return SOCKET_INVALIDO;
    }

	/*Configuracion de TCP*/
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
	{
		printf("Bind fallido: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
       // WSACleanup();
        return SOCKET_ERROR_INICIALIZAR;
    }

    freeaddrinfo(result);

	/* Preparar escucha */
    iResult = listen(ListenSocket, conexionesSimultaneas);
    if (iResult == SOCKET_ERROR) 
	{
        printf("Listen fallido: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return SOCKET_ERROR_INICIALIZAR;
    }
          
   	return ListenSocket;
   	
}

SOCKET aceptarConexion(SOCKET servidor)
{
	SOCKET socket_aceptado;
	struct sockaddr_in addrinfo;
	unsigned int address_size = sizeof(addrinfo);

	socket_aceptado = accept(servidor,(struct sockaddr*)&addrinfo, &address_size);
	if (socket_aceptado == INVALID_SOCKET) 
	{
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(socket_aceptado);
		WSACleanup();
		return SOCKET_INVALIDO;
	}
	
	printf("SERVIDOR FTP: nueva conexion de %s en socket %d, Puerto: %d\n", inet_ntoa(addrinfo.sin_addr), socket_aceptado,addrinfo.sin_port);
	
	return socket_aceptado;

}

/********************************************************************************************/
void NumToBinPASV (unsigned char * cpMensaje, char* IP,int iPuerto)
{
	unsigned char buffer[2];
	char IPaux[25];
	unsigned char numero[30];

	if (iPuerto>256-1)
	{
		buffer[1]=iPuerto/256;
		iPuerto-=256;
	}
	else
	buffer[1]=0;

	if (iPuerto>0)
	{
		buffer[0]=iPuerto;
		iPuerto-=0;
	}
	else
	buffer[0]=0;

	//"227 passive Mode (10,0,0,4,78,34)\r\n"
	copiar(cpMensaje,"227 passive Mode (");
	copiar(IPaux,IP);
	reemplazar(IPaux,'.',',');
	concatenar(cpMensaje,IPaux);
	concatenar(cpMensaje,",");
	miitoa(numero,(long)buffer[1]);
	concatenar(cpMensaje,numero);
	concatenar(cpMensaje,",");
	miitoa(numero,(long)buffer[0]);
	concatenar(cpMensaje,numero);
	concatenar(cpMensaje,")\r\n");	
}


int enviaArchivo(char* nombreArchivo, SOCKET iSocket, HANDLE * memoriaHilo)
{ 	
	long lAcum, lBytesSend;
	char cpBuf[1000];
	int iLBUFFER; 
	HANDLE hArch; /* Handle del archivo a enviar */
	PLARGE_INTEGER lpFileSize = NULL;
	DWORD lpNumberOfBytesRead;

	lBytesSend=0;

	iLBUFFER = sizeof(cpBuf);
	lpFileSize = (PLARGE_INTEGER)HeapAlloc(*memoriaHilo, HEAP_ZERO_MEMORY, sizeof(LARGE_INTEGER));

	hArch = CreateFileA(nombreArchivo, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	
	if(hArch == INVALID_HANDLE_VALUE)
	{
		if(GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			/* ver el caso de error ****************************************/
			
			if (enviarAlgo(iSocket, cpBuf, lBytesSend)!=1) return -10;
			printf("Archivo no encontrado: %s\n", nombreArchivo);
			HeapFree(*memoriaHilo,0,lpFileSize);
			return 10;
		}
		printf("Problema al intentar abrir el archivo %s\n", nombreArchivo);
		HeapFree(*memoriaHilo,0,lpFileSize);
		return -11; /* Surgio algun problema con el archivo */
	}

	if(GetFileSizeEx(hArch, lpFileSize) == 0)
	{
		printf("Error al obtener el tamanio del archivo\n");
		HeapFree(GetProcessHeap(),0,lpFileSize);
		return -12;
	}

	/* Envio tamanio de archivo en cadena */
	printf("Numero de bytes: %ld\n", *lpFileSize);	
	printf ("Preparando archivo a enviar... Enviando\n");
	
	lAcum = 0;
		
	/* Envia tamaño de archivo */
	if(lAcum < *((long *)lpFileSize))
	{
		lAcum = 0;
		
		while(lAcum <= *(long *)lpFileSize)
		{
				
			if(ReadFile(hArch, (char *)cpBuf, (DWORD)iLBUFFER, &lpNumberOfBytesRead, NULL) == 0)
			{
				printf("Error al leer el archivo\n");
				if (!CloseHandle(hArch)) 
				{
					perror("close()");
					HeapFree(GetProcessHeap(),0,lpFileSize);
					return -3;
				}
				HeapFree(GetProcessHeap(),0,lpFileSize);
				
				return -13;
			}

			if (enviarAlgo(iSocket, cpBuf, lpNumberOfBytesRead)!=1) 
			{
				if (!CloseHandle(hArch)) 
				{
					printf("Error en close");
					HeapFree(GetProcessHeap(),0,lpFileSize);
					return -14;
				}
				return 0;
			}
			lAcum += iLBUFFER;
			
		}
	}
	
	printf("Archivo enviado con exito\n");
	if (!CloseHandle(hArch))
	{
		HeapFree(GetProcessHeap(),0,lpFileSize);
		return -3;
	}
	HeapFree(GetProcessHeap(),0,lpFileSize);
	return 1;
} /* Fin de la funcion EnvioArchivo */



int recibirArchivo(char* nombreArchivo, SOCKET iSocket, HANDLE * memoriaHilo)
{ 	
	long lAcum;
	char cpBuf[2000]; 
	HANDLE hArch; /* Handle del archivo a enviar */
	PLARGE_INTEGER lpFileSize = NULL;
	int bufferEscritos,bufferRecepcion;

	
	hArch = CreateFileA(nombreArchivo, FILE_APPEND_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0); 
	
	lAcum = 0;
	while((bufferRecepcion=recv(iSocket, cpBuf, 1024,0))>0)
	{
		WriteFile(hArch,cpBuf,bufferRecepcion,&bufferEscritos, NULL);
		lAcum=lAcum+bufferEscritos;
	}


	printf("Archivo recibido con exito\n");
CloseHandle(hArch);
return 1;
}


/********************************************************************************************/
void crearCabecera (HEADERIRCIPC *cabecera, unsigned char ucPayloadDescriptor, unsigned int uiPayloadLength)
{
	generarID (cabecera->cpDescriptorID);
	cabecera->ucPayloadDescriptor = ucPayloadDescriptor;
	cabecera->uiPayloadLength = uiPayloadLength;

}

/********************************************************************************************/

void copiarCabecera(HEADERIRCIPC *cabecera, unsigned char ucPayloadDescriptor, unsigned int uiPayloadLength)
{
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








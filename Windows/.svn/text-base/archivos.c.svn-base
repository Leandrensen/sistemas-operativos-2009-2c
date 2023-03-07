#define _CRT_SECURE_NO_DEPRECATE
#define NOMBRE_PROCESO			"PROCESO"
#define MAX_MSG_BUF_SIZE 512

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <winsock.h>
#include "cadenas.h"
#include "archivos.h"
#include "estructuras.h"


int BuscoConfiguracion(sCONFIGARCH* datos, char* palabra)
{
	int cant;
	char nombreProc[16]="Servidor FTP";
	char procesoid[11]={0};
	char tipoLog[7];
	char idThread[11]= {0};
	HANDLE hFile;
	LPTSTR lpMsgBuf;
	LPDWORD BytesEscribir = 0;
	char error[256];
	int iIndice;
	DWORD dwBytesRead = 0;
	int iFin;

	hFile = CreateFileA("./config.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//aca va escribolog si hay error en creat.
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		miitoa(procesoid,(int)GetCurrentProcessId(),&cant);
		procesoid[10]='\0';
		concatenar(tipoLog,"ERROR:");
		tipoLog[6]='\0';

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | 40, NULL, GetLastError(), MAKELANGID(0, SUBLANG_ENGLISH_US), (LPTSTR) &lpMsgBuf, MAX_MSG_BUF_SIZE, NULL);
		iIndice=0;
		while(lpMsgBuf[iIndice]!= '.')
		{	
			error[iIndice]=(char)lpMsgBuf[iIndice];
			iIndice++;
		}
		error[iIndice]='\0';
		idThread[0]='\0';
		escribirLog(BytesEscribir, idThread, procesoid, nombreProc, tipoLog, error, "\r\n");
		return 0;
	}
	
	if (FALSE == ReadFile(hFile, palabra, 1046, &dwBytesRead, NULL)) 
	{
		miitoa(procesoid,(int)GetCurrentProcessId(),&cant);
		procesoid[10]='\0';
		concatenar(tipoLog,"ERROR:");
		tipoLog[6]='\0';

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | 40, NULL, GetLastError(), MAKELANGID(0, SUBLANG_ENGLISH_US), (LPTSTR) &lpMsgBuf, MAX_MSG_BUF_SIZE, NULL);
		iIndice=0;
		while(lpMsgBuf[iIndice]!= '.')
		{	
			error[iIndice]=(char)lpMsgBuf[iIndice];
			iIndice++;
		}
		error[iIndice]='\0';
		idThread[0]='\0';
		escribirLog('E',lpMsgBuf);
		return 0;
	}

	printf("Fichero CONFIG.TXT abierto con exito\n");

	datos->cpIp = MiStrTok(palabra,0,'-',&iFin);
	datos->cpPuerto = MiStrTok(palabra,iFin+1,'-',&iFin);
	datos->cpDirectorio = MiStrTok(palabra,iFin+1,'-',&iFin);
	datos->cpIpVSK = MiStrTok(palabra,iFin+1,'-',&iFin);
	datos->cpPuertoVSK = MiStrTok(palabra,iFin+1,'-',&iFin);

	printf("IP: %s\n", datos->cpIp);
	printf("PUERTO: %s\n", datos->cpPuerto);
	printf("DIRECTORIO: %s\n",datos->cpDirectorio );
	printf("IP VSK: %s\n",datos->cpIpVSK );
	printf("PUERTO VSK: %s\n",datos->cpPuertoVSK );

	if (!CloseHandle(hFile))
	{
		printf("No se puede cerrar el fichero CONFIG.TXT\n");
		return 0;
	}

	printf("Fichero CONFIG.TXT cerrado con exito\n\n");

	return 1;
}




int escribirLog( char cTipoLog, char* cpMensaje)
{
	char cvTexto[500];
	char cvTipoLog[10];
	int iCantidad = 0, iEscritos;
	HANDLE hFile;
	char cFecha[10]="00:00:00";
	time_t tFechaSistema;

	time(&tFechaSistema);		
	strftime(cFecha,9,"%H:%M:%S",localtime(&tFechaSistema));
	cFecha[9]='\0';	 

	hFile = CreateFileA("./Log.txt", FILE_APPEND_DATA, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
    { 
        hFile = CreateFileA("./Log.txt", FILE_APPEND_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);	
		if (hFile == INVALID_HANDLE_VALUE) 
		{     	
			return 0;
		}
	}

	switch (cTipoLog)
	{
		case 'I': case 'i': copiar(cvTipoLog, "INFO"); break;
		case 'E': case 'e': copiar(cvTipoLog, "ERROR"); break;
		case 'W': case 'w': copiar(cvTipoLog, "WARNING"); break;
		case 'D': case 'd': copiar(cvTipoLog, "DEBUG"); break;
	}


	miSprintf(cvTexto,"%s [%d] [%s]: %s: %s.\r\n",cFecha,GetProcessId(GetCurrentProcess()),NOMBRE_PROCESO,cvTipoLog,cpMensaje);
	iCantidad=longitudCadena(cvTexto);
	WriteFile(hFile,cvTexto,(DWORD)iCantidad,&iEscritos,NULL);

	CloseHandle(hFile);
	return 1;
}


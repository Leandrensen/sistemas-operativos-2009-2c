#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include "sendrecv.h"
#include "cadenas.h"
#include "funcionesExpDLL.h"

#pragma comment (lib, "Ws2_32.lib")

typedef struct configDll
{
	char configIPVSK[16];
	char configPUERTOVSK[8];
	char configDirectorio[256];
	HANDLE memoriaHilo;

}CONFIGDLL, *pConfigDll;

typedef struct datosArchivo
{
   SOCKET socket;
   int descriptorArchivo;
   unsigned char cabecera[17];
}DATOSARCHIVO, *pdatosArchivo;

CONFIGDLL datosVSK;
DATOSARCHIVO datosArchivoDescriptor[1000];

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwreason, LPVOID lpReserved)
{
	switch (fdwreason)
	{
		case DLL_PROCESS_ATTACH:
			printf("DLL cargada sobre el proceso con exito \n");
			break;
		case DLL_THREAD_ATTACH:
			printf("DLL cargada sobre el hilo con exito \n");
			break;
		case DLL_PROCESS_DETACH:
			printf("DLL descargada sobre el proceso con exito\n");
			break;
		case DLL_THREAD_DETACH:
			printf("DLL descargada sobre el hilo con exito\n");
			break;
	}

			return TRUE;
		
}

__declspec (dllexport) int vsk_remoteInitialize (void * params)
{
	pConfigDll configuracion = params;

	int i;

	for( i=0; configuracion->configDirectorio[i]!='\0'; datosVSK.configDirectorio[i]=configuracion->configDirectorio[i], i++);

	datosVSK.configDirectorio[i]='\0';

	for( i=0; configuracion->configIPVSK[i]!='\0'; datosVSK.configIPVSK[i]=configuracion->configIPVSK[i], i++);

	datosVSK.configIPVSK[i]='\0';

	for( i=0; configuracion->configPUERTOVSK[i]!='\0'; datosVSK.configPUERTOVSK[i]=configuracion->configPUERTOVSK[i], i++);

	datosVSK.configPUERTOVSK[i]='\0';

	datosVSK.memoriaHilo=configuracion->memoriaHilo;

	/*INICIALIZAR ESTRUCTURAS*/
	inicializarVector(datosArchivoDescriptor);

	return 1;
}

__declspec (dllexport) char * vsk_remoteFList (char * path)
{

	HEADERIRCIPC cabecera;
	SOCKET conectarVSK;
	char * buffer;

	crearCabecera(&cabecera,SYS_LIST,strlen(path));
	printf("el directorio es : %s\n",path);
	conectarVSK = iniciarCliente(datosVSK.configIPVSK,miatoi(datosVSK.configPUERTOVSK));
	enviarH(conectarVSK,cabecera);
	enviarAlgo(conectarVSK,path,strlen(path));
	recibirH(conectarVSK,&cabecera);

	if(cabecera.ucPayloadDescriptor==SYS_LIST_OK)
	{
		buffer = (char*)HeapAlloc(GetProcessHeap(),NULL,cabecera.uiPayloadLength+1);
		recibirAlgo(conectarVSK,buffer,cabecera.uiPayloadLength);
		buffer[cabecera.uiPayloadLength]='\0';
		copiar(path,buffer);
	}
	
	return buffer;

}

__declspec (dllexport) int vsk_remoteOpenFile (const char * fileName, int flags, int mode)
{
	HEADERIRCIPC cabecera;
	SOCKET conectarVSK;
	char archivo [1000];
	char bandera [1000];
	char modo [1000];
	int longTemp,iCabecera,i;
	copiar(archivo,fileName);
	concatenar(archivo,"|");
	longTemp = longitudCadena(archivo);
	switch (flags)
			{
				case 101: archivo[longitudCadena(archivo)]=0xC0; break;
				case 102: archivo[longitudCadena(archivo)]=0xA0; break;
				case 103: archivo[longitudCadena(archivo)]=0xE0; break;
			}
	archivo[longTemp+1]='\0';

	crearCabecera(&cabecera,SYS_OPEN,strlen(archivo));
	conectarVSK = iniciarCliente(datosVSK.configIPVSK,miatoi(datosVSK.configPUERTOVSK));

	i=posicionLibre(datosArchivoDescriptor);

	for(iCabecera=0;iCabecera<16;iCabecera++)
		datosArchivoDescriptor[i].cabecera[iCabecera]=cabecera.cpDescriptorID[iCabecera];
		datosArchivoDescriptor[i].cabecera[16]='\0';
		datosArchivoDescriptor[i].socket=conectarVSK;

	enviarH(conectarVSK,cabecera);
	enviarAlgo(conectarVSK,archivo,strlen(archivo));
	recibirH(conectarVSK,&cabecera);
	printf("contenido de cabecera: %d\n", cabecera.ucPayloadDescriptor);
	if(cabecera.ucPayloadDescriptor==OPEN_FILE_OK)
		{
			if(conectarVSK<0)
				printf("Hubo un problema en la Conexion. FuncionDLL\n");
			return (int)conectarVSK;
		}
	return (int)conectarVSK/*OPEN_FILE_FAILE*/;
}

__declspec (dllexport) int vsk_remoteCloseFile (int fileDescriptor)
{
	HEADERIRCIPC cabecera;
	SOCKET conectarVSK;
	char descriptorArchivo [1000];

	crearCabecera(&cabecera,SYS_CLOSE,0);
	enviarH(fileDescriptor,cabecera);
	recibirH(fileDescriptor,&cabecera);
	if(cabecera.ucPayloadDescriptor==CLOSE_FILE_OK)
		{
		return 1;
		}
		return -1;
	
}


__declspec (dllexport) int vsk_remoteReadFile (int fileDescriptor, char * buffer)

{
	HEADERIRCIPC cabecera;
	SOCKET conectarVSK;

	crearCabecera(&cabecera,SYS_READ,0);
	enviarH(fileDescriptor,cabecera);
	recibirH(fileDescriptor,&cabecera);
	if(cabecera.ucPayloadDescriptor==READ_FILE_OK)
		{
		recibirAlgo(fileDescriptor,buffer,cabecera.uiPayloadLength);
		return cabecera.uiPayloadLength;
		}
		return -1;
	
}



__declspec (dllexport) int vsk_remoteWriteFile (int fileDescriptor, const char * buffer, int size)

{
	HEADERIRCIPC cabecera;
	char descriptorArchivo [1000];
	char tamanioArchivo [1000];
	char archivo [1000];
	int iTotalEnviado=0,iCant;

	if (buscarDescriptor(datosArchivoDescriptor,cabecera) == 1)
	/*for(iCant=0;iCant<16;cabecera.cpDescriptorID[iCant]=datosArchivoDescriptor->cabecera[iCant],iCant++);*/
	cabecera.ucPayloadDescriptor=SYS_WRITE;
	cabecera.uiPayloadLength=size;
	/*crearCabecera(&cabecera,SYS_WRITE,size);*/
	enviarH((SOCKET)fileDescriptor,cabecera);
	enviarAlgo((SOCKET)fileDescriptor,buffer,cabecera.uiPayloadLength);
	recibirH((SOCKET)fileDescriptor,&cabecera);

	if(cabecera.ucPayloadDescriptor==WRITE_FILE_OK)
		{
		return 1;
		}
	return -1;
	
}

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include "sendrecv.h"
#include "archivos.h"
#include "cadenas.h"
#include "comandosClientes.h"
#include "ayuda.h"
#include "directorios.h"
#include "estructuras.h"
#include "archivos.h"
#include "protocoloComunicacionFTP.h"

#pragma comment (lib, "Ws2_32.lib")

unsigned __stdcall hiloDatos (struct info*);
unsigned __stdcall hiloDeDescarga (struct info*);
unsigned __stdcall hiloComando (struct info* );

typedef struct configDll
{
	char configIPVSK[16];
	char configPUERTOVSK[8];
	char configDirectorio[256];
	HANDLE memoriaHilo;
}CONFIGDLL, *pConfigDll;

/*FUNCIONES DE LA DLL*/
typedef int (*vsk_remoteInitialize) (void * params);
typedef char (*vsk_remoteFList) (char * path);
typedef char (*vsk_remoteOpenFile) (const char * fileName, int flags, int mode);
typedef int (*vsk_remoteCloseFile) (int fileDescriptor);
typedef int (*vsk_remoteReadFile )(int fileDescriptor, const char * buffer);
typedef int (*vsk_remoteWriteFile) (int fileDescriptor, const char * buffer, int size);
/*FIN FUNCIONES DE LA DLL*/

/*****************************************************/


sCONFIGARCH datos;

int main()
{
	WSADATA wsaData;
	SOCKET  servidorFTP,socket_aceptado,SocketParaDatos,SocketParaComandos;
	fd_set masterFds;
	fd_set leeFds;
	struct timeval tv;
	int iResult = 0;
	ptrClientes datosControl;
	int i,PuertoAux,nroCli=0;
	HANDLE Comandos,Datos;
	char cpCadenaDestino[10],config[1000];


	
	ayudaComandos();/*Cargamos la ayuda consola*/

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) 
	{
		escribirLog('E', "WSAStartup fallido");
        printf("WSAStartup fallido: %d\n", iResult);
        return 0;
    }

	datosControl = (ptrClientes)HeapAlloc(GetProcessHeap(),NULL,sizeof(INFO)*1000);
	/***************************************************************************************/
	if(!BuscoConfiguracion(&datos,config))
	{
		printf("Error al leer el archivo de configuracion\n");
		return 0;
	}

	/************************** Conexion como servidor *************************************/
	servidorFTP= iniciarServidor(datos.cpIp,datos.cpPuerto, 20);

	switch(servidorFTP)
	{
		case SOCKET_ERROR_INICIALIZAR: escribirLog('E', "Socket error al inicializar"); return 0; break;
		case SOCKET_INVALIDO: escribirLog('E', "Socket Invalido"); return 0; break;
	}

	FD_ZERO(&masterFds);     /* borramos los dos conjuntos */
	FD_ZERO(&leeFds);
	
	/* añadimos servSocket al conjunto masterFds */
    FD_SET(servidorFTP, &masterFds);

	while(1) 
	{
		leeFds = masterFds; 
		tv.tv_sec = 1; 		/* cant segundos */
		tv.tv_usec = 0; 	/* cant microsegundos */  

		if(select(20, &leeFds, NULL, NULL, &tv) == SOCKET_ERROR)
		{
			escribirLog('E', "Error en select"); return 0;
		}
		
		if(FD_ISSET(servidorFTP, &leeFds)) 
      	{ 
			socket_aceptado = aceptarConexion(servidorFTP);

			if (socket_aceptado != SOCKET_INVALIDO)
			{
			
				send(socket_aceptado,"220 Bienvenido FTP Tecladeitor\r\n",(int)strlen("220 Bienvenido FTP Tecladeitor\r\n"),0);
				PuertoAux=miatoi(datos.cpPuerto);
				datosControl[nroCli].iSocketFTP=socket_aceptado; /* cliente FTP aceptado */
				copiar(datosControl[nroCli].IP,datos.cpIp);
				do
				{
					PuertoAux=PuertoAux+1;
					miitoa (cpCadenaDestino, PuertoAux);					
				}
				while((SocketParaComandos=iniciarServidor(datos.cpIp,cpCadenaDestino,1000))==SOCKET_ERROR_INICIALIZAR);
			
				datosControl[nroCli].iPuertoComandos = PuertoAux; /* asigno el puerto de escucha del de comandos */
				datosControl[nroCli].iSocketComandos= SocketParaComandos; /* socket de escucha para el de Comandos*/
			
				do
				{
					PuertoAux=PuertoAux+1;
					miitoa (cpCadenaDestino, PuertoAux);
				}
				while((SocketParaDatos=iniciarServidor(datos.cpIp,cpCadenaDestino,1000))==SOCKET_ERROR_INICIALIZAR);

				datosControl[nroCli].iPuertoDatos = PuertoAux; /* asigno el puerto de escucha del de datos */
				datosControl[nroCli].iSocketDatos = SocketParaDatos; /* socket de escucha para el de Datos*/
				datosControl[nroCli].iSocketFTP = socket_aceptado;
				datosControl[nroCli].sem_mensajes = CreateSemaphore(NULL,0,1,NULL);
				
				Comandos = (HANDLE) _beginthreadex(NULL,1024*1024, &hiloComando, &datosControl[nroCli], 0, &i);	
				Datos	 = (HANDLE) _beginthreadex(NULL,1024*1024, &hiloDatos, &datosControl[nroCli], 0, &i);	
				nroCli++;
			}
			else
				escribirLog('E',"Conexion rechazada");
				
		} /* Fin verificar si hay nuevas conexiones */	
	}

	return 1;

}
unsigned __stdcall hiloComando (struct info* datosControl)
{ 
	fd_set masterFds;
	fd_set leeFds;
	char cvMensaje[1000], aux[1000];
	struct timeval tv;
	int iResult = 0;
	unsigned char primerParametro[1000],segundoParametro[1000];
	HEADERIRCIPC cabecera;
	SOCKET socketHiloDatos;
	char directorioActual [1000]={'\0'};
	

	socketHiloDatos = iniciarCliente(datosControl->IP,datosControl->iPuertoDatos);

	FD_ZERO(&masterFds);     /* borramos los dos conjuntos */
	FD_ZERO(&leeFds);
	FD_SET(datosControl->iSocketFTP, &masterFds);
	FD_SET(datosControl->iSocketComandos, &masterFds);
	

	while(1) 
	{
		leeFds = masterFds; 
		tv.tv_sec = 1; 		/* cant segundos */
		tv.tv_usec = 0; 	/* cant microsegundos */ 
		if(select(20, &leeFds, NULL, NULL, &tv) == SOCKET_ERROR)
		{
			escribirLog('E', "Error en select"); return 0;
		}
		if(FD_ISSET(datosControl->iSocketFTP, &leeFds)) /* aqui para el cliente ftp */
      	{
			for(iResult = 0; iResult < sizeof(cvMensaje);cvMensaje[iResult]='\0',iResult++);
			iResult = recv(datosControl->iSocketFTP, cvMensaje, 1000, 0);/*Recibo el Mensaje del CLIENTE*/
			if (iResult < 1)
			{
				FD_CLR(datosControl->iSocketFTP,&masterFds);
				CloseHandle((HANDLE)datosControl->iSocketFTP);
			}
			else
			{
				cvMensaje[iResult]='\0';
				parserRecepcion (cvMensaje, primerParametro,segundoParametro);

				switch(opcionesUsuario (primerParametro))
				{
					case CMD_LS: 
						cabecera.ucPayloadDescriptor=IPCIRC_LIST;
						cabecera.uiPayloadLength=longitudCadena(directorioActual);
						enviarH(socketHiloDatos,cabecera);
						enviarAlgo(socketHiloDatos,directorioActual,cabecera.uiPayloadLength);
						break;
					case CMD_PWD:/*Muestra el directorio activo en el servidor*/
						if (directorioActual[0]=='\0')
							copiar(directorioActual,"/");
						send(datosControl->iSocketFTP,"257 \"/\" is current directory\r\n",(int)strlen("257 \"/\" is current directory\r\n"),0);
						break;
					case CMD_HELP:
						ayudaComandos();
						break;
					case CMD_NOOP: /*(No Operation) */
						send(datosControl->iSocketFTP,"200 Comando Recibido\r\n",(int)strlen("200 Comando Recibido\r\n"),0);
						break;
					case CMD_PASS: 
						send(datosControl->iSocketFTP,"331 PASS\r\n",(int)strlen("331 PASS\r\n"),0);
						break;
					case CMD_USER:
						send(datosControl->iSocketFTP,"230 Usuario Conectado\r\n",(int)strlen("230 Usuario Conectado\r\n"),0);
						break;
					case CMD_TYPE:
						send(datosControl->iSocketFTP,"200 EBCDIC\r\n",(int)strlen("200 EBCDIC\r\n"),0);
						break;
					case CMD_PASV:/* con PORT Comandos necesarios para iniciar la comunicación de modo pasivo.*/
						NumToBinPASV (cvMensaje,datosControl->IP,datosControl->iPuertoDatos);
						send(datosControl->iSocketFTP,cvMensaje,longitudCadena(cvMensaje),0);
						send(datosControl->iSocketFTP,"125 Connection accepted\r\n",(int)strlen("150 Connection accepted\r\n"),0);
						send(datosControl->iSocketFTP,"226 Transfer OK\r\n",(int)strlen("226 Transfer OK\r\n"),0);
						send(datosControl->iSocketFTP,"200 Listo para recibir comandos\r\n",(int)strlen("200 Listo para recibir comandos\r\n"),0);
						break;
					case CMD_SYST:/* SYST*/
						send(datosControl->iSocketFTP,"215 UNIX\r\n",(int)strlen("215 UNIX\r\n"),0);
						break;
					case CMD_SITE:/* SITE*/
						send(datosControl->iSocketFTP,"211 Informacion del SISTEMA\r\n",(int)strlen("211 Informacion del SISTEMA\r\n"),0);
						break;
					case CMD_OPTS:
						send(datosControl->iSocketFTP,"200 Listo para recibir comandos\r\n",(int)strlen("200 Listo para recibir comandos\r\n"),0);
						break;
					case CMD_SIZE:
						cabecera.ucPayloadDescriptor=IPCIRC_SIZE;
						cabecera.uiPayloadLength=longitudCadena(segundoParametro);
						enviarH(socketHiloDatos,cabecera);
						enviarAlgo(socketHiloDatos,segundoParametro,cabecera.uiPayloadLength);
						recv(socketHiloDatos,cvMensaje,1000,0);
						iResult = recibirH(socketHiloDatos,&cabecera);
						recibirAlgo(socketHiloDatos,cvMensaje,cabecera.uiPayloadLength);
						cvMensaje[cabecera.uiPayloadLength]='\0';
						copiar(aux,"213 ");
						concatenar(aux,cvMensaje);
						concatenar(aux,"\r\n");
						send(datosControl->iSocketFTP,aux,longitudCadena(aux),0);
						break;
					case CMD_RETR:
						cabecera.ucPayloadDescriptor=IPCIRC_RETR;
						cabecera.uiPayloadLength = longitudCadena(segundoParametro);
						enviarH(socketHiloDatos,cabecera);
						enviarAlgo(socketHiloDatos,segundoParametro,cabecera.uiPayloadLength);
						/*recibirH(socketHiloDatos,&cabecera);*/
						WaitForSingleObject(datosControl->sem_mensajes,INFINITE);
						send(datosControl->iSocketFTP,"150 Connection accepted\r\n",(int)strlen("150 Connection accepted\r\n"),0);
						ReleaseSemaphore(datosControl->sem_mensajes,1,NULL);
						WaitForSingleObject(datosControl->sem_mensajes,INFINITE);
						send(datosControl->iSocketFTP,"226 Transfer OK\r\n",(int)strlen("226 Transfer OK\r\n"),0);
						break;
					case CMD_STOR:
						cabecera.ucPayloadDescriptor=IPCIRC_STOR;
						cabecera.uiPayloadLength = longitudCadena(segundoParametro);
						enviarH(socketHiloDatos,cabecera);
						enviarAlgo(socketHiloDatos,segundoParametro,cabecera.uiPayloadLength);
						WaitForSingleObject(datosControl->sem_mensajes,INFINITE);
						send(datosControl->iSocketFTP,"150 Connection accepted\r\n",(int)strlen("150 Connection accepted\r\n"),0);
						ReleaseSemaphore(datosControl->sem_mensajes,1,NULL);
						WaitForSingleObject(datosControl->sem_mensajes,INFINITE);
						send(datosControl->iSocketFTP,"226 Transfer OK\r\n",(int)strlen("226 Transfer OK\r\n"),0);
						break;
					case CMD_DELE:
						cabecera.ucPayloadDescriptor=IPCIRC_DELE;
						cabecera.uiPayloadLength = longitudCadena(segundoParametro);
						enviarH(socketHiloDatos,cabecera);
						enviarAlgo(socketHiloDatos,segundoParametro,cabecera.uiPayloadLength);
						recibirH(socketHiloDatos,&cabecera);
						send(datosControl->iSocketFTP,"226 Transfer OK\r\n",(int)strlen("226 Transfer OK\r\n"),0);
						break;
					case CMD_CD:
						copiar(directorioActual,segundoParametro);
						cabecera.ucPayloadDescriptor=IPCIRC_CD;
						cabecera.uiPayloadLength=longitudCadena(segundoParametro);
						enviarH(socketHiloDatos,cabecera);
						enviarAlgo(socketHiloDatos,segundoParametro,cabecera.uiPayloadLength);
						send(datosControl->iSocketFTP,"250 Directorio cambiado con exito\r\n",(int)strlen("250 Directorio cambiado con exito\r\n"),0);
					break;
					default: 
						send(datosControl->iSocketFTP,"510 COMANDO INCORRECTO INGRESE COMANDO NUEVAMENTE\r\n",(int)strlen("510 COMANDO INCORRECTO INGRESE COMANDO NUEVAMENTE\r\n"),0);
						send(datosControl->iSocketFTP,"200 Listo para recibir comandos\r\n",(int)strlen("200 Listo para recibir comandos\r\n"),0);
				}
			}
		}	

	} 

	return 1;

}


unsigned __stdcall hiloDatos (struct info* datosControl)

{ 
	fd_set masterFds;
	fd_set leeFds;
	struct timeval tv;
	int ciclo = 0, iBandera = 0;
	char archivo[1000];
	char Directorio[100000]=".\\";
	char directorioAux[10000], cvTamanio[20];
	char *tamanioDeDescarga;
	LISTA enlistado;
	struct configDll datosConfig;
	ptrInfoArchivo listaArchivos=NULL,pPila=NULL;
	HANDLE memoriaHilo,DescargaCliente;
	HEADERIRCIPC cabecera;
	char recepcion [1000];
	SOCKET socketComandos,socketClienteFTP;
	ptrClientes datosArchivo;
	int i;

	HINSTANCE hinstLib;

	/*declaraciones de variables para DLL*/
	vsk_remoteInitialize inicializarConfiguracion;
	vsk_remoteFList enlistarArchivos;
	vsk_remoteWriteFile escribirArchivo;
	/**************************************/
	
	socketComandos = aceptarConexion(datosControl->iSocketDatos);
	copiar(Directorio,"");
	
	FD_ZERO(&masterFds);     /* borramos los dos conjuntos */
	FD_ZERO(&leeFds);
	FD_SET(datosControl->iSocketDatos, &masterFds);
	FD_SET(socketComandos, &masterFds);


	memoriaHilo=HeapCreate(HEAP_GENERATE_EXCEPTIONS,1024*1024,0);

	/******************COMIENZO inicializarConfiguracion*/

	hinstLib = LoadLibraryA ("funcionesExpDLL.dll"); /*Cargamos la DLL*/
	copiar(datosConfig.configDirectorio,datos.cpDirectorio);
	copiar(datosConfig.configIPVSK,datos.cpIpVSK);
	copiar(datosConfig.configPUERTOVSK,datos.cpPuertoVSK);
	datosConfig.memoriaHilo=memoriaHilo;	
	inicializarConfiguracion = (vsk_remoteInitialize) GetProcAddress(hinstLib, "vsk_remoteInitialize");
	inicializarConfiguracion (&datosConfig);

	/******************FIN inicializarConfiguracion*/

	while(1) 
	{
		leeFds = masterFds; 
		tv.tv_sec = 1; 		/* cant segundos */
		tv.tv_usec = 0; 	/* cant microsegundos */  

		if(select(20, &leeFds, NULL, NULL, &tv) == SOCKET_ERROR)
		{
			escribirLog('E', "Error en select"); return 0;
		}
		if (FD_ISSET(datosControl->iSocketDatos, &leeFds))
		{
			socketClienteFTP = aceptarConexion(datosControl->iSocketDatos);
			iBandera = 1;
		}
		if(FD_ISSET(socketComandos, &leeFds)) 
      	{
			recibirH(socketComandos,&cabecera);

			switch(cabecera.ucPayloadDescriptor)
			{
			case IPCIRC_LIST:
				if(listaArchivos!=NULL)
				{
					liberarLista(&listaArchivos,&enlistado);
				}
				if (iBandera == 0)
				{
					socketClienteFTP = aceptarConexion(datosControl->iSocketDatos);
					iBandera = 1;
				}
				copiar(recepcion,"");
				recv(socketComandos,recepcion,cabecera.uiPayloadLength,0);
				recepcion[cabecera.uiPayloadLength]='\0';
				copiar(Directorio,"");
				concatenar(Directorio,recepcion);
				copiar(directorioAux,Directorio);

				enlistarArchivos = (vsk_remoteFList) GetProcAddress(hinstLib, "vsk_remoteFList");
				enlistarArchivos (&Directorio);
				if(strlen(Directorio)!=0)
				{
 					crearListasArchivos(Directorio,&enlistado,&listaArchivos);
					enviarAlgo(socketClienteFTP,enlistado.enlistado,(int)strlen(enlistado.enlistado));
				}
				else	
				{
					enviarAlgo(socketClienteFTP,"",(int)longitudCadena(""));
				}
					CloseHandle((HANDLE)socketClienteFTP);
					iBandera = 0;
				CloseHandle((HANDLE)socketClienteFTP);
				iBandera = 0;
				
				break;
			case IPCIRC_SIZE:
				recv(socketComandos,recepcion,cabecera.uiPayloadLength,0);
				recepcion[cabecera.uiPayloadLength]='\0';
				tamanioDeDescarga = buscaArchivo(&listaArchivos,recepcion,cvTamanio);
				cabecera.uiPayloadLength = longitudCadena(tamanioDeDescarga)-1;
				enviarH(socketComandos,cabecera);
				enviarAlgo(socketComandos,cvTamanio,cabecera.uiPayloadLength);
				break;
			case IPCIRC_RETR:
				
				recv(socketComandos,recepcion,cabecera.uiPayloadLength,0);
				recepcion[cabecera.uiPayloadLength]='\0';
				if (iBandera == 0)
				{
					socketClienteFTP = aceptarConexion(datosControl->iSocketDatos);
					iBandera = 1;
				}
				cabecera.ucPayloadDescriptor=IPCIRC_150;
				ReleaseSemaphore(datosControl->sem_mensajes,1,NULL);
				WaitForSingleObject(datosControl->sem_mensajes,INFINITE);
				copiar(archivo,directorioAux);
				/*concatenar(archivo,"\\");*/
				concatenar(archivo,recepcion);
				/*enviaArchivo(archivo,socketClienteFTP,&memoriaHilo);*/
				copiar(datosArchivo->cArchivo,archivo);
				copiar(datosArchivo->cTamanio,tamanioDeDescarga);
				datosArchivo->hinstLib=hinstLib;
				datosArchivo->iSocketDatos=socketClienteFTP;
				DescargaCliente	 = (HANDLE) _beginthreadex(NULL,1024*1024, &hiloDeDescarga, &datosArchivo, 0, &i);
				WaitForSingleObject(DescargaCliente, INFINITE);
				/*envioArchivoDLL(socketClienteFTP,archivo, miatoi(tamanioDeDescarga), hinstLib);*/
				cabecera.ucPayloadDescriptor=IPCIRC_226;
				ReleaseSemaphore(datosControl->sem_mensajes,1,NULL);
				CloseHandle((HANDLE) DescargaCliente);
				CloseHandle((HANDLE)socketClienteFTP);
				iBandera = 0;
				break;	
			case IPCIRC_STOR:
				recv(socketComandos,recepcion,cabecera.uiPayloadLength,0);
				recepcion[cabecera.uiPayloadLength]='\0';
				if (iBandera == 0)
				{
					socketClienteFTP = aceptarConexion(datosControl->iSocketDatos);
					iBandera = 1;
				}
				cabecera.ucPayloadDescriptor=IPCIRC_150;
				ReleaseSemaphore(datosControl->sem_mensajes,1,NULL);
				WaitForSingleObject(datosControl->sem_mensajes,INFINITE);
				copiar(archivo,Directorio);
				/*concatenar(archivo,"\\");*/
				concatenar(archivo,recepcion);
				subeArchivoDLL(socketClienteFTP,archivo, hinstLib);
				/*recibirArchivo(archivo,socketClienteFTP,&memoriaHilo);*/
				ReleaseSemaphore(datosControl->sem_mensajes,1,NULL);
				CloseHandle((HANDLE)socketClienteFTP);
				iBandera = 0;
				break;	
			case IPCIRC_DELE:
				recv(socketComandos,recepcion,cabecera.uiPayloadLength,0);
				recepcion[cabecera.uiPayloadLength]='\0';
				copiar(archivo,Directorio);
				concatenar(archivo,"\\");
				concatenar(archivo,recepcion);
				DeleteFileA(archivo);
				enviarH(socketComandos,cabecera);
				break;
			case IPCIRC_CD:
				copiar(Directorio,"");
				recibirAlgo(socketComandos,recepcion,cabecera.uiPayloadLength);
				recepcion[cabecera.uiPayloadLength]='\0';
				concatenar(Directorio,recepcion);
				enviarH(socketComandos,cabecera);
				break;
			default:
				printf("Protocolo Erroneo\n");
				break;
			}
		}
		} /* Fin verificar si hay nuevas conexiones */
	FreeLibrary(hinstLib); /*Liberamos La DLL*/
	return 1;

}

unsigned __stdcall hiloDeDescarga (struct info* datosArchivo)
{

#include "sendrecv.h" 

	envioArchivoDLL(datosArchivo->iSocketDatos,datosArchivo->cArchivo, miatoi(datosArchivo->cTamanio), datosArchivo->hinstLib);

		return 1;
}




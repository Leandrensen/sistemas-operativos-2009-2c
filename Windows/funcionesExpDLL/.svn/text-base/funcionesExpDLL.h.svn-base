#ifndef _FUNCIONESEXPDLL_H
#define _FUNCIONESEXPDLL_H

/*Procotolo Comunicacion entre FTPCliente - ServidorFTP - VSK*/


#define SYS_OPEN					0x30
#define SYS_CLOSE					0x31
#define SYS_LIST					0x32
#define SYS_READ					0x33
#define SYS_WRITE					0x34

#define SYS_LIST_OK					0x35
#define OPEN_FILE_OK				0x36
#define OPEN_FILE_FAILE				0x37
#define CLOSE_FILE_OK				0x38
#define READ_FILE_OK				0x39
#define WRITE_FILE_OK				0x40


/*typedef struct configDll
{
	char configIPVSK[16];
	char configPUERTOVSK[8];
	char configDirectorio[256];
	HANDLE memoriaHilo;

}CONFIGDLL, *pConfigDll;*/

__declspec (dllexport) int vsk_remoteInitialize (void * params);

__declspec (dllexport) char * vsk_remoteFList (char * path);

__declspec (dllexport) int vsk_remoteOpenFile (const char * fileName, int flags, int mode);

__declspec (dllexport) int vsk_remoteCloseFile (int fileDescriptor);

__declspec (dllexport) int vsk_remoteReadFile (int fileDescriptor, const char * buffer);

__declspec (dllexport) int vsk_remoteWriteFile (int fileDescriptor, const char * buffer, int size);



#endif





















/*Con esta función se inicializa la biblioteca pasando la información de donde se encuentra el Kernel (dirección
IP y puerto de escucha) así como algún dato más que se crea conveniente.*/




/*Esta función tiene como objetivo ejecutar un comando del kernel para listar el directorio path en forma
remota estableciendo una conexión por cada llamado y la libera al retornar la respuesta. La información de
respuesta es almacenada en un buffer temporal el cual el usuario que lo utilice será el responsable de liberar
la memoria reservada para el mismo.*/


/*El parámetro fileName indica el nombre del archivo que se va a abrir según el modo de apertura indicado
por la variable flags. El parámetro mode indica el derecho de acceso sobre el archivo. El valor de retorno es
el descriptor de entrada/salida correspondiente al archivo abierto. Comandos ftp que lo ejecutan: put, get,
delete.
Los flags son los siguientes:
VSK_REMOTE_READ Apertura del archivo para solo lectura.
VSK_REMOTE_WRITE Apertura del archivo para solo escritura.
VSK_REMOTE_DELETE Elimina el archivo si existe.
Establece una nueva conexión por sockets al VSK que se mantendrá abierta mientras viva el descriptor del
archivo.*/


/*El parámetro fileDescriptor corresponde al descriptor devuelto por la llamada anterior. Retorna error si el
descriptor no es válido. Además deberá cerrar la conexión asociada al descriptor establecida al abrir el
archivo. Comandos que lo ejecutan: put, get, delete.*/


/*Esta función provoca la lectura de hasta 1024 bytes del archivo especificado por fileDescriptor. Los bytes son
volcados a una memoria intermedia referenciada por buffer. La función devuelve la cantidad de bytes
efectivamente leídos, cero en caso de no haber más datos para leer y un valor negativo en caso de error. La
comunicación se realiza utilizando la conexión asociada al descriptor del archivo. Comandos ftp que lo
ejecutan: get.*/


/*Esta función escribe los datos en el descriptor fileDescriptor que se encuentran en la memoria referenciada
por buffer, cuya longitud en bytes se pasa en la variable size. La comunicación se realiza utilizando la
conexión asociada al descriptor del archivo. Comandos ftp que lo ejecutan: put.*/
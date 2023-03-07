#ifndef _SENDRECV_H
#define	_SENDRECV_H


#include <winsock2.h>

/*******************	CONSTANTES		*************************/

#define		SOCKET_ERROR_DESCONEXION		0
#define		SOCKET_ERROR_SENDRECV			-1
#define		SOCKET_ERROR_INICIALIZAR		(SOCKET)(-4)
#define		SOCKET_ERROR_CONEXION			(SOCKET)(-3)
#define		SOCKET_INVALIDO					(SOCKET)(-2)
#define		SOCKET_OK_SENDRECV				1

/*******************	ESTRUCTURAS		*************************/

typedef struct header_ipcirc /* Estructura para el manejo del header IPC/IRC */
{
    unsigned char cpDescriptorID[16];
    unsigned char ucPayloadDescriptor;
    unsigned int uiPayloadLength;
} HEADERIRCIPC;


/*******************	CABECERAS		*************************/

int enviarH(SOCKET sock, HEADERIRCIPC cabecera);
int recibirH(SOCKET sock, HEADERIRCIPC * cabecera);
int recibirAlgo(SOCKET sock, unsigned char * buffer, int iCuanto);
int enviarAlgo(SOCKET sock, unsigned char * buffer, int iCuanto);
unsigned int BinToNum (unsigned char * buffer, int desde);
void NumToBin (unsigned char * buffer, int total, int desde);
SOCKET iniciarCliente(char* IPdestino, int PUERTOdestino);
SOCKET iniciarServidor(char* IP, char* Puerto, int conexionesSimultaneas);
SOCKET aceptarConexion(SOCKET servidor);
void NumToBinPASV (unsigned char * cpMensaje, char* IP,int iPuerto);
int enviaArchivo(char* nombreArchivo, SOCKET iSocket, HANDLE * memoriaHilo);
int recibirArchivo(char* nombreArchivo, SOCKET iSocket, HANDLE * memoriaHilo);
int envioArchivoDLL(SOCKET iSocket, char* nombreArchivo, int iTamanio, HINSTANCE hinstLib);
int subeArchivoDLL(SOCKET iSocket,char* nombreArchivo, HINSTANCE hinstLib);
#endif	/* _SENDRECV_H */


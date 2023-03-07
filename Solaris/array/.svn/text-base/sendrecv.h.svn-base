#ifndef _SENDRECV_H
#define _SENDRECV_H

/**********                   CONSTANTES                 ********************/

#define	SOCKET_ERROR_DESCONEXION	-1
#define	SOCKET_ERROR_SENDRECV	-2
#define	SOCKET_ERROR_CONEXION	-3
#define	SOCKET_ERROR_INICIALIZAR	-4
#define	SOCKET_OK_SENDRECV		1
#define	SOCKET_INVALIDO		-5


/**********                   ESTRUCTURAS                    *****************/

typedef struct header_ipcirc /* Estructura para el manejo del header IPC/IRC */
{
    unsigned char cpDescriptorID[16];
    unsigned char ucPayloadDescriptor;
    unsigned int uiPayloadLength;
} HEADERIRCIPC;


/***********                 CABECERAS                  **********************/

int enviarH(int iSocket, HEADERIRCIPC cabecera);
int recibirH(int iSocket, HEADERIRCIPC* cabecera);
int recibirAlgo(int iSocket, unsigned char* buffer, int iCuanto);
int enviarAlgo(int iSocket, unsigned char* buffer, int iCuanto);
int iniciarServidor (char* IPlocal, int PUERTOLocal, int conexionesSimultaneas);
int iniciarCliente(char* IPdestino, int PUERTODestino);
unsigned int BinToNum (unsigned char *buf, int iDesde);
void NumToBin (unsigned char *ucvBuf, int iTotal, int iDesde);
int aceptarConexion (int iSocket);
void crearCabecera (HEADERIRCIPC *cabecera, unsigned char ucPayloadDescriptor, unsigned int uiPayloadLength);
void generarID(unsigned char* ucpID);
int generarSocketHilo(char* IPdestino, int iPuertoTemporal);
int compararCabecera(unsigned char* cpDescriptorID1, unsigned char* cpDescriptorID2);
void copiarCabecera(unsigned char* cpDescriptorID1, unsigned char* cpDescriptorID2);


#endif

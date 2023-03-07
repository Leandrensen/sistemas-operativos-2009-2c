#ifndef _PROTOCOLO_H
#define _PROTOCOLO_H

/* PROTOCOLOS DE CONEXION ***************************/
#define HANDSHAKE_OK		0x98
#define HANDSHAKE_FAIL	0x99

/* RECEPCION DE CONEXIONES **************************/
#define ARRAY			0x91
#define DISCO			0x91
#define SFS			0x92
#define FTP			0x93
#define CLIENTE		0x95
#define ERROR			0x97

/* SYSTEM CALLS DE FTP ********************/
#define FTP_FLIST_OPEN	0x51
#define FTP_FLIST_CLOSE	0x52

/* ESTRUCTURA DE CONEXIONES *****************************/
typedef struct infoConexiones
{
	int iTipo;
	int iSocket;
	unsigned char ucvNombre[100];
	char cMontado; /* solo para tipo ARRAY (igual en disco) 1 si, 0 no */
	/* FALTA IDENTIFICADOR */
}INFOCONEXIONES;

typedef struct conexiones
{
	INFOCONEXIONES conexion;
	void* sgte;
}CONEXIONES, *pConexiones;

/* CABECERAS FUNCIONES ***********************************/

int meterConexion(pConexiones* pila, INFOCONEXIONES info);
int quitarConexionPorSocket(pConexiones* pila, int unSocket, INFOCONEXIONES* info);
pConexiones buscarConexionPorSocket (pConexiones* pila, int unSocket);
pConexiones buscarConexionPorTipo (pConexiones* pila, int iTipo);
pConexiones buscarConexionPorDA (pConexiones* pila, unsigned char* ucpNombreDA);

#endif

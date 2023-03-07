#ifndef _CONSOLA_H
#define _CONSOLA_H

#include "sendrecv.h"
/* CONSTANTES *************************************/
#define CONSOLA				0x00				
#define COMANDO_GET_SECTORES		0x60
#define COMANDO_PUT_SECTORES		0x61
#define COMANDO_GET_CHS			0x62
#define COMANDO_ELIMINAR			0x63
#define COMANDO_AGREGAR			0x64
#define COMANDO_SERVICIO			0x65
#define COMANDO_AYUDA			0x66
#define COMANDO_SALIR			0x67
#define COMANDO_LIMPIAR			0x68
#define COMANDO_MONTAR			0x69
#define COMANDO_DESMONTAR			0x70
#define COMANDO_FORMATEAR			0x71
#define COMANDO_MD5SUM			0x72
#define COMANDO_GET_SECTORES_RTA		0x73
#define COMANDO_PUT_SECTORES_RTA		0x74
#define COMANDO_GET_CHS_RTA			0x75
#define CANTIDAD_COMANDOS			13
#define BUFFER				2000

/* CASO ESPECIAL DE GETCHS*/
#define GETCHS			0x14

typedef struct comandos
{
	char cvNombre[30];
	int iCantidadArgumentos;
	char cvDescripcion[100];
	int iCodigo;
}LISTACOMANDOS;

/* CABECERA *********************************************/
char consola(HEADERIRCIPC * cabecera, unsigned char* ucvMensaje);
void ingresoDatos(char* vComando);
void mensajeError (char * cpComando, int iValorError);
void inicializarComandos (LISTACOMANDOS *misComandos);
int analizarComando(char * cvComando, LISTACOMANDOS * misComandos, char* arg1, char* arg2, char* arg3, char* arg4);
int comandoSalir(int iSocket);
void ayuda(LISTACOMANDOS * misComandos);

#endif

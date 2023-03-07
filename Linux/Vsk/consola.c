#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consola.h"
#include "cadenas.h"
#include "sendrecv.h"
#include "protocolo.h"

char consola(HEADERIRCIPC * cabecera, unsigned char* ucvMensaje)
{
	char vComando[BUFFER];
	LISTACOMANDOS cvComandos[CANTIDAD_COMANDOS];
	int i;
	int iValorError;
	int iArg1, iArg2;
	char cvArg1[100]="";
	char cvArg2[512]="";
	char cvArg3[100]="";
	char cvArg4[512]="";


	inicializarComandos (cvComandos);
		ingresoDatos(vComando);
		switch(iValorError = analizarComando(vComando, cvComandos, cvArg1,cvArg2, cvArg3, cvArg4))
		{
			case COMANDO_AYUDA: /* todos en consola.h */
				printf("ayuda\n"); 
				ayuda(cvComandos);
				break;

			case COMANDO_GET_SECTORES:
				iArg1 = miatoi(cvArg1);
				iArg2 = miatoi(cvArg2);
				cabecera->uiPayloadLength = 8;
				NumToBin (ucvMensaje, iArg1, 0); 
				NumToBin (ucvMensaje, iArg2, 4);
				cabecera->ucPayloadDescriptor = COMANDO_GET_SECTORES;
				/*leerSectores();*/
				break;

			case COMANDO_PUT_SECTORES:
				iArg1 = miatoi(cvArg1);
				iArg2 = miatoi(cvArg3);
				cabecera->ucPayloadDescriptor = COMANDO_PUT_SECTORES;
				cabecera->uiPayloadLength = 1032;
				NumToBin (ucvMensaje, iArg1, 0); 
				for (i = 0; i < 512; ucvMensaje[i+4] = cvArg2[i],ucvMensaje[i+520] = cvArg4[i],i++);
				NumToBin (ucvMensaje, iArg2, 516);
				printf("Enviando\n");

				/*escribirSectores();*/
				break;

			case COMANDO_GET_CHS:
				cabecera->ucPayloadDescriptor = COMANDO_GET_CHS;
				cabecera->uiPayloadLength = 0;
				/*pedirInformacion();*/
				break;

			case COMANDO_ELIMINAR:
				printf("eliminar\n"); 
				/*pedirInformacion();*/
				break;
				
			case COMANDO_MONTAR:
				printf("Montando\n"); 
				cabecera->ucPayloadDescriptor = COMANDO_MONTAR;
				cabecera->uiPayloadLength = strlen(cvArg1);
				copiar ((char*)ucvMensaje, cvArg1);
				break;
				
			case COMANDO_DESMONTAR:
				printf("Desmontando\n"); 
				cabecera->ucPayloadDescriptor = COMANDO_DESMONTAR;
				cabecera->uiPayloadLength = strlen(cvArg1);
				copiar ((char*)ucvMensaje, cvArg1);
				break;
				
			case COMANDO_FORMATEAR:
				printf("Montando\n"); 
				cabecera->ucPayloadDescriptor = COMANDO_FORMATEAR;
				cabecera->uiPayloadLength = strlen(cvArg1);
				copiar ((char*)ucvMensaje, cvArg1);
				break;

			case COMANDO_SERVICIO:
				printf("servicio\n"); 
				/*pedirInformacion();*/
				break;

			case COMANDO_LIMPIAR:
				system("clear");
				break;

			case COMANDO_SALIR:
				printf("Finalizando...\n");
				cabecera->ucPayloadDescriptor = COMANDO_SALIR;
				break;
			
			default:
				cabecera->ucPayloadDescriptor = 0;
				mensajeError(vComando, iValorError);

		}
		
		return cabecera->ucPayloadDescriptor;
}


void ingresoDatos (char* entrada)
{
	printf("vsk: ");
	fgets(entrada, BUFFER, stdin);
}

void mensajeError (char * cpComando, int iValorError)
{
	cpComando[longitudCadena(cpComando)]='\0';
	switch (iValorError)
	{
		case -1: printf("El comando: %s, no es un comando valido para la consola\n", cpComando); break;
		case -2: printf("La cantidad de argumentos para el comando no es valido.\n"); break;
	}
	
}

void inicializarComandos (LISTACOMANDOS *misComandos)
{
	copiar (misComandos[0].cvNombre, "getsectores");
	copiar (misComandos[0].cvDescripcion, "Lee 2 sectores del disco\nArgumentos: Sector1 Sector2");
	misComandos[0].iCantidadArgumentos = 2;
	misComandos[0].iCodigo = COMANDO_GET_SECTORES;
	copiar (misComandos[1].cvNombre, "putsectores");
	copiar (misComandos[1].cvDescripcion, "Escribe 2 sectores del disco\nArgumentos: Sector1 Contenido1 Sector2 Contenido2");
	misComandos[1].iCantidadArgumentos = 4;
	misComandos[1].iCodigo = COMANDO_PUT_SECTORES;
	copiar (misComandos[2].cvNombre, "getchs");
	copiar (misComandos[2].cvDescripcion, "Obtinene informacion del disco");
	misComandos[2].iCantidadArgumentos = 0;
	misComandos[2].iCodigo = COMANDO_GET_CHS;
	copiar (misComandos[3].cvNombre, "eliminar");
	copiar (misComandos[3].cvDescripcion, "Desmonta un DA");
	misComandos[3].iCantidadArgumentos = 1;
	misComandos[3].iCodigo = COMANDO_ELIMINAR;
	copiar (misComandos[4].cvNombre, "servicio");
	copiar (misComandos[4].cvDescripcion, "Activar o desactivar servicio");
	misComandos[4].iCantidadArgumentos = 1;
	misComandos[4].iCodigo = COMANDO_SERVICIO;
	copiar (misComandos[5].cvNombre, "agregar");
	copiar (misComandos[5].cvDescripcion, "Monta un DA conectado");
	misComandos[5].iCantidadArgumentos = 1;
	misComandos[5].iCodigo = COMANDO_AGREGAR;
	copiar (misComandos[6].cvNombre, "ayuda");
	copiar (misComandos[6].cvDescripcion, "Informacion de comandos");
	misComandos[6].iCantidadArgumentos = 0;
	misComandos[6].iCodigo = COMANDO_AYUDA;
	copiar (misComandos[7].cvNombre, "salir");
	copiar (misComandos[7].cvDescripcion, "Sale de la consola");
	misComandos[7].iCantidadArgumentos = 0;
	misComandos[7].iCodigo = COMANDO_SALIR;
	copiar (misComandos[8].cvNombre, "limpiar");
	copiar (misComandos[8].cvDescripcion, "Limpiar la pantalla");
	misComandos[8].iCantidadArgumentos = 0;
	misComandos[8].iCodigo = COMANDO_LIMPIAR;
	copiar (misComandos[9].cvNombre, "montar");
	copiar (misComandos[9].cvDescripcion, "Montar un DA");
	misComandos[9].iCantidadArgumentos = 1;
	misComandos[9].iCodigo = COMANDO_MONTAR;
	copiar (misComandos[10].cvNombre, "desmontar");
	copiar (misComandos[10].cvDescripcion, "Desmontar un DA");
	misComandos[10].iCantidadArgumentos = 1;
	misComandos[10].iCodigo = COMANDO_DESMONTAR;
	copiar (misComandos[11].cvNombre, "formatear");
	copiar (misComandos[11].cvDescripcion, "Formatear un DA");
	misComandos[11].iCantidadArgumentos = 1;
	misComandos[11].iCodigo = COMANDO_FORMATEAR;
	copiar (misComandos[12].cvNombre, "md5sum");
	copiar (misComandos[12].cvDescripcion, "Obtener el md5sum de un archivo");
	misComandos[12].iCantidadArgumentos = 3;
	misComandos[12].iCodigo = COMANDO_MD5SUM;

}

/* 	Nombre de la Función: analizarComando
	Versión: 0.5
	Autor/es: Emanuel Villalva
	Fechas de creación: 20/09/2009
	modificación:  28/09/2009
	Objetivo de la Función: deretminar el comando con sus argumentos
	Entrada:	cvComandos: cadena que contiene el comando
			misComandos: vector de comandos
	Salida:		retorna el codigo del comando ingresado
*/

int analizarComando(char * cvComandos, LISTACOMANDOS * misComandos, char* cvArg1, char* cvArg2, char* cvArg3, char* cvArg4)
{
	int encontrado = 0, j = 0;
	char unComando[100];
	char* pArgumento;

	pArgumento = strtok (cvComandos, " \n");
	copiar (unComando, cvComandos);

	while ((j < CANTIDAD_COMANDOS) && (encontrado ==0))
	{
		if (comparar(unComando, misComandos[j].cvNombre) == 0)
			encontrado = 1;
		else j++;
	}

	if (encontrado == 1)
	{
		if (misComandos[j].iCantidadArgumentos == 0) 
			return misComandos[j].iCodigo;
		else /* para 1 argumento */
		if (misComandos[j].iCantidadArgumentos == 1)
		{
			pArgumento = strtok (NULL, " \n");
			if (pArgumento != NULL)
			{
				copiar (cvArg1, pArgumento );
				return misComandos[j].iCodigo;
			}
			else 
				return -2;
		}
		else if (misComandos[j].iCantidadArgumentos == 2) /* para 2 argumentos */
		{

			pArgumento  = strtok (NULL, " \n");
			if (pArgumento != NULL)
			{
				copiar (cvArg1, pArgumento );
				pArgumento  = strtok (NULL, " \n");
				if (pArgumento != NULL)
				{
					copiar (cvArg2, pArgumento );
					return misComandos[j].iCodigo;
				}

			}
			return -2;

		}
		else if (misComandos[j].iCantidadArgumentos == 4)
		{
			pArgumento  = strtok (NULL, " \n");
			if (pArgumento != NULL)
			{
				copiar (cvArg1, pArgumento );
				pArgumento  = strtok (NULL, " \n");
				if (pArgumento != NULL)
				{
					copiar (cvArg2, pArgumento );
					pArgumento  = strtok (NULL, " \n");
					if (pArgumento != NULL)
					{
						copiar (cvArg3, pArgumento );
						pArgumento  = strtok (NULL, " \n");
						if (pArgumento !=NULL)
						{
							copiar (cvArg4, pArgumento );
							return misComandos[j].iCodigo;

						}
					}
				}

			}
			return -2;

			
		}
	}
	
	return -1;
}

void ayuda(LISTACOMANDOS * misComandos)
{
	int i;
	for (i=0; i < CANTIDAD_COMANDOS; i++)
		if (i != 3)
			printf ("%s: %s\n", misComandos[i].cvNombre, misComandos[i].cvDescripcion);
}

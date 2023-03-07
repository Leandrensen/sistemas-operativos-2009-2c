#include <stdio.h>
#include <stdarg.h>
#include "cadenas.h"
#include "sendrecv.h"
#include <winsock2.h>
#include <ctype.h> //<---aqui se encuentran las funciones toupper y tolower

typedef struct datosArchivo
{
   SOCKET socket;
   int descriptorArchivo;
   unsigned char cabecera[17];
}DATOSARCHIVO, *pdatosArchivo;

 /***********************************************************************************************/
int comparar (char * cpCad1, char* cpCad2)
{
	int i;

	for( i=0 ; (cpCad1[i] != '\0') && (cpCad2[i] != '\0') && (cpCad1[i]==cpCad2[i]) ; i++);

	if ((cpCad1[i]=='\0') && (cpCad2[i]=='\0')) return 0;
	if ((cpCad1[i]>cpCad2[i])) return 1;
	if ((cpCad1[i]<cpCad2[i])) return -1;

	return -2;
}

int compararU (unsigned char * cpCad1, unsigned char* cpCad2)
{
	int i;

	for( i=0 ; (cpCad1[i] != '\0') && (cpCad2[i] != '\0') && (cpCad1[i]==cpCad2[i]) ; i++);

	if ((cpCad1[i]=='\0') && (cpCad2[i]=='\0')) return 0;
	if ((cpCad1[i]>cpCad2[i])) return 1;
	if ((cpCad1[i]<cpCad2[i])) return 1;

	return -2;
}


 /***********************************************************************************************/
void concatenar (char * cpCad1, char* cpCad2)
{
	int i,j;

	for( i=0; cpCad1[i]!='\0'; i++);

	for( j=0; (cpCad2[j]!='\0') ; cpCad1[i]=cpCad2[j], i++,j++);

	cpCad1[i]='\0';
}

void concatenarU (unsigned char * cpCad1, unsigned char* cpCad2)
{
	int i,j;

	for( i=0; cpCad1[i]!='\0'; i++);

	for( j=0; (cpCad2[j]!='\0') ; cpCad1[i]=cpCad2[j], i++,j++);

	cpCad1[i]='\0';
}


 /***********************************************************************************************/
void copiar(char * cpCad1, char* cpCad2)
{
	int i;

	for( i=0; cpCad2[i]!='\0'; cpCad1[i]=cpCad2[i], i++);

	cpCad1[i]='\0';
}

void copiarU(unsigned char * cpCad1, unsigned char* cpCad2)
{
	int i;

	for( i=0; cpCad2[i]!='\0'; cpCad1[i]=cpCad2[i], i++);

	cpCad1[i]='\0';
}


 /***********************************************************************************************/
int longitudCadena(char* cpCad)
{
	int i;
	for(i=0;cpCad[i]!='\0'; i++);

	return i;
}

int longitudCadenaU(unsigned char* ucpCad)
{
	int i;
	for(i=0;ucpCad[i]!='\0'; i++);

	return i;
}


 /***********************************************************************************************/

void miitoa (char* cpCadenaDestino, long lNumero)
{
	int i=0, j, pivote;

	if (lNumero==0)
	{
		cpCadenaDestino[0]='0';
		cpCadenaDestino[1]='\0';
		return;
	}
	
	while (lNumero!=0)
	{
		cpCadenaDestino[i] = (char)(lNumero%10 + '0');
		lNumero = lNumero / 10;
		i++;
	}
	cpCadenaDestino[i]='\0';

	for (j=0; j< i/2; j++){
		pivote= cpCadenaDestino[j];
		cpCadenaDestino[j]= cpCadenaDestino[i-j-1];
		cpCadenaDestino[i-j-1]=pivote;
	}
}

 /***********************************************************************************************/

int miatoi (char* cpCadena)
{	
	int i, num = 0;
	
	for (i=0; (cpCadena[i]>='0') && (cpCadena[i]<='9') ; i++)
		num = num*10 + (cpCadena[i] - '0');
	
	return num;
}

 /***********************************************************************************************/

int contarDigitos (long numero)
{
	char valor[40];
	miitoa (valor, numero);
	return longitudCadena (valor);
}

 /***********************************************************************************************/

void reemplazar (char* cpCadena, char cCaracterBuscado, char cCaracterDeReemplazo)
{
	int i;

	for (i=0; cpCadena[i]!='\0'; i++)
		if (cpCadena[i] == cCaracterBuscado) cpCadena[i]=cCaracterDeReemplazo;

}

/***********************************************************************************************/

void miSprintf(char* cadDestino, char* formato,...)
{
	int posDest=0, posFormato=0, d;
	va_list ap;
	char c, *s;

	va_start(ap, formato);

	while ((formato[posFormato])!='\0')
	{
		for(; (formato[posFormato]!='\0') && (formato[posFormato]!='%') ; cadDestino[posDest]=formato[posFormato] , posDest++, posFormato++);

		if (formato[posFormato]!='\0')
		{
			posFormato++;
			switch(formato[posFormato])
			{
				case 's':           /* string */
					s = va_arg(ap, char *);
					copiar(&cadDestino[posDest],s);
					posDest += longitudCadena(s);
					posFormato++;
					break;
				case 'd':           /* int */
					d = va_arg(ap, int);
					miitoa(&cadDestino[posDest],d);
					posDest+=contarDigitos(d);
					posFormato++;
				break;
				case 'c':           /* char */
					/* Aquí es necesaria una conversión forzada
					* de tipos ya que va_arg sólo toma tipos
					* totalmente ‘ascendidos’.
					*/
				c = (char) va_arg(ap, char);
						cadDestino[posDest] = c;
						posDest++;
						posFormato++;
					break;
			}
		}
	}

	cadDestino[posDest]='\0';
}


void parserRecepcion (char* recibirRespuesta, char*primerParametro,char*segundoParametro)
{
	int i,longitud=0,posicionPrimerEspacio=0,j=0;

	longitud=calcularLongitudCadena(recibirRespuesta);
	posicionPrimerEspacio=buscarPrimerEspacio(recibirRespuesta);

	for(i=0;i<1000;i++)
	{
		primerParametro[i]='\0';
		segundoParametro[i]='\0';
	}

	if(posicionPrimerEspacio!=0)
	{
		for(i=0;i!=posicionPrimerEspacio;i++)
		{
			primerParametro[i] = recibirRespuesta[i];
		}
		primerParametro[i++]='\0';


		for(i=posicionPrimerEspacio+1,j=0;i<longitud;i++,j++)
		{
			segundoParametro[j]=recibirRespuesta[i];
		}
		segundoParametro[j]='\0';
	}
	else
	{
	
		for(i=0;i<longitud;i++)
		{
			primerParametro[i]=recibirRespuesta[i];
		}
		primerParametro[i]='\0';
	}

	for(i=0;i<1000;i++)
		recibirRespuesta[i]='\0';
}

int buscarPrimerEspacio (char* recibirRespuesta)

{
	int i=0;
	for(i=0;recibirRespuesta[i]!=' ' && recibirRespuesta[i]!='\0';i++);
			
		if (recibirRespuesta[i]==' ')
			return i;
		else
			return 0;
}

int calcularLongitudCadena (char* recibirRespuesta)
{
	int i=0;

	for (i=0;(recibirRespuesta[i]!='\r') && (recibirRespuesta[i]!='\n') ;i++);

		return i;
}

void cadAmayuscula (char* cad)
{
   int i;
   for(i = 0; cad[i]!='\0'; i++) 
   {
     cad[i] = toupper(cad[i]);//<--convierte a mayuscula la cadena
   }
	 cad[i++] = '\0';

   
}

/* 	Nombre de la Función: MiStrTok
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 03/05/2009
	modificación: --/--/----
	Objetivo de la Función: Similar a strtok
	Entrada:	cCadena: cadena que se quiere analizar
				iInicio: posicion desde donde se analiza
				cCaracter: caracter donde se ubicara el \0
	Salida:		fin: ubicacion del \0 que se incorporo
				return: direccion del inicio de la cadena
*/

char* MiStrTok(char* cCadena, int iInicio, char cCaracter, int* fin)
{
	int i;
	for (i=iInicio; cCadena[i]!=cCaracter; i++);
	cCadena[i]='\0';
	*fin=i;
	return &cCadena[iInicio];
}

int inicializarVector(DATOSARCHIVO* vector)
{
int i;

for(i=0;i<1000;i++)
	{
	vector[i].descriptorArchivo=0;
	vector[i].cabecera[0]='\0';
	}
return 1;
}

int buscarDescriptor(DATOSARCHIVO*vector,HEADERIRCIPC cabecera)
{
int i,iCant;
	{
		for(i=0;i<1000;i++)
			for(iCant=0;iCant<16;cabecera.cpDescriptorID[iCant]=vector[i].cabecera[iCant],iCant++);
		      return 1;
	}
		return 2;
}
int posicionLibre(DATOSARCHIVO*vector)
{
	int i;
	for(i=0;i<1000,vector[i].cabecera[0]='\0';i++);
		
		return i;

}


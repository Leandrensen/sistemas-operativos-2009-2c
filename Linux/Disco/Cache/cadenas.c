#include <stdio.h>
#include <stdarg.h>
#include "cadenas.h"

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

int longitudCadenaU(unsigned char* cpCad)
{
	int i;
	for(i=0;cpCad[i]!='\0'; i++);

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
		lNumero= lNumero/ 10;
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
int contarDigitos (long lNumero)
{
	char valor[40];
	miitoa (valor, lNumero);
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
int incluido (unsigned char* ucpCadena, unsigned char* ucpCadenaABuscar)
{
	int i,j;
	for (i=0, j=0; ucpCadena[i]!='\0'; i++)
	{
		if (ucpCadena[i] == ucpCadenaABuscar[j])
			j++;
		else
			j = 0;
		if (ucpCadenaABuscar[j] == '\0') return 1;
	}
	return 0;
}

/***********************************************************************************************/
void miitoa2 (unsigned char *v, long size)
{
    int i=0, j, pivote;
    if (size == 0)
    {
	v[0]='0';
	i++;
	v[i]='\0';
	return;
    }

    while (size != 0)
    {
	v[i] = size % 10 + '0';
	size = size / 10;
	i++;
    }

    v[i]='\0';

    for (j=0; j<i/2; j++)
    {
	pivote = v[j];
	v[j] = v [i-j-1];
	v[i-j-1] = pivote;
    }

}

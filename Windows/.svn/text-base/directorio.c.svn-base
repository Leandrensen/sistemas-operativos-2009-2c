#include <windows.h>
#include <process.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include "cadenas.h"
#include "directorios.h"

#pragma comment (lib, "Ws2_32.lib")

int crearListasArchivos(char* Directorio,LISTA* archivoAmostrar,ptrInfoArchivo *listaArchivos)
{

	LPWIN32_FIND_DATAA ffd;
	int  iTamanioCadena=0;
	HANDLE hFind = INVALID_HANDLE_VALUE;
/*	HANDLE memoria;*/
	char carpeta[1000];
	/*HANDLE memoriaHilo;*/
	INFOARCHIVO Datos;
	ptrInfoArchivo P=NULL;
	FDD fdd;
	int i,iFin;
/*	char cvTamanio[30];*/
	char * palabra;

	carpeta[0]='\0';
	concatenar(carpeta,Directorio);
	concatenar(carpeta,"\\*");
	iFin=0;
	/**********************************************************************************************************/
	palabra=(char*)HeapAlloc(GetProcessHeap(),NULL,strlen(Directorio)+1);
	copiar(palabra,Directorio);
	for(i=0;i<(strlen(palabra)-1);)
		{
			copiar(fdd.cFileName,MiStrTok(Directorio,iFin,'|',&iFin));
			copiar(fdd.nFileSizeLow,MiStrTok(Directorio,iFin+1,'|',&iFin));
			copiar(Datos.cvArchivo,fdd.cFileName);
			copiar(Datos.cvTamanio,fdd.nFileSizeLow);
			cargarNodo(Datos,&P);
			iTamanioCadena=iTamanioCadena+(longitudCadena(Datos.cvArchivo))+1;
		i=iFin+1;
		iFin++;
		}
	iTamanioCadena=iTamanioCadena+20;
	
	archivoAmostrar->enlistado = (char *)malloc( (iTamanioCadena + 1) * sizeof(char) );
	archivoAmostrar->enlistado[0]='\0';
	*listaArchivos = P;
	for(;P!=NULL;)
	{

		sacarNodo(&Datos,&P);
		concatenar(archivoAmostrar->enlistado,Datos.cvArchivo);
	}
	concatenar(archivoAmostrar->enlistado,"\r\n");
	FindClose(hFind);
	return 1;
}

   
   


int cargarNodo(INFOARCHIVO Cola,ptrInfoArchivo *pPila)
{
/*	int i;*/
	ptrInfoArchivo P;

	P = (ptrInfoArchivo) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, sizeof(INFOARCHIVO));

	copiar(P->cvArchivo,Cola.cvArchivo);
	copiar(P->cvTamanio,Cola.cvTamanio);

	P->sgte = *pPila;
	*pPila = P;


	return 1;

}


/* No hace free para no perder la lista */
int sacarNodo(INFOARCHIVO *Cola,ptrInfoArchivo *pPila)
{
/*	int i;*/
	ptrInfoArchivo p=NULL;
	
	p = *pPila;
	copiar(Cola->cvArchivo,p->cvArchivo);
	copiar(Cola->cvTamanio,p->cvTamanio);
	*pPila = p->sgte;

	return 1;

}

int liberarLista(ptrInfoArchivo *pPila, LISTA *enlista)
{
	ptrInfoArchivo p=NULL,pAux=NULL;
	p = *pPila;
	for(;p!=NULL;)
	{
		pAux = p;
		p=p->sgte;
		HeapFree(GetProcessHeap(),0,pAux);
	}
	*pPila = NULL;
	HeapFree(GetProcessHeap(),0,enlista->enlistado);
	return 1;

}

char *buscaArchivo(ptrInfoArchivo *pPila,char* nombreArchivo, char* tamanio)
{
	ptrInfoArchivo p=*pPila;
	concatenar(nombreArchivo,"\n");
	for(;(p!=NULL) && (comparar(p->cvArchivo,nombreArchivo)!=0);p=p->sgte);
	if(p!=NULL)
	{
		copiar(tamanio,p->cvTamanio);
		return p->cvTamanio;
	}
	return NULL;
}
	
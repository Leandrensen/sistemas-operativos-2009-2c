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
	HANDLE memoria;
	char carpeta[1000];
	HANDLE memoriaHilo;
	INFOARCHIVO Datos;
	ptrInfoArchivo P=NULL;
	int *numPtr,i;
	char cvTamanio[30];

	carpeta[0]='\0';
	concatenar(carpeta,Directorio);
	concatenar(carpeta,"\\*");
	(LPWIN32_FIND_DATAA) ffd = (LPWIN32_FIND_DATAA) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS,sizeof(WIN32_FIND_DATAA));
	/* Se busca el primer archivo del directorio */
	hFind = FindFirstFileA(carpeta, ffd);
 
	/* Procesamiento de archivos y directorios */
	do
	{
		if ((comparar(((char*)ffd->cFileName), ".") != 0) && (comparar(((char*)ffd->cFileName), "..") != 0))
		{
			if(ffd->dwFileAttributes == 16)
			{	
				concatenar(Datos.cvArchivo,ffd->cFileName);
				concatenar(Datos.cvArchivo,"/\n");
				cargarNodo(Datos,&P);
				iTamanioCadena=iTamanioCadena+(longitudCadena(Datos.cvArchivo)/*+longitudCadena(Datos.cvTamanio)*/)+1;

			}
			else
			{
			copiar(Datos.cvArchivo,ffd->cFileName);
			miitoa (Datos.cvTamanio, (long)ffd->nFileSizeLow);
			concatenar(Datos.cvArchivo,"\n");
			
		//	concatenar(Datos.cvTamanio,"\n");
			cargarNodo(Datos,&P);
			iTamanioCadena=iTamanioCadena+(longitudCadena(Datos.cvArchivo)/*longitudCadena(Datos.cvTamanio)*/)+1;
			}
		}
				
	}while (FindNextFileA(hFind, ffd) != 0);
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
	int i;
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
	int i;
	ptrInfoArchivo p=NULL;
	
	p = *pPila;
	copiar(Cola->cvArchivo,p->cvArchivo);
	copiar(Cola->cvTamanio,p->cvTamanio);
	*pPila = p->sgte;

	return 1;

}

int liberarLista(ptrInfoArchivo *pPila, LISTA enlista)
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
	HeapFree(GetProcessHeap(),0,enlista.enlistado);
	return 1;

}

char *buscaArchivo(ptrInfoArchivo *pPila,char* nombreArchivo)
{
	ptrInfoArchivo p=*pPila;
	concatenar(nombreArchivo,"\n");
	for(;(p!=NULL) && (comparar(p->cvArchivo,nombreArchivo)!=0);p=p->sgte);
	if(p!=NULL)
	return p->cvTamanio;

	return NULL;
}
	
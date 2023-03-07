#ifndef LISTAS_H_
#define LISTAS_H_

#include <stdlib.h>

typedef struct
{
	unsigned int uiSector;
	char cEnCache;
} infoCola;	

typedef struct
{
	infoCola info;
	void *sgte;	
} nodoCola, *pNodoCola;

int agregar(pNodoCola *pColaFte, pNodoCola *pColaFin, infoCola info);
int suprimir(pNodoCola *pColaFte, pNodoCola *pColaFin, infoCola *info);
unsigned int cantNodos(pNodoCola sectoresLeidosPpio);

#endif /*LISTAS_H_*/

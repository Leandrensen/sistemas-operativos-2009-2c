#include "listas.h"

int agregar(pNodoCola *pColaFte, pNodoCola *pColaFin, infoCola info)
{
	pNodoCola pCola;
	
	/* Pido memoria */
	pCola = (pNodoCola) malloc(sizeof(nodoCola));
		
	if (pCola == NULL)
		return 0;

	/* Cargo el nodo */
	pCola->info.uiSector = info.uiSector;
	pCola->info.cEnCache = info.cEnCache;

	if (*pColaFte == NULL)
		*pColaFte = pCola;
	else
		(*pColaFin)->sgte = pCola;
	
	*pColaFin = pCola;

	return 1;
}

int suprimir(pNodoCola *pColaFte, pNodoCola *pColaFin, infoCola *info)
{
	pNodoCola pAux;
	pAux = *pColaFte;
	
	info->uiSector = pAux->info.uiSector;
	info->cEnCache = pAux->info.cEnCache;

	*pColaFte = pAux->sgte;

	free(pAux);

	if (*pColaFte == NULL)
		*pColaFin = NULL;

	return 1;
}


unsigned int cantNodos(pNodoCola sectoresLeidosPpio)
{	
	int i = 0;
	pNodoCola p;
	p = sectoresLeidosPpio;
	while(p != NULL)
	{
		i++;
		p = p->sgte;
	}
	return i;
}

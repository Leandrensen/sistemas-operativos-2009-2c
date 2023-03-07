#ifndef _COLAS_H
#define _COLAS_H

#include "estructuras.h"

int Agregar (pNodoCola*, pNodoCola*, INFOCOLA);
int Suprimir (pNodoCola*, pNodoCola*, INFOCOLA*);
int ColaVacia (pNodoCola*);
long convertirAMilisegundos(SYSTEMTIME);
int Verificartiempo (pNodoCola*, long);

void cargarNodoCola (SOCKET ,unsigned char*, INFOCOLA*, unsigned char*,SOCKETIP *, int );
void imprimirListaEnEspera(pNodoCola*);
#endif
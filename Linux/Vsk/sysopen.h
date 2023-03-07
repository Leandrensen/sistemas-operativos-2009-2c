#ifndef _SYSOPEN_H
#define _SYSOPEN_H

#include <stdio.h>
#include <stdlib.h>
#include "oft.h"
#include "fd.h"
#include "sendrecv.h"

void separarParametrosOpen (unsigned char* ucpMensaje, unsigned char* ucpArchivo, unsigned int * ipFlag, unsigned int* ipMode);
int sys_open (int iSocketSFS, int iSocketCliente, pOft* L_Oft, pNodoFD* pFD, HEADERIRCIPC cabecera);


#endif

#ifndef CACHE_H_
#define CACHE_H_
#include <stdio.h>
#include "sendrecv.h"
#include "defines.h"

int peticionCache(int iSocketCache, unsigned char *ucBufIdSectores, unsigned char *ucBufSectores);
int ponerEnCache(int iSocketCache, unsigned char *ucBufIdSector, unsigned char *ucBufSector);

#endif /*CACHE_H_*/


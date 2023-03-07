#include <stdio.h>
#include "estructuras.h"

int analizarNombreArchivo (unsigned char* , ptrPalabrasTablasOtros*, int* , HANDLE*);
ptrTablaHtml buscaTagsHtml(char* , unsigned char*, int*, HANDLE*);
int tagTitle(HANDLE ,unsigned char* );
int tagMetaDescription (HANDLE ,unsigned char* );
int tagMetaKeywords (HANDLE ,ptrPalabraTablaHtml* , int*, HANDLE*);
int MeterPalabraHtml (ptrPalabraTablaHtml *, unsigned char* ,HANDLE*);
int tagA_Href(HANDLE ,ptrUrlTablaHtml*, HANDLE*);
int MeterUrlHtml (ptrUrlTablaHtml *, unsigned char* , HANDLE *);
int tagImgScr(HANDLE ,unsigned char *);
int miStrCmp(char *, char *);
int miStrlen(char* );
void obtenerTamanioArchivo(char *, unsigned char* , HANDLE*);
void buscarExtension(unsigned char * , unsigned char* );

#ifndef _OFT_H
#define _OFT_H


/************ TIPOS ACCIONES *****************************/
#define APERTURA_LECTURA		101
#define APERTURA_ESCRITURA		102
#define APERTURA_ELIMINAR		103
#define ENCONTRADO			1
#define NO_ENCONTRADO		0

typedef struct infoOFT
{
	int iDescriptor;
	unsigned char cvNombreDA[100];
	unsigned char cvNombreArchivo[40];
	int iModoApertura;
	int iTamanio;
	unsigned char ucvBuffer[1024];
	int iAperturas;
	void* sectores;	
}INFOOFT;

typedef struct nodoOft
{
	INFOOFT fcb;
	void* sgte;
}NODOOFT, *pOft;

/************* CABECERAS **************************************/
int meterOft (pOft* pila, INFOOFT fcb);
int quitarOftPorNombreArchivo(pOft* pila, unsigned char* cpNombreArchivo);
void crearfcbOFT (INFOOFT * fcb, int iDescriptor, unsigned char* cpNombreDA, unsigned char* cpNombreArchivo, int iModoApertura, int iTamanio, int* ipListaSectores);
int quitarOftPorNombreDescriptor(pOft* pila, int iDescriptor);
int verificarArchivoAbiertoPara(pOft* pila, unsigned char* unAlmacenamiento, unsigned char* nombreArchivo, int imodoApertura);
int crearListaSectores (int * pLista, int iCantidad, unsigned char* ucpLista);
void obtenerNombres (unsigned char* ucpNombre, unsigned char* ucpNombreDA, unsigned char* ucpNombreArchivo);
int verificarArchivoAbierto(pOft* pila, unsigned char* unAlmacenamiento, unsigned char* nombreArchivo);

#endif



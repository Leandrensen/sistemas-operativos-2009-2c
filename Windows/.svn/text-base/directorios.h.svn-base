typedef struct infoArchivo
{
	char  cvArchivo[1000];
	char cvTamanio[1000];
	char cvFechaCreacion[1000];
	void* sgte;
}INFOARCHIVO, *ptrInfoArchivo;

typedef struct fdd
{
	char  cFileName[1000];
	char  nFileSizeLow[1000];
	void* sgte;
}FDD, *fdd;

typedef struct lista
{
	char* enlistado;
}LISTA;

int crearListasArchivos(char* Directorio,LISTA* archivoAmostrar,ptrInfoArchivo *listaArchivos);
int cargarNodo(INFOARCHIVO ,ptrInfoArchivo *);
int sacarNodo(INFOARCHIVO *Cola,ptrInfoArchivo *pPila);
char *buscaArchivo(ptrInfoArchivo *pPila,char* nombreArchivo, char* tamanio);
int liberarLista(ptrInfoArchivo *pPila, LISTA *enlista);

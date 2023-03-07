#ifndef _ARCHIVOS_H
#define _ARCHIVOS_H

/*************            ESTRUCTURAS                     ******************/

typedef struct config
{
	char cvIP[16];
	int iPuerto;
	char cvIPLdap[16];
	int iPuertoLdap;
	char cvUsuarioLdap[30];
	char cvContraseniaLdap[30];
	int iCache; /* 1 si, 0 no */
	int iTamanioCache;
}CONFIGURACION;

/************             CABECERAS                       ******************/

int cargarConfiguracion (CONFIGURACION*);
int creaLog ();
int escribirLog (char cTipoLog, char* cpMensaje);
void mostrarConfiguracion (CONFIGURACION configuraciones);

#endif	

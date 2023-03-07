#include "cadenas.h"
#include "protocoloComunicacionFTP.h"


int opcionesUsuario (char *primerParametro)
	{
				
		cadAmayuscula (primerParametro);

 			if(comparar (primerParametro,"LS")==0) return CMD_LS;

			if(comparar (primerParametro,"LIST")==0) return CMD_LS;

			if(comparar (primerParametro,"NLST")==0) return CMD_LS;

			if(comparar (primerParametro,"GET")==0) return CMD_GET;
			
			if(comparar (primerParametro,"PUT")==0) return CMD_PUT;

			if(comparar (primerParametro,"SIZE")==0) return CMD_SIZE;

			if(comparar (primerParametro,"CD")==0)return CMD_CD;
		
			if(comparar (primerParametro,"PWD")==0) return CMD_PWD;

			if(comparar (primerParametro,"CWD")==0) return CMD_CD;
		
			if(comparar (primerParametro,"DELETE")==0) return CMD_DEL;
	
			if(comparar (primerParametro,"?")==0) return CMD_HELP;

			if(comparar (primerParametro,"HELP")==0) return CMD_HELP;
	
			if(comparar (primerParametro,"NOOP")==0) return CMD_NOOP;

			if(comparar (primerParametro,"USER")==0) return CMD_USER;

			if(comparar (primerParametro,"PASS")==0) return CMD_PASS;

			if(comparar (primerParametro,"TYPE")==0) return CMD_TYPE;

			if(comparar (primerParametro,"PASV")==0) return CMD_PASV;		

			if(comparar (primerParametro,"OPTS")==0) return CMD_OPTS;	

			if(comparar (primerParametro,"SYST")==0) return CMD_SYST;

			if(comparar (primerParametro,"SITE")==0) return CMD_SITE;

			if(comparar (primerParametro,"STOR")==0) return CMD_STOR;

			if(comparar (primerParametro,"DELE")==0) return CMD_DELE;

			if(comparar (primerParametro,"RETR")==0) return CMD_RETR;

return 1;
}





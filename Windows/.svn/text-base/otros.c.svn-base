#include "otros.h"
#include <math.h>

/* 	Nombre de la Función: miitoa
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 12/04/2009
	modificación: 12/04/2009
	Objetivo de la Función: Convierte un numero en cadena
	Entrada:	num: Numero a convertir
	Salida:		v : cadena que contiene el numero
				cant: longitud de la cadena
*/
	
void miitoa (char v[], int num, int *cant){
	int i=1, j, pivote;
	v[0]=']';
	while (num!=0)
	{
		v[i] = num%10 + '0';
		num = num / 10;
		i++;
	}
	v[i]='[';
	i++;
	*cant = i;
	v[i]='\0';

	for (j=0; j< i/2; j++){
		pivote= v[j];
		v[j]= v[i-j-1];
		v[i-j-1]=pivote;
	}
}/* FIN DE MIITOA --------------------------------------------------------------*/


/* 	Nombre de la Función: miitoa
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 12/04/2009
	modificación: 12/04/2009
	Objetivo de la Función: Convierte un numero en cadena
	Entrada:	num: Numero a convertir
	Salida:		v : cadena que contiene el numero
				cant: longitud de la cadena
*/
	
void miitoa2 (char v[], long num, int *cant){
	int i=0, j, pivote;

	if (num==0)
	{
		*cant = 1;
		v[0]='0';
		v[1]='\0';
		return;
	}
	
	while (num!=0)
	{
		v[i] = (char)(num%10 + '0');
		num = num / 10;
		i++;
	}

	*cant = i;
	v[i]='\0';

	for (j=0; j< i/2; j++){
		pivote= v[j];
		v[j]= v[i-j-1];
		v[i-j-1]=pivote;
	}
}/* FIN DE MIITOA --------------------------------------------------------------*/

/* 	Nombre de la Función: miatoi
	Versión: 1.0
	Autor/es: Villalva Emiliano
	Fechas de creación: 20/04/2009
	modificación: 20/04/2009
	Objetivo de la Función: Convierte una cadena en numero
	Entrada:	v: cadena a convertir
	Salida:		return: numero convertido
*/
int miatoi (char* v)
{	
	int i, num = 0;
	
	for (i=0; (v[i]>='0') && (v[i]<='9') ; i++)
		num = num*10 + (v[i] - '0');
	
	return num;
}

/* 	Nombre de la Función: MiStrCat
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 03/05/2009
	modificación: --/--/----
	Objetivo de la Función: Concatena dos cadenas
	Entrada:	origen: cadena que se quiere adicionar
				destino: cadena en donde se quiere adicionar
	Salida:
*/

void MiStrCat(char* destino, char* origen)
{
	int i,j;
	for (i=0; destino[i]!='\0';i++);
	for (j=0; origen[j]!='\0';  i++, j++)
		destino[i]=origen[j];
	destino[i]='\0';

}

/* 	Nombre de la Función: MiStrTok
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 03/05/2009
	modificación: --/--/----
	Objetivo de la Función: Similar a strtok
	Entrada:	cCadena: cadena que se quiere analizar
				iInicio: posicion desde donde se analiza
				cCaracter: caracter donde se ubicara el \0
	Salida:		fin: ubicacion del \0 que se incorporo
				return: direccion del inicio de la cadena
*/

char* MiStrTok(char* cCadena, int iInicio, char cCaracter, int* fin)
{
	int i;
	for (i=iInicio; cCadena[i]!=cCaracter; i++);
	cCadena[i]='\0';
	*fin=i;
	return &cCadena[iInicio];
}

/* 	Nombre de la Función: convertirASegundos
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 05/05/2009
	modificación: --/--/----
	Objetivo de la Función: Retorna los segundos correspondientes desde las 0hs
	Entrada:	h: hora que se desea analizar
	Salida:		return: devuelve los segundos
*/

float convertirASegundos(SYSTEMTIME h)
{float v;	
	v=((float)h.wMilliseconds)/1000;
	return (float)(h.wHour*3600 + h.wMinute*60 + h.wSecond + v);

}

/* 	Nombre de la Función: convertirAST
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 05/05/2009
	modificación: --/--/----
	Objetivo de la Función: Retorna la estructura SYSTEMTIME correspondiente a los segundos transcurridos
							desde la 0hs.
	Entrada:	h: segundos que se desea analizar
	Salida:		return: devuelve en formato SYSTEMTIME los segundos analizados
*/

SYSTEMTIME convertirAST(float h)
{
	SYSTEMTIME valor;

	valor.wHour =(WORD)floor((double)(h/3600));
	valor.wMinute = (WORD)floor((double)(h-valor.wHour*3600)/60);
	valor.wSecond = (WORD)floor((double)(h-valor.wHour*3600-valor.wMinute*60));
	valor.wMilliseconds = (WORD)((h - valor.wHour*3600 - valor.wMinute*60 - valor.wSecond)*1000);
	return valor;
}

/* 	Nombre de la Función: deSTaCadena
	Versión: 1.0
	Autor/es: Villalva Emanuel
	Fechas de creación: 05/05/2009
	modificación: --/--/----
	Objetivo de la Función: Convierte un SISTEMTIME a una cadena con formato[HH:MM:SS.sss]
	Entrada:	hora: SYSTEMTIME a convertir
	Salida:		v[]: vector que contiene la hora con el formato indicado
*/

void deSTaCadena (char v[], SYSTEMTIME hora) {
	v[0]='[';
	v[1]=hora.wHour / 10 + '0';
	v[2]=hora.wHour % 10 + '0';
	v[3]=':';
	v[4]=hora.wMinute / 10 + '0';
	v[5]=hora.wMinute % 10 + '0';
	v[6]=':';
	v[7]=hora.wSecond / 10 + '0';
	v[8]=hora.wSecond % 10 + '0';
	v[9]='.';
	v[10]=hora.wMilliseconds / 100 + '0';
	v[11]=(hora.wMilliseconds / 10)%10 + '0';
	v[12]=hora.wMilliseconds % 10 + '0';
	v[13]=']';
	v[14]='\0';
}

void NumToBin (unsigned char buf[], int total, int desde)
{

	if (total>16777216-1)
	{
		buf[desde+3]=total/16777216;
		total-=16777216;
	}
	else
	buf[desde+3]=0;

	if (total>65536-1)
	{
		buf[desde+2]=total/65536;
		total-=65536;
	}
	else
	buf[desde+2]=0;

	if (total>256-1)
	{
		buf[desde+1]=total/256;
		total-=256;
	}
	else
	buf[desde+1]=0;

	if (total>0)
	{
		buf[desde+0]=total;
		total-=0;
	}
	else
	buf[desde+0]=0;
}

unsigned int BinToNum (unsigned char buf[], int desde)
{
	unsigned total;
	
	total = buf [desde+0] + 256*buf[desde+1] + 65563*buf[desde+2] + 16777216*buf[desde+3];
	
	return total;
}

void Reemplazar (char* ucpCadena, char cEliminar, char cReemplazar)
{
	int i;

	for (i=0; ucpCadena[i]!='\0'; i++)
	{
		if (ucpCadena[i]==cEliminar)
			ucpCadena[i] = cReemplazar;
	}

}

int vecanum (char v[])
{
	int i, num=0;
	for (i=0;v[i]>='0' && v[i]<='9';i++)
		num= num*10 + (v[i]-'0');
	return num;

}

int comparar(unsigned char* cad1, unsigned char* cad2)
{
	int i;
	for (i=0;(cad1[i]!='\0') && (cad2[i]!='\0') && (cad1[i]==cad2[i]);i++);

		if (cad1[i]==cad2[i]) return 1;

		return 0;
}

int compararnou(char* cad1, char* cad2)
{
	int i;
	for (i=0;(cad1[i]!='\0') && (cad2[i]!='\0') && (cad1[i]==cad2[i]);i++);

		if (cad1[i]==cad2[i]) return 1;

		return 0;
}

void concatenarUnsigned(unsigned char* cad1, unsigned char* cad2)
{
	int i,j;
	for(i=0;cad1[i]!='\0';i++);

	for(j=0;cad2[j]!='\0';cad1[i]=cad2[j],i++,j++);

	cad1[i]='\0';
}

void concatenar( char* cad1,  char* cad2)
{
	int i,j;
	for(i=0;cad1[i]!='\0';i++);

	for(j=0;cad2[j]!='\0';cad1[i]=cad2[j],i++,j++);

	cad1[i]='\0';
}

int pasoElTiempoDeEsperaCrawler(float tiempoConfiguracion, float *tiempoQuePaso)
{
	SYSTEMTIME horaActual;
	float tiempoActual;
	
	GetSystemTime(&horaActual); 
	
	tiempoActual = convertirASegundos(horaActual);

	if ((tiempoActual - (*tiempoQuePaso)) >= tiempoConfiguracion)
	{
		*tiempoQuePaso=tiempoActual;
		return 1;
	}

		return 0;
}

float miatoiFloat (char* v)
{	
	int i;
	float num = 0;
	
	for (i=0; (v[i]>='0') && (v[i]<='9') ; i++)
		num = num*10 + (v[i] - '0');
	
	return num;
}



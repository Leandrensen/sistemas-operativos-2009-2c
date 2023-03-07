#ifndef _CADENAS_H
#define _CADENAS_H

int comparar (char * cad1, char* cad2);
int compararU (unsigned char * cad1, unsigned char* cad2);
void concatenar (char * cad1, char* cad2);
void concatenarU (unsigned char * cad1, unsigned char* cad2);
void copiar(char * cad1, char* cad2);
void copiarU(unsigned char * cad1, unsigned char* cad2);
int longitudCadena(char* cad);
int longitudCadenaU(unsigned char* cad);
void miitoa (char cadenaDestino[], long numero);
int miatoi (char* cadena);
void reemplazar (char* cadena, char caracterBuscado, char caracterDeReemplazo);
int contarDigitos (long numero);
void miSprintf(char* cadDestino, char* formato,...);
void parserRecepcion (char* recibirRespuesta, char*primerParametro,char*segundoParametro);
int buscarPrimerEspacio (char* recibirRespuesta);
int calcularLongitudCadena (char* recibirRespuesta);
void cadAmayuscula (char* cad);
char* MiStrTok(char* cCadena, int iInicio, char cCaracter, int* fin);

#endif




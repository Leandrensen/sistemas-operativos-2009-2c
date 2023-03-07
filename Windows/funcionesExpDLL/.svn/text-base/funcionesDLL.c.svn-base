#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
typedef int (__cdecl *imprimo)(char*);


int main()
{
	HINSTANCE hinstLib;
	BOOL fRunTimeLinkSuccess = FALSE;
	imprimo imprimimos;
	LPCWSTR libreria = L"funcionesExpDLL.dll";
	hinstLib = LoadLibrary (libreria);
	imprimimos = (imprimo) GetProcAddress(hinstLib, "imprimir");
	imprimimos("HOLA MUNDO");
	FreeLibrary(hinstLib);
	system ("PAUSE");
	return 0;
}
#define PWD  "PWD        - Muestra el directorio activo en el servidor"
#define CD   "CD         - directorio Cambia el directorio de trabajo en el servidor"
#define LS   "LS         - Muestra el contenido del directorio en el que estamos en el servidor"
#define DEL  "DELETE     - archivo Borra un archivo en el servidor"
#define GET  "GET        - archivo Obtiene un archivo"
#define PUT  "PUT        - archivo Envia el archivo indicado al directorio activo del servidor"
#define HELP "HELP o ?   - Muestra una lista de los comandos disponibles"
#define NOOP "NOOP       - (No Operation) Se le comunica al servidor que el cliente esta en modo de no operacion, el servidor usualmente responde con un ZZZ y refresca el contador de tiempo inactivo del usuario"
#define PASV "PASV PORT  - Comandos necesarios para iniciar la comunicacion de modo pasivo"

/*****************************************************************************************/

void ayudaComandos (void);
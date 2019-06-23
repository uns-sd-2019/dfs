#define PATH_VERS "vers" 
#define MAX_PATH_LENGTH 100
#define MAX_VERSION_LENGTH 20

struct versiones_arch {
	char fName[MAX_PATH_LENGTH];
	int cant;
};

/**
 * Muestra el archivo de versiones indicando cuantas hay de cada archivo
 * en el nodo.
 * */
void printArchivoVers (); 

/**
 * Si no existe el archivo de versiones lo crea.
 * Dado un filePath versiona al archivo si este ya existe.
 * Actualiza archivo de versiones. 
 * Retorna filePath si el archivo no se versiono, si se versiono 
 * retorna el nombre de la version anterior. 
 * */
char * versionar(char *filePath);

#include "filehandler.h"
#include "Coordinador.h"
#include "Nodo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <rpc/rpc.h>
#include <string.h>

// "Clear" the screen by scrolling down
#define clear_screen() printf("\033[H\033[J");

CLIENT *clntcoor;   // Se usara para comunicarse con el coordinador.
CLIENT *clntnodo;   // Se usara para comunicarse con el nodo a relacionarse.
char *coordinador;	// IP del servidor Coordinador
char *nodo;         // IP del nodo con el que relacionarse.

// MiniShell Interactiva
void menu(){
	clear_screen();

	int menuoption;
	int stop = 0;

	char *rutaDFS = (char*) malloc(64);		// PATH del DFS (el usuario lo obtiene mediante ls)
	char *rutaLocal = (char*) malloc(64);	// PATH de un archivo local del usuario (ej: /home/user/myfile.txt)
	char *arbolDFS = (char*) malloc(1024);	// Arbol de directorios completo del DFS (respuesta del ls)
	char *vacio;  //Puntero vacio que se debe enviar en la consulta RPC ls. Hay que declararlo char* y castearlo cuando se invoca ls

	while (stop == 0) {
		printf(" --------------------- MENU ------------------- \n");
		printf("Indique la operación a realizar:\n");
		printf("1. Listar archivos del DFS\n");
		printf("2. Subir un archivo al DFS\n");
		printf("3. Descargar un archivo del DFS\n");
		printf("\n");
		printf("0. Salir de la aplicacion\n");
		printf("\n");
		printf(" ---------------------------------------------- \n");
		printf("\n");

		printf("Ejecutar: ");
		scanf("%i", &menuoption);

		switch (menuoption) {
			case 1: ////////////////////////////////////////// ls()
				
				arbolDFS = *( ls_1((void*)&vacio,clntcoor) );
				//arbolDFS = "├ Directorio1\n│\t├ Archivo1\n│\t└ Archivo2\n└ Archivo3"; // para testing de formato
				printf ("El arbol de directorios es:\n\n");
				printf("%s\n", arbolDFS);
				break;

			case 2: ////////////////////////////////////////// subir(path,file)

				// Obtenemos la ruta local del archivo que desea subir el usuario.
				printf("Ingrese la ruta del archivo local: (e.g. /home/user/myfile.txt)\n");
				scanf("%s", rutaLocal);

				// Verificamos que el archivo exista
				if (access( rutaLocal, F_OK ) == -1){
					printf("ERROR: El archivo no fue encontrado. Verifique la ruta.\n");
					break;
				}

				// Obtenemos donde se ubicara el archivo.
				printf("Ingrese la ruta del DFS donde desea que se guarde el archivo:\n");
				scanf("%s", rutaDFS);

				// Pedimos al coordinador la direccion al nodo correspondiente.
				nodo = *rqsubir_1(&rutaDFS,clntcoor);
				printf("Recibi la direccion %s\n", nodo );
				// Iniciamos la conexión con el nodo
				clntnodo = clnt_create(nodo, NODO, VERSION1, "TCP");
				if(clntnodo == (CLIENT *) NULL) {
				  printf("ERROR: Fallo la conexion con el nodo.\n");
				  break;
				}
				// Armamos el struct para pasar como parametro.
				int resultadoSubir;
				file_to_send sendfile;
				strcpy(sendfile.name, rutaDFS);
				file_info* fi = malloc(sizeof(file_info));
				read_file(rutaLocal,fi);	//read_file toma el contenido del archivo en rutaLocal y lo carga en un buffer junto con tu tamaño
				sendfile.data=fi->buffer;
				sendfile.size=fi->buffer_length;
				//printf("Shell: El contenido del buffer a transmitir es: %s y su longitud %i\n", sendfile.data, sendfile.size);
				printf("Subiendo el archivo local %s cuya longitud es %i, y su nombre remoto sera %s\n", rutaLocal, sendfile.size, sendfile.name );

				// Enviamos el archivo al nodo.
				resultadoSubir=*subir_1(&sendfile, clntnodo);
				if (&resultadoSubir == (int*) NULL) {
					//clnt_perror (clnt, "call failed"); // No se que es esta linea
				} else {
					if (resultadoSubir == 0)
						printf("Shell: subida de archivo exitosa\n");
				    else
				    	printf("Shell: ocurrio un error por parte del servidor\n");
				}
				
				break;
				
			case 3: ////////////////////////////////////////// bajar(file)

				// Obtenemos la ruta del archivo que desea descargar el usuario y el nombre con el que lo quiere guardar
				printf("Ingrese la ruta del archivo en el DFS.\n");
				scanf("%s",rutaDFS);
				printf("Ingrese el nombre del archivo en su sistema.\n");
				scanf("%s",rutaLocal);

				// Pedimos al coordinador la direccion al nodo correspondiente.
				nodo = *rqbajar_1(&rutaDFS, clntcoor);

				if(nodo == (char *) NULL)
				{
					printf("No se pudo encontrar el archivo %s en el DFS, intente nuevamente con otra ruta\n", rutaDFS );
					break;
				}
				// Iniciamos la conexión con el nodo:
				clntnodo = clnt_create(nodo, NODO, VERSION1, "TCP");
				if(clntnodo == (CLIENT *) NULL) {
				  printf("ERROR: Fallo la conexion con el nodo.\n");
				  exit(1);
				}

				file_to_send *recivedFile; // Creamos la estructura para recibir al archivo.
				// Recibimos el archivo al nodo:
				recivedFile = bajar_1(&rutaDFS, clntnodo);
				if (recivedFile == (file_to_send *) NULL) {
					// clnt_perror (clnt, "call failed"); // No se que es esta linea
					printf("Error: no se recibio ningun archivo\n");
					break;
					
				}

				// Guardamos el archivo en el almacenamiento permanente.
				int file_length = recivedFile->size;
				char* buffer = recivedFile->data;
				FILE* newfile;
				if (!(newfile= (FILE*) fopen(rutaLocal,"wb"))) {
					printf("Shell: Error al crear el nuevo archivo\n");
					fclose(newfile);
					exit(0);
				}
				//printf("Shell: El contenido del buffer es %s\n", buffer);
				int items_writen = fwrite(buffer, file_length, 1, newfile);
				printf("Shell: Se escribieron %i bytes en el archivo %s\n", file_length*items_writen, rutaLocal);
				fclose(newfile);

				break;

			case 0: // Salir
				printf("\nBye bye...\n");
				stop = 1;
				break;

			default:
				printf("Opcion Incorrecta.\n");
				break;
		}

		if (stop != 1) {
			printf("\n\nPresione cualquier tecla para continuar...");
			scanf("%c", (char*) &menuoption); // | hack para esperar un enter para continuar
			getchar();						  // |
			clear_screen();
		}
	}
}

// Al iniciar el cliente se carga la direccion ip del coordinador: cliente(ip)
int main(int argc, char *argv[]) {
	
	//Caso en que no se paso unicamente la direccion ip del coordinador por parametro.
	if (argc != 2) {
	  printf ("ERROR: Cantidad de parametros invalida. Falta IP del coordinador?\n");
	  exit(1);
	}

	coordinador = argv[1]; // Capturar ip del servidor coordinador.
	
	// Conectarse con el servidor coordinador
	
	clntcoor = clnt_create(coordinador, COORDINADOR, VERSION1, "TCP");
	if(clntcoor == (CLIENT *) NULL) {
	  printf("ERROR: Fallo la conexion con el coordinador.\n");
	  exit(1);
	}
	
	
	menu();
}



#include "Nodo.h"
#include "Coordinador.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "filehandler.h"
#include "versionado.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>

#define FILES_FOLDER "archivosDFS/";

CLIENT *clntcoor;   // Se usara para comunicarse con el coordinador.
char *coordinadorDir;

/*
 * Chequea si el path es valido.
 * retorna 0 si la validacion fue exitosa. Sino un numero > 0 que indica el error:
 * 1 -> Null path.
 * 2 -> El path contiene mas de un directorio.
 * 3 -> El nombre del directorio pertenece a un archivo.
 * 4 -> El nombre del archivo pertenece a un directorio.
 */
int validarPath(char *path){
    if (path == NULL) {
		printf("validarPath(): null path\n"); 
        return 1;
    }
    char *dir_name = dirname(strdup(path));  // Operaciones destructivas
    char *file_name = basename(strdup(path));
    int has_dir = 0;  // flag para saber si el path tiene directorio.

    // Miramos si tiene directorio
    if (strcmp(dir_name, ".") != 0 && strcmp(dir_name, "/") != 0) {
        has_dir = 1;
        // Si el caracter inicial es '/' se elimina. Trae problemas.
        if (dir_name[0] == '/')
            strcpy(dir_name, &dir_name[1]);
        // Maximo una carpeta.
        if (strchr(dir_name, '/') != NULL) { 
			printf("validarPath(): Nombre de archivo invalido. Máximo una carpeta\n"); 
            return 2;
        }
    }
    struct stat stat_buf;
    // dir_name no puede tener el nombre de un archivo.
    if (has_dir) {
        if (stat(dir_name, &stat_buf) == 0) {
            if (!(stat_buf.st_mode & S_IFDIR)) {  // No es un directorio
                printf("validarPath(): El nombre de la carpeta pertence a un archivo\n");
                return 3;
            }
        }

    } else {
        // "file_name" no puede tener el nombre de una carpeta.
        if (stat(file_name, &stat_buf) == 0) {
            if (!(stat_buf.st_mode & S_IFREG)) {  // No es un archivo
                printf("validarPath(): El nombre del archivo pertenece a una carpeta\n");
                return 4;
            }
        }
    }
    // Si el caracter inicial es '/' se elimina. Trae problemas.
    if (path[0] == '/')
        strcpy(path, &path[1]);
	return 0;
}

void updateCoordinadorDir(char *dirCord)
{
	coordinadorDir = malloc(32);
	strcpy(coordinadorDir,dirCord);
}

void printSeparador(char *titulo){
	printf("═══════════════════════════ %s ═══════════════════════════\n", titulo);
}

int * subir_1_svc(file_to_send * argp, struct svc_req *rqstp){
	static int  result;
	int file_length = argp->size;
	char* buffer = argp->data;
	char* filename = argp->name;
	FILE* newfile;
	
	printSeparador("subir");
	
	result = validarPath(filename);

	if(result == 0){ // path valido
		// Versionado. 
		char *newName = malloc(sizeof(char) * MAX_PATH_LENGTH);
		strcpy(newName, versionar( filename ));		
		if (strcmp(newName, filename) != 0) {
			printf("\n");
			printf("Nuevo control de versiones:\n");
			printArchivoVers();
			printf("\n");
		}
		// ---	
		// Abriendo/creando archivo para escribirlo
		newfile = fopen(filename, "w");
		if (newfile == NULL) {
			// Hay que crear la carpeta
			char *dir_name = dirname(strdup(filename));
			printf("Creando nueva carpeta: '%s'\n", dir_name);
			mkdir(dir_name, 0700);
		}
		newfile = fopen(filename, "w");
		if (newfile == NULL) { 
			printf("Error inesperado al crear el nuevo archivo\n");
			result = 5; 
		}else{
			// Escribiendo el archivo.
			// printf("Nodo: El contenido del buffer es %s\n", buffer );       //Cuando funcione hay que eliminar esta linea
			int items_writen = fwrite(buffer, file_length, 1 ,newfile);
			printf("Nodo: Se escribieron %i bytes en el archivo %s\n", file_length*items_writen, filename);
			fclose(newfile);

			// Notificando coordinador
			printf("Nodo: Notificando al coordinador la subida del archivo...\n");

			clntcoor = clnt_create(coordinadorDir, COORDINADOR, VERSION1, "TCP");
			if(clntcoor == (CLIENT *) NULL) {
			printf("ERROR: Fallo la conexion con el coordinador.\n");
			exit(1);
			}

			int ack = *subirack_1(&newName,clntcoor);
			if(ack)
				printf("Notifique con exito al coordinador la subida del archivo %s\n", filename );
			else
				printf("No se pudo notificar al coordinador la subida del archivo %s\n", filename );

			clnt_destroy(clntcoor);
			free(newName);
		}
	}
	return &result;
}

/**
 * Retorna un archivo con size -1 en caso de error. 
 * */
file_to_send * bajar_1_svc(char ** argp, struct svc_req *rqstp){
	static file_to_send result;
	char* path = *argp;
	
	printSeparador("bajar");
	
	int validacion = validarPath(path);
	
	if ( validacion == 0) {
		printf("Se enviara el archivo %s\n", *argp );

		file_info* fi = malloc(sizeof(file_info));
		strcpy(result.name,*argp);
		
		read_file(*argp,fi);
		result.size=fi->buffer_length;
		result.data=fi->buffer;
		//printf("Nodo: El contenido del archivo a transferir es %s y su longitud es de %i bytes\n", result.data, result.size );
		printf("Transferi el archivo %s\n", path );
	} else {
		printf("Error en el path %s, codigo %i. Ver errores en validarPath.\n", *argp, validacion );
		
		result.size = -1;
	}	

	return &result;
}

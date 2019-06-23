#include "Nodo.h"
#include "Coordinador.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "filehandler.h"
#include "versionado.h"

CLIENT *clntcoor;   // Se usara para comunicarse con el coordinador.
char *coordinadorDir;


void updateCoordinadorDir(char *dirCord)
{
	coordinadorDir = malloc(32);
	strcpy(coordinadorDir,dirCord);
}


int * subir_1_svc(file_to_send * argp, struct svc_req *rqstp){
	static int  result;
	int file_length = argp->size;
	char* buffer = argp->data;
	char* filename = argp->name;
	FILE* newfile;
	
	// Versionado. 
	char *newName = malloc(sizeof(char) * MAX_PATH_LENGTH);
	
	strcpy(newName, versionar( argp->name ));		
	
	if (strcmp(newName, argp->name) != 0) {
		printf("\n");
		printf("Nuevo control de versiones:\n");
		printArchivoVers();
		printf("\n");
	}
	// ---	

	if (!(newfile= (FILE*) fopen(filename,"wb")) ) {
		printf("Nodo: Error al crear el nuevo archivo\n");

		result= 1;
		return &result;
	}

    //printf("Nodo: El contenido del buffer es %s\n", buffer );       //Cuando funcione hay que eliminar esta linea
	int items_writen = fwrite(buffer, file_length, 1 ,newfile);
	printf("Nodo: Se escribieron %i bytes en el archivo %s\n", file_length*items_writen, filename);

	fclose(newfile);

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

	result= 0;

	clnt_destroy(clntcoor);
	free(newName);

	return &result;

}
file_to_send * bajar_1_svc(char ** argp, struct svc_req *rqstp){
	static file_to_send result;
	char* path = *argp;

	file_info* fi = malloc(sizeof(file_info));
	strcpy(result.name,*argp);
	read_file(*argp,fi);
	result.size=fi->buffer_length;
	result.data=fi->buffer;
	//printf("Nodo: El contenido del archivo a transferir es %s y su longitud es de %i bytes\n", result.data, result.size );
	printf("Transferi el archivo %s\n", path );

	return &result;
}

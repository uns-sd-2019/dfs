#include "Nodo.h"
#include "Coordinador.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "filetransfer.h"


int * subir_1_svc(file_to_send * argp, struct svc_req *rqstp){
  static int  result;

  int file_length = argp->size;
  char* buffer = argp->data;
  char* filename = argp->name;
  FILE* newfile;

  if (!(newfile= (FILE*) fopen(filename,"wb")) )
  {
		printf("Nodo: Error al crear el nuevo archivo\n");

		result= 1;
		return &result;
	}

  printf("Nodo: El contenido del buffer es %s\n", buffer );       //Cuando funcione hay que eliminar esta linea
	int items_writen = fwrite(buffer, file_length, 1 ,newfile);
	printf("Nodo: Se escribieron %i bytes en el archivo %s\n", file_length*items_writen, filename);

	fclose(newfile);

	result= 0;

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
	printf("Nodo: El contenido del archivo a transferir es %s y su longitud es de %i bytes\n", result.data, result.size );

	return &result;
}
#include "filehandler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_filelength(FILE* file){
	fseek(file,0,SEEK_END);
  	int fl = ftell(file);
  	fseek(file,0,SEEK_SET);
  	return fl;
}

void read_file(char* path,file_info* fi){
  //A partir de un archivo ubicado en path, devuelve un buffer con los datos del archivo cargados

  FILE *file;
  char *buffer;
  long file_length;

  //Abrir el archivo a leer, se usa "rb" porque no necesariamente es un archivo de texto
  file = fopen(path, "rb");
  //file = fopen(path, "r");
  if(file == NULL){
    perror("Filehandler: Unable to open file");
    exit(1);
  }
  printf("Filehandler: file opened\n");

  //Esto obtiene el tamaño del archivo en bytes
  file_length= get_filelength(file);
  fi->buffer_length= file_length;

  printf("Filehandler: la longitud del archivo a enviar es de %li bytes\n", file_length );
  /*
  fseek(file,0,SEEK_END);
  file_length = ftell(file);
  fseek(file,0,SEEK_SET);*/

  //Se ingresan todos los datos del archivo en un buffer y se devuelve
  //buffer=(char *)malloc(file_length);
  buffer=(void *)malloc(file_length*sizeof(char));
  if(buffer == NULL){
    perror("Filehandler: Error allocating memory");
    exit(1);
  }
  printf("Filehandler: Memory allocated\n");

  //fread(buffer, file_length, 1, file);

  //fread(buffer, file_length , 1, file);
  int items_read=fread(buffer, 1, file_length , file);
  printf("Filehandler: Se leyeron %i bytes del archivo a subir\n", items_read);
  fclose(file);

  printf("Filehandler: El contenido del buffer es:%s\n El cual tiene %li items\n", buffer, strlen(buffer));

  fi->buffer=buffer;

  printf("Filehandler: File imported, fd closed\n");
}

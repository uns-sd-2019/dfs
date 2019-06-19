#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <stdio.h>


typedef struct file_info{
	char * buffer;
	int buffer_length;
} file_info;

int get_filelength(FILE* file);
/*	A partir del descriptor de archivo pasado por parametro
 *	obtiene la cantidad de caracteres que tiene el archivo 
 */

void read_file(char* path,file_info* fi);
/*	Dada una ruta y una estructura file_info, se vuelca todo
 *	el contenido del archivo ubicado en la ruta, en un buffer dentro de la estructura,
 *	junto con el tamaño del mismo-	
 */
#endif
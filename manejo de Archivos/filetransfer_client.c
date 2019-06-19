#include "filetransfer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "filehandler.h"
/*
	Forma de uso:

	invocar el programa ./filetransfer_client [host] [filename] [path] [mode]
		[host] : nombre o direccion del server
		[filename] : nombre que el archivo va a tener en el destino, ya sea subiendolo al server u obteniendolo del mismo
		[path] : ruta en donde se ubica el archivo a subir/bajar
		[mode] : 0 subir, 1 bajar
*/


void
file_prog_1(char *host, char *filename, char *path, int mode)
{
	CLIENT *clnt;
	int  *result_1;
	file_to_send *result_2;
	file_to_send  sendfile_1_arg;
	char* getfile_1_arg;

	clnt = clnt_create (host, FILE_PROG, FILE_VER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	if(!mode){
		//El usuario quiere subir un archivo al dfs
		//sendfile_1_arg.filename=malloc(sizeof(filename));
		//strcpy(sendfile_1_arg.filename,filename);

		//Preparo la estructura donde ira el buffer con los datos del archivo, su nombre y su longitud
		strcpy(sendfile_1_arg.name,filename);

		file_info* fi = malloc(sizeof(file_info));
		read_file(path,fi);
		sendfile_1_arg.data=fi->buffer;
		sendfile_1_arg.size=fi->buffer_length;
		printf("Shell: El contenido del buffer a transmitir es: %s y su longitud %i\n", sendfile_1_arg.data,sendfile_1_arg.size);

		result_1 = sendfile_1(&sendfile_1_arg, clnt);
		if (result_1 == (int *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		else{
			if(*result_1==0)
				printf("Shell: subida de archivo exitosa\n");
			else
				printf("Shell: ocurrio un error por parte del servidor\n");
		}
		//free(fi->buffer);
		//free(fi);
	}
	else if (mode){
		//El usuario quiere obtener un archivo del dfs
		getfile_1_arg = path;

		result_2 = getfile_1(&getfile_1_arg, clnt);
		if (result_2 == (file_to_send *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		int file_length = result_2->size;
		char* buffer = result_2->data;
		//char* filename = result_2->name;
		FILE* newfile;

		if (!(newfile= (FILE*) fopen(filename,"wb")) )
		//if (!(newfile= (FILE*) fopen(filename,"w")) )
		{
			printf("Shell: Error al crear el nuevo archivo\n");
			fclose(newfile);
			exit(0);
		}

		printf("Shell: El contenido del buffer es %s\n", buffer );
		int items_writen = fwrite(buffer, file_length, 1 ,newfile);
		printf("Shell: Se escribieron %i bytes en el archivo %s\n", file_length*items_writen, filename);

		fclose(newfile);

	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host, *filename, *path;
	int mode;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	printf("Shell: filename es %s\nShell: path es %s\n", argv[2], argv[3]);

	host = argv[1];
	filename = argv[2];
	path = argv[3];
	mode = atoi(argv[4]);

	printf("Shell: mode es %i\n", mode );

	file_prog_1 (host, filename, path, mode);
exit (0);
}

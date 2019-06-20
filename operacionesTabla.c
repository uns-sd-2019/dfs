#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* 
Esta tabla debe tener el siguiente formato:
path1@bitValido1@IP1 
path2@bitValido2@IP2 
...

asi tal cual (definir un archivo de nombre "file.txt" con la tabla adentro)
*/

//agregara una entrada a la tabla al final del archivo
void agregar(char * filename, char * path, char * bitValido, char * IP){	
	FILE *file = fopen ( filename, "r+" );

	//tengo que definir un buffer porque el strcat pide memoria "definida" no en el aire	
	char buffer[256];
	strncpy(buffer,path,256);
	fseek(file,0,SEEK_END);
	strcat(buffer,"@");
	strcat(buffer,bitValido);
	strcat(buffer,"@");
	strcat(buffer,IP);
	strcat(buffer,"\n");
	fputs(buffer,file);
	fflush(file);
	fclose(file);
	//fseek(archivo, desde que offset quiero reemplazar, desde que punto del archivo quiero reemplazar);
}


//modificara aquella linea de la tabla con el path, poniendo el nuevo path, nuevo bit, o IP
int modificar(char * filename, char * path, char * newPath, char * bitValido, char * IP){
	
	char delim[]="@";
	FILE *file = fopen ( filename, "r+" );
	FILE *tmp_file = fopen("temporary.txt","w");
	if ( file != NULL ){
		char line [ 128 ]; /* or other suitable maximum line size */
		int encontre = 0;
		while ( fgets ( line, sizeof line, file ) != NULL)/* read a line */
		{
			char aux[128];
			strncpy(aux,line,128);		
			
			//aca hace algo que rompe la linea
			char *ptr = strtok(line,delim); //me fijo solo con el path
			
			if(ptr != NULL){
				if(strcmp(ptr,path) != 0 || encontre == 1){
					fputs(aux,tmp_file);
				}else{
					char buffer[256];
					strncpy(buffer,newPath,256);
					strcat(buffer,"@");
					strcat(buffer,bitValido);
					strcat(buffer,"@");
					strcat(buffer,IP);
					strcat(buffer,"\n");
					fputs(buffer,tmp_file);
					encontre=1;
				}
				ptr = strtok(NULL,delim);	
			}
			
			
		}
		fflush(file);
		fclose(file);
		fclose(tmp_file);
		remove(filename);
		rename("temporary.txt",filename);

		if(encontre == 0){
			return 1;
		}
	}else{
		perror ( filename ); /* why didn't the file open? */
	}
	return 0; //exitoso
}

//eliminara aquella linea de la tabla que se corresponda con el path
int suprimir(char * filename, char * path){
	
	char delim[]="@";
	FILE *file = fopen ( filename, "r+" );
	FILE *tmp_file = fopen("temporary.txt","w");
	if ( file != NULL ){
		char line [ 128 ]; /* or other suitable maximum line size */
		int encontre = 0;
		while ( fgets ( line, sizeof line, file ) != NULL)/* read a line */
		{
			char aux[128];
			strncpy(aux,line,128);		
			
			//aca hace algo que rompe la linea
			char *ptr = strtok(line,delim); //me fijo solo con el path
			
			if(ptr != NULL){
				if(strcmp(ptr,path) != 0 || encontre == 1){
					fputs(aux,tmp_file);
				}else{
					encontre = 1;
				}
				ptr = strtok(NULL,delim);	
			}
			
			
		}
		fflush(file);
		fclose(file);
		fclose(tmp_file);
		remove(filename);
		rename("temporary.txt",filename);

		if(encontre == 0){
			return 1;
		}
	}else{
		perror ( filename ); /* why didn't the file open? */
	}
	return 0; //exitoso
}

//Leera el contenido del archivo que tiene la tabla
int leer(char * filename){
	char delim[]="@";
	FILE *file = fopen ( filename, "r" );
	if ( file != NULL ){
		char line [ 128 ]; /* or other suitable maximum line size */
		while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
		{
			fputs ( line, stdout ); /* write the line */
			char *ptr = strtok(line,delim);
			int i = 0;			
			while(ptr != NULL){
				if(i == 0){
					//ptr es nombreArchivo
					i++;
					printf("es nombre: %s\n",ptr);
				}else{
					if(i == 1){
						//ptr es bitValido
						i++;
						printf("es bitValido: %s\n",ptr);
					}else{
						//ptr es IP
						i=0;
						printf("es IP: %s\n",ptr);
					}
					
				}
				ptr = strtok(NULL,delim);
			}

		}
		fflush(file);
		fclose ( file );
		
	}else{
		perror ( filename ); 
	}
	return 0;
}

int main(int argc, char *argv[]) {
	
	char * path = "path";
	char * bit = "1";
	char * IP = "hola";
	char * filename = "file.txt";
	leer(filename);
	//agregar(filename,path,bit,IP);
	//int exitoso = modificar(filename,path,path,bit,IP);
	//int exitoso = suprimir(filename,path);
	exit (0);
}




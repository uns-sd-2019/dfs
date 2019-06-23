#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "versionado.h"

void printArchivoVers () {
	// Todos los archivos segun vers
	struct versiones_arch cursor;
	int bytes_rw;	
	FILE *fVers = fopen(PATH_VERS, "r");
	if (fVers == 0) {
		printf("[Print Versionado] error. Archivo existe?\n");
	} else {
		fseek(fVers, 0, SEEK_SET);
	
		while ( !feof(fVers) ) {
			bytes_rw = fread(&cursor, sizeof(struct versiones_arch), 1, fVers);
			if (bytes_rw > 0) { // el ultimo "leido" trae problemas
				printf("File name: %s\n", cursor.fName);
				printf("Cant: %i\n", cursor.cant);
			}		
		}		
		fclose(fVers);
	}	
}

char * versionar(char *filePath) {
	
	char fName[MAX_PATH_LENGTH];
	strcpy(fName, filePath);
	
	char *newName = malloc(sizeof(char) * MAX_PATH_LENGTH);
	char version[MAX_VERSION_LENGTH];
	
	struct versiones_arch cursor;
	FILE* newFile;	
	FILE *fVers;	
	int bytes_rw;		
	
	fVers = fopen(PATH_VERS, "rb+");
	if (fVers == 0 ) {
		printf("Se crea archivo vers\n");
		fVers = fopen(PATH_VERS, "wb+");
		if (fVers == 0 ) {
			printf("Error al crear vers\n");
		}		
	}	
	
	if (access(fName, F_OK) != -1) {
		printf("%s ya existe\n", fName);
		
		// Se busca ubicacion en archivo de versiones
		fseek(fVers, 0, SEEK_SET);
		int encontrado = 0;
		int i = 0;
		while (!feof(fVers) && !encontrado) {
			fread(&cursor, sizeof(struct versiones_arch), 1, fVers);
			if (strcmp(cursor.fName, fName) == 0) {
				encontrado = 1;			
			} else {
				i++;
			}
		}
		
		// Se actualiza cantidad en archivo de versiones
		cursor.cant++;
		fseek(fVers, sizeof(struct versiones_arch) * i, SEEK_SET);
		fwrite(&cursor, sizeof(struct versiones_arch), 1, fVers);
		
		// Se renombra el archivo segun la version. 		
		sprintf(version, "#ver%d", cursor.cant - 1);
		strcpy(newName, fName);
		strcat(newName, version);
		
		if (rename(fName, newName) == 0) {
			printf("Archivo renombrado a %s\n", newName);
		} else {
			printf("Error al renombrar\n");
		}
		
	} else {
		printf("%s NO Existe\n", fName);			
		// Se agrega entrada en archivo de versiones.	
		strcpy(cursor.fName, fName);
		cursor.cant = 1;
		fseek(fVers, 0, SEEK_END);
		bytes_rw = fwrite(&cursor, sizeof(struct versiones_arch), 1, fVers);				
		
		strcpy(newName, fName);
		
		if (bytes_rw > 0) {
			printf("Se actualizo archivo de versiones\n");
		} else {
			printf("Error al actualizar archivo de versiones\n");
		}
	}		
	
	fclose(fVers);		
	
	return newName;
}

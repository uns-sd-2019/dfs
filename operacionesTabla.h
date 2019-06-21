#ifndef OPERACIONESTABLA_H_
#define OPERACIONESTABLA_H_

#define TABLA_SIZE 64
#define rutaTabla "tablaArchivos.txt"

#include <stdio.h>

void optabla_agregar(char * filename, char * path, int bitValido, char * IP);

int optabla_modificar(char * filename, char * path, char * newPath, int bitValido, char * IP);

int optabla_suprimir(char * filename, char * path);

int optabla_leer(char * filename);

#endif

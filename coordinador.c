#include "Coordinador.h"
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLA_SIZE 64
#define MAX_NODOS 10

// Tabla provisoria.
struct celdas{
  char nombre[64];
  int disponible;
  char ipNodo[16];
};

struct nodo{
	char direccion[16];
	int carga;
};


struct nodo tablaNodos[MAX_NODOS];	//Tabla que mantendra la
int cant_nodos=0;

struct celdas tablaArchivos[TABLA_SIZE];	//Tabla que tendra un maximo de TABLA_SIZE elementos compuestos de nombre, bit de valido e IP del nodo que contenga ese archivo
int cant_archivos=0;     //Cuenta los archivos almacenados en la tabla.

//Funcion para buscar si el archivo existe a la tabla, retorna 0 en caso de que no este y la posicion en el arreglo en caso de que exista
int buscarTablaArchivos(char* archivo){
  int aparecio = 0;
  int i=0;
  for(i=0;i<cant_archivos && aparecio==0; i++) {
    if(strcmp(tablaArchivos[i].nombre,archivo))
      aparecio=i;
  }
  return aparecio;
}
 //Funcion para buscar si un nodo existe en la tabla de nodos, retorna 0 en caso de que no este y la posicion en la misma en caso de que exista
int buscarTablaNodos(char* nodo){
  int aparecio = 0;
  int i=0;
  for(i=0;i<cant_nodos && aparecio==0; i++) {
    if(strcmp(tablaNodos[i].direccion,nodo))
      aparecio=i;
  }
  return aparecio;
}

char ** rqsubir_1_svc(char ** path, struct svc_req *cliente){
  printf("El cliente pidio subir el archivo %s.\n",*path);
  //Buscar en la tabla si existe el archivo deseado, sino elegir un nuevo nodo;
  int ipLoc;
  static char * resultadoFinal;
  resultadoFinal = malloc(16);
  if (ipLoc=buscarTablaArchivos(*path)){     //Si existe el archivo
    // Informar al nodo anterior que actualice el archivo y marcar al archivo como no disponible.

    // Retornar el ip de la tabla
    strcpy(resultadoFinal,tablaArchivos[ipLoc].ipNodo);
  } else {
    // Elegir un nuevo ip (el menos usado) y enviarlo al cliente
    resultadoFinal = "192.168.1.1";      // IP para probar.
  }
  return (&resultadoFinal);
}
char ** rqbajar_1_svc(char ** path, struct svc_req *cliente){
  printf("El cliente pidio por el archivo %s.\n",*path);
  int ipLoc;
  static char * resultadoFinal;
  resultadoFinal = malloc(16);
  //Buscar en la tabla si existe el archivo deseado, sino elegir un nuevo nodo;
  if (ipLoc=buscarTablaArchivos(*path)){     //Si existe el archivo retorna la ip del nodo que lo contiene
    strcpy(resultadoFinal,tablaArchivos[ipLoc].nombre);
    return (&resultadoFinal);
    // Retornar el ip de la tabla
  } else {
    // Retorna un error.
  }
}
char ** ls_1_svc(void * vacio, struct svc_req *cliente){
  // Retornar el arbol de directorios (falta crearlo).
  static char * resultadoFinal;
  resultadoFinal = malloc(16);
  resultadoFinal = "/home/sd";      // Ruta para probar.
  return (&resultadoFinal);
}

int * subirack_1_svc(char ** path, struct svc_req *cliente){
  // Agregar a la tabla el archivo que aclara el path.
  printf("El nodo subio el archivo %s.\n",*path);
  //Retorna un valor para avisar si fue correcto o no;

  static int codigo;
  codigo = 1;
  return &codigo;
}

int * anunciarnodo_1_svc(char** direccion,struct svc_req *cliente){
	//Funcion invocada por el nodo al iniciar su ejecucion, para anunciar su direccion IP
	static int result;

	printf("Se anuncia la llegada del nodo con ip: %s\n", *direccion );

	printf("Analizando si el nodo ya se encuentra registrado...\n");

	if(!buscarTablaNodos( *direccion ))
	{
		printf("El nodo %s no se encuentra registrado, agregando a la tabla de nodos activos\n", *direccion );
		struct nodo *nuevo_nodo = malloc(sizeof(struct nodo));

		strcpy(nuevo_nodo->direccion,*direccion);
		nuevo_nodo->carga=0;
		tablaNodos[cant_nodos]=*nuevo_nodo;
		result=cant_nodos;
		cant_nodos++;
		printf("Se agrego con exito el nodo a la tabla de nodos activos\n" );

	}
	else
	{
		printf("El nodo %s ya se encuentra registrado como activo\n", *direccion );
		result=0;
	}

	return &result;
}

int* bajarnodo_1_svc(char** direccion,struct svc_req *cliente){
	//Funcion invocada por el nodo antes de terminar su ejecucion, para bajar su ip de la lista de nodos activos
	static int result;

	printf("El nodo %s quiere bajarse de la lista de nodos activos\n", *direccion );

	printf("Buscando al nodo en la lista...\n");

	int ubicacionNodo = buscarTablaNodos(*direccion);

	if(!ubicacionNodo)
	{
		printf("Error: el nodo %s no se encuentra en la lista de activos\n", *direccion );
		result=1;
	}
	else
	{
		printf("Nodo encontrado, eliminandolo de la lista...\n" );
		tablaNodos[ubicacionNodo] = tablaNodos[cant_nodos];	//Reemplazo la entrada del nodo a borrar por la ultima de la tabla
		cant_nodos--;	//Decremento la cantidad de nodos para dejar inutilizable la ultima entrada
		printf("Nodo %s eliminado con exito de la lista de activos\n", *direccion );
		result=0;
	}

	return &result;
}

#include "Coordinador.h"
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Tabla provisoria.
struct celdas{
  char nombre[64];
  int disponible;
  char ipNodo[16];
};

struct celdas tabla[30];
int contador=0;     //Cuenta los archivos almacenados en la tabla.

//Funcion para buscar si el archivo existe a la tabla, retorna 0 en caso de que no este y la posicion en el arreglo en caso de que exista
int buscar(char archivo[64]){
  int aparecio = 0;
  int i=0;
  for(i=0;i<30 && aparecio==0; i++) {
    if(strcmp(tabla[i].nombre,archivo))
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
  if (ipLoc=buscar(*path)){     //Si existe el archivo
    // Informar al nodo anterior que actualice el archivo y marcar al archivo como no disponible.

    // Retornar el ip de la tabla
    strcpy(resultadoFinal,tabla[ipLoc].ipNodo);
  } else {
    // Elegir un nuevo ip (el menos usado) y enviarlo al cliente
    resultadoFinal = "192.168.1.1";      // IP para probar.
  }
  return (&resultadoFinal);
}
char ** rqbajar_1_svc(char ** path, struct svc_req *cliente){
  printf("El nodo pidio por el archivo %s.\n",*path);
  int ipLoc;
  static char * resultadoFinal;
  resultadoFinal = malloc(16);
  //Buscar en la tabla si existe el archivo deseado, sino elegir un nuevo nodo;
  if (ipLoc=buscar(*path)){     //Si existe el archivo retorna la ip del nodo que lo contiene
    strcpy(resultadoFinal,tabla[ipLoc].nombre);
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
  resultadoFinal = "/home/sd";      // IP para probar.
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

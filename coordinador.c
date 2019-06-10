#include "CoordinadorCliente.h"
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char ** rqsubir_1_svc(char ** path, struct svc_req *cliente){
  printf("El nodo pidio por el archivo %s.\n",*path);
  //Buscar en la tabla si existe el archivo deseado, sino elegir un nuevo nodo;
  if (0){     //Si existe el archivo
    // Informar al nodo anterior que actualice el archivo y marcar al archivo como no disponible.

    // Retornar el ip de la tabla
  } else {
    // Elegir un nuevo ip (el menos usado) y enviarlo al cliente
    static char * resultadoFinal;
    resultadoFinal = malloc(16);
    resultadoFinal = "192.168.1.1";      // IP para probar.
    return (&resultadoFinal);
  }
}
char ** rqbajar_1_svc(char ** path, struct svc_req *cliente){
  printf("El nodo pidio por el archivo %s.\n",*path);
  //Buscar en la tabla si existe el archivo deseado, sino elegir un nuevo nodo;
  if (1){     //Si existe el archivo retorna la ip del nodo que lo contiene
    static char * resultadoFinal;
    resultadoFinal = malloc(16);
    resultadoFinal = "192.168.1.1";      // IP para probar.
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

int * subirACK_1_svc(char ** path, struct svc_req *cliente){
  // Agregar a la tabla el archivo que aclara el path.
  printf("El nodo subio el archivo %s.\n",*path);
  //Retorna un valor para avisar si fue correcto o no;
  static int codigo;
  codigo = 1;
  return &codigo;
}

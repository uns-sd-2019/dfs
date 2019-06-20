#include "Coordinador.h"
#include "operacionesTabla.h"
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLA_SIZE 64
#define MAX_NODOS 10

// Tabla provisoria.
struct celda{
  char nombre[64];
  int disponible;
  char ipNodo[16];
};

struct nodo{
	char direccion[16];
	int carga;
};

//hay que crear este archivo.txt y dejarlo ahi agregado
char * tablaArchivosFile = "tablaArchivos.txt";

struct nodo tablaNodos[MAX_NODOS];	//Tabla que mantendra la 
int cant_nodos=0;

struct celda tablaArchivos[TABLA_SIZE];	//Tabla que tendra un maximo de TABLA_SIZE elementos compuestos de nombre, bit de valido e IP del nodo que contenga ese archivo
int cant_archivos=0;     //Cuenta los archivos almacenados en la tabla.

//FIX: OJO CON ESTA FUNCIÓN, SI EL ARCHIVO ESTA EN LA POSICIÓN 0 DE LA TABLA, ENTONCES SERÍA CONSIDERADO ERROR
//ARREGLAR
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

//FIX: OJO CON ESTA FUNCIÓN, SI EL NODO ESTA EN LA POSICIÓN 0 DE LA TABLA, ENTONCES SERÍA CONSIDERADO ERROR
//ARREGLAR
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



//Función que agrega en la tabla local de archivos, y en el archivo de consistencia de tabla, el path del archivo, el bit y la ip del nodo que se encarga
void agregarEnTablaArchivos(char * path, int bitValido, char * ipNodo){
	struct celda *nueva_celda = malloc(sizeof(struct celda));
	strcpy(nueva_celda->nombre,path);
	nueva_celda->disponible = bitValido;
	strcpy(nueva_celda->ipNodo,ipNodo);
	tablaArchivos[cant_archivos]=*nueva_celda;
	cant_archivos++;
	
	//agrego en la tabla de archivos para mantener consistencia
	if(bitValido==0){
		optabla_agregar(tablaArchivosFile,path,0,ipNodo);
	}else{
		optabla_agregar(tablaArchivosFile,path,1,ipNodo);
	}
	
	//aumento la carga del nodo ya que le agregue un archivo a el
	int i;
	for(i=0;i<cant_nodos; i++) {
		if(strcmp(tablaNodos[i].direccion,ipNodo)){
		  tablaNodos[i].carga = tablaNodos[i].carga + 1;
		  break;
		}
	}

}

//elimina de la tabla de archivos y del archivo de consistencia la entrada pasada
int eliminarDeTablaArchivos(char * path){
	int pos = buscarTablaArchivos(path);
	if(pos){
		//reemplazo directamente los structs
		strcpy(tablaArchivos[pos].nombre,tablaArchivos[cant_archivos-1].nombre);
		strcpy(tablaArchivos[cant_archivos-1].nombre,""); //limpio por las dudas esta componente
		strcpy(tablaArchivos[pos].ipNodo,tablaArchivos[cant_archivos-1].ipNodo);
		strcpy(tablaArchivos[cant_archivos-1].ipNodo,"");
		tablaArchivos[pos].disponible = tablaArchivos[cant_archivos-1].disponible;
		//elimino la entrada del archivo de consistencia
		optabla_suprimir(tablaArchivosFile, path);
		cant_archivos--;
		return 0;
	}else{
		//no lo encontró en la tabla de archivos
		return 1;
	}
}
//Función para encontrar el nodo que tenga menor carga, retorna la IP del nodo con menor carga
char* obtenerNodoMenosUsado(){
	int min = 100000;
	static char * resultadoFinal;
	resultadoFinal = malloc(16);
	int i;
	for(i=0;i<cant_nodos; i++) {
		if(tablaNodos[i].carga < min){
			min=tablaNodos[i].carga;
			strcpy(resultadoFinal,tablaNodos[i].direccion);
		}
	}
	return resultadoFinal;
}

char ** rqsubir_1_svc(char ** path, struct svc_req *cliente){
  printf("El cliente pidio subir el archivo %s.\n",*path);
  //Buscar en la tabla si existe el archivo deseado, sino elegir un nuevo nodo;
  int ipLoc;
  static char * resultadoFinal;
  resultadoFinal = malloc(16);
  if (ipLoc=buscarTablaArchivos(*path)){     //Si existe el archivo
    // Informar al nodo anterior que actualice el archivo y marcar al archivo como no disponible, en la tabla y en el archivo de consistencia.
	tablaArchivos[ipLoc].disponible = 0;
	optabla_modificar(tablaArchivosFile,tablaArchivos[ipLoc].nombre,tablaArchivos[ipLoc].nombre,tablaArchivos[ipLoc].disponible,tablaArchivos[ipLoc].ipNodo);
    // Retornar el ip de la tabla
    strcpy(resultadoFinal,tablaArchivos[ipLoc].ipNodo);
  } else {
    // Elegir un nuevo ip (el menos usado) y enviarlo al cliente
    strcpy(resultadoFinal,obtenerNodoMenosUsado());
	
    resultadoFinal = "192.168.1.1";      // IP para probar. HAY QUE BORRAR ESTA LINEA LUEGO
	
	//agrego en las tablas del coordinador el nuevo archivo, deberé modificar el bit de válido y ponerlo en 1 una vez subido HACER LUEGO
	agregarEnTablaArchivos(*path,0,resultadoFinal);
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


//HACER FIX AQUI, TERMINAR
int * subirack_1_svc(char ** path, struct svc_req *cliente){
  // Agregar a la tabla el archivo que aclara el path. NO ME CONVIENE HACER ESTO ACA PORQUE NO TENGO EL NODO Y NO SE CUAL FUE
  //EL MENOS USADO QUE SE ELIGIÓ, ME CONVIENE AGREGAR ANTES A LA TABLA EL ARCHIVO y si falla en algun lado, notificar y aquí eliminar esta entrada
  //agregada
  
  //VER COMO HACER PARA VERIFICAR SI FUE CORRECTO O NO, CUANDO TENGO UNA VARIABLE QUE ME INDICA SI ES CORRECTO, ENTONCES:
  int esCorrecto = 1;
  static int codigo = 0;
  if(esCorrecto){
	  codigo = 1;
	  printf("El nodo subio el archivo %s.\n",*path);
  }else{
	  //elimino de la tabla de archivos a este archivo agregado, y de el archivo de consistencia ya que hubo un error
	  if(eliminarDeTablaArchivos(*path)){
		  //que tire algun cartel de error
	  }
  }
  

 
  //codigo = 1;
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

//FIX: debería borrar los archivos de la tabla y del archivo que se corresponden con este nodo, ya que no esta activo
//y luego debería redistribuir los archivos entre los demás nodos. TERMINAR

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
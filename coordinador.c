#include "Coordinador.h"
#include "operacionesTabla.h"
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
char * tablaArchivosFile = rutaTabla;

struct nodo tablaNodos[MAX_NODOS];	//Tabla que mantendra la 
int cant_nodos=0;

struct celda tablaArchivos[TABLA_SIZE];	//Tabla que tendra un maximo de TABLA_SIZE elementos compuestos de nombre, bit de valido e IP del nodo que contenga ese archivo
int cant_archivos=0;     //Cuenta los archivos almacenados en la tabla.

void listarTablaArchivos()
//Procedimiento con fines de debugging
{
	printf("Tabla de archivos:\n");
	for (int i = 0; i < cant_archivos; i++)
	{
		printf("\tRuta: %s, Valido: %i, Nodo: %s\n", tablaArchivos[i].nombre, tablaArchivos[i].disponible, tablaArchivos[i].ipNodo );
	}
}

//FIX: OJO CON ESTA FUNCIÓN, SI EL ARCHIVO ESTA EN LA POSICIÓN 0 DE LA TABLA, ENTONCES SERÍA CONSIDERADO ERROR
//ARREGLAR
//FIX PROPUESTO: GUARDAR EN LA VARIABLE POSICION LA MISMA SI ES QUE APARECE Y RETORNARLA, SINO RETORNAR -1
//Funcion para buscar si el archivo existe a la tabla, retorna 0 en caso de que no este y la posicion en el arreglo en caso de que exista
int buscarTablaArchivos(char* archivo){
  int aparecio = 0;
  int posicion = 0;
  int i=0;
  for(i=0;i<cant_archivos && aparecio==0; i++) {
    if(strcmp(tablaArchivos[i].nombre,archivo)==0)	//STRCMP RETORNA 0 SI SON IGUALES D:
    {
      aparecio=1;
  	  posicion=i;
  	}
  }
  if (aparecio)
  	return posicion;
  else
  	return -1;
}

//FIX: OJO CON ESTA FUNCIÓN, SI EL NODO ESTA EN LA POSICIÓN 0 DE LA TABLA, ENTONCES SERÍA CONSIDERADO ERROR
//ARREGLAR
//FIX PROPUESTO: GUARDAR EN LA VARIABLE POSICION LA MISMA SI ES QUE APARECE Y RETORNARLA, SINO RETORNAR -1
//Funcion para buscar si un nodo existe en la tabla de nodos, retorna 0 en caso de que no este y la posicion en la misma en caso de que exista
int buscarTablaNodos(char* nodo){
  int aparecio = 0;
  int posicion = 0;
  int i=0;
  for(i=0;i<cant_nodos && aparecio==0; i++) {
    if(strcmp(tablaNodos[i].direccion,nodo)==0)	//STRCMP RETORNA 0 SI SON IGUALES D:
    {
      aparecio=1;
  	  posicion=i;
  	}
  }
  if (aparecio)
  	return posicion;
  else
  	return -1;
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
	/*if(bitValido==0){
		optabla_agregar(tablaArchivosFile,path,0,ipNodo);
	}else{
		optabla_agregar(tablaArchivosFile,path,1,ipNodo);
	}
	*/
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
	static char * resultadoFinal;
	
	printf("═══════════════════════════ rqsubir ═══════════════════════════\n");
	
	// Si el caracter inicial es '/' se elimina. Rutas relativas en dfs. 
    if ((*path)[0] == '/') {
		strcpy(*path, &((*path)[1]));
	}    
	
	printf("El cliente pidio subir el archivo %s.\n", *path);
	//Buscar en la tabla si existe el archivo deseado, sino elegir un nuevo nodo;
	int ipLoc;	
	listarTablaArchivos();
	resultadoFinal = malloc(16);
	char* v_path = *path;


	if ( (ipLoc=buscarTablaArchivos(v_path)) !=-1 ) {
		printf("Ya existe el archivo %s, se sube nueva version y se renombra anterior\n", v_path);
		//TODO Aca va el IP del nodo que ya tenia el archivo.
		printf("El archivo debe ir al nodo %s\n", tablaArchivos[ipLoc].ipNodo );
		strcpy(resultadoFinal , tablaArchivos[ipLoc].ipNodo); 
	} else {
		// Elegir un nuevo ip (el menos usado) y enviarlo al cliente
		//strcpy(resultadoFinal,obtenerNodoMenosUsado());
		printf("Todavia no existe el archivo %s\n", v_path);
		
		strcpy(resultadoFinal,obtenerNodoMenosUsado());
		
		//agrego en las tablas del coordinador el nuevo archivo, deberé modificar el bit de válido y ponerlo en 1 una vez subido HACER LUEGO
		agregarEnTablaArchivos(v_path, 0, resultadoFinal);
	}		

	listarTablaArchivos();
	
	return (&resultadoFinal);
}

char ** rqbajar_1_svc(char ** path, struct svc_req *cliente){
	
	printf("═══════════════════════════ rqbajar ═══════════════════════════\n");
	
	// Si el caracter inicial es '/' se elimina. Rutas relativas en dfs. 
    if ((*path)[0] == '/') {
		strcpy(*path, &((*path)[1]));
	} 
	
	char* v_path = *path;
	printf("El cliente pidio por el archivo %s.\n",v_path);
	int ipLoc;
	static char * resultadoFinal;
	resultadoFinal = malloc(16);
	//Buscar en la tabla si existe el archivo deseado, sino elegir un nuevo nodo;
	ipLoc=buscarTablaArchivos(v_path);
	int is_valid= tablaArchivos[ipLoc].disponible;
	if ( ((ipLoc != -1) && is_valid) ){     	//Si existe el archivo y es valido retorna la ip del nodo que lo contiene
	strcpy(resultadoFinal,tablaArchivos[ipLoc].ipNodo);		//Aca se copia la direccion del nodo que mantiene ese archivo
	printf("El archivo %s lo tiene el nodo %s\n", v_path, resultadoFinal );
	return (&resultadoFinal);
	// Retornar el ip de la tabla
	} else {
		printf("No se encontro el archivo %s en  la tabla de archivos\n", v_path );
		listarTablaArchivos();
	// Retorna un error. NULL pointer?
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
int * subirack_1_svc(char ** path, struct svc_req *cliente) {	
	static int codigo = 0;
	
	printf("═══════════════════════════ subirack ═══════════════════════════\n");
		
	char *needle = "#ver";
	char *ptr = strstr(*path, needle);
	if (ptr != NULL) 
	{	
		char *delim = "#";
		char *pathAux = malloc(sizeof(char) * strlen(*path) + 1);
		strcpy(pathAux, *path);
		// Se modifica el primer arg
		char *token = strtok(pathAux, delim);
		printf("Nueva version de %s disponible\n", token);
		
		// TODO: cambiar una vez que se pueda obtener IP segun path.
		char *dirIP = tablaArchivos[buscarTablaArchivos(token)].ipNodo;
		printf("La nueva version se ubicada en el nodo %s y con el nombre %s\n", dirIP, *path );
		agregarEnTablaArchivos(*path, 1, dirIP);
		int posEnTablaNodos=buscarTablaNodos(dirIP);
		if(posEnTablaNodos != -1)
		{
			tablaNodos[posEnTablaNodos].carga++;
			codigo=1;
		}
		else
		{
			printf("Error, no se encuentra el nodo en la tabla de nodos\n");
		}

		free(pathAux);
		codigo = 1;
	} 
	else 
	{
		printf("Un nodo me quiere notificar sobre la subida del archivo %s\n", *path );
		
		// Agregar a la tabla el archivo que aclara el path. NO ME CONVIENE HACER ESTO ACA PORQUE NO TENGO EL NODO Y NO SE CUAL FUE
		//EL MENOS USADO QUE SE ELIGIÓ, ME CONVIENE AGREGAR ANTES A LA TABLA EL ARCHIVO y si falla en algun lado, notificar y aquí eliminar esta entrada
		//agregada

		//VER COMO HACER PARA VERIFICAR SI FUE CORRECTO O NO, CUANDO TENGO UNA VARIABLE QUE ME INDICA SI ES CORRECTO, ENTONCES:
		int esCorrecto = buscarTablaArchivos(*path);				
		if(esCorrecto!=-1){
		  tablaArchivos[esCorrecto].disponible=1;
		  codigo = 1;		  
		}else{
		  //elimino de la tabla de archivos a este archivo agregado, y de el archivo de consistencia ya que hubo un error
		  //if(eliminarDeTablaArchivos(*path)){
			  //que tire algun cartel de error
			printf("No encuentro el archivo que se quiere confirmar como subido.\n");
		}		
	}
	
	if (codigo ==1 ) {
		printf("El nodo me confirma que subio el archivo %s.\n",*path);
		listarTablaArchivos();
	}
	
	return &codigo;
}

int * anunciarnodo_1_svc(char** direccion,struct svc_req *cliente){
	//Funcion invocada por el nodo al iniciar su ejecucion, para anunciar su direccion IP
	static int result;
	
	printf("═══════════════════════════ anunciarnodo ═══════════════════════════\n");

	printf("Se anuncia la llegada del nodo con ip: %s\n", *direccion );

	printf("Analizando si el nodo ya se encuentra registrado...\n");

	if(buscarTablaNodos( *direccion )==-1)
	{
		printf("El nodo %s no se encuentra registrado, agregando a la tabla de nodos activos\n", *direccion );
		struct nodo *nuevo_nodo = malloc(sizeof(struct nodo));

		//Agrego nodo a memoria estable
			FILE *fd = fopen("./ArchivoNodos.txt","ab");
			if(fd == NULL)
				printf("Coordinador: Error al crear ArchivoNodos.txt\n");
			else
				fprintf(fd, "%s\n", *direccion );
			fclose(fd);
		///////////////////////////////	

		
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
	}

	return &result;
}

//FIX: debería borrar los archivos de la tabla y del archivo que se corresponden con este nodo, ya que no esta activo
//y luego debería redistribuir los archivos entre los demás nodos. TERMINAR

int* bajarnodo_1_svc(char** direccion,struct svc_req *cliente){
	//Funcion invocada por el nodo antes de terminar su ejecucion, para bajar su ip de la lista de nodos activos
	static int result;
	
	printf("═══════════════════════════ bajarnodo ═══════════════════════════\n");

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





// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
// operacionesTabla.c:
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
/*
* 
Esta tabla debe tener el siguiente formato:
path1@bitValido1@IP1 
path2@bitValido2@IP2 
...

asi tal cual (definir un archivo de nombre "file.txt" con la tabla adentro)
*/

//agregara una entrada a la tabla al final del archivo
void optabla_agregar(char * filename, char * path, int bitValido, char * IP){	
	FILE *file = fopen ( filename, "r+" );

	//tengo que definir un buffer porque el strcat pide memoria "definida" no en el aire	
	char buffer[256];
	strncpy(buffer,path,256);
	fseek(file,0,SEEK_END);
	strcat(buffer,"@");
	bitValido ? strcat(buffer,"1") : strcat(buffer,"0");
	strcat(buffer,"@");
	strcat(buffer,IP);
	strcat(buffer,"\n");
	fputs(buffer,file);
	fflush(file);
	fclose(file);
	//fseek(archivo, desde que offset quiero reemplazar, desde que punto del archivo quiero reemplazar);
}


//modificara aquella linea de la tabla con el path, poniendo el nuevo path, nuevo bit, o IP
int optabla_modificar(char * filename, char * path, char * newPath, int bitValido, char * IP){
	
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
					bitValido ? strcat(buffer,"1") : strcat(buffer,"0");
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
int optabla_suprimir(char * filename, char * path){
	
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
int optabla_leer(char * filename){
	printf("Chequeo archivo txt por si hay algo escrito:\n");
	char delim[]="@";
	FILE *file = fopen ( filename, "r" );	
	if ( file != NULL ){
		char line [ 128 ]; /* or other suitable maximum line size */
		while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
		{
			//fputs ( line, stdout ); /* write the line */
			char *ptr = strtok(line,delim);
			int i = 0;
			struct celda *nueva_celda = malloc(sizeof(struct celda)); // Donde se va a insertar los datos que leo.
			while(ptr != NULL){
				if(i == 0){
					//ptr es nombreArchivo
					i++;
					strcpy(nueva_celda->nombre,ptr); // insert
				}else{
					if(i == 1){
						//ptr es bitValido
						i++;
						nueva_celda->disponible = atoi(ptr); // insert
					}else{
						//ptr es IP
						i=0;
						strcpy(nueva_celda->ipNodo,ptr); // insert
					}
					
				}
				ptr = strtok(NULL,delim);				
			}			
			//Inserts en el array de los datos conseguidos.
			//NO insertar en el txt porq ya están ahi.	
			tablaArchivos[cant_archivos]=*nueva_celda; // Add al array
			cant_archivos++;
		}
		fflush(file);
		fclose ( file );
		
	}else{
		perror ( filename ); 
	}
	listarTablaArchivos(); // << Debug, se podria borrar.
	return 0;
}

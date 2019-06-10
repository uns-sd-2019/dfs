/*
*****IMPORTANTE*****
Puede haber errores en el manejo de las llamadas a rpc por el tema de punteros!.
*/


#include "CoordinadorCliente.h"
#include "NodoCliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <string.h>
//Al iniciar el cliente se carga la direccion ip del servidor: cliente(ip)
int main(int argc,char *argv[]) {
  int opcion=0;
  CLIENT *clntcoor;   // Se usara para comunicarse con el coordinador.
  CLIENT *clntnodo;   // Se usara para comunicarse con el nodo a relacionarse.
  char *server;
  char *nodo;         // Se usara para almacenar la direccion ip del nodo a relacionarse.
  char ipNodo[15];
  //Caso en que no se paso unicamente la direccion ip del servidor por parametro.
  if (argc != 2) {
    printf ("Error cantidad de parametros.\n");
    exit(1);
  }
  //Capturar ip del servidor.
  server = argv[1];
  /*
  clntcoor = clnt_create(server,COORDINADORCLIENTE, VERSION1, "TCP");
  if(clntcoor == (CLIENT *) NULL) {
    printf("No hubo conexión\n");
    exit(1);
  }
  */
  while(1){
    //Imprimir las operaciones posibles.
    printf("¿Que desea realizar?\n\
            \t 1 - Listar árbol de directorios.\n\
            \t 2 - Cargar un archivo al DFS.\n\
            \t 3 - Descargar un archivo del DFS.\n\
            \t 4 - Salir.\n");
    //Capturar la opcion deseada.
    scanf("%i",&opcion);
    if (opcion==4)
      break;
    //Actuar acorde, si la opcion no es correcta se vuelve a pedir la operacion.
    switch(opcion){
      // Opcion 1: ls().
      case 1:{
        void *vacio;  //Puntero vacio que se debe enviar en la consulta RPC.
        ls_1(vacio,clntcoor);
        printf ("El arbol de directorios es:\n");
      }break;
      // Opcion 2: subir(path,file)
      case 2:{
        //Obtenemos la ruta del archivo que desea subir el usuario.
        printf("Ingrese la ruta del archivo en el DFS.\n");
        char *rutaDFS = (char*) malloc(64);
        scanf("%s",rutaDFS);
        //Obtenemos donde se ubicara el archivo.
        printf("Ingrese la ruta del archivo local.\n");
        char *ruta = (char*) malloc(64);
        scanf("%s",ruta);
        //Pedimos al coordinador la direccion al nodo correspondiente.
        nodo = *rqsubir_1(&ruta,clntcoor);
        //Iniciamos la conexión con el nodo:
        clntnodo = clnt_create(nodo,NODOCLIENTE, VERSION1, "TCP");
        if(clntnodo == (CLIENT *) NULL) {
          printf("No hubo conexión con el nodo.\n");
          exit(1);
        }
        //Armar el struct para pasar como parametro.
        paths rutas;
        strcpy(rutas.pathDFS,rutaDFS);
        strcpy(rutas.filename,ruta);
        //Enviamos el archivo al nodo.
        subir_1(&rutas,clntnodo);
      }break;
      // Opcion 3: bajar(,file)

      case 3:{
        //Obtenemos la ruta del archivo que desea descargar el usuario.
        printf("Ingrese la ruta del archivo en el DFS.\n");
        char *rutaDFS = (char*) malloc(64);
        scanf("%s",rutaDFS);
        //Pedimos al coordinador la direccion al nodo correspondiente.
        nodo = *rqbajar_1(&rutaDFS,clntcoor);
        //Iniciamos la conexión con el nodo:
        clntnodo = clnt_create(nodo,NODOCLIENTE, VERSION1, "TCP");
        if(clntnodo == (CLIENT *) NULL) {
          printf("No hubo conexión con el nodo.\n");
          exit(1);
        }
        //Enviamos el archivo al nodo.
        bajar_1(&rutaDFS,clntnodo);
      }break;
    }
  }
}

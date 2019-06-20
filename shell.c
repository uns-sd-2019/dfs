#include "filehandler.h"
#include "Coordinador.h"
#include "Nodo.h"
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
  char ipNodo[16];
  //Caso en que no se paso unicamente la direccion ip del servidor por parametro.
  if (argc != 2) {
    printf ("Error cantidad de parametros.\n");
    exit(1);
  }
  //Capturar ip del servidor.
  server = argv[1];
  /*
  clntcoor = clnt_create(server,COORDINADOR, VERSION1, "TCP");
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
        clntnodo = clnt_create(nodo,NODO, VERSION1, "TCP");
        if(clntnodo == (CLIENT *) NULL) {
          printf("No hubo conexión con el nodo.\n");
          exit(1);
        }
        //Armar el struct para pasar como parametro.
        int resultadoSubir;
        file_to_send  sendfile;
        strcpy(sendfile.name,rutaDFS);
        file_info* fi = malloc(sizeof(file_info));
        read_file(ruta,fi);
        sendfile.data=fi->buffer;
    		sendfile.size=fi->buffer_length;
    		printf("Shell: El contenido del buffer a transmitir es: %s y su longitud %i\n", sendfile.data,sendfile.size);
        resultadoSubir=*subir_1(&sendfile,clntnodo);
        if (&resultadoSubir == (int*) NULL) {
//No se que es esta linea    			clnt_perror (clnt, "call failed");
    		}

    		else{
    			if(resultadoSubir==0)
    				printf("Shell: subida de archivo exitosa\n");
    			else
    				printf("Shell: ocurrio un error por parte del servidor\n");
    		}
/*
        strcpy(rutas.pathDFS,rutaDFS);
        strcpy(rutas.filename,ruta);
*/
        //Enviamos el archivo al nodo.
        //subir_1(&rutas,clntnodo);
      }break;
      // Opcion 3: bajar(,file)

      case 3:{
        //Obtenemos la ruta del archivo que desea descargar el usuario.
        printf("Ingrese la ruta del archivo en el DFS.\n");
        char *rutaDFS = (char*) malloc(64);
        scanf("%s",rutaDFS);
        printf("Ingrese el nombre del archivo en su sistema.\n");
        char *ruta = (char*) malloc(64);
        scanf("%s",ruta);
        //Pedimos al coordinador la direccion al nodo correspondiente.
        nodo = *rqbajar_1(&rutaDFS,clntcoor);
        //Iniciamos la conexión con el nodo:
        clntnodo = clnt_create(nodo,NODO, VERSION1, "TCP");
        if(clntnodo == (CLIENT *) NULL) {
          printf("No hubo conexión con el nodo.\n");
          exit(1);
        }
        //Creamos la estructura para recibir al archivo.
        file_to_send *recivedFile;
        //Recibimos el archivo al nodo.
        recivedFile = bajar_1(&rutaDFS,clntnodo);

        if (recivedFile == (file_to_send *) NULL) {
//No se que es esta linea    			clnt_perror (clnt, "call failed");
    		}
        //Guardamos el archivo en el almacenamiento permanente.
        int file_length = recivedFile->size;
    		char* buffer = recivedFile->data;
    		FILE* newfile;
        if (!(newfile= (FILE*) fopen(rutaDFS,"wb")) )
    		//if (!(newfile= (FILE*) fopen(filename,"w")) )
    		{
    			printf("Shell: Error al crear el nuevo archivo\n");
    			fclose(newfile);
    			exit(0);
    		}
        printf("Shell: El contenido del buffer es %s\n", buffer );
        int items_writen = fwrite(buffer, file_length, 1 ,newfile);
        printf("Shell: Se escribieron %i bytes en el archivo %s\n", file_length*items_writen, ruta);

        fclose(newfile);
      }break;
    }
  }
}

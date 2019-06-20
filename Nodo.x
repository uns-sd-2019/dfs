/*
  subir: Envia el archivo al nodo correspondiente, primer parametro sera la ruta y el nombre en el DFS, segundo parametro es el archivo real.
  bajar: El cliente le solicita el archivo al nodo, el parametro es la ruta y el nombre del archivo en el DFS.
*/

const MAXLEN = 1024;
const FILELEN = 1000000000;
typedef string my_file<FILELEN>;

typedef char filename[MAXLEN];

struct file_to_send{
	filename name;
	my_file data;
	int size;
};

/*
	Lo comento por ahora, no se si lo vamos a usar
struct paths{
  char pathDFS[64];
  char filename[64];
};
*/
program NODO{
  version VERSION1{
    int subir(file_to_send) = 1;
    file_to_send bajar(string) = 2;
  }=1;
}=0x20000003;

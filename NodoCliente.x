/*
  subir: Envia el archivo al nodo correspondiente, primer parametro sera la ruta y el nombre en el DFS, segundo parametro es el archivo real.
  bajar: El cliente le solicita el archivo al nodo, el parametro es la ruta y el nombre del archivo en el DFS.
*/
struct paths{
  char pathDFS[64];
  char filename[64];
};
program NODOCLIENTE{
  version VERSION1{
    int subir(paths) = 1;
    string bajar(string) = 2;
  }=1;
}=0x20000003;

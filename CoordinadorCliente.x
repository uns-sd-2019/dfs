/*
  rqsubir: El cliente pide ip para encontrar el archivo al coordinador.
  rqbajar: El cliente pide ip para subir un archivo al coordinador.
  ls: El cliente pide arbol de directorios al coordinador.
*/
program COORDINADORCLIENTE{
  version VERSION1{
    string rqsubir(string) = 1;
    string rqbajar(string) = 2;
    string ls() = 3;
  }=1;
}=0x200000001;

/*
  rqsubir: El cliente pide ip para encontrar el archivo al coordinador.
  rqbajar: El cliente pide ip para subir un archivo al coordinador.
  ls: El cliente pide arbol de directorios al coordinador.
  subirACK: El nodo avisa al coordinador que un archivo fue cargado al DFS de manera correcta.
  (Hay que ver el tipo de retorno (solo informa errores))
*/
/*  Implementacion anterior de ls
const MAXRUTA = 64;
const CANTRUTAS = 100;
typedef string ruta<MAXRUTA>;
struct rutasListar {
  int cantRutas;
  ruta rutas[CANTRUTAS];
};
*/
program COORDINADOR{
  version VERSION1{
    string rqsubir(string) = 1;
    string rqbajar(string) = 2;
    string ls() = 3;
    int subirack(string) = 4;
    int anunciarnodo(string) = 5;
    int bajarnodo(string) = 6;
  }=1;
}=0x20000001;

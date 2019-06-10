/*
  subirACK: El nodo avisa al coordinador que un archivo fue cargado al DFS de manera correcta.
  (Hay que ver el tipo de retorno (solo informa errores))
*/
program COORDINADORNODO{
  version VERSION1{
    int subirACK(string) = 1;
  }=1;
}=0x20000002;

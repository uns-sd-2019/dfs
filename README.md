# DFS
Distributed File System

### ¿Cómo ejecutar?

1. Abrir una terminal en la carpeta donde esten los codigos fuentes y ejecutar
      - make [fedora | debian]

2. Se obtendran 3 ejecutables: coordinador, nodo y shell. Ubicarlos en las 
   maquinas que se desee formen parte del DFS. Tiene que haber un unico 
   coordinador, tantos nodos y sheels como se deseen. Luego ejecutar donde
   corresponda:
      - ./coordinador
      - ./nodo [DIR_IP_NODO] [DIR_IP_COORD]
      - ./shell [DIR_IP_COORD]

3. La interfaz de consola brinda mensajes de guia para utilizar el 
   sistema. 
  

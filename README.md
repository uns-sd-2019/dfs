# dfs
Distributed File System

----

## To Do List
_last update 2019-06-21 23:23_

**Soportar carpetas en el dfs**

En el coordinador creo que no cambiaria nada, la ruta se almacena de la forma "directorio/archivo" y no hay ningun problema
En el nodo, en la implementacion de la funcion subir archivo antes de crear el archivo con el nombre que recibe como ruta, ver si la ruta es de la forma directorio/archivo, y si es asi crear primero el directorio (enrealidad no se si se crearia con el fopen, puede ser no lo probe) y luego si crear el archivo dentro


**Soportar versionado de archivos**

El coordinador puede mantener las versiones?
Las maneja solo el nodo o debe haber comunicacion entre el coordinador y el nodo?
Las versiones quedan visibles para el usuario al listar los archivos, o es solo en caso de tener una hipotetica copia de respaldo que se guardan?


**Listado de archivos**

en orden alfabetico o algun otro?
por directorio, o todo junto?


**Guardar cada insercion/eliminacion de archivos en el archivo persistente**

ya se soporta levantar el coordinador y que lea del archivo persistente, ahora hay que hacer que cada vez que se sube un archivo, o se elimina (pueden eliminarse?) se actualice este archivo persistente


**Guardar la lista de nodos activos en disco**

es necesario hacerlo?

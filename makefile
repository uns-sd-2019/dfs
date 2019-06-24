help:
	@echo "Formato de uso:"
	@echo "# make debian : Compilar para sistemas operativos Debian"
	@echo "# make fedora : Compilar para sistemas operativos Fedora"
	@echo "# make clean : Eliminar archivos compilados, dejando los fuentes"
.PHONY: help

fedora:
	make all PARAM="-ltirpc -I/usr/include/tirpc"

debian:
	make all PARAM=""

all: shell coordinador nodo
.PHONY: all

shell: shell.c Nodo_clnt.c Nodo.h Nodo_xdr.c Coordinador_clnt.c Coordinador.h filehandler.c
	gcc ${PARAM} -lnsl -o shell shell.c Coordinador_clnt.c Nodo_clnt.c Coordinador.h Nodo.h Nodo_xdr.c filehandler.c

nodo: nodo.c Nodo_svc.c Nodo.h Nodo_xdr.c Coordinador_clnt.c Coordinador.h filehandler.c versionado.c
	gcc ${PARAM} -lnsl -o nodo nodo.c Coordinador_clnt.c Nodo_svc.c Coordinador.h Nodo.h Nodo_xdr.c filehandler.c versionado.c

coordinador: coordinador.c Coordinador_svc.c Coordinador.h listar.c
	gcc ${PARAM} -lnsl -o coordinador coordinador.c Coordinador_svc.c Coordinador.h listar.c

Nodo_clnt.c Nodo_xdr.c: Nodo.x
	rpcgen -l -o Nodo_clnt.c Nodo.x
	rpcgen -c -o Nodo_xdr.c Nodo.x

Coordinador_clnt.c Coordinador.h: Coordinador.x
	rpcgen -h -o Coordinador.h Coordinador.x
	rpcgen -l -o Coordinador_clnt.c Coordinador.x

clean :
	$(RM) coordinador shell nodo Coordinador_clnt.c Coordinador.h Nodo_xdr.c Nodo_clnt.c
.PHONY: clean

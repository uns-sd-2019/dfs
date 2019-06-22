.PHONY: all
all: shell coordinador nodo
.PHONY: clean
clean :
	rm shell coordinador Coordinador_clnt.c Coordinador.h Nodo_xdr.c Nodo_clnt.c nodo

shell: shell.c Nodo_clnt.c Nodo.h Nodo_xdr.c Coordinador_clnt.c Coordinador.h filehandler.c
	gcc -lnsl -o shell shell.c Coordinador_clnt.c Nodo_clnt.c Coordinador.h Nodo.h Nodo_xdr.c filehandler.c

nodo: nodo.c Nodo_svc.c Nodo.h Nodo_xdr.c Coordinador_clnt.c Coordinador.h filehandler.c
	gcc -lnsl -o nodo nodo.c Coordinador_clnt.c Nodo_svc.c Coordinador.h Nodo.h Nodo_xdr.c filehandler.c

coordinador: coordinador.c Coordinador_svc.c Coordinador.h
	gcc -lnsl -o coordinador coordinador.c Coordinador_svc.c Coordinador.h

Nodo_clnt.c Nodo_xdr.c: Nodo.x
	rpcgen -l -o Nodo_clnt.c Nodo.x
	rpcgen -c -o Nodo_xdr.c Nodo.x
Coordinador_clnt.c Coordinador.h: Coordinador.x
	rpcgen -h -o Coordinador.h Coordinador.x
	rpcgen -l -o Coordinador_clnt.c Coordinador.x
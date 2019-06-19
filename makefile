.PHONY: all
all: shell coordinador
.PHONY: clean
clean :
	rm shell coordinador Coordinador_clnt.c Coordinador_svc.c Coordinador.h Nodo_xdr.c Nodo_clnt.c Nodo_svc.c Nodo.h

shell: shell.c Nodo_clnt.c Nodo.h Nodo_xdr.c Coordinador_clnt.c Coordinador.h filehandler.c
	gcc -lnsl -o shell shell.c Coordinador_clnt.c Nodo_clnt.c Coordinador.h Nodo.h Nodo_xdr.c filehandler.c

coordinador: coordinador.c Coordinador_svc.c Coordinador.h
	gcc -lnsl -o coordinador coordinador.c Coordinador_svc.c Coordinador.h

Nodo_clnt.c Nodo_svc.c Nodo_xdr.c Nodo.h: Nodo.x
	rpcgen Nodo.x
Coordinador_clnt.c Coordinador_svc.c Coordinador.h: Coordinador.x
	rpcgen Coordinador.x

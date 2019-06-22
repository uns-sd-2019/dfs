/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "Nodo.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>


//Necesario para el anunciarnodo
#include "Coordinador.h"

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
nodo_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		file_to_send subir_1_arg;
		char *bajar_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case subir:
		_xdr_argument = (xdrproc_t) xdr_file_to_send;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) subir_1_svc;
		break;

	case bajar:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_file_to_send;
		local = (char *(*)(char *, struct svc_req *)) bajar_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}


//---
void notificarAlCoord(char *dirNodo, char *dirCoord)
{
	CLIENT *clntcoor;
	clntcoor = clnt_create(dirCoord, COORDINADOR, VERSION1, "TCP");
	if(clntcoor == (CLIENT *) NULL) {
	  printf("ERROR: Fallo la conexion con el coordinador.\n");
	  exit(1);
	}
	int notificarCoord = *anunciarnodo_1(&dirNodo,clntcoor);

	if(notificarCoord==-1)
	{
		printf("Error: coordinador no puede registrar el nodo.\n");
		printf("Puede ya estár registrado, o IP incorrecta.\n");
		clnt_destroy(clntcoor); // << si va abajo del else, el exit nunca deja que se rompa la conexion.
		exit(1);
	}
	else
	{
		printf("Nodo registrado correctamente!\n");
		updateCoordinadorDir(dirCoord); //metodo en nodo.c para que updatee su dir de coordinador.
		clnt_destroy(clntcoor);
	}
}



int
main (int argc, char **argv)
{
	register SVCXPRT *transp;
	if(argc<3)
	{
		printf("Uso: %s IP_NODO IP_COORD\n",argv[0]);
		printf("#IP_NODO: ip que será anunciada al coordinador.\n");
		exit(1);	
	}
	
	// Si se configuró bien, anuncio el nodo.
	notificarAlCoord(argv[1],argv[2]);


	pmap_unset (NODO, VERSION1);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, NODO, VERSION1, nodo_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (NODO, VERSION1, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, NODO, VERSION1, nodo_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (NODO, VERSION1, tcp).");
		exit(1);
	}
	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
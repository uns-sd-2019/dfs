/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _NODO_H_RPCGEN
#define _NODO_H_RPCGEN

#include <rpc/rpc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXLEN 1024
#define FILELEN 1000000000

typedef char *my_file;

typedef char filename[MAXLEN];

struct file_to_send {
	filename name;
	my_file data;
	int size;
};
typedef struct file_to_send file_to_send;

#define NODO 0x20000003
#define VERSION1 1

void updateCoordinadorDir(char *dirCord);


#if defined(__STDC__) || defined(__cplusplus)
#define subir 1
extern  int * subir_1(file_to_send *, CLIENT *);
extern  int * subir_1_svc(file_to_send *, struct svc_req *);
#define bajar 2
extern  file_to_send * bajar_1(char **, CLIENT *);
extern  file_to_send * bajar_1_svc(char **, struct svc_req *);
extern int nodo_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define subir 1
extern  int * subir_1();
extern  int * subir_1_svc();
#define bajar 2
extern  file_to_send * bajar_1();
extern  file_to_send * bajar_1_svc();
extern int nodo_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_my_file (XDR *, my_file*);
extern  bool_t xdr_filename (XDR *, filename);
extern  bool_t xdr_file_to_send (XDR *, file_to_send*);

#else /* K&R C */
extern bool_t xdr_my_file ();
extern bool_t xdr_filename ();
extern bool_t xdr_file_to_send ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_NODO_H_RPCGEN */

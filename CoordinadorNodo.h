/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _COORDINADORNODO_H_RPCGEN
#define _COORDINADORNODO_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define COORDINADORNODO 0x20000002
#define VERSION1 1

#if defined(__STDC__) || defined(__cplusplus)
#define subirACK 1
extern  int * subirack_1(char **, CLIENT *);
extern  int * subirack_1_svc(char **, struct svc_req *);
extern int coordinadornodo_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define subirACK 1
extern  int * subirack_1();
extern  int * subirack_1_svc();
extern int coordinadornodo_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_COORDINADORNODO_H_RPCGEN */

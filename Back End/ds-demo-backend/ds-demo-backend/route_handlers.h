#ifndef ROUTE_HANDLERS_H
#define ROUTE_HANDLERS_H

#include "civetweb.h"

#ifdef __cplusplus
extern "C" {
#endif

	/* Register all endpoints with the CivetWeb context.
	   We'll define the data structures as needed or pass them in. */
	void register_endpoints(struct mg_context* ctx);

#ifdef __cplusplus
}
#endif

#endif /* ROUTE_HANDLERS_H */

#ifndef ROUTE_HANDLERS_H
#define ROUTE_HANDLERS_H

#include "civetweb.h"

#ifdef __cplusplus
extern "C" {
#endif


	void register_endpoints(struct mg_context* ctx);

#ifdef __cplusplus
}
#endif

#endif /* ROUTE_HANDLERS_H */

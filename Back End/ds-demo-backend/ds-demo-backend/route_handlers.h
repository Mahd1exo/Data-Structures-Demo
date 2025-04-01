#ifndef ROUTE_HANDLERS_H
#define ROUTE_HANDLERS_H

#include "civetweb.h"
#include "task_queue.h"

#define MAX_BODY_LEN 1024
#define MAX_VALUE_LEN 64
#define MAX_KEY_LEN 64
#define SUCCESS_RESPONSE_CODE 200
#define FAILURE_RESPONSE_CODE 400
#define RESPONSE_JSON_TYPE "application/json"

#ifdef __cplusplus
extern "C" {
#endif


	void register_endpoints(struct mg_context* ctx);
	void clear_all_data_structures(void);

#ifdef __cplusplus
}
#endif

#endif /* ROUTE_HANDLERS_H */

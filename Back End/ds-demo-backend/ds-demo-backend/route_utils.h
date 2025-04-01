#ifndef ROUTE_UTILS_H
#define ROUTE_UTILS_H

#include <civetweb.h>
#include <stddef.h>  
#include "thread_pool.h"



/* read_request_body: read entire request into buf, ensure null-termination */
int read_request_body(struct mg_connection* conn, char* buf, size_t bufSize);

/* send_json: send a JSON string with appropriate headers */
void send_json(struct mg_connection* conn, const char* json);

/* extract_value_from_body: parse {"value":"X"} */
int extract_value_from_body(const char* body, char* outVal, int maxLen);

/* extract_key_from_body: parse {"key":"X"} */
int extract_key_from_body(const char* body, char* outKey, int keyMax);

/* extract_key_val: parse {"key":"...","val":"..."} */
int extract_key_val(const char* body, char* outKey, int keyMax, char* outVal, int valMax);

/* build_json_array_response: e.g. { "stack": ["A","B"] } */
char* build_json_array_response(const char* wrapper, const char** items, int count);
int extract_index_from_body(const char* body);

#endif /* ROUTE_UTILS_H */

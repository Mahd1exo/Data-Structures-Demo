#include "route_handlers.h"
#include "route_utils.h"     
#include "ds_maxheap.h"         
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern MaxHeap g_heap;

static int handle_heap_get(struct mg_connection* conn, void* cbdata) {
    char* json = maxheap_to_json(&g_heap);
    send_json(conn, json ? json : "{ \"heap\": [] }");
    if (json)
        free(json);
    return SUCCESS_RESPONSE_CODE;
}

static int handle_heap_insert(struct mg_connection* conn, void* cbdata) {
	char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        int intValue = atoi(value);
        maxheap_insert(&g_heap, intValue);
    }
    return handle_heap_get(conn, NULL);
}

static int handle_heap_extract_top(struct mg_connection* conn, void* cbdata) {
    int topVal = maxheap_extractTop(&g_heap);
    // Optionally, you can log or process topVal here.
    return handle_heap_get(conn, NULL);
}

static int handle_heap_clear(struct mg_connection* conn, void* cbdata) {
    maxheap_clear(&g_heap);
    return handle_heap_get(conn, NULL);
}

void register_heap_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/heap", handle_heap_get, NULL);
    mg_set_request_handler(ctx, "/api/heap/insert", handle_heap_insert, NULL);
    mg_set_request_handler(ctx, "/api/heap/extract-top", handle_heap_extract_top, NULL);
    mg_set_request_handler(ctx, "/api/heap/clear", handle_heap_clear, NULL);
}

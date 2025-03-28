#include "route_handlers.h"
#include "route_utils.h"
#include "ds_minheap.h"
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern MinHeap g_minheap;

static int handle_minheap_get(struct mg_connection* conn, void* cbdata) {
    char* json = minheap_to_json(&g_minheap);
    send_json(conn, json ? json : "{ \"heap\": [] }");
    if (json)
        free(json);
    return SUCCESS_RESPONSE_CODE;
}

static int handle_minheap_insert(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        // Convert string value to an integer
        int intValue = atoi(value);
        minheap_insert(&g_minheap, intValue);
    }
    return handle_minheap_get(conn, NULL);
}

static int handle_minheap_extract_min(struct mg_connection* conn, void* cbdata) {
    // Extract the minimum value (if any)
    int minVal = minheap_extractMin(&g_minheap);
    // Optionally, you can log or process minVal here.
    return handle_minheap_get(conn, NULL);
}

static int handle_minheap_clear(struct mg_connection* conn, void* cbdata) {
    minheap_clear(&g_minheap);
    return handle_minheap_get(conn, NULL);
}

void register_minheap_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/minheap", handle_minheap_get, NULL);
    mg_set_request_handler(ctx, "/api/minheap/insert", handle_minheap_insert, NULL);
    mg_set_request_handler(ctx, "/api/minheap/extract-min", handle_minheap_extract_min, NULL);
    mg_set_request_handler(ctx, "/api/minheap/clear", handle_minheap_clear, NULL);
}

#include "route_handlers.h"
#include "ds_bst.h"
#include "route_utils.h"
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern SimpleBST g_bst;

static int handle_bst_get(struct mg_connection* conn, void* cbdata) {
    char* json = bst_to_json(&g_bst);
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Content-Length: %d\r\n\r\n%s",
        (int)strlen(json),
        json);
    free(json);
    return 200;
}

static int handle_bst_insert(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        bst_insert(&g_bst, value);
    }
    return handle_bst_get(conn, NULL);
}

static int handle_bst_remove(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        bst_remove(&g_bst, value);
    }
    return handle_bst_get(conn, NULL);
}

static int handle_bst_clear(struct mg_connection* conn, void* cbdata) {
    bst_clear(&g_bst);
    return handle_bst_get(conn, NULL);
}

void register_bst_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/bst", handle_bst_get, NULL);
    mg_set_request_handler(ctx, "/api/bst/insert", handle_bst_insert, NULL);
    mg_set_request_handler(ctx, "/api/bst/remove", handle_bst_remove, NULL);
    mg_set_request_handler(ctx, "/api/bst/clear", handle_bst_clear, NULL);
}

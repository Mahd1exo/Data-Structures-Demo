#include "route_handlers.h"
#include "route_utils.h"
#include "ds_avl.h"
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern SimpleAVL g_avl;

static int handle_avl_get(struct mg_connection* conn, void* cbdata) {
    char* json = avl_to_json(&g_avl);
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

static int handle_avl_insert(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        avl_insert(&g_avl, value);
    }
    return handle_avl_get(conn, NULL);
}

static int handle_avl_remove(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        avl_remove(&g_avl, value);
    }
    return handle_avl_get(conn, NULL);
}

static int handle_avl_clean(struct mg_connection* conn, void* cbdata) {
    // Clean the AVL tree
    avl_clear(&g_avl);
    return handle_avl_get(conn, NULL);
}

void register_avl_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/avl", handle_avl_get, NULL);
    mg_set_request_handler(ctx, "/api/avl/insert", handle_avl_insert, NULL);
    mg_set_request_handler(ctx, "/api/avl/remove", handle_avl_remove, NULL);
    mg_set_request_handler(ctx, "/api/avl/clear", handle_avl_clean, NULL);
}

#include "route_handlers.h"
#include "route_utils.h"
#include "ds_avl.h"
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern SimpleAVL g_avl;

/* GET: /api/avl */
static int handle_avl_get(struct mg_connection* conn, void* cbdata) {
    char* json = avl_to_json(&g_avl);
    send_json(conn, json ? json : "{ \"avl\": [] }");
    if (json) free(json);
    return SUCCESS_RESPONSE_CODE;
}

/* POST: /api/avl/insert */
static int handle_avl_insert(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        avl_insert(&g_avl, value);
    }
    return handle_avl_get(conn, NULL);
}

/* POST: /api/avl/remove */
static int handle_avl_remove(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        avl_remove(&g_avl, value);
    }
    return handle_avl_get(conn, NULL);
}

/* DELETE: /api/avl/clear */
static int handle_avl_clear(struct mg_connection* conn, void* cbdata) {
    avl_clear(&g_avl);
    return handle_avl_get(conn, NULL);
}

void register_avl_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/avl", handle_avl_get, NULL);
    mg_set_request_handler(ctx, "/api/avl/insert", handle_avl_insert, NULL);
    mg_set_request_handler(ctx, "/api/avl/remove", handle_avl_remove, NULL);
    mg_set_request_handler(ctx, "/api/avl/clear", handle_avl_clear, NULL);
}

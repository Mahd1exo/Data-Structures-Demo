#include "route_handlers.h"
#include "ds_bst.h"
#include "route_utils.h"
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern SimpleBST g_bst;

/* GET: /api/bst */
static int handle_bst_get(struct mg_connection* conn, void* cbdata) {
    char* json = bst_to_json(&g_bst);
    send_json(conn, json ? json : "{ \"bst\": [] }");
    if (json) free(json);
    return SUCCESS_RESPONSE_CODE;
}

/* POST: /api/bst/insert */
static int handle_bst_insert(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        bst_insert(&g_bst, value);
    }
    return handle_bst_get(conn, NULL);
}

/* POST: /api/bst/remove */
static int handle_bst_remove(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        bst_remove(&g_bst, value);
    }
    return handle_bst_get(conn, NULL);
}

/* DELETE: /api/bst/clear */
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

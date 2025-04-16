#include "route_handlers.h"
#include "ds_sortedlist.h"
#include "route_utils.h"
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

/* The global sorted list */
extern SortedList g_sortedlist;

/* GET: /api/sorted-list */
static int handle_sl_get(struct mg_connection* conn, void* cbdata) {
    int count = 0;
    char** items = sortedlist_collect(&g_sortedlist, &count);
    /* Build a JSON array: { "sortedList": ["...", "..."] } */
    char* json = build_json_array_response("sortedList", (const char**)items, count);
    if (items) free(items);
    send_json(conn, json ? json : "{ \"sortedList\": [] }");
    if (json) free(json);
    return SUCCESS_RESPONSE_CODE;
}

/* POST: /api/sorted-list/insert */
static int handle_sl_insert(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        sortedlist_insert(&g_sortedlist, value);
    }
    return handle_sl_get(conn, NULL);
}

/* POST: /api/sorted-list/remove */
static int handle_sl_remove(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        sortedlist_remove(&g_sortedlist, value);
    }
    return handle_sl_get(conn, NULL);
}

/* DELETE: /api/sorted-list/clear */
static int handle_sl_clear(struct mg_connection* conn, void* cbdata) {
    sortedlist_clear(&g_sortedlist);
    return handle_sl_get(conn, NULL);
}

void register_sortedlist_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/sorted-list", handle_sl_get, NULL);
    mg_set_request_handler(ctx, "/api/sorted-list/insert", handle_sl_insert, NULL);
    mg_set_request_handler(ctx, "/api/sorted-list/remove", handle_sl_remove, NULL);
    mg_set_request_handler(ctx, "/api/sorted-list/clear", handle_sl_clear, NULL);
}

#include "route_handlers.h"
#include "ds_circularlist.h"
#include "route_utils.h"
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern CircularList g_circularlist;

/* GET: /api/circular-list */
static int handle_circularlist_get(struct mg_connection* conn, void* cbdata) {
    int count = 0;
    char** items = cl_collect_data(&g_circularlist, &count);
    char* json = build_json_array_response("circularList", (const char**)items, count);
    if (items) free(items);
    send_json(conn, json ? json : "{ \"circularList\": [] }");
    if (json) free(json);
    return 200;
}

/* POST: /api/circular-list/insert - default add at end */
static int handle_circularlist_insert(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        cl_add_end(&g_circularlist, value);
    }
    return handle_circularlist_get(conn, NULL);
}

/* POST: /api/circular-list/insert-front */
static int handle_circularlist_insert_front(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        cl_add_front(&g_circularlist, value);
    }
    return handle_circularlist_get(conn, NULL);
}

/* POST: /api/circular-list/insert-by-index */
static int handle_circularlist_insert_by_index(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    int index = extract_index_from_body(body);
    if (index < 0) {
        send_json(conn, "{ \"error\": \"Missing or invalid index\" }");
        return 400;
    }
    if (extract_value_from_body(body, value, sizeof(value))) {
        cl_insert_by_index(&g_circularlist, value, index);
    }
    return handle_circularlist_get(conn, NULL);
}

/* POST: /api/circular-list/remove - remove by value */
static int handle_circularlist_remove(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        cl_remove(&g_circularlist, value);
    }
    return handle_circularlist_get(conn, NULL);
}

/* DELETE: /api/circular-list/remove-front */
static int handle_circularlist_remove_front(struct mg_connection* conn, void* cbdata) {
    cl_remove_front(&g_circularlist);
    return handle_circularlist_get(conn, NULL);
}

/* DELETE: /api/circular-list/remove-end */
static int handle_circularlist_remove_end(struct mg_connection* conn, void* cbdata) {
    cl_remove_end(&g_circularlist);
    return handle_circularlist_get(conn, NULL);
}

/* DELETE: /api/circular-list/remove-by-index */
static int handle_circularlist_remove_by_index(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    int index = extract_index_from_body(body);
    if (index < 0) {
        send_json(conn, "{ \"error\": \"Missing or invalid index\" }");
        return 400;
    }
    cl_remove_by_index(&g_circularlist, index);
    return handle_circularlist_get(conn, NULL);
}

/* DELETE: /api/circular-list/clear */
static int handle_circularlist_clear(struct mg_connection* conn, void* cbdata) {
    cl_clear(&g_circularlist);
    return handle_circularlist_get(conn, NULL);
}

void register_circularlist_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/circular-list", handle_circularlist_get, NULL);
    mg_set_request_handler(ctx, "/api/circular-list/insert", handle_circularlist_insert, NULL);
    mg_set_request_handler(ctx, "/api/circular-list/insert-front", handle_circularlist_insert_front, NULL);
    mg_set_request_handler(ctx, "/api/circular-list/insert-by-index", handle_circularlist_insert_by_index, NULL);
    mg_set_request_handler(ctx, "/api/circular-list/remove", handle_circularlist_remove, NULL);
    mg_set_request_handler(ctx, "/api/circular-list/remove-front", handle_circularlist_remove_front, NULL);
    mg_set_request_handler(ctx, "/api/circular-list/remove-end", handle_circularlist_remove_end, NULL);
    mg_set_request_handler(ctx, "/api/circular-list/remove-by-index", handle_circularlist_remove_by_index, NULL);
    mg_set_request_handler(ctx, "/api/circular-list/clear", handle_circularlist_clear, NULL);
}

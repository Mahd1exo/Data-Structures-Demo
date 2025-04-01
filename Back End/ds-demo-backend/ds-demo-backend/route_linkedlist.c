#include "route_handlers.h"
#include "ds_linkedlist.h"
#include "route_utils.h"  
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern LinkedList g_list;

static int handle_get_linked_list(struct mg_connection* conn, void* cbdata) {
    int count = 0;
    char** items = ll_collect_data(&g_list, &count);
    char* json = build_json_array_response("list", (const char**)items, count);
    if (items)
        free(items);
    send_json(conn, json ? json : "{ \"list\": [] }");
    if (json)
        free(json);
    return SUCCESS_RESPONSE_CODE;
}

static int handle_post_list_add_front(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        ll_add_front(&g_list, value);
    }
    return handle_get_linked_list(conn, NULL);
}

static int handle_post_list_add_end(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        ll_add_end(&g_list, value);
    }
    return handle_get_linked_list(conn, NULL);
}

static int handle_post_list_add_by_index(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    int index = extract_index_from_body(body);
    if (index < 0) {
        send_json(conn, "{ \"error\": \"Missing or invalid 'index'\" }");
        return FAILURE_RESPONSE_CODE;
    }
    if (extract_value_from_body(body, value, sizeof(value))) {
        ll_add_by_index(&g_list, value, index);
    }
    return handle_get_linked_list(conn, NULL);
}

static int handle_delete_list_remove_by_index(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    int index = extract_index_from_body(body);
    if (index < 0) {
        send_json(conn, "{ \"error\": \"Missing or invalid 'index'\" }");
        return FAILURE_RESPONSE_CODE;
    }
    ll_remove_by_index(&g_list, index);
    return handle_get_linked_list(conn, NULL);
}

static int handle_delete_list_remove_front(struct mg_connection* conn, void* cbdata) {
    ll_remove_front(&g_list);
    return handle_get_linked_list(conn, NULL);
}

static int handle_delete_list_remove_end(struct mg_connection* conn, void* cbdata) {
    ll_remove_end(&g_list);
    return handle_get_linked_list(conn, NULL);
}

/* New endpoint: Clear the entire linked list */
static int handle_delete_list_clear(struct mg_connection* conn, void* cbdata) {
    ll_clear(&g_list);
    return handle_get_linked_list(conn, NULL);
}

void register_linkedlist_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/linked-list", handle_get_linked_list, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/add-front", handle_post_list_add_front, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/add-end", handle_post_list_add_end, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/add-by-index", handle_post_list_add_by_index, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/remove-front", handle_delete_list_remove_front, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/remove-end", handle_delete_list_remove_end, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/remove-by-index", handle_delete_list_remove_by_index, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/clear", handle_delete_list_clear, NULL);
}

#include "route_handlers.h"
#include "ds_stack.h"
#include "route_utils.h" 
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern Stack g_stack;  // Global declared in route_handlers.c

static int handle_get_stack(struct mg_connection* conn, void* cbdata) {
    int count = 0;
    char** items = stk_collect_data(&g_stack, &count);
    char* json = build_json_array_response("stack", (const char**)items, count);
    if (items)
        free(items);
    send_json(conn, json ? json : "{ \"stack\": [] }");
    if (json)
        free(json);
    return 200;
}

static int handle_post_stack_push(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        stk_push(&g_stack, value);
    }
    return handle_get_stack(conn, NULL);
}

static int handle_delete_stack_pop(struct mg_connection* conn, void* cbdata) {
    stk_pop(&g_stack);
    return handle_get_stack(conn, NULL);
}

static int handle_delete_stack_clear(struct mg_connection* conn, void* cbdata) {
    stk_clear(&g_stack);
    return handle_get_stack(conn, NULL);
}

void register_stack_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/stack", handle_get_stack, NULL);
    mg_set_request_handler(ctx, "/api/stack/push", handle_post_stack_push, NULL);
    mg_set_request_handler(ctx, "/api/stack/pop", handle_delete_stack_pop, NULL);
    mg_set_request_handler(ctx, "/api/stack/clear", handle_delete_stack_clear, NULL);
}

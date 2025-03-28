#include "route_handlers.h"
#include "ds_queue.h"
#include "route_utils.h" 
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern Queue g_queue;  // Global declared in route_handlers.c

static int handle_get_queue(struct mg_connection* conn, void* cbdata) {
    int count = 0;
    char** items = que_collect_data(&g_queue, &count);
    char* json = build_json_array_response("queue", (const char**)items, count);
    if (items)
        free(items);
    send_json(conn, json ? json : "{ \"queue\": [] }");
    if (json)
        free(json);
    return SUCCESS_RESPONSE_CODE;
}

static int handle_post_queue_enqueue(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        que_enqueue(&g_queue, value);
    }
    return handle_get_queue(conn, NULL);
}

static int handle_delete_queue_dequeue(struct mg_connection* conn, void* cbdata) {
    que_dequeue(&g_queue);
    return handle_get_queue(conn, NULL);
}

static int handle_delete_queue_clear(struct mg_connection* conn, void* cbdata) {
    que_clear(&g_queue);
    return handle_get_queue(conn, NULL);
}

void register_queue_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/queue", handle_get_queue, NULL);
    mg_set_request_handler(ctx, "/api/queue/enqueue", handle_post_queue_enqueue, NULL);
    mg_set_request_handler(ctx, "/api/queue/dequeue", handle_delete_queue_dequeue, NULL);
    mg_set_request_handler(ctx, "/api/queue/clear", handle_delete_queue_clear, NULL);
}

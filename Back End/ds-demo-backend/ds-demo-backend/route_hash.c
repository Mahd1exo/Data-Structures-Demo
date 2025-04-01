#include "route_handlers.h"
#include "ds_hashtable.h"
#include "route_utils.h" 
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern SimpleHashTable g_hash;  // Global declared in route_handlers.c

static int handle_hash_getall(struct mg_connection* conn, void* cbdata) {
    int count = 0;
    char** items = hash_collect(&g_hash, &count);
    char* json = build_json_array_response("hash", (const char**)items, count);
    if (items) {
        for (int i = 0; i < count; i++) {
            free(items[i]);
        }
        free(items);
    }
    send_json(conn, json ? json : "{ \"hash\": [] }");
    if (json)
        free(json);
    return SUCCESS_RESPONSE_CODE;
}

static int handle_hash_insert(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char key[MAX_KEY_LEN], val[MAX_VALUE_LEN];
    if (extract_key_val(body, key, sizeof(key), val, sizeof(val))) {
        hash_insert(&g_hash, key, val);
    }
    return handle_hash_getall(conn, NULL);
}

static int handle_hash_remove(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char key[MAX_KEY_LEN];
    if (!extract_key_from_body(body, key, sizeof(key))) {
        send_json(conn, "{ \"error\": \"Missing or invalid 'key'\" }");
        return FAILURE_RESPONSE_CODE;
    }
    hash_remove(&g_hash, key);
    return handle_hash_getall(conn, NULL);
}

static int handle_hash_contains(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char key[MAX_KEY_LEN];
    int result = 0;
    if (extract_key_from_body(body, key, sizeof(key))) {
        result = hash_contains(&g_hash, key);
    }
    char resp[MAX_KEY_LEN];
    snprintf(resp, sizeof(resp), "{ \"contains\": %s }", result ? "true" : "false");
    send_json(conn, resp);
    return SUCCESS_RESPONSE_CODE;
}

static int handle_hash_clear(struct mg_connection* conn, void* cbdata) {
    hash_clear(&g_hash);
    return handle_hash_getall(conn, NULL);
}

void register_hash_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/hash", handle_hash_getall, NULL);
    mg_set_request_handler(ctx, "/api/hash/insert", handle_hash_insert, NULL);
    mg_set_request_handler(ctx, "/api/hash/remove", handle_hash_remove, NULL);
    mg_set_request_handler(ctx, "/api/hash/contains", handle_hash_contains, NULL);
    mg_set_request_handler(ctx, "/api/hash/clear", handle_hash_clear, NULL);
}

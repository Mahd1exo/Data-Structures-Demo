/* route_handlers.c */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "route_handlers.h"
#include <civetweb.h>

/* Data structure headers – make sure these exist and are compiled into your project */
#include "ds_bst.h"
#include "ds_linkedlist.h"
#include "ds_stack.h"
#include "ds_queue.h"
#include "ds_tree.h"
#include "ds_heap.h"
#include "ds_hashtable.h"
#include "ds_json_utils.h"  /* Optional: if you have a separate JSON helper, otherwise see below */

/*
   Global data structures
*/
static LinkedList g_list;
static Stack g_stack;
static Queue g_queue;
static SimpleTree g_tree;
static SimpleHeap g_heap;
static SimpleHashTable g_hash;
static SimpleBST g_bst;

/* ---------------------------------------------------------------------------
   Helper Functions
--------------------------------------------------------------------------- */

/* Read the entire request body into buf, ensuring null termination */
static int read_request_body(struct mg_connection* conn, char* buf, size_t bufSize)
{
    if (!conn || !buf)
        return 0;
    int r = mg_read(conn, buf, (int)bufSize - 1);
    if (r < 0)
        r = 0;
    buf[r] = '\0';
    return r;
}

/* Send JSON response with proper headers */
static void send_json(struct mg_connection* conn, const char* json)
{
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Connection: close\r\n\r\n");
    mg_printf(conn, "%s", json);
}

/*
   A naive JSON parser for {"value":"someValue"}
   Copies the extracted value into outVal (up to maxLen-1 chars)
*/
static int extract_value_from_body(const char* body, char* outVal, int maxLen)
{
    const char* p = strstr(body, "\"value\"");
    if (!p)
        return 0;
    p = strchr(p, ':');
    if (!p)
        return 0;
    p++; // skip colon
    while (*p && (*p == ' ' || *p == '\"'))
        p++;
    int i = 0;
    while (*p && *p != '\"' && *p != '}' && i < (maxLen - 1)) {
        outVal[i++] = *p++;
    }
    outVal[i] = '\0';
    return 1;
}
/* A simple parser for JSON bodies like {"key":"someKey"} */
static int extract_key_from_body(const char* body, char* outKey, int keyMax) {
    const char* p = strstr(body, "\"key\"");
    if (!p) return 0;
    p = strchr(p, ':');
    if (!p) return 0;
    p++;
    /* skip spaces and quotes */
    while (*p && (*p == ' ' || *p == '\"')) p++;
    int i = 0;
    while (*p && *p != '\"' && *p != '}' && i < (keyMax - 1)) {
        outKey[i++] = *p++;
    }
    outKey[i] = '\0';
    return 1;
}

/*
   A naive JSON parser for {"key":"someKey","val":"someValue"}
   Copies the extracted key into outKey and value into outVal.
*/
static int extract_key_val(const char* body, char* outKey, int keyMax, char* outVal, int valMax)
{
    const char* kp = strstr(body, "\"key\"");
    const char* vp = strstr(body, "\"val\"");
    if (!kp || !vp)
        return 0;

    /* Parse key */
    kp = strchr(kp, ':');
    if (!kp)
        return 0;
    kp++;
    while (*kp && (*kp == ' ' || *kp == '\"'))
        kp++;
    int i = 0;
    while (*kp && *kp != '\"' && *kp != '}' && i < (keyMax - 1)) {
        outKey[i++] = *kp++;
    }
    outKey[i] = '\0';

    /* Parse value */
    vp = strchr(vp, ':');
    if (!vp)
        return 0;
    vp++;
    while (*vp && (*vp == ' ' || *vp == '\"'))
        vp++;
    i = 0;
    while (*vp && *vp != '\"' && *vp != '}' && i < (valMax - 1)) {
        outVal[i++] = *vp++;
    }
    outVal[i] = '\0';

    return 1;
}

/*
   Build a JSON array response from an array of strings.
   For example, if wrapper is "stack" and items = ["A", "B"],
   returns: { "stack": ["A","B"] }
*/
static char* build_json_array_response(const char* wrapper, const char** items, int count)
{
    int capacity = 1024 + 64 * count;
    char* json = (char*)malloc(capacity);
    if (!json)
        return NULL;

    snprintf(json, capacity, "{ \"%s\": [", wrapper);
    for (int i = 0; i < count; i++) {
        char temp[256];
        snprintf(temp, sizeof(temp), "\"%s\"", items[i]);
        strncat(json, temp, capacity - strlen(json) - 1);
        if (i < count - 1)
            strncat(json, ",", capacity - strlen(json) - 1);
    }
    strncat(json, "]}", capacity - strlen(json) - 1);
    return json;
}

/* ---------------------------------------------------------------------------
   LinkedList Endpoints
--------------------------------------------------------------------------- */
static int handle_get_linked_list(struct mg_connection* conn, void* cbdata)
{
    int count = 0;
    char** items = ll_collect_data(&g_list, &count);
    char* json = build_json_array_response("list", (const char**)items, count);
    if (items)
        free(items);
    send_json(conn, json ? json : "{ \"list\": [] }");
    if (json)
        free(json);
    return 200;
}

static int handle_post_list_add_front(struct mg_connection* conn, void* cbdata)
{
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        ll_add_front(&g_list, value);
    }
    return handle_get_linked_list(conn, NULL);
}

static int handle_post_list_add_end(struct mg_connection* conn, void* cbdata)
{
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        ll_add_end(&g_list, value);
    }
    return handle_get_linked_list(conn, NULL);
}

static int handle_delete_list_remove_front(struct mg_connection* conn, void* cbdata)
{
    ll_remove_front(&g_list);
    return handle_get_linked_list(conn, NULL);
}

static int handle_delete_list_remove_end(struct mg_connection* conn, void* cbdata)
{
    ll_remove_end(&g_list);
    return handle_get_linked_list(conn, NULL);
}

/* ---------------------------------------------------------------------------
   Stack Endpoints
--------------------------------------------------------------------------- */
static int handle_get_stack(struct mg_connection* conn, void* cbdata)
{
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

static int handle_post_stack_push(struct mg_connection* conn, void* cbdata)
{
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        stk_push(&g_stack, value);
    }
    return handle_get_stack(conn, NULL);
}

static int handle_delete_stack_pop(struct mg_connection* conn, void* cbdata)
{
    stk_pop(&g_stack);
    return handle_get_stack(conn, NULL);
}

/* ---------------------------------------------------------------------------
   Queue Endpoints
--------------------------------------------------------------------------- */
static int handle_get_queue(struct mg_connection* conn, void* cbdata)
{
    int count = 0;
    char** items = que_collect_data(&g_queue, &count);
    char* json = build_json_array_response("queue", (const char**)items, count);
    if (items)
        free(items);
    send_json(conn, json ? json : "{ \"queue\": [] }");
    if (json)
        free(json);
    return 200;
}

static int handle_post_queue_enqueue(struct mg_connection* conn, void* cbdata)
{
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        que_enqueue(&g_queue, value);
    }
    return handle_get_queue(conn, NULL);
}

static int handle_delete_queue_dequeue(struct mg_connection* conn, void* cbdata)
{
    que_dequeue(&g_queue);
    return handle_get_queue(conn, NULL);
}

/* ---------------------------------------------------------------------------
   Tree Endpoints (by unique node id)
--------------------------------------------------------------------------- */

static char* tree_to_json(TreeNode* node) {
    if (!node) {
        return _strdup("null");
    }

    char* childrenJson = NULL;
    if (node->childrenCount > 0 && node->children) {
        int capacity = 128;
        childrenJson = (char*)malloc(capacity);
        if (!childrenJson) return NULL;
        strcpy(childrenJson, "[");
        for (int i = 0; i < node->childrenCount; i++) {
            char* childJson = tree_to_json(node->children[i]);
            int needed = (int)strlen(childJson) + 2;
            if ((int)strlen(childrenJson) + needed >= capacity) {
                capacity *= 2;
                childrenJson = (char*)realloc(childrenJson, capacity);
            }
            strcat(childrenJson, childJson);
            free(childJson);
            if (i < node->childrenCount - 1) {
                strcat(childrenJson, ",");
            }
        }
        strcat(childrenJson, "]");
    }
    else {
        childrenJson = _strdup("[]");
    }

    int bufSize = 128 + (int)strlen(childrenJson);
    char* result = (char*)malloc(bufSize);
    if (result) {
        snprintf(result, bufSize, "{ \"id\": %d, \"data\": \"%s\", \"children\": %s }",
            node->id, node->data, childrenJson);
    }
    free(childrenJson);
    return result;
}

/* GET /api/tree */
static int handle_tree_get(struct mg_connection* conn, void* cbdata) {
    char* treeJson = tree_to_json(g_tree.root);
    int bufSize = (int)strlen(treeJson) + 64;
    char* json = (char*)malloc(bufSize);
    if (json) {
        snprintf(json, bufSize, "{ \"tree\": %s }", treeJson);
    }
    free(treeJson);
    send_json(conn, json ? json : "{ \"tree\": null }");
    if (json) free(json);
    return 200;
}

/* POST /api/tree/insert: Insert a node using default insertion */
static int handle_tree_insert(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        tree_insert(&g_tree, value);
    }
    return handle_tree_get(conn, NULL);
}

/* POST /api/tree/remove: Remove a node by id.*/
static int handle_tree_remove(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    int id = -1;
    const char* p = strstr(body, "\"id\"");
    if (p) {
        p = strchr(p, ':');
        if (p) {
            p++;
            while (*p && (*p == ' ' || *p == '\"')) p++;
            id = atoi(p);
        }
    }
    if (id >= 0) {
        tree_remove_by_id(&g_tree, id);
    }
    return handle_tree_get(conn, NULL);
}

/* POST /api/tree/insert-child: Insert a child under specified parent.*/
static int handle_tree_insert_child(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        int parentId = -1;
        const char* p = strstr(body, "\"parent\"");
        if (p) {
            p = strchr(p, ':');
            if (p) {
                p++;
                while (*p && (*p == ' ' || *p == '\"')) p++;
                parentId = atoi(p);
            }
        }
        if (parentId >= 0) {
            tree_insert_child(&g_tree, parentId, value);
        }
    }
    return handle_tree_get(conn, NULL);
}

/* POST /api/tree/remove-child: Remove a child by id under specified parent.*/
static int handle_tree_remove_child(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    int parentId = -1, childId = -1;
    const char* p = strstr(body, "\"parent\"");
    if (p) {
        p = strchr(p, ':');
        if (p) {
            p++;
            while (*p && (*p == ' ' || *p == '\"')) p++;
            parentId = atoi(p);
        }
    }
    p = strstr(body, "\"id\"");
    if (p) {
        p = strchr(p, ':');
        if (p) {
            p++;
            while (*p && (*p == ' ' || *p == '\"')) p++;
            childId = atoi(p);
        }
    }
    if (parentId >= 0 && childId >= 0) {
        tree_remove_child_by_id(&g_tree, parentId, childId);
    }
    return handle_tree_get(conn, NULL);
}


/* ---------------------------------------------------------------------------
   Heap Endpoints
--------------------------------------------------------------------------- */
static int handle_heap_get(struct mg_connection* conn, void* cbdata)
{
    int count = 0;
    char** items = heap_collect(&g_heap, &count);
    char* json = build_json_array_response("heap", (const char**)items, count);
    if (items)
        free(items);
    send_json(conn, json ? json : "{ \"heap\": [] }");
    if (json)
        free(json);
    return 200;
}

static int handle_heap_insert(struct mg_connection* conn, void* cbdata)
{
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char value[64];
    if (extract_value_from_body(body, value, sizeof(value))) {
        heap_insert(&g_heap, value);
    }
    return handle_heap_get(conn, NULL);
}

static int handle_heap_extract_top(struct mg_connection* conn, void* cbdata)
{
    char tmp[64];
    if (heap_extract_top(&g_heap, tmp, sizeof(tmp))) {
        /* extracted value can be logged if needed */
    }
    return handle_heap_get(conn, NULL);
}

/* ---------------------------------------------------------------------------
   Hash Table Endpoints
--------------------------------------------------------------------------- */

static int handle_hash_getall(struct mg_connection* conn, void* cbdata) {
    int count = 0;
    char** items = hash_collect(&g_hash, &count);
    /* items is an array of "key:value" strings */
    char* json = build_json_array_response("hash", (const char**)items, count);
    /* free all arr[i], then arr */
    if (items) {
        for (int i = 0; i < count; i++) {
            free(items[i]);
        }
        free(items);
    }
    send_json(conn, json ? json : "{ \"hash\": [] }");
    if (json) free(json);
    return 200;
}

static int handle_hash_insert(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));
    char key[64], val[64];
    if (extract_key_val(body, key, sizeof(key), val, sizeof(val))) {
        hash_insert(&g_hash, key, val);
    }
    return handle_hash_getall(conn, NULL);
}

static int handle_hash_remove(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));

    char key[64];
    if (!extract_key_from_body(body, key, sizeof(key))) {
        /* If parsing failed, we can send an error or just ignore */
        send_json(conn, "{ \"error\": \"Missing or invalid 'key'\" }");
        return 400;
    }
    hash_remove(&g_hash, key);

    /* Return updated table after removal */
    return handle_hash_getall(conn, NULL);
}

static int handle_hash_contains(struct mg_connection* conn, void* cbdata) {
    char body[1024];
    read_request_body(conn, body, sizeof(body));

    char key[64];
    int result = 0;
    // Use a helper that extracts just the "key" field from a JSON like {"key": "hi"}
    if (extract_key_from_body(body, key, sizeof(key))) {
        result = hash_contains(&g_hash, key);
    }

    char resp[64];
    snprintf(resp, sizeof(resp), "{ \"contains\": %s }", result ? "true" : "false");
    send_json(conn, resp);
    return 200;
}
/* ---------------------------------------------------------------------------
   BST Endpoints
--------------------------------------------------------------------------- */

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


/* ---------------------------------------------------------------------------
   register_endpoints
--------------------------------------------------------------------------- */
void register_endpoints(struct mg_context* ctx) {
    /* Initialize global data structures */
    ll_init(&g_list);
    stk_init(&g_stack);
    que_init(&g_queue);
    tree_init(&g_tree);
    heap_init(&g_heap);
    hash_init(&g_hash);
    bst_init(&g_bst);

    /* LinkedList Endpoints */
    mg_set_request_handler(ctx, "/api/linked-list", handle_get_linked_list, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/add-front", handle_post_list_add_front, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/add-end", handle_post_list_add_end, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/remove-front", handle_delete_list_remove_front, NULL);
    mg_set_request_handler(ctx, "/api/linked-list/remove-end", handle_delete_list_remove_end, NULL);

    /* Stack Endpoints */
    mg_set_request_handler(ctx, "/api/stack", handle_get_stack, NULL);
    mg_set_request_handler(ctx, "/api/stack/push", handle_post_stack_push, NULL);
    mg_set_request_handler(ctx, "/api/stack/pop", handle_delete_stack_pop, NULL);

    /* Queue Endpoints */
    mg_set_request_handler(ctx, "/api/queue", handle_get_queue, NULL);
    mg_set_request_handler(ctx, "/api/queue/enqueue", handle_post_queue_enqueue, NULL);
    mg_set_request_handler(ctx, "/api/queue/dequeue", handle_delete_queue_dequeue, NULL);

    /* Tree Endpoints */
    mg_set_request_handler(ctx, "/api/tree", handle_tree_get, NULL);
    mg_set_request_handler(ctx, "/api/tree/insert", handle_tree_insert, NULL);
    mg_set_request_handler(ctx, "/api/tree/remove", handle_tree_remove, NULL);
    mg_set_request_handler(ctx, "/api/tree/insert-child", handle_tree_insert_child, NULL);
    mg_set_request_handler(ctx, "/api/tree/remove-child", handle_tree_remove_child, NULL);

    /* Register Heap endpoints */
    mg_set_request_handler(ctx, "/api/heap", handle_heap_get, NULL);
    mg_set_request_handler(ctx, "/api/heap/insert", handle_heap_insert, NULL);
    mg_set_request_handler(ctx, "/api/heap/extract-top", handle_heap_extract_top, NULL);

    /* Register Hash endpoints */
    mg_set_request_handler(ctx, "/api/hash", handle_hash_getall, NULL);
    mg_set_request_handler(ctx, "/api/hash/insert", handle_hash_insert, NULL);
    mg_set_request_handler(ctx, "/api/hash/remove", handle_hash_remove, NULL);
    mg_set_request_handler(ctx, "/api/hash/contains", handle_hash_contains, NULL);

    /* BST Endpoints */
    mg_set_request_handler(ctx, "/api/bst", handle_bst_get, NULL);
    mg_set_request_handler(ctx, "/api/bst/insert", handle_bst_insert, NULL);
    mg_set_request_handler(ctx, "/api/bst/remove", handle_bst_remove, NULL);
}

#define _CRT_SECURE_NO_WARNINGS
#include "route_handlers.h"
#include "ds_tree.h"
#include "route_utils.h" 
#include <civetweb.h>
#include <stdlib.h>
#include <string.h>

extern SimpleTree g_tree;  // Global declared in route_handlers.c

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

static int handle_tree_get(struct mg_connection* conn, void* cbdata) {
    char* treeJson = tree_to_json(g_tree.root);
    int bufSize = (int)strlen(treeJson) + 64;
    char* json = (char*)malloc(bufSize);
    if (json) {
        snprintf(json, bufSize, "{ \"tree\": %s }", treeJson);
    }
    free(treeJson);
    send_json(conn, json ? json : "{ \"tree\": null }");
    if (json)
        free(json);
    return SUCCESS_RESPONSE_CODE;
}

static int handle_tree_insert(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
    if (extract_value_from_body(body, value, sizeof(value))) {
        tree_insert(&g_tree, value);
    }
    return handle_tree_get(conn, NULL);
}

static int handle_tree_remove(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
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

static int handle_tree_insert_child(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
    read_request_body(conn, body, sizeof(body));
    char value[MAX_VALUE_LEN];
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

static int handle_tree_remove_child(struct mg_connection* conn, void* cbdata) {
    char body[MAX_BODY_LEN];
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

/* New endpoint: Clear the entire tree */
static int handle_tree_clear(struct mg_connection* conn, void* cbdata) {
    tree_clear(&g_tree);
    return handle_tree_get(conn, NULL);
}

void register_tree_endpoints(struct mg_context* ctx) {
    mg_set_request_handler(ctx, "/api/tree", handle_tree_get, NULL);
    mg_set_request_handler(ctx, "/api/tree/insert", handle_tree_insert, NULL);
    mg_set_request_handler(ctx, "/api/tree/remove", handle_tree_remove, NULL);
    mg_set_request_handler(ctx, "/api/tree/insert-child", handle_tree_insert_child, NULL);
    mg_set_request_handler(ctx, "/api/tree/remove-child", handle_tree_remove_child, NULL);
    mg_set_request_handler(ctx, "/api/tree/clear", handle_tree_clear, NULL);
}

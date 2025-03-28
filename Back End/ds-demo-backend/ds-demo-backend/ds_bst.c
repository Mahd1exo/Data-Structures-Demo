#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_bst.h"


// O(n)
/* Helper: safely copy string with null termination */
static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

/* Create a new BST node with a unique id */
static BSTNode* create_node(SimpleBST* tree, const char* value) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    if (!node) return NULL;
    node->id = tree->nextId;
    tree->nextId++;
    safeCopy(node->value, value, BST_MAX_LEN);
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Initialize the BST */
void bst_init(SimpleBST* tree) {
    if (!tree) return;
    tree->root = NULL;
    tree->nextId = 0;
}

/* Recursive helper for BST insertion using numeric comparison */
static BSTNode* bst_insert_node(BSTNode* root, SimpleBST* tree, const char* value) {
    if (!root) {
        return create_node(tree, value);
    }
    double newVal = atof(value);
    double nodeVal = atof(root->value);
    if (newVal < nodeVal) {
        root->left = bst_insert_node(root->left, tree, value);
    }
    else if (newVal > nodeVal) {
        root->right = bst_insert_node(root->right, tree, value);
    }
    else {
        /* Duplicate: update the value */
        safeCopy(root->value, value, BST_MAX_LEN);
    }
    return root;
}


/* Insert a value into the BST */
void bst_insert(SimpleBST* tree, const char* value) {
    if (!tree) return;
    tree->root = bst_insert_node(tree->root, tree, value);
}

/* Find the node with minimum numeric value in a subtree */
static BSTNode* find_min(BSTNode* node) {
    while (node && node->left)
        node = node->left;
    return node;
}

/* Recursive helper for BST removal using numeric comparison */
static BSTNode* bst_remove_node(BSTNode* root, const char* value) {
    if (!root) return NULL;
    double newVal = atof(value);
    double nodeVal = atof(root->value);
    if (newVal < nodeVal) {
        root->left = bst_remove_node(root->left, value);
    }
    else if (newVal > nodeVal) {
        root->right = bst_remove_node(root->right, value);
    }
    else {
        /* Found node to remove */
        if (!root->left) {
            BSTNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (!root->right) {
            BSTNode* temp = root->left;
            free(root);
            return temp;
        }
        else {
            BSTNode* temp = find_min(root->right);
            safeCopy(root->value, temp->value, BST_MAX_LEN);
            root->right = bst_remove_node(root->right, temp->value);
        }
    }
    return root;
}

/* Remove a node (by value) from the BST */
void bst_remove(SimpleBST* tree, const char* value) {
    if (!tree) return;
    tree->root = bst_remove_node(tree->root, value);
}

/* Recursive helper to perform in-order traversal and collect node values */
static void inorder_collect(BSTNode* root, const char*** arr, int* count, int* cap) {
    if (!root) return;
    inorder_collect(root->left, arr, count, cap);
    if (*count >= *cap) {
        *cap *= 2;
        const char** temp = (const char**)realloc((void*)*arr, sizeof(char*) * (*cap));
        if (temp) {
            *arr = temp;
        }
    }
    (*arr)[*count] = root->value;
    (*count)++;
    inorder_collect(root->right, arr, count, cap);
}

/* Collect BST nodes in in-order (sorted order) */
char** bst_collect_inorder(const SimpleBST* tree, int* count) {
    if (!tree) {
        if (count) *count = 0;
        return NULL;
    }
    int capacity = 16, c = 0;
    const char** arr = (const char**)malloc(sizeof(char*) * capacity);
    if (!arr) {
        if (count) *count = 0;
        return NULL;
    }
    inorder_collect(tree->root, &arr, &c, &capacity);
    char** res = (char**)arr;  // cast away const, values belong to nodes.
    if (count) *count = c;
    return res;
}

/* Recursive helper to free BST nodes */
static void bst_free_nodes(BSTNode* root) {
    if (!root) return;
    bst_free_nodes(root->left);
    bst_free_nodes(root->right);
    free(root);
}

/* Clear the entire BST */
void bst_clear(SimpleBST* tree) {
    if (!tree) return;
    bst_free_nodes(tree->root);
    tree->root = NULL;
}

/* --- Convert BST to JSON --- */
/* Recursive function to convert a BST node to JSON.
   If a child is missing, returns "null".
   Format: { "id": <id>, "value": "<value>", "left": <leftJson>, "right": <rightJson> }
*/
static char* bst_to_json_recursive(BSTNode* node) {
    if (!node) {
        return _strdup("null");
    }
    char* leftJson = bst_to_json_recursive(node->left);
    char* rightJson = bst_to_json_recursive(node->right);
    int bufSize = 256 + (int)strlen(leftJson) + (int)strlen(rightJson);
    char* result = (char*)malloc(bufSize);
    if (result) {
        snprintf(result, bufSize, "{ \"id\": %d, \"value\": \"%s\", \"left\": %s, \"right\": %s }",
            node->id, node->value, leftJson, rightJson);
    }
    free(leftJson);
    free(rightJson);
    return result;
}

/* Convert entire BST to JSON */
char* bst_to_json(const SimpleBST* tree) {
    if (!tree) return _strdup("{ \"bst\": null }");
    char* nodeJson = bst_to_json_recursive(tree->root);
    int bufSize = (int)strlen(nodeJson) + 64;
    char* result = (char*)malloc(bufSize);
    if (result) {
        snprintf(result, bufSize, "{ \"bst\": %s }", nodeJson);
    }
    free(nodeJson);
    return result;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_avl.h"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

void avl_init(SimpleAVL* tree) {
    if (!tree) return;
    tree->root = NULL;
}

void avl_insert(SimpleAVL* tree, const char* value) {
    if (!tree) return;
    tree->root = avl_insert_node(tree->root, value);
}

void avl_remove(SimpleAVL* tree, const char* value) {
    if (!tree) return;
    tree->root = avl_remove_node(tree->root, value);
}

void avl_clear(SimpleAVL* tree) {
    if (!tree) return;
    avl_clear_nodes(tree->root);
    tree->root = NULL;
}

char* avl_to_json(SimpleAVL* tree) {
    if (!tree || !tree->root) {
        char* empty_json = (char*)malloc(16);
        strcpy(empty_json, "{\"bst\":null}");
        return empty_json;
    }
    int next_id = 0;
    char* root_json = avl_node_to_json(tree->root, &next_id);
    int needed = snprintf(NULL, 0, "{\"bst\":%s}", root_json);
    char* out = (char*)malloc(needed + 1);
    sprintf(out, "{\"bst\":%s}", root_json);
    free(root_json);
    return out;
}

static int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

static int height(AVLNode* node) {
    return node ? node->height : 0;
}

static int getBalance(AVLNode* node) {
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

static AVLNode* create_node(const char* value) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) return NULL;
    node->data = atoi(value);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

static AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = maxInt(height(y->left), height(y->right)) + 1;
    x->height = maxInt(height(x->left), height(x->right)) + 1;
    return x;
}

static AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = maxInt(height(x->left), height(x->right)) + 1;
    y->height = maxInt(height(y->left), height(y->right)) + 1;
    return y;
}

static AVLNode* avl_insert_node(AVLNode* node, const char* value) {
    if (!node) {
        return create_node(value);
    }
    int newVal = atoi(value);
    if (newVal < node->data) {
        node->left = avl_insert_node(node->left, value);
    }
    else if (newVal > node->data) {
        node->right = avl_insert_node(node->right, value);
    }
    else {
        // Duplicate valu just return
        return node;
    }

    node->height = 1 + maxInt(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && newVal < node->left->data)
        return rightRotate(node);
    // Right Right Case
    if (balance < -1 && newVal > node->right->data)
        return leftRotate(node);
    // Left Right Case
    if (balance > 1 && newVal > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right Left Case
    if (balance < -1 && newVal < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

static AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

static AVLNode* avl_remove_node(AVLNode* root, const char* value) {
    if (!root) return root;

    int target = atoi(value);
    if (target < root->data) {
        root->left = avl_remove_node(root->left, value);
    }
    else if (target > root->data) {
        root->right = avl_remove_node(root->right, value);
    }
    else {
        // Node to be deleted found.
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                // No child case.
                temp = root;
                root = NULL;
            }
            else {
                // One child case: copy the contents.
                *root = *temp;
            }
            free(temp);
        }
        else {
            // Node with two children: Get the inorder successor.
            AVLNode* temp = minValueNode(root->right);
            root->data = temp->data;
            // Convert temp->data back to string for removal.
            char buffer[32];
            sprintf(buffer, "%d", temp->data);
            root->right = avl_remove_node(root->right, buffer);
        }
    }

    if (!root) return root;

    root->height = 1 + maxInt(height(root->left), height(root->right));
    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

static void avl_clear_nodes(AVLNode* node) {
    if (!node) return;
    avl_clear_nodes(node->left);
    avl_clear_nodes(node->right);
    free(node);
}

static char* avl_node_to_json(AVLNode* node, int* next_id) {
    if (!node) {
        char* null_str = (char*)malloc(5);
        strcpy(null_str, "null");
        return null_str;
    }
    int my_id = (*next_id)++;
    char* left_json = avl_node_to_json(node->left, next_id);
    char* right_json = avl_node_to_json(node->right, next_id);
    int needed = snprintf(NULL, 0, "{\"id\":%d,\"value\":%d,\"left\":%s,\"right\":%s}",
        my_id, node->data, left_json, right_json);
    char* out = (char*)malloc(needed + 1);
    sprintf(out, "{\"id\":%d,\"value\":%d,\"left\":%s,\"right\":%s}",
        my_id, node->data, left_json, right_json);
    free(left_json);
    free(right_json);
    return out;
}

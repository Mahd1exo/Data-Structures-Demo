#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_tree.h"

/* Helper: Safely copy src into dest */
static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

/* Free a subtree recursively */
static void free_subtree(TreeNode* node) {
    if (!node) return;
    for (int i = 0; i < node->childrenCount; i++) {
        free_subtree(node->children[i]);
    }
    free(node->children);
    free(node);
}

/* Initialize the tree */
void tree_init(SimpleTree* tree) {
    if (!tree) return;
    tree->root = NULL;
    tree->nextId = 0;  /* IDs start at 0 */
}

/* Create a new node with a unique id */
static TreeNode* create_node(SimpleTree* tree, const char* value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->id = tree->nextId;
    tree->nextId++;
    safeCopy(node->data, value, TREE_MAX_LEN);
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}

/* Default insertion: if tree is empty, new node becomes root;
   otherwise, insert as a child of the root */
void tree_insert(SimpleTree* tree, const char* value) {
    if (!tree) return;
    if (!tree->root) {
        tree->root = create_node(tree, value);
    }
    else {
        int newCount = tree->root->childrenCount + 1;
        TreeNode** newArr = (TreeNode**)realloc(tree->root->children, newCount * sizeof(TreeNode*));
        if (!newArr) return;
        tree->root->children = newArr;
        tree->root->children[newCount - 1] = create_node(tree, value);
        tree->root->childrenCount = newCount;
    }
}

/* Recursive helper to find a node by id */
static TreeNode* find_node_by_id(TreeNode* node, int targetId) {
    if (!node) return NULL;
    if (node->id == targetId) return node;
    for (int i = 0; i < node->childrenCount; i++) {
        TreeNode* found = find_node_by_id(node->children[i], targetId);
        if (found) return found;
    }
    return NULL;
}

/* Insert a node as a child of the node with given parentId */
void tree_insert_child(SimpleTree* tree, int parentId, const char* value) {
    if (!tree) return;
    if (!tree->root) {
        tree->root = create_node(tree, value);
        return;
    }
    TreeNode* parent = find_node_by_id(tree->root, parentId);
    if (!parent) return;
    int newCount = parent->childrenCount + 1;
    TreeNode** newArr = (TreeNode**)realloc(parent->children, newCount * sizeof(TreeNode*));
    if (!newArr) return;
    parent->children = newArr;
    parent->children[newCount - 1] = create_node(tree, value);
    parent->childrenCount = newCount;
}

/* Remove a node by id using BFS; if the root matches, clear the tree */
void tree_remove_by_id(SimpleTree* tree, int id) {
    if (!tree || !tree->root) return;
    if (tree->root->id == id) {
        free_subtree(tree->root);
        tree->root = NULL;
        return;
    }
    int capacity = 16, front = 0, rear = 0;
    TreeNode** queue = (TreeNode**)malloc(capacity * sizeof(TreeNode*));
    if (!queue) return;
    queue[rear++] = tree->root;
    while (front < rear) {
        TreeNode* curr = queue[front++];
        for (int i = 0; i < curr->childrenCount; i++) {
            if (curr->children[i]->id == id) {
                free_subtree(curr->children[i]);
                for (int j = i; j < curr->childrenCount - 1; j++) {
                    curr->children[j] = curr->children[j + 1];
                }
                curr->childrenCount--;
                if (curr->childrenCount > 0) {
                    curr->children = (TreeNode**)realloc(curr->children, curr->childrenCount * sizeof(TreeNode*));
                }
                else {
                    free(curr->children);
                    curr->children = NULL;
                }
                free(queue);
                return;
            }
        }
        for (int i = 0; i < curr->childrenCount; i++) {
            if (rear >= capacity) {
                capacity *= 2;
                queue = (TreeNode**)realloc(queue, capacity * sizeof(TreeNode*));
            }
            queue[rear++] = curr->children[i];
        }
    }
    free(queue);
}

/* Remove a child node under a specific parent by child id */
void tree_remove_child_by_id(SimpleTree* tree, int parentId, int childId) {
    if (!tree || !tree->root) return;
    TreeNode* parent = find_node_by_id(tree->root, parentId);
    if (!parent) return;
    for (int i = 0; i < parent->childrenCount; i++) {
        if (parent->children[i]->id == childId) {
            free_subtree(parent->children[i]);
            for (int j = i; j < parent->childrenCount - 1; j++) {
                parent->children[j] = parent->children[j + 1];
            }
            parent->childrenCount--;
            if (parent->childrenCount > 0) {
                parent->children = (TreeNode**)realloc(parent->children, parent->childrenCount * sizeof(TreeNode*));
            }
            else {
                free(parent->children);
                parent->children = NULL;
            }
            return;
        }
    }
}

/* Clear the entire tree */
void tree_clear(SimpleTree* tree) {
    if (!tree) return;
    free_subtree(tree->root);
    tree->root = NULL;
}

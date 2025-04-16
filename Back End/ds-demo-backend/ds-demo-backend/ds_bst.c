/*
* FILE : ds_bst.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a Binary Search Tree (BST) data structure.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_bst.h"

// FUNCTION     : create_node
// DESCRIPTION  :
// Creates a new BST node with the given value.
// PARAMETERS   : tree - pointer to the BST structure
//              value - the value to store in the node (as a string)
// RETURNS      : pointer to the new node (NULL if allocation fails)
static BSTNode* create_node(SimpleBST* tree, const char* value) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    if (!node) return NULL;
    node->id = tree->nextId;
    tree->nextId++;
    node->data = atoi(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// FUNCTION     : bst_init
// DESCRIPTION  :
// Initializes the BST.
// PARAMETERS   : tree - pointer to the BST structure
// RETURNS      : none
void bst_init(SimpleBST* tree) {
    if (!tree) return;
    tree->root = NULL;
    tree->nextId = 0;
}

// FUNCTION     : bst_insert_node
// DESCRIPTION  :
// Inserts a value into the BST recursively.
//// PARAMETERS   : root - pointer to the current node
////              tree - pointer to the BST structure
////              value - the value to insert (as a string)
// RETURNS      : pointer to the new root of the subtree after insertion
static BSTNode* bst_insert_node(BSTNode* root, SimpleBST* tree, const char* value) {
    if (!root) {
        return create_node(tree, value);
    }
    int newVal = atoi(value);
    int nodeVal = root->data;
    if (newVal < nodeVal) {
        root->left = bst_insert_node(root->left, tree, value);
    }
    else if (newVal > nodeVal) {
        root->right = bst_insert_node(root->right, tree, value);
    }
    else {
        root->data = newVal;
    }
    return root;
}

// FUNCTION     : bst_insert
// DESCRIPTION  :
// Inserts a value into the BST.
// PARAMETERS   : tree - pointer to the BST structure
//			  value - the value to insert (as a string)
// RETURNS      : none
void bst_insert(SimpleBST* tree, const char* value) {
    if (!tree) return;
    tree->root = bst_insert_node(tree->root, tree, value);
}

// FUNCTION     : find_min
// DESCRIPTION  :
//  Finds the node with the minimum value in a subtree.
//// PARAMETERS   : node - pointer to the root of the subtree
//// RETURNS      : pointer to the node with the minimum value
static BSTNode* find_min(BSTNode* node) {
    while (node && node->left)
        node = node->left;
    return node;
}

// FUNCTION     : bst_remove_node
// DESCRIPTION  :
// Removes a node with the given value from the BST recursively.
// PARAMETERS   : root - pointer to the current node
//              value - the value to remove (as a string)
// RETURNS      : pointer to the new root of the subtree after removal
static BSTNode* bst_remove_node(BSTNode* root, const char* value) {
    if (!root) return NULL;
    int newVal = atoi(value);
    int nodeVal = root->data;
    if (newVal < nodeVal) {
        root->left = bst_remove_node(root->left, value);
    }
    else if (newVal > nodeVal) {
        root->right = bst_remove_node(root->right, value);
    }
    else {
        // Found node to remove
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
            root->data = temp->data;
            char buffer[32];
            sprintf(buffer, "%d", temp->data);
            root->right = bst_remove_node(root->right, buffer);
        }
    }
    return root;
}

// FUNCTION     : bst_remove
// DESCRIPTION  :
// Removes a value from the BST.
// PARAMETERS   : tree - pointer to the BST structure
//              value - the value to remove (as a string)
// RETURNS      : none
void bst_remove(SimpleBST* tree, const char* value) {
    if (!tree) return;
    tree->root = bst_remove_node(tree->root, value);
}

// FUNCTION     : bst_collect_inorder
// DESCRIPTION  :
// Collects the BST nodes in in-order (sorted order).
// PARAMETERS   : tree - pointer to the BST structure
//			  count - pointer to store the number of nodes collected
//            arr - pointer to store the array of strings
//            capacity - pointer to store the capacity of the array
//// RETURNS      : pointer to an array of strings representing the node values
static void inorder_collect(BSTNode* root, char*** arr, int* count, int* capacity) {
    if (!root) return;
    inorder_collect(root->left, arr, count, capacity);
    if (*count >= *capacity) {
        *capacity *= 2;
        char** temp = (char**)realloc(*arr, sizeof(char*) * (*capacity));
        if (temp) {
            *arr = temp;
        }
    }
    // Convert the integer data to a string.
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", root->data);
    (*arr)[*count] = (char*)malloc(strlen(buffer) + 1);
    if ((*arr)[*count]) {
        strcpy((*arr)[*count], buffer);
    }
    (*count)++;
    inorder_collect(root->right, arr, count, capacity);
}

// FUNCTION     : bst_collect_inorder
// DESCRIPTION  :
// Collects the BST nodes in in-order (sorted order).
// PARAMETERS   : tree - pointer to the BST structure
// 			count - pointer to store the number of nodes collected
// RETURNS      : pointer to an array of strings representing the node values
char** bst_collect_inorder(const SimpleBST* tree, int* count) {
    if (!tree) {
        if (count) *count = 0;
        return NULL;
    }
    int capacity = 16, c = 0;
    char** arr = (char**)malloc(sizeof(char*) * capacity);
    if (!arr) {
        if (count) *count = 0;
        return NULL;
    }
    inorder_collect(tree->root, &arr, &c, &capacity);
    if (count) *count = c;
    return arr;
}

// FUNCTION     : bst_free_nodes
// DESCRIPTION  :
// Frees all nodes in the BST recursively.
// PARAMETERS   : root - pointer to the current node
// RETURNS      : none
static void bst_free_nodes(BSTNode* root) {
    if (!root) return;
    bst_free_nodes(root->left);
    bst_free_nodes(root->right);
    free(root);
}

// FUNCTION     : bst_clear
// DESCRIPTION  :
// Clears the BST, freeing all nodes.
//// PARAMETERS   : tree - pointer to the BST structure
//// RETURNS      : none
void bst_clear(SimpleBST* tree) {
    if (!tree) return;
    bst_free_nodes(tree->root);
    tree->root = NULL;
}

// FUNCTION     : bst_to_json_recursive
// DESCRIPTION  :
// Converts a single BST node to a JSON string representation.
//// PARAMETERS   : node - pointer to the BST node
//// RETURNS      : JSON string representing the node
static char* bst_to_json_recursive(BSTNode* node) {
    if (!node) {
        char* null_str = (char*)malloc(5); 
        if (null_str) {
            strcpy(null_str, "null");
        }
        return null_str;
    }
    char* leftJson = bst_to_json_recursive(node->left);
    char* rightJson = bst_to_json_recursive(node->right);
    int bufSize = 256 + (int)strlen(leftJson) + (int)strlen(rightJson);
    char* result = (char*)malloc(bufSize);
    if (result) {
        snprintf(result, bufSize, "{ \"id\": %d, \"value\": %d, \"left\": %s, \"right\": %s }",
            node->id, node->data, leftJson, rightJson);
    }
    free(leftJson);
    free(rightJson);
    return result;
}

// FUNCTION     : bst_to_json
// DESCRIPTION  :
// Converts the entire BST to a JSON string representation.
//// PARAMETERS   : tree - pointer to the BST structure
//// RETURNS      : JSON string representing the BST
// RETURNS      : JSON string (caller is responsible for freeing it)
char* bst_to_json(const SimpleBST* tree) {
    if (!tree) {
        const char* bst_null = "{ \"bst\": null }";
        char* result = (char*)malloc(strlen(bst_null) + 1);
        if (result) {
            strcpy(result, bst_null);
        }
        return result;
    }
    char* nodeJson = bst_to_json_recursive(tree->root);
    int bufSize = (int)strlen(nodeJson) + 64;
    char* result = (char*)malloc(bufSize);
    if (result) {
        snprintf(result, bufSize, "{ \"bst\": %s }", nodeJson);
    }
    free(nodeJson);
    return result;
}

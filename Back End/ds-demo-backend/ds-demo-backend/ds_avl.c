/*
* FILE : ds_avl.c
* PROJECT : SENG1050 - Data Structures 
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of AVL tree data structure
* REFERENCES :
*  1. https://en.wikipedia.org/wiki/AVL_tree
*  2. https://www.geeksforgeeks.org/c-program-to-implement-avl-tree/
*  3. https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
*  4. https://github.com/xieqing/avl-tree
*/
// Header files
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_avl.h"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

// FUNCTION     : avl_init
// DESCRIPTION  :
// Initializes the AVL tree.
// PARAMETERS   : tree - pointer to the AVL tree structure
// RETURNS      : none
void avl_init(SimpleAVL* tree) {
    if (!tree) return;
    tree->root = NULL;
}

// FUNCTION     : avl_insert
// DESCRIPTION  :
// Inserts a value into the AVL tree.
// PARAMETERS   : tree - pointer to the AVL tree structure
//               value - the value to insert (as a string)
// RETURNS      : none
void avl_insert(SimpleAVL* tree, const char* value) {
    if (!tree) return;
    tree->root = avl_insert_node(tree->root, value);
}

// FUNCTION     : avl_remove
// DESCRIPTION  :
// Removes a value from the AVL tree.
// PARAMETERS   : tree - pointer to the AVL tree structure
//               value - the value to remove (as a string)
// RETURNS      : none
void avl_remove(SimpleAVL* tree, const char* value) {
    if (!tree) return;
    tree->root = avl_remove_node(tree->root, value);
}

// FUNCTION     : avl_clear
// DESCRIPTION  :
// Clears the AVL tree, freeing all nodes.
// PARAMETERS   : tree - pointer to the AVL tree structure
// RETURNS      : none
void avl_clear(SimpleAVL* tree) {
    if (!tree) return;
    avl_clear_nodes(tree->root);
    tree->root = NULL;
}

// FUNCTION     : avl_to_json
// DESCRIPTION  :
// Converts the AVL tree to a JSON string representation.
// PARAMETERS   : tree - pointer to the AVL tree structure
// 		   returns a JSON string representing the tree
// RETURNS      : JSON string (caller is responsible for freeing it)
char* avl_to_json(SimpleAVL* tree) {

	// Check if the tree is empty
    if (!tree || !tree->root) {
        char* empty_json = (char*)malloc(MAX_JSON_BUFFER);
        strcpy(empty_json, "{\"bst\":null}");
        return empty_json;
    }
	// Create a JSON representation of the AVL tree
    int next_id = 0;
    char* root_json = avl_node_to_json(tree->root, &next_id);
    int needed = snprintf(NULL, 0, "{\"bst\":%s}", root_json);
    char* out = (char*)malloc(needed + 1);
    sprintf(out, "{\"bst\":%s}", root_json);
    free(root_json);
    return out;
}

// Helper functions for AVL tree operations
// FUNCTION     : maxInt
// DESCRIPTION  :
// Returns the maximum of two integers.
// PARAMETERS   : a - first integer
//			   b - second integer
// RETURNS      : maximum of a and b
static int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

// FUNCTION     : height
// DESCRIPTION  :
// Returns the height of a node.
// PARAMETERS   : node - pointer to the AVL node
// RETURNS      : height of the node (0 if NULL)
static int height(AVLNode* node) {
    return node ? node->height : 0;
}

// FUNCTION     : getBalance
// DESCRIPTION  :
// Returns the balance factor of a node.
// PARAMETERS   : node - pointer to the AVL node
// RETURNS      : balance factor (height of left subtree - height of right subtree)
static int getBalance(AVLNode* node) {
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

// FUNCTION     : create_node
// DESCRIPTION  :
// Creates a new AVL node with the given value.
// PARAMETERS   : value - the value to store in the node (as a string)
// RETURNS      : pointer to the new node (NULL if allocation fails)
static AVLNode* create_node(const char* value) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) return NULL;
    node->data = atoi(value);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// FUNCTION     : rightRotate
// DESCRIPTION  :
// Performs a right rotation on the given subtree.
// PARAMETERS   : root - pointer to the root of the subtree
// RETURNS      : new root of the subtree after rotation
static AVLNode* rightRotate(AVLNode* root) {
	// Perform right rotation
    AVLNode* leftSubtree = root->left;
	// Update the left child of the root
    AVLNode* tempRight = leftSubtree->right;

    // Perform rotation
    leftSubtree->right = root;
    root->left = tempRight;

    // Update heights
    root->height = maxInt(height(root->left), height(root->right)) + 1;
    leftSubtree->height = maxInt(height(leftSubtree->left), height(leftSubtree->right)) + 1;
    return leftSubtree;
}

// FUNCTION     : leftRotate
// DESCRIPTION  :
// Performs a left rotation on the given subtree.
// PARAMETERS   : root - pointer to the root of the subtree
// RETURNS      : new root of the subtree after rotation
static AVLNode* leftRotate(AVLNode* root) {
	// Perform left rotation
    AVLNode* rightSubtree = root->right;
    AVLNode* tempLeft = rightSubtree->left;

    // Perform rotation
    rightSubtree->left = root;
    root->right = tempLeft;

    // Update heights
    root->height = maxInt(height(root->left), height(root->right)) + 1;
    rightSubtree->height = maxInt(height(rightSubtree->left), height(rightSubtree->right)) + 1;
    return rightSubtree;
}

// FUNCTION     : avl_insert_node
// DESCRIPTION  :
// Inserts a value into the AVL tree recursively.
// PARAMETERS   : node - pointer to the current node
//			      value - the value to insert (as a string)
// RETURNS      : pointer to the new root of the 
//			       subtree after insertion
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

// FUNCTION     : minValueNode
// DESCRIPTION  :
// Finds the node with the minimum value in a subtree.
// PARAMETERS   : node - pointer to the root of the subtree
// RETURNS      : pointer to the node with the minimum value
static AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

// FUNCTION     : avl_remove_node
// DESCRIPTION  :
// Removes a node with the given value from the AVL tree recursively.
// PARAMETERS   : root - pointer to the current node
// 		      value - the value to remove (as a string)
// RETURNS      : pointer to the new root of the subtree after removal
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
                *root = *temp;
            }
            free(temp);
        }
        else {
            // Node with two children: Get the inorder successor.
            AVLNode* temp = minValueNode(root->right);
            root->data = temp->data;
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
// FUNCTION     : avl_node_to_json
// DESCRIPTION  :
// Converts a single AVL node to a JSON string representation.
// PARAMETERS   : node - pointer to the AVL node
//               next_id - pointer to the next ID to use for the node
// RETURNS      : JSON string representing the node
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

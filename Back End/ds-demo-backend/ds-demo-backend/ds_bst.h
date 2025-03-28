/*
* FILE : ds_bst.h
* PROJECT : FINAL PROJECT - DATA STRUCTURE - Data Structure Demo
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2024-03-10
*
* DESCRIPTION :
* This file contains the declaration of the Binary Search Tree (BST) data structure and its functions.
*/
#ifndef DS_BST_H
#define DS_BST_H

#define BST_MAX_LEN 64

/* BST Node structure */
typedef struct BSTNode {
    char value[BST_MAX_LEN];   
    struct BSTNode* left;
    struct BSTNode* right;
    int id;                    
} BSTNode;


typedef struct {
    BSTNode* root;
    int nextId;
} SimpleBST;

#ifdef __cplusplus
extern "C" {
#endif

    void bst_init(SimpleBST* tree);
    void bst_insert(SimpleBST* tree, const char* value);
    void bst_remove(SimpleBST* tree, const char* value);
    char** bst_collect_inorder(const SimpleBST* tree, int* count);
    void bst_clear(SimpleBST* tree);
    char* bst_to_json(const SimpleBST* tree);


#ifdef __cplusplus
}
#endif

#endif /* DS_BST_H */

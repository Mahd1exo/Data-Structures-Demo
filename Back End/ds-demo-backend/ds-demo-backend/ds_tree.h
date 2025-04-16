/*
* FILE : ds_tree.h
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the declaration of a simple tree data structure and its functions.
*/
#ifndef DS_TREE_H
#define DS_TREE_H

#define TREE_MAX_LEN 64


typedef struct TreeNode {
    int id;                      
    char data[TREE_MAX_LEN];     
    struct TreeNode** children;  
    int childrenCount;
} TreeNode;

typedef struct {
    TreeNode* root;
    int nextId;
} SimpleTree;

#ifdef __cplusplus
extern "C" {
#endif

    void tree_init(SimpleTree* tree);
    void tree_insert(SimpleTree* tree, const char* value);
    void tree_insert_child(SimpleTree* tree, int parentId, const char* value);
    void tree_remove_by_id(SimpleTree* tree, int id);
    void tree_remove_child_by_id(SimpleTree* tree, int parentId, int childId);
    void tree_clear(SimpleTree* tree);

#ifdef __cplusplus
}
#endif

#endif /* DS_TREE_H */

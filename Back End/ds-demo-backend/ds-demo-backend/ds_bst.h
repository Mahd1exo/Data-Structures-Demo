#ifndef DS_BST_H
#define DS_BST_H

#define BST_MAX_LEN 64

/* BST Node structure */
typedef struct BSTNode {
    char value[BST_MAX_LEN];   /* Node value stored as a string */
    struct BSTNode* left;
    struct BSTNode* right;
    int id;                    /* Unique id assigned at insertion */
} BSTNode;

/* Simple BST structure with a pointer to the root and a counter for unique ids */
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

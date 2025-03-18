#ifndef DS_TREE_H
#define DS_TREE_H

#define TREE_MAX_LEN 64

/* A general tree node that can have any number of children.
   Each node stores a unique integer id and a data string. */
typedef struct TreeNode {
    int id;                      /* Unique identifier */
    char data[TREE_MAX_LEN];     /* Node value */
    struct TreeNode** children;  /* Dynamic array of child pointers */
    int childrenCount;
} TreeNode;

typedef struct {
    TreeNode* root;
    int nextId; /* Counter for unique IDs */
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


#ifndef DS_AVL_H
#define DS_AVL_H

#define AVL_MAX_LEN 64

typedef struct AVLNode {
    char data[AVL_MAX_LEN];
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

typedef struct {
    AVLNode* root;
} SimpleAVL;

#ifdef __cplusplus
extern "C" {
#endif

    void avl_init(SimpleAVL* tree);
    void avl_insert(SimpleAVL* tree, const char* value);
    void avl_remove(SimpleAVL* tree, const char* value);
    char* avl_to_json(SimpleAVL* tree);
    void avl_clear(SimpleAVL* tree);


    //header
    static int maxInt(int a, int b);
    static int height(AVLNode* node);
    static int getBalance(AVLNode* node);
    static AVLNode* create_node(const char* value);
    static AVLNode* rightRotate(AVLNode* y);
    static AVLNode* leftRotate(AVLNode* x);
    static AVLNode* avl_insert_node(AVLNode* node, const char* value);
    static AVLNode* avl_remove_node(AVLNode* root, const char* value);
    static AVLNode* minValueNode(AVLNode* node);
    static void avl_clear_nodes(AVLNode* node);
    static char* avl_node_to_json(AVLNode* node, int* next_id);

#ifdef __cplusplus
}
#endif

#endif /* DS_AVL_H */

#ifndef DS_LINKEDLIST_H
#define DS_LINKEDLIST_H

#define LL_MAX_DATA_LEN 64

/* Linked List Node */
typedef struct LLNode {
    char data[LL_MAX_DATA_LEN];
    struct LLNode* next;
} LLNode;

/* Linked List: Head pointer and functions */
typedef struct LinkedList {
    LLNode* head;
} LinkedList;

#ifdef __cplusplus
extern "C" {
#endif

    void ll_init(LinkedList* list);
    void ll_add_front(LinkedList* list, const char* value);
    void ll_add_end(LinkedList* list, const char* value);
    void ll_remove_front(LinkedList* list);
    void ll_remove_end(LinkedList* list);

    /* Returns an array of char* with all node data, along with a node count
       The caller is responsible for freeing the array (but NOT the strings themselves). */
    char** ll_collect_data(const LinkedList* list, int* count);

#ifdef __cplusplus
}
#endif

#endif /* DS_LINKEDLIST_H */

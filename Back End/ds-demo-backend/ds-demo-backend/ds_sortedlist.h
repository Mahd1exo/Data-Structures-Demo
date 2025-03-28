#ifndef DS_SORTEDLIST_H
#define DS_SORTEDLIST_H

#define SL_MAX_DATA_LEN 64

/* A node in the sorted linked list */
typedef struct SortedListNode {
    int data;
    struct SortedListNode* next;
} SortedListNode;

/* The sorted list itself */
typedef struct {
    SortedListNode* head;
} SortedList;

#ifdef __cplusplus
extern "C" {
#endif

    void sl_init(SortedList* list);

    void sl_insert(SortedList* list, const char* value);

    void sl_remove(SortedList* list, const char* value);

    char** sl_collect(const SortedList* list, int* count);

    void sl_clear(SortedList* list);

#ifdef __cplusplus
}
#endif

#endif /* DS_SORTEDLIST_H */

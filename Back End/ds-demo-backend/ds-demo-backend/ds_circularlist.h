#ifndef DS_CIRCULARLIST_H
#define DS_CIRCULARLIST_H

#define CL_MAX_DATA_LEN 64

typedef struct CircularListNode {
    char data[CL_MAX_DATA_LEN];
    struct CircularListNode* next;
} CircularListNode;

typedef struct {
    CircularListNode* head;
} CircularList;

#ifdef __cplusplus
extern "C" {
#endif

    void cl_init(CircularList* list);

    /* Insertion Functions */
    void cl_insert(CircularList* list, const char* value);                
    void cl_insert_by_index(CircularList* list, const char* value, int index); 
    void cl_add_front(CircularList* list, const char* value);               
    void cl_add_end(CircularList* list, const char* value);                 

    /* Removal Functions */
    void cl_remove(CircularList* list, const char* value);                  
    void cl_remove_front(CircularList* list);                         
    void cl_remove_end(CircularList* list);                                 
    void cl_remove_by_index(CircularList* list, int index);                 

    char** cl_collect_data(const CircularList* list, int* count);
    void cl_clear(CircularList* list);

#ifdef __cplusplus
}
#endif

#endif /* DS_CIRCULARLIST_H */

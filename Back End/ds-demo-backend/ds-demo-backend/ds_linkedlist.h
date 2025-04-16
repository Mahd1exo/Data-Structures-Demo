/*
* FILE : ds_linkedlist.h
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the declaration of a Linked List data structure and its functions.
*/
#ifndef DS_LINKEDLIST_H
#define DS_LINKEDLIST_H

#define LL_MAX_DATA_LEN 64


typedef struct LLNode {
    char data[LL_MAX_DATA_LEN];
    struct LLNode* next;
} LLNode;


typedef struct LinkedList {
    LLNode* head;
} LinkedList;

#ifdef __cplusplus
extern "C" {
#endif

    void linkedlist_init(LinkedList* list);
    void linkedlist_add_front(LinkedList* list, const char* value);
    void linkedlist_add_end(LinkedList* list, const char* value);
    void linkedlist_remove_front(LinkedList* list);
    void linkedlist_remove_end(LinkedList* list);
    char** linkedlist_collect_data(const LinkedList* list, int* count);
	void linkedlist_clear(LinkedList* list);
	void linkedlist_add_by_index(LinkedList* list, const char* value, int index);
	void linkedlist_remove_by_index(LinkedList* list, int index);


#ifdef __cplusplus
}
#endif

#endif /* DS_LINKEDLIST_H */

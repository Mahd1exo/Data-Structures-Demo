/*
* FILE : ds_sortedlist.h
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the declaration of the Sorted List data structure and its functions.
*/
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

    void sortedlist_init(SortedList* list);
    void sortedlist_insert(SortedList* list, const char* value);
    void sortedlist_remove(SortedList* list, const char* value);
    char** sortedlist_collect(const SortedList* list, int* count);
    void sortedlist_clear(SortedList* list);

#ifdef __cplusplus
}
#endif

#endif /* DS_SORTEDLIST_H */

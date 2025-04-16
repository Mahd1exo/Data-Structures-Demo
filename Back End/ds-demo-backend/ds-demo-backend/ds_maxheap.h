/*
* FILE : ds_maxheap.h
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the declaration of the Max Heap data structure and its functions.
*/
#ifndef DS_MAXHEAP_H
#define DS_MAXHEAP_H

#define INITIAL_HEAP_CAPACITY 128

typedef struct {
    int* data;
    int size;
    int capacity;
} MaxHeap;

#ifdef __cplusplus
extern "C" {
#endif

    void maxheap_init(MaxHeap* heap);
    void maxheap_insert(MaxHeap* heap, int value);
    int maxheap_extractTop(MaxHeap* heap);  
    char* maxheap_to_json(MaxHeap* heap);
    void maxheap_clear(MaxHeap* heap);
	void maxheap_clear_null(MaxHeap* heap);



#ifdef __cplusplus
}
#endif

#endif /* DS_MAXHEAP_H */

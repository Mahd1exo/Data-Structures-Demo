/*
* FILE : ds_minheap.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a Min Heap data structure.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_minheap.h"

// FUNCTION     : minheap_init
// DESCRIPTION  :
// Initializes the min-heap.
// PARAMETERS   : heap - pointer to the MinHeap structure
// RETURNS      : none
static void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// FUNCTION     : bubbleUp
// DESCRIPTION  :
// Bubbles up the value at the given index to maintain the min-heap property.
//// PARAMETERS   : heap - pointer to the MinHeap structure
////              index - the index of the value to bubble up
//// RETURNS      : none
static void bubbleUp(MinHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[index] < heap->data[parent]) {
            swap(&heap->data[index], &heap->data[parent]);
            index = parent;
        }
        else {
            break;
        }
    }
}

// FUNCTION     : bubbleDown
// DESCRIPTION  :
// Bubbles down the value at the given index to maintain the min-heap property.
//// PARAMETERS   : heap - pointer to the MinHeap structure
////              index - the index of the value to bubble down
//// RETURNS      : none
static void bubbleDown(MinHeap* heap, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        if (left < heap->size && heap->data[left] < heap->data[smallest])
            smallest = left;
        if (right < heap->size && heap->data[right] < heap->data[smallest])
            smallest = right;
        if (smallest != index) {
            swap(&heap->data[index], &heap->data[smallest]);
            index = smallest;
        }
        else {
            break;
        }
    }
}

// FUNCTION     : minheap_init
// DESCRIPTION  :
// Initializes the min-heap.
//// PARAMETERS   : heap - pointer to the MinHeap structure
//// RETURNS      : none
void minheap_init(MinHeap* heap) {
    if (!heap) return;
    heap->capacity = INITIAL_HEAP_CAPACITY;
    heap->size = 0;
    heap->data = (int*)malloc(sizeof(int) * heap->capacity);
}

// FUNCTION     : minheap_insert
// DESCRIPTION  :
// Inserts a value into the min-heap.
//// PARAMETERS   : heap - pointer to the MinHeap structure
////              value - the value to insert
//// RETURNS      : none
void minheap_insert(MinHeap* heap, int value) {
    if (!heap) return;
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->data = (int*)realloc(heap->data, sizeof(int) * heap->capacity);
        if (!heap->data) return;
    }
    heap->data[heap->size] = value;
    bubbleUp(heap, heap->size);
    heap->size++;
}

// FUNCTION     : minheap_peek
// DESCRIPTION  :
// Returns the minimum value from the min-heap without removing it.
//// PARAMETERS   : heap - pointer to the MinHeap structure
////              returns the minimum value (or -1 if the heap is empty)
//// RETURNS      : minimum value
int minheap_extractMin(MinHeap* heap) {
    if (!heap || heap->size <= 0)
        return -1; 
    int minVal = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    bubbleDown(heap, 0);
    return minVal;
}

// FUNCTION     : minheap_peek
// DESCRIPTION  :
// Returns the minimum value from the min-heap without removing it.
//// PARAMETERS   : heap - pointer to the MinHeap structure
////              returns the minimum value (or -1 if the heap is empty)
//// RETURNS      : minimum value
char* minheap_to_json(MinHeap* heap) {
    if (!heap) return NULL;
    int bufferSize = 64;
    char* json = (char*)malloc(bufferSize);
    if (!json) return NULL;
    int offset = snprintf(json, bufferSize, "{\"heap\":[");
    for (int i = 0; i < heap->size; i++) {
        int needed = snprintf(NULL, 0, "%d", heap->data[i]) + 2; 
        while (offset + needed >= bufferSize) {
            bufferSize *= 2;
            json = (char*)realloc(json, bufferSize);
            if (!json) return NULL;
        }
        offset += sprintf(json + offset, "%d", heap->data[i]);
        if (i < heap->size - 1) {
            offset += sprintf(json + offset, ",");
        }
    }
    while (offset + 3 >= bufferSize) {
        bufferSize *= 2;
        json = (char*)realloc(json, bufferSize);
        if (!json) return NULL;
    }
    sprintf(json + offset, "]}");
    return json;
}

// FUNCTION     : minheap_clear_null
// DESCRIPTION  :
// Clears the min-heap, freeing all memory.
//// PARAMETERS   : heap - pointer to the MinHeap structure
//// RETURNS      : none
void minheap_clear_null(MinHeap* heap) {
    if (!heap) return;
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}
// FUNCTION     : minheap_clear
// DESCRIPTION  :
// Clears the min-heap and reinitializes it.
//// PARAMETERS   : heap - pointer to the MinHeap structure
//// RETURNS      : none
void minheap_clear(MinHeap* heap) {
    maxheap_clear_null(heap);
    minheap_init(heap);
}


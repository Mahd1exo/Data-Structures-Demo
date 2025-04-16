/*
* FILE :    ds_maxheap.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
*  This file contains the implementation of a Max Heap data structure.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_maxheap.h"

// FUNCTION     : swap
// DESCRIPTION  :
// Swaps two integers.
//// PARAMETERS   : num1 - pointer to the first integer
//// ////              num2 - pointer to the second integer
//// RETURNS      : none
static void swap(int* num1, int* num2) {
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

// FUNCTION     : bubbleUp
// DESCRIPTION  :
// Bubbles up the value at the given index to maintain the max-heap property.
//// PARAMETERS   : heap - pointer to the MaxHeap structure
////              index - the index of the value to bubble up
//// RETURNS      : none
static void bubbleUp(MaxHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[index] > heap->data[parent]) {
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
// Bubbles down the value at the given index to maintain the max-heap property.
//// PARAMETERS   : heap - pointer to the MaxHeap structure
////              index - the index of the value to bubble down
//// RETURNS      : none
static void bubbleDown(MaxHeap* heap, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;
        if (left < heap->size && heap->data[left] > heap->data[largest])
            largest = left;
        if (right < heap->size && heap->data[right] > heap->data[largest])
            largest = right;
        if (largest != index) {
            swap(&heap->data[index], &heap->data[largest]);
            index = largest;
        }
        else {
            break;
        }
    }
}

// FUNCTION     : maxheap_init
// DESCRIPTION  :
// Initializes the MaxHeap structure.
//// PARAMETERS   : heap - pointer to the MaxHeap structure
//// RETURNS      : none
void maxheap_init(MaxHeap* heap) {
    if (!heap) return;
    heap->capacity = INITIAL_HEAP_CAPACITY;
    heap->size = 0;
    heap->data = (int*)malloc(sizeof(int) * heap->capacity);
}

// FUNCTION     : maxheap_insert
// DESCRIPTION  :
// Inserts a value into the max-heap.
//// PARAMETERS   : heap - pointer to the MaxHeap structure
////              value - the value to insert
//// RETURNS      : none
void maxheap_insert(MaxHeap* heap, int value) {
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

// FUNCTION     : maxheap_extractTop
// DESCRIPTION  :
// Extracts the maximum value from the max-heap.
//// PARAMETERS   : heap - pointer to the MaxHeap structure
////              returns the maximum value (or -1 if the heap is empty)
//// RETURNS      : maximum value
int maxheap_extractTop(MaxHeap* heap) {
    if (!heap || heap->size <= 0)
        return -1;  
    int maxVal = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    bubbleDown(heap, 0);
    return maxVal;
}

// FUNCTION     : maxheap_peek
//  DESCRIPTION  :
// Returns the maximum value from the max-heap without removing it.
//// PARAMETERS   : heap - pointer to the MaxHeap structure
////              returns the maximum value (or -1 if the heap is empty)
//// RETURNS      : maximum value
char* maxheap_to_json(MaxHeap* heap) {
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

// FUNCTION     : maxheap_clear_null
// DESCRIPTION  :
// Clears the max-heap and sets its data pointer to NULL.
//// PARAMETERS   : heap - pointer to the MaxHeap structure
//// RETURNS      : none
void maxheap_clear_null(MaxHeap* heap) {
    if (!heap) return;
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

// FUNCTION     : maxheap_clear
// DESCRIPTION  :
// Clears the max-heap and reinitializes it.
//// PARAMETERS   : heap - pointer to the MaxHeap structure
//// RETURNS      : none
void maxheap_clear(MaxHeap* heap) {
    maxheap_clear_null(heap);
    maxheap_init(heap);
}
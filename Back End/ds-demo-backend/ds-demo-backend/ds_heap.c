#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ds_heap.h"

/* Helper function: duplicate a string */
static char* str_dup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    if (copy) {
        strcpy(copy, s);
    }
    return copy;
}

/* Initialize the heap */
void heap_init(SimpleHeap* heap) {
    if (!heap) return;
    heap->size = 0;
    for (int i = 0; i < HEAP_CAPACITY; i++) {
        heap->items[i] = NULL;
    }
}

/* Swap two pointers */
static void swap(char** a, char** b) {
    char* temp = *a;
    *a = *b;
    *b = temp;
}

/* Convert a string to a double */
static double str_to_double(const char* s) {
    return atof(s);
}

/* Heapify up: ensure max-heap property after insertion */
static void heapify_up(SimpleHeap* heap, int index) {
    if (index <= 0) return;
    int parent = (index - 1) / 2;
    if (parent >= 0 && str_to_double(heap->items[index]) > str_to_double(heap->items[parent])) {
        swap(&heap->items[index], &heap->items[parent]);
        heapify_up(heap, parent);
    }
}

/* Heapify down: ensure max-heap property after extraction */
static void heapify_down(SimpleHeap* heap, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < heap->size && str_to_double(heap->items[left]) > str_to_double(heap->items[largest])) {
        largest = left;
    }
    if (right < heap->size && str_to_double(heap->items[right]) > str_to_double(heap->items[largest])) {
        largest = right;
    }
    if (largest != index) {
        swap(&heap->items[index], &heap->items[largest]);
        heapify_down(heap, largest);
    }
}

/* Insert a new value into the max heap */
void heap_insert(SimpleHeap* heap, const char* value) {
    if (!heap) return;
    if (heap->size >= HEAP_CAPACITY) {
        // Heap is full; you might want to handle this error.
        return;
    }
    heap->items[heap->size] = str_dup(value); // Duplicate the string
    heapify_up(heap, heap->size);
    heap->size++;
}

/* Extract the top (maximum) element from the heap.
   Copies the extracted value into outValue.
*/
int heap_extract_top(SimpleHeap* heap, char* outValue, int outSize) {
    if (!heap || heap->size == 0) return 0;
    // Copy the top element to outValue
    strncpy(outValue, heap->items[0], outSize - 1);
    outValue[outSize - 1] = '\0';
    free(heap->items[0]);
    // Move the last element to the root
    heap->items[0] = heap->items[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return 1;
}

/* Collect heap items into an array. The caller must free the array (not the strings). */
char** heap_collect(SimpleHeap* heap, int* count) {
    if (!heap) {
        if (count) *count = 0;
        return NULL;
    }
    int size = heap->size;
    char** arr = (char**)malloc(sizeof(char*) * size);
    for (int i = 0; i < size; i++) {
        arr[i] = heap->items[i];
    }
    if (count) *count = size;
    return arr;
}

/* Clear the heap by freeing all allocated strings and resetting size */
void heap_clear(SimpleHeap* heap) {
    if (!heap) return;
    for (int i = 0; i < heap->size; i++) {
        free(heap->items[i]);
        heap->items[i] = NULL;
    }
    heap->size = 0;
}

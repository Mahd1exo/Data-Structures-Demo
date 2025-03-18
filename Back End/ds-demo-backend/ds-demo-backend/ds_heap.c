#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_heap.h"

static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

void heap_init(SimpleHeap* heap) {
    if (!heap) return;
    heap->size = 0;
}

/* A naive min-heap: use 1-based or 0-based indexing? We'll do 0-based.
   parent(i) = (i-1)/2, left(i) = 2i+1, right(i)=2i+2 */
static void swap_str(char* a, char* b) {
    char temp[HEAP_MAX_LEN];
    safeCopy(temp, a, HEAP_MAX_LEN);
    safeCopy(a, b, HEAP_MAX_LEN);
    safeCopy(b, temp, HEAP_MAX_LEN);
}

static int cmp_str(const char* a, const char* b) {
    return strcmp(a, b); /* for min-heap: if a < b => a is "higher priority" */
}

static void heapify_up(SimpleHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (cmp_str(heap->data[idx], heap->data[parent]) < 0) {
            swap_str(heap->data[idx], heap->data[parent]);
            idx = parent;
        }
        else {
            break;
        }
    }
}

static void heapify_down(SimpleHeap* heap, int idx) {
    int size = heap->size;
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        if (left < size && cmp_str(heap->data[left], heap->data[smallest]) < 0) {
            smallest = left;
        }
        if (right < size && cmp_str(heap->data[right], heap->data[smallest]) < 0) {
            smallest = right;
        }
        if (smallest == idx) break;
        swap_str(heap->data[idx], heap->data[smallest]);
        idx = smallest;
    }
}

void heap_insert(SimpleHeap* heap, const char* value) {
    if (!heap || heap->size >= HEAP_MAX_SIZE) return;
    safeCopy(heap->data[heap->size], value, HEAP_MAX_LEN);
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

/* extract top (the smallest) */
int heap_extract_top(SimpleHeap* heap, char* outValue, int outSize) {
    if (!heap || heap->size == 0) return 0;
    /* root is data[0], copy it out */
    safeCopy(outValue, heap->data[0], outSize);
    /* move last to root */
    heap->size--;
    if (heap->size > 0) {
        safeCopy(heap->data[0], heap->data[heap->size], HEAP_MAX_LEN);
        heapify_down(heap, 0);
    }
    return 1;
}

char** heap_collect(const SimpleHeap* heap, int* count) {
    if (!heap) {
        if (count) *count = 0;
        return NULL;
    }
    int n = heap->size;
    if (count) *count = n;
    if (n <= 0) return NULL;
    /* copy pointers to each string in order [0..size-1] */
    char** arr = (char**)malloc(sizeof(char*) * n);
    for (int i = 0; i < n; i++) {
        arr[i] = (char*)heap->data[i]; /* pointer to that string in the array */
    }
    return arr;
}

void heap_clear(SimpleHeap* heap) {
    if (!heap) return;
    heap->size = 0;
}

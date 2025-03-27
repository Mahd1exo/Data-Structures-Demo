#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_maxheap.h"

/* Swap two integers */
static void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Bubble up to maintain the max-heap property */
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

/* Bubble down to maintain the max-heap property */
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

void maxheap_init(MaxHeap* heap) {
    if (!heap) return;
    heap->capacity = INITIAL_HEAP_CAPACITY;
    heap->size = 0;
    heap->data = (int*)malloc(sizeof(int) * heap->capacity);
}

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

int maxheap_extractTop(MaxHeap* heap) {
    if (!heap || heap->size <= 0)
        return -1;  
    int maxVal = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    bubbleDown(heap, 0);
    return maxVal;
}

char* maxheap_to_json(MaxHeap* heap) {
    if (!heap) return NULL;
    int bufferSize = 64;
    char* json = (char*)malloc(bufferSize);
    if (!json) return NULL;
    int offset = snprintf(json, bufferSize, "{\"heap\":[");
    for (int i = 0; i < heap->size; i++) {
        int needed = snprintf(NULL, 0, "%d", heap->data[i]) + 2; // +2 for comma or end bracket
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

void maxheap_clear_null(MaxHeap* heap) {
    if (!heap) return;
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}
void maxheap_clear(MaxHeap* heap) {
    maxheap_clear_null(heap);
    maxheap_init(heap);
}
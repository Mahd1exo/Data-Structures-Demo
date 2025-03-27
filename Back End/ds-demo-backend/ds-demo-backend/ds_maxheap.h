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
    int maxheap_extractTop(MaxHeap* heap);  // Extracts and returns the maximum element.
    char* maxheap_to_json(MaxHeap* heap);
    void maxheap_clear(MaxHeap* heap);

#ifdef __cplusplus
}
#endif

#endif /* DS_MAXHEAP_H */

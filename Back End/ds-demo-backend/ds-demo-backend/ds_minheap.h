#ifndef DS_MINHEAP_H
#define DS_MINHEAP_H

#define INITIAL_HEAP_CAPACITY 128

typedef struct {
    int* data;
    int size;
    int capacity;
} MinHeap;

#ifdef __cplusplus
extern "C" {
#endif

    void minheap_init(MinHeap* heap);
    void minheap_insert(MinHeap* heap, int value);
    int minheap_extractMin(MinHeap* heap);
    char* minheap_to_json(MinHeap* heap);
    void minheap_clear(MinHeap* heap);
	void minheap_clear_null(MinHeap* heap);


#ifdef __cplusplus
}
#endif

#endif /* DS_MINHEAP_H */

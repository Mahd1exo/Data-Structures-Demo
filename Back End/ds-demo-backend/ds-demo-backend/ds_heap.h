#ifndef DS_HEAP_H
#define DS_HEAP_H

#define HEAP_MAX_SIZE 100
#define HEAP_MAX_LEN 64

typedef struct {
    char data[HEAP_MAX_SIZE][HEAP_MAX_LEN]; /* array of strings */
    int size;
} SimpleHeap;

#ifdef __cplusplus
extern "C" {
#endif

    void heap_init(SimpleHeap* heap);

    /* Insert a string into a naive min-heap or max-heap (let's do min-heap) */
    void heap_insert(SimpleHeap* heap, const char* value);

    /* Extract top (root) from the min-heap, returns 1 if success, 0 if empty. */
    int heap_extract_top(SimpleHeap* heap, char* outValue, int outSize);

    /* Collect the heap as an array of strings for JSON. index 0 is root. */
    char** heap_collect(const SimpleHeap* heap, int* count);

    /* Clear the heap */
    void heap_clear(SimpleHeap* heap);

#ifdef __cplusplus
}
#endif

#endif /* DS_HEAP_H */

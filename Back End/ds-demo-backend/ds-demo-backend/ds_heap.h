#ifndef DS_HEAP_H
#define DS_HEAP_H

#define HEAP_CAPACITY 100   /* Maximum number of elements in the heap */
#define VALUE_MAX_LEN 64    /* Maximum length for each value string */

typedef struct {
    char* items[HEAP_CAPACITY];  /* Array of string pointers */
    int size;                    /* Current number of items */
} SimpleHeap;

#ifdef __cplusplus
extern "C" {
#endif


    void heap_init(SimpleHeap* heap);

    void heap_insert(SimpleHeap* heap, const char* value);

    int heap_extract_top(SimpleHeap* heap, char* outValue, int outSize);

    char** heap_collect(SimpleHeap* heap, int* count);

    void heap_clear(SimpleHeap* heap);

#ifdef __cplusplus
}
#endif

#endif /* DS_HEAP_H */

#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#define QUEUE_MAX_DATA_LEN 64

typedef struct QueueNode {
    char data[QUEUE_MAX_DATA_LEN];
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

#ifdef __cplusplus
extern "C" {
#endif

    void que_init(Queue* queue);
    void que_enqueue(Queue* queue, const char* value);
    void que_dequeue(Queue* queue);

    /* Returns array of char* [front -> array[0]]
       caller frees array, not strings themselves. */
    char** que_collect_data(const Queue* queue, int* count);

#ifdef __cplusplus
}
#endif

#endif /* DS_QUEUE_H */

/*
* FILE : ds_queue.h
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the declaration of a Queue data structure and its functions.
*/
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

    void queue_init(Queue* queue);
    void queue_enqueue(Queue* queue, const char* value);
    void queue_dequeue(Queue* queue);
    char** queue_collect_data(const Queue* queue, int* count);
    void queue_clear(Queue* queue);

#ifdef __cplusplus
}
#endif

#endif /* DS_QUEUE_H */

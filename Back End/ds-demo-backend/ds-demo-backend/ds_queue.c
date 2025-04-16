/*
* FILE : ds_queue.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a Queue data structure.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_queue.h"

// FUNCTION     : safeCopy
// DESCRIPTION  :
// Safely copies a string to a destination buffer, ensuring null termination.
// PARAMETERS   : dest - destination buffer
//             src - source string
//              maxLen - maximum length of the destination buffer
// RETURNS      : none
static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

void queue_init(Queue* queue) {
    if (!queue) return;
    queue->front = NULL;
    queue->rear = NULL;
}

// FUNCTION     : queue_enqueue
// DESCRIPTION  :
// Adds a new node with the given value at the end of the queue.
// PARAMETERS   : queue - pointer to the Queue structure
//             value - the value to insert (as a string)
// RETURNS      : none
void queue_enqueue(Queue* queue, const char* value) {
    if (!queue) return;
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!node) return;
    safeCopy(node->data, value, QUEUE_MAX_DATA_LEN);
    node->next = NULL;

    if (!queue->front) {
        queue->front = node;
        queue->rear = node;
    }
    else {
        queue->rear->next = node;
        queue->rear = node;
    }
}

// FUNCTION     : queue_dequeue
// DESCRIPTION  :
// Removes the first node from the queue.
// PARAMETERS   : queue - pointer to the Queue structure
// RETURNS      : none
void queue_dequeue(Queue* queue) {
    if (!queue || !queue->front) return;
    QueueNode* temp = queue->front;
    queue->front = temp->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
}

// FUNCTION     : queue_remove
// DESCRIPTION  :
// Removes a node with the given value from the queue.
// PARAMETERS   : queue - pointer to the Queue structure
////              value - the value to remove (as a string)
// RETURNS      : none
void queue_clear(Queue* queue) {
    if (!queue) return;
    while (queue->front) {
        queue_dequeue(queue);
    }
    queue->rear = NULL;
}

// FUNCTION     : queue_remove
// DESCRIPTION  :
// Removes a node with the given value from the queue.
// PARAMETERS   : queue - pointer to the Queue structure
//             value - the value to remove (as a string)
// RETURNS      : none
char** queue_collect_data(const Queue* queue, int* count) {
    if (!queue) {
        if (count) *count = 0;
        return NULL;
    }
    int counter = 0;
    QueueNode* temp = queue->front;
    while (temp) {
        counter++;
        temp = temp->next;
    }
    if (count) *count = counter;

    if (counter == 0) {
        return NULL;
    }
    char** array = (char**)malloc(sizeof(char*) * counter);
    temp = queue->front;
    for (int i = 0; i < counter; i++) {
        array[i] = temp->data;
        temp = temp->next;
    }
    return array;
}

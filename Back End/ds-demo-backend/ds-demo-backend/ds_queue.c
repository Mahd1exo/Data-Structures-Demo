#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_queue.h"

static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

void que_init(Queue* queue) {
    if (!queue) return;
    queue->front = NULL;
    queue->rear = NULL;
}

void que_enqueue(Queue* queue, const char* value) {
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

void que_dequeue(Queue* queue) {
    if (!queue || !queue->front) return;
    QueueNode* temp = queue->front;
    queue->front = temp->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
}

char** que_collect_data(const Queue* queue, int* count) {
    if (!queue) {
        if (count) *count = 0;
        return NULL;
    }
    int c = 0;
    QueueNode* temp = queue->front;
    while (temp) {
        c++;
        temp = temp->next;
    }
    if (count) *count = c;

    if (c == 0) {
        return NULL;
    }
    char** array = (char**)malloc(sizeof(char*) * c);
    temp = queue->front;
    for (int i = 0; i < c; i++) {
        array[i] = temp->data;
        temp = temp->next;
    }
    return array;
}

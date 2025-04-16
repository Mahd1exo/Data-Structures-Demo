/*
* FILE : task_queue.h
* PROJECT : FINAL PROJECT - DATA STRUCTURE - Data Structure Demo
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2024-03-10
*
* DESCRIPTION :
* This file contains the declaration of the task queue data structure and its functions.
*/
#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <stdlib.h>
#include <stdio.h>


typedef void (*task_func_t)(void* context);

typedef struct TaskThread {
    task_func_t func;
    void* context;
    struct TaskThread* next;
} TaskThread;

typedef struct {
    TaskThread* front;
    TaskThread* rear;
} TaskQueue;

void task_queue_init(TaskQueue* queue);
int task_queue_is_empty(TaskQueue* queue);
void task_queue_enqueue(TaskQueue* queue, task_func_t func, void* context);
task_func_t task_queue_dequeue(TaskQueue* queue, void** context);
void task_queue_clear(TaskQueue* queue);
void process_task_queue(TaskQueue* queue);

#endif /* TASK_QUEUE_H */


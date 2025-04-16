/*
* FILE : task_queue.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a task queue for managing tasks in a multi-threaded environment.
*/
#include "task_queue.h"

// FUNCTION     : task_queue_init
// DESCRIPTION  :
// Initializes the task queue.
//// PARAMETERS   : q - pointer to the TaskQueue structure
//// RETURNS      : none
void task_queue_init(TaskQueue* queue) {
    queue->front = queue->rear = NULL;
}

// FUNCTION     : task_queue_is_empty
// DESCRIPTION  :
// Checks if the task queue is empty.
// PARAMETERS   : queue - pointer to the TaskQueue structure
//// RETURNS      : 1 if empty, 0 otherwise

int task_queue_is_empty(TaskQueue* queue) {
    return (queue->front == NULL);
}
// FUNCTION     : task_queue_enqueue
// DESCRIPTION  :
// Adds a new task to the end of the task queue.
//// PARAMETERS   : queue - pointer to the TaskQueue structure
////              func - function pointer to the task function
////              context - context to be passed to the task function
//// RETURNS      : none
void task_queue_enqueue(TaskQueue* queue, task_func_t func, void* context) {
    TaskThread* newTask = (TaskThread*)malloc(sizeof(TaskThread));
    if (!newTask) {
        fprintf(stderr, "Memory allocation error in task_queue_enqueue\n");
        exit(EXIT_FAILURE);
    }
    newTask->func = func;
    newTask->context = context;
    newTask->next = NULL;

    if (queue->rear == NULL) {  
        queue->front = queue->rear = newTask;
    }
    else {
        queue->rear->next = newTask;
        queue->rear = newTask;
    }
}
// FUNCTION     : task_queue_dequeue
// DESCRIPTION  :
// Removes a task from the front of the task queue.
//// PARAMETERS   : queue - pointer to the TaskQueue structure
////              context - pointer to store the context of the dequeued task
//// RETURNS      : function pointer to the dequeued task function
task_func_t task_queue_dequeue(TaskQueue* queue, void** context) {
    if (task_queue_is_empty(queue)) {
        return NULL;
    }
    TaskThread* temp = queue->front;
    task_func_t func = temp->func;
    if (context) {
        *context = temp->context;
    }
    queue->front = temp->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return func;
}
// FUNCTION     : task_queue_clear
// DESCRIPTION  :
// Clears the task queue, freeing all tasks.
//// PARAMETERS   : queue - pointer to the TaskQueue structure
//// RETURNS      : none
void task_queue_clear(TaskQueue* queue) {
    while (!task_queue_is_empty(queue)) {
        void* dummy = NULL;
        task_queue_dequeue(queue, &dummy);
    }
}
// FUNCTION     : process_task_queue
// DESCRIPTION  :
// Processes all tasks in the task queue.
//  PARAMETERS   : queue - pointer to the TaskQueue structure
// RETURNS      : none
void process_task_queue(TaskQueue* queue) {
    void* taskContext = NULL;
    task_func_t task;
    while ((task = task_queue_dequeue(queue, &taskContext)) != NULL) {
        task(taskContext);
    }
}

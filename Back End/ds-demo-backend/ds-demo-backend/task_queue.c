#include "task_queue.h"

void task_queue_init(TaskQueue* q) {
    q->front = q->rear = NULL;
}

int task_queue_is_empty(TaskQueue* q) {
    return (q->front == NULL);
}

void task_queue_enqueue(TaskQueue* q, task_func_t func, void* context) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    if (!newTask) {
        fprintf(stderr, "Memory allocation error in task_queue_enqueue\n");
        exit(EXIT_FAILURE);
    }
    newTask->func = func;
    newTask->context = context;
    newTask->next = NULL;

    if (q->rear == NULL) {  // Queue is empty
        q->front = q->rear = newTask;
    }
    else {
        q->rear->next = newTask;
        q->rear = newTask;
    }
}

task_func_t task_queue_dequeue(TaskQueue* q, void** context) {
    if (task_queue_is_empty(q)) {
        return NULL;
    }
    Task* temp = q->front;
    task_func_t func = temp->func;
    if (context) {
        *context = temp->context;
    }
    q->front = temp->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return func;
}

void task_queue_clear(TaskQueue* q) {
    while (!task_queue_is_empty(q)) {
        void* dummy = NULL;
        task_queue_dequeue(q, &dummy);
    }
}

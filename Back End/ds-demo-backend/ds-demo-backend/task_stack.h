#ifndef TASK_STACK_H
#define TASK_STACK_H

#include <stdlib.h>

// Define a generic task function type.
typedef void (*task_func_t)(void* context);

// Structure for a single task stack node.
typedef struct TaskStackNode {
    task_func_t func;
    void* context;
    struct TaskStackNode* next;
} TaskStackNode;

// Structure for the task stack.
typedef struct {
    TaskStackNode* top;
} TaskStack;

// Function prototypes.
void task_stack_init(TaskStack* stack);
void task_stack_push(TaskStack* stack, task_func_t func, void* context);
task_func_t task_stack_pop(TaskStack* stack, void** context);
void task_stack_clear(TaskStack* stack);

#endif // TASK_STACK_H

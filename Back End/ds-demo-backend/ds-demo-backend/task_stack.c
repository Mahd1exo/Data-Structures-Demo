#include "task_stack.h"
#include <stdio.h>
#include <stdlib.h>

void task_stack_init(TaskStack* stack) {
    stack->top = NULL;
}

void task_stack_push(TaskStack* stack, task_func_t func, void* context) {
    TaskStackNode* node = (TaskStackNode*)malloc(sizeof(TaskStackNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error in task_stack_push\n");
        exit(EXIT_FAILURE);
    }
    node->func = func;
    node->context = context;
    node->next = stack->top;
    stack->top = node;
}

task_func_t task_stack_pop(TaskStack* stack, void** context) {
    if (stack->top == NULL) {
        return NULL;
    }
    TaskStackNode* node = stack->top;
    task_func_t func = node->func;
    if (context) {
        *context = node->context;
    }
    stack->top = node->next;
    free(node);
    return func;
}

void task_stack_clear(TaskStack* stack) {
    void* dummy = NULL;
    while (task_stack_pop(stack, &dummy) != NULL) {
        // All nodes are freed.
    }
}

/*
* FILE : task_stack.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a task stack for managing tasks in a multi-threaded environment.
*/
#include "task_stack.h"
#include <stdio.h>
#include <stdlib.h>

// FUNCTION     : task_stack_init
// DESCRIPTION  :
// Initializes the task stack.
//// PARAMETERS   : stack - pointer to the TaskStack structure
// //// RETURNS      : none
void task_stack_init(TaskStack* stack) {
    stack->top = NULL;
}

// FUNCTION     : task_stack_push
// DESCRIPTION  :
// Pushes a new task onto the stack.
//// PARAMETERS   : stack - pointer to the TaskStack structure
////              func - function pointer to the task function
////              context - context to be passed to the task function
//// RETURNS      : none
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

// FUNCTION     : task_stack_pop
// DESCRIPTION  :
// Pops the top task from the stack.
//// PARAMETERS   : stack - pointer to the TaskStack structure
////              context - pointer to store the context of the popped task
//// RETURNS      : function pointer to the popped task function
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

// FUNCTION     : task_stack_peek
// DESCRIPTION  :
// Peeks at the top task without removing it from the stack.
//// PARAMETERS   : stack - pointer to the TaskStack structure
////              context - pointer to store the context of the top task
//// RETURNS      : function pointer to the top task function
void task_stack_clear(TaskStack* stack) {
    void* dummy = NULL;
    while (task_stack_pop(stack, &dummy) != NULL) { }
}

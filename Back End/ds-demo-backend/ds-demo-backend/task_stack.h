/*
* FILE : task_stack.h
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
*  This file contains the declaration of a task stack for managing tasks in a multi-threaded environment.
*/
#ifndef TASK_STACK_H
#define TASK_STACK_H

#include <stdlib.h>

// Define a generic task function type.
typedef void (*task_func_t)(void* context);


typedef struct TaskStackNode {
    task_func_t func;
    void* context;
    struct TaskStackNode* next;
} TaskStackNode;

typedef struct {
    TaskStackNode* top;
} TaskStack;

// Function prototypes.
void task_stack_init(TaskStack* stack);
void task_stack_push(TaskStack* stack, task_func_t func, void* context);
task_func_t task_stack_pop(TaskStack* stack, void** context);
void task_stack_clear(TaskStack* stack);

#endif // TASK_STACK_H

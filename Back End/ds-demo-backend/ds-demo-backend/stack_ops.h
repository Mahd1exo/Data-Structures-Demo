/*
* FILE : stack_ops.h
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the declaration of stack operations for a command history stack.
*/
#ifndef STACK_OPS_H
#define STACK_OPS_H

#include <stdbool.h>

// Define the stack node structure.
typedef struct StackNode {
    char* command;
    struct StackNode* next;
} StackNode;

// Define the stack type.
typedef struct {
    StackNode* top;
} Stack;

// Function prototypes.
void stack_ops_init(Stack* s);
void stack_ops_push(Stack* s, const char* command);
char* stack_ops_pop(Stack* s);
bool stack_ops_is_empty(const Stack* s);
void stack_ops_free(Stack* s);
void print_stack_ops(const Stack* s);

#endif // STACK_OPS_H

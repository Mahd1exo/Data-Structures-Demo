/*
* FILE : stack_ops.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of stack operations for a command history stack.
*/
#include "stack_ops.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// FUNCTION     : stack_ops_init
// DESCRIPTION  :
// Initializes the stack.
// PARAMETERS   : stack - pointer to the Stack structure
// RETURNS      : none
void stack_ops_init(Stack* stack) {
    stack->top = NULL;
}

// FUNCTION     : stack_ops_push
// DESCRIPTION  :
// Pushes a new command onto the stack.
// PARAMETERS   : stack - pointer to the Stack structure
//              command - the command to push (as a string)
// RETURNS      : none
void stack_ops_push(Stack* stack, const char* command) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (node) {
        node->command = _strdup(command); 
        node->next = stack->top;
        stack->top = node;
    }
}

// FUNCTION     : stack_ops_pop
// DESCRIPTION  :
// Pops the top command from the stack.
// PARAMETERS   : stack - pointer to the Stack structure
// RETURNS      : the popped command (as a string) or NULL if the stack is empty
char* stack_ops_pop(Stack* stack) {
    if (stack->top == NULL)
        return NULL;
    StackNode* node = stack->top;
    stack->top = node->next;
    char* command = node->command;
    free(node);
    return command;
}

// FUNCTION     : stack_ops_peek
// DESCRIPTION  :
// Returns the top command from the stack without removing it.
// PARAMETERS   : stack - pointer to the Stack structure
// RETURNS      : the top command (as a string) or NULL if the stack is empty
bool stack_ops_is_empty(const Stack* stack) {
    return (stack->top == NULL);
}

// FUNCTION     : stack_ops_free
// DESCRIPTION  :
// Frees the memory allocated for the stack and its nodes.
// PARAMETERS   : stack - pointer to the Stack structure
// RETURNS      : none
void stack_ops_free(Stack* stack) {
    while (!stack_ops_is_empty(stack)) {
        char* cmd = stack_ops_pop(stack);
        free(cmd);
    }
}

// FUNCTION     : print_stack_ops
// DESCRIPTION  :
// Prints the command history from the stack.
// PARAMETERS   : stack - pointer to the Stack structure
// RETURNS      : none
void print_stack_ops(const Stack* stack) {
    if (stack->top == NULL) {
        printf("No operations recorded.\n");
        return;
    }
    printf("Operation History (most recent first):\n");
    StackNode* current = stack->top;
    while (current) {
        printf(" - %s\n", current->command);
        current = current->next;
    }
}

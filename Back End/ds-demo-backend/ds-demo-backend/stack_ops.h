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
void stack_init(Stack* s);
void stack_push(Stack* s, const char* command);
char* stack_pop(Stack* s);
bool stack_is_empty(const Stack* s);
void stack_free(Stack* s);
void print_stack(const Stack* s);

#endif // STACK_OPS_H

#include "stack_ops.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Initialize the stack.
void stack_init(Stack* s) {
    s->top = NULL;
}

// Push a command string onto the stack.
void stack_push(Stack* s, const char* command) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (node) {
        // Duplicate the command string.
        node->command = _strdup(command); // On POSIX systems, you can use strdup()
        node->next = s->top;
        s->top = node;
    }
}

// Pop the top command string from the stack.
char* stack_pop(Stack* s) {
    if (s->top == NULL)
        return NULL;
    StackNode* node = s->top;
    s->top = node->next;
    char* command = node->command;
    free(node);
    return command;
}

// Check if the stack is empty.
bool stack_is_empty(const Stack* s) {
    return (s->top == NULL);
}

// Free all nodes in the stack.
void stack_free(Stack* s) {
    while (!stack_is_empty(s)) {
        char* cmd = stack_pop(s);
        free(cmd);
    }
}

// Print the contents of the stack (most recent first).
void print_stack(const Stack* s) {
    if (s->top == NULL) {
        printf("No operations recorded.\n");
        return;
    }
    printf("Operation History (most recent first):\n");
    StackNode* current = s->top;
    while (current) {
        printf(" - %s\n", current->command);
        current = current->next;
    }
}

/*
* FILE : ds_stack.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a Stack data structure.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_stack.h"

// FUNCTION     : safeCopy
// DESCRIPTION  :
// Safely copies a string to a destination buffer, ensuring null termination.
//// PARAMETERS   : dest - destination buffer
////              src - source string
////              maxLen - maximum length of the destination buffer
//// RETURNS      : none
static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

// FUNCTION     : stack_init
// DESCRIPTION  :
// Initializes the stack.
// PARAMETERS   : stack - pointer to the Stack structure
// RETURNS      : none
void stack_init(Stack* stack) {
    if (!stack) return;
    stack->top = NULL;
}

// FUNCTION     : stack_push
// DESCRIPTION  :
// Pushes a new value onto the stack.
// PARAMETERS   : stack - pointer to the Stack structure
//              value - the value to push (as a string)
// RETURNS      : none
void stack_push(Stack* stack, const char* value) {
    if (!stack) return;
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) return;
    safeCopy(node->data, value, STK_MAX_DATA_LEN);
    node->next = stack->top;
    stack->top = node;
}

// FUNCTION     : stack_pop
// DESCRIPTION  :
// Pops the top value from the stack.
// PARAMETERS   : stack - pointer to the Stack structure
// RETURNS      : none
void stack_pop(Stack* stack) {
    if (!stack || !stack->top) return;
    StackNode* temp = stack->top;
    stack->top = temp->next;
    free(temp);
}

// FUNCTION     : stack_peek
// DESCRIPTION  :
// Returns the top value from the stack without removing it.
// PARAMETERS   : stack - pointer to the Stack structure
//              value - pointer to store the top value (as a string)
// RETURNS      : none
void stack_clear(Stack* stack) {
    if (!stack) return;
    while (stack->top) {
        stack_ops_pop(stack);
    }
}

// FUNCTION     : stack_clear_null
// DESCRIPTION  :
// Clears the stack, freeing all nodes.
// PARAMETERS   : stack - pointer to the Stack structure
// RETURNS      : none
//void stk_clear_null(Stack* stack) {
//	if (!stack) return;
//	while (stack->top) {
//		stk_pop(stack);
//	}
//	stack->top = NULL;
//}

char** stack_collect_data(const Stack* stack, int* count) {
    if (!stack) {
        if (count) *count = 0;
        return NULL;
    }
    int c = 0;
    StackNode* temp = stack->top;
    while (temp) {
        c++;
        temp = temp->next;
    }
    if (count) *count = c;

    if (c == 0) {
        return NULL;
    }
    char** array = (char**)malloc(sizeof(char*) * c);
    temp = stack->top;
    for (int i = 0; i < c; i++) {
        array[i] = temp->data;  
        temp = temp->next;
    }
    return array;
}

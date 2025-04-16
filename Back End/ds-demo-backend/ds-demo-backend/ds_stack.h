/*
* FILE : ds_stack.h
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the declaration of the Stack data structure and its functions.
*/
#ifndef DS_STACK_H
#define DS_STACK_H

#define STK_MAX_DATA_LEN 64

typedef struct StackNode {
    char data[STK_MAX_DATA_LEN];
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* top;
} Stack;

#ifdef __cplusplus
extern "C" {
#endif

    void stack_init(Stack* stack);
    void stack_push(Stack* stack, const char* value);
    void stack_pop(Stack* stack);
	void stack_clear(Stack* stack);
    char** stack_collect_data(const Stack* stack, int* count);

#ifdef __cplusplus
}
#endif

#endif /* DS_STACK_H */

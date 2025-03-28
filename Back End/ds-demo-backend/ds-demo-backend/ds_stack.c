#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_stack.h"

static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

void stk_init(Stack* stack) {
    if (!stack) return;
    stack->top = NULL;
}

void stk_push(Stack* stack, const char* value) {
    if (!stack) return;
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) return;
    safeCopy(node->data, value, STK_MAX_DATA_LEN);
    node->next = stack->top;
    stack->top = node;
}

void stk_pop(Stack* stack) {
    if (!stack || !stack->top) return;
    StackNode* temp = stack->top;
    stack->top = temp->next;
    free(temp);
}

void stk_clear(Stack* stack) {
    if (!stack) return;
    while (stack->top) {
        stk_pop(stack);
    }
}
//void stk_clear_null(Stack* stack) {
//	if (!stack) return;
//	while (stack->top) {
//		stk_pop(stack);
//	}
//	stack->top = NULL;
//}

char** stk_collect_data(const Stack* stack, int* count) {
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
        array[i] = temp->data;  // top -> array[0]
        temp = temp->next;
    }
    return array;
}

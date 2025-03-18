#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_linkedlist.h"

static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

void ll_init(LinkedList* list) {
    if (!list) return;
    list->head = NULL;
}

void ll_add_front(LinkedList* list, const char* value) {
    if (!list) return;
    LLNode* node = (LLNode*)malloc(sizeof(LLNode));
    if (!node) return;
    safeCopy(node->data, value, LL_MAX_DATA_LEN);
    node->next = list->head;
    list->head = node;
}

void ll_add_end(LinkedList* list, const char* value) {
    if (!list) return;
    LLNode* node = (LLNode*)malloc(sizeof(LLNode));
    if (!node) return;
    safeCopy(node->data, value, LL_MAX_DATA_LEN);
    node->next = NULL;

    if (!list->head) {
        list->head = node;
    }
    else {
        LLNode* temp = list->head;
        while (temp->next) temp = temp->next;
        temp->next = node;
    }
}

void ll_remove_front(LinkedList* list) {
    if (!list || !list->head) return;
    LLNode* temp = list->head;
    list->head = temp->next;
    free(temp);
}

void ll_remove_end(LinkedList* list) {
    if (!list || !list->head) return;

    if (!list->head->next) {
        /* only one node */
        free(list->head);
        list->head = NULL;
        return;
    }
    LLNode* prev = list->head;
    LLNode* curr = list->head->next;
    while (curr->next) {
        prev = curr;
        curr = curr->next;
    }
    prev->next = NULL;
    free(curr);
}

char** ll_collect_data(const LinkedList* list, int* count) {
    if (!list) {
        if (count) *count = 0;
        return NULL;
    }
    int c = 0;
    LLNode* temp = list->head;
    while (temp) {
        c++;
        temp = temp->next;
    }
    if (count) *count = c;

    if (c == 0) {
        return NULL;
    }
    char** array = (char**)malloc(sizeof(char*) * c);
    if (!array) {
        if (count) *count = 0;
        return NULL;
    }
    temp = list->head;
    for (int i = 0; i < c; i++) {
        array[i] = temp->data; // pointing directly to node's data
        temp = temp->next;
    }
    return array;
}

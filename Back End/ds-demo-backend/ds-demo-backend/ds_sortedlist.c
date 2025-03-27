#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_sortedlist.h"

/* Safely copy a string into dest, ensuring null-termination. */
static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

/* Initialize an empty sorted list. */
void sl_init(SortedList* list) {
    if (!list) return;
    list->head = NULL;
}

/*
 * Insert a value into the list, maintaining ascending numeric order.
 * Duplicate values (when converted to integers) are not allowed.
 */
void sl_insert(SortedList* list, const char* value) {
    if (!list) return;

    int newVal = atoi(value);

    // Check for duplicate based on numeric value.
    SortedListNode* cur = list->head;
    while (cur) {
        if (atoi(cur->data) == newVal) {
            // Duplicate found; do not insert.
            return;
        }
        cur = cur->next;
    }

    // Create a new node.
    SortedListNode* node = (SortedListNode*)malloc(sizeof(SortedListNode));
    if (!node) return;
    safeCopy(node->data, value, SL_MAX_DATA_LEN);
    node->next = NULL;

    // If list is empty or new node should be at the front.
    if (!list->head || newVal < atoi(list->head->data)) {
        node->next = list->head;
        list->head = node;
        return;
    }

    // Otherwise, find the proper insertion point.
    cur = list->head;
    while (cur->next && newVal >= atoi(cur->next->data)) {
        cur = cur->next;
    }
    node->next = cur->next;
    cur->next = node;
}

/* Remove the first node matching 'value' (numeric comparison). */
void sl_remove(SortedList* list, const char* value) {
    if (!list || !list->head) return;
    int remVal = atoi(value);
    if (atoi(list->head->data) == remVal) {
        SortedListNode* temp = list->head;
        list->head = temp->next;
        free(temp);
        return;
    }
    SortedListNode* cur = list->head;
    while (cur->next && atoi(cur->next->data) != remVal) {
        cur = cur->next;
    }
    if (cur->next) {
        SortedListNode* temp = cur->next;
        cur->next = temp->next;
        free(temp);
    }
}

/* Collect data pointers in an array. Caller frees the array, not the strings. */
char** sl_collect(const SortedList* list, int* count) {
    if (!list) {
        if (count) *count = 0;
        return NULL;
    }
    int c = 0;
    SortedListNode* temp = list->head;
    while (temp) {
        c++;
        temp = temp->next;
    }
    if (count) *count = c;
    if (c == 0) return NULL;

    char** arr = (char**)malloc(sizeof(char*) * c);
    if (!arr) {
        if (count) *count = 0;
        return NULL;
    }
    temp = list->head;
    for (int i = 0; i < c; i++) {
        arr[i] = temp->data; /* These pointers refer to the node's data */
        temp = temp->next;
    }
    return arr;
}

/* Clear the entire list, freeing all nodes. */
void sl_clear(SortedList* list) {
    if (!list) return;
    SortedListNode* temp = list->head;
    while (temp) {
        SortedListNode* toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    list->head = NULL;
}

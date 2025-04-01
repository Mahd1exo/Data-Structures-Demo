#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_sortedlist.h"

/* Initialize an empty sorted list. */
void sl_init(SortedList* list) {
    if (!list) return;
    list->head = NULL;
}


void sl_insert(SortedList* list, const char* value) {
    if (!list || !value) return;

    int newVal = atoi(value);

    // Check for duplicate based on numeric value.
    SortedListNode* cur = list->head;
    while (cur) {
        if (cur->data == newVal) {
            // Duplicate found; do not insert.
            return;
        }
        cur = cur->next;
    }

    // Create a new node.
    SortedListNode* node = (SortedListNode*)malloc(sizeof(SortedListNode));
    if (!node) return;
    node->data = newVal;
    node->next = NULL;

    // If list is empty or the new node should be at the front.
    if (!list->head || newVal < list->head->data) {
        node->next = list->head;
        list->head = node;
        return;
    }

    // Otherwise, find the proper insertion point.
    cur = list->head;
    while (cur->next && cur->next->data < newVal) {
        cur = cur->next;
    }
    node->next = cur->next;
    cur->next = node;
}

/*
 * Remove the first node matching 'value' (using numeric comparison).
 */
void sl_remove(SortedList* list, const char* value) {
    if (!list || !list->head || !value) return;

    int remVal = atoi(value);

    // Special case: if the head node is the one to remove.
    if (list->head->data == remVal) {
        SortedListNode* temp = list->head;
        list->head = temp->next;
        free(temp);
        return;
    }

    SortedListNode* cur = list->head;
    while (cur->next && cur->next->data != remVal) {
        cur = cur->next;
    }
    if (cur->next) {
        SortedListNode* temp = cur->next;
        cur->next = temp->next;
        free(temp);
    }
}

/*
 * Collect string representations of the node data in a contiguous array.
 *
 * This function allocates one block of memory that contains both:
 *   - An array of char* pointers (one per node)
 *   - A contiguous block of memory for all the fixed-size string buffers.
 *
 * Each string buffer is of length SL_MAX_DATA_LEN, and the integer value is
 * converted into a string using snprintf.
 *
 * The caller is responsible for freeing the returned pointer (which frees both
 * the array and the string storage).
 */
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

    /* Allocate one contiguous block:
       - Space for c pointers: sizeof(char*) * c
       - Space for c string buffers: c * SL_MAX_DATA_LEN * sizeof(char)
    */
    size_t total_size = sizeof(char*) * c + sizeof(char) * SL_MAX_DATA_LEN * c;
    char** arr = (char**)malloc(total_size);
    if (!arr) {
        if (count) *count = 0;
        return NULL;
    }

    // The block immediately following the pointer array will hold the strings.
    char* string_block = (char*)(arr + c);

    // Fill the array: each pointer gets its own fixed-size string buffer.
    temp = list->head;
    for (int i = 0; i < c; i++) {
        arr[i] = string_block + i * SL_MAX_DATA_LEN;
        snprintf(arr[i], SL_MAX_DATA_LEN, "%d", temp->data);
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

/*
* FILE : ds_sortedlist.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a Sorted List data structure.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_sortedlist.h"

// FUNCTION     : sortedlist_init
// DESCRIPTION  :
// Initializes the sorted list.
// PARAMETERS   : list - pointer to the SortedList structure
// RETURNS      : none
void sortedlist_init(SortedList* list) {
    if (!list) return;
    list->head = NULL;
}

// FUNCTION     : sortedlist_insert
// DESCRIPTION  :
// Inserts a new node with the given value into the sorted list.
// PARAMETERS   : list - pointer to the SortedList structure
//              value - the value to insert (as a string)
// RETURNS      : none
void sortedlist_insert(SortedList* list, const char* value) {
    if (!list || !value) return;

    int newVal = atoi(value);

    // Check for duplicate based on numeric value.
    SortedListNode* current = list->head;
    while (current) {
        if (current->data == newVal) {
            return;
        }
        current = current->next;
    }

    // Create a new node.
    SortedListNode* node = (SortedListNode*)malloc(sizeof(SortedListNode));
    if (!node) return;
    node->data = newVal;
    node->next = NULL;

    if (!list->head || newVal < list->head->data) {
        node->next = list->head;
        list->head = node;
        return;
    }

    current = list->head;
    while (current->next && current->next->data < newVal) {
        current = current->next;
    }
    node->next = current->next;
    current->next = node;
}

// FUNCTION     : sortedlist_remove
// DESCRIPTION  :
// Removes a node with the given value from the sorted list.
// PARAMETERS   : list - pointer to the SortedList structure
//              value - the value to remove (as a string)
// RETURNS      : none
void sortedlist_remove(SortedList* list, const char* value) {
    if (!list || !list->head || !value) return;

    int remVal = atoi(value);

    if (list->head->data == remVal) {
        SortedListNode* temp = list->head;
        list->head = temp->next;
        free(temp);
        return;
    }

    SortedListNode* current = list->head;
    while (current->next && current->next->data != remVal) {
        current = current->next;
    }
    if (current->next) {
        SortedListNode* temp = current->next;
        current->next = temp->next;
        free(temp);
    }
}

// FUNCTION     : sortedlist_collect
// DESCRIPTION  :
// Collects all data from the sorted list into an array of strings.
// PARAMETERS   : list - pointer to the SortedList structure
//              count - pointer to store the number of nodes collected
// RETURNS      : pointer to an array of strings representing the node values
char** sortedlist_collect(const SortedList* list, int* count) {
    if (!list) {
        if (count) *count = 0;
        return NULL;
    }

    int counter = 0;
    SortedListNode* temp = list->head;
    while (temp) {
        counter++;
        temp = temp->next;
    }

    if (count) *count = counter;
    if (counter == 0) return NULL;

    size_t total_size = sizeof(char*) * counter + sizeof(char) * SL_MAX_DATA_LEN * counter;
    char** collected_values = (char**)malloc(total_size);
    if (!collected_values) {
        if (count) *count = 0;
        return NULL;
    }

    char* string_block = (char*)(collected_values + counter);

    temp = list->head;
    for (int i = 0; i < counter; i++) {
        collected_values[i] = string_block + i * SL_MAX_DATA_LEN;
        snprintf(collected_values[i], SL_MAX_DATA_LEN, "%d", temp->data);
        temp = temp->next;
    }

    return collected_values;
}

// FUNCTION     : sortedlist_clear
// DESCRIPTION  :
// Clears the sorted list, freeing all nodes.
// PARAMETERS   : list - pointer to the SortedList structure
// RETURNS      : none
void sortedlist_clear(SortedList* list) {
    if (!list) return;

    SortedListNode* temp = list->head;
    while (temp) {
        SortedListNode* toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    list->head = NULL;
}

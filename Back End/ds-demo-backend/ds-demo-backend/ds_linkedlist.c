/*
* FILE : ds_linkedlist.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of AVL tree data structure
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_linkedlist.h"

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

void linkedlist_init(LinkedList* list) {
    if (!list) return;
    list->head = NULL;
}

// FUNCTION     : linkedlist_add_front
// DESCRIPTION  :
// Inserts a new node with the given value at the front of the linked list.
//// PARAMETERS   : list - pointer to the LinkedList structure
////              value - the value to insert (as a string)
//// RETURNS      : none
void linkedlist_add_front(LinkedList* list, const char* value) {
    if (!list) return;
    LLNode* node = (LLNode*)malloc(sizeof(LLNode));
    if (!node) return;
    safeCopy(node->data, value, LL_MAX_DATA_LEN);
    node->next = list->head;
    list->head = node;
}

// FUNCTION     : linkedlist_add_end
// DESCRIPTION  :
// Inserts a new node with the given value at the end of the linked list.
//// PARAMETERS   : list - pointer to the LinkedList structure
////              value - the value to insert (as a string)
//// RETURNS      : none
void linkedlist_add_end(LinkedList* list, const char* value) {
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

// FUNCTION     : linkedlist_add_by_index
// DESCRIPTION  :
// Inserts a new node with the given value at the specified index in the linked list.
// PARAMETERS   : list - pointer to the LinkedList structure
//             value - the value to insert (as a string)
//              index - the index at which to insert the new node
// RETURNS      : none
void linkedlist_add_by_index(LinkedList* list, const char* value, int index) {
    if (!list) return;
    if (index <= 0) {
        linkedlist_add_front(list, value);
        return;
    }
    LLNode* node = (LLNode*)malloc(sizeof(LLNode));
    if (!node) return;
    safeCopy(node->data, value, LL_MAX_DATA_LEN);
    node->next = NULL;

    LLNode* current = list->head;
    int currentIndex = 0;
    while (current && currentIndex < index - 1) {
        current = current->next;
        currentIndex++;
    }
    if (!current) {
        linkedlist_add_end(list, value);
    }
    else {
        node->next = current->next;
        current->next = node;
    }
}

// FUNCTION     : linkedlist_remove_front
// DESCRIPTION  :
//  Removes the first node from the linked list.
// PARAMETERS   : list - pointer to the LinkedList structure
//              value - the value to remove (as a string)
// RETURNS      : none
void linkedlist_remove_front(LinkedList* list) {
    if (!list || !list->head) return;
    LLNode* temp = list->head;
    list->head = temp->next;
    free(temp);
}

// FUNCTION     : linkedlist_remove_end
// DESCRIPTION  :
//  Removes the last node from the linked list.
//// PARAMETERS   : list - pointer to the LinkedList structure
////              value - the value to remove (as a string)
//// RETURNS      : none
void linkedlist_remove_end(LinkedList* list) {
    if (!list || !list->head) return;
    if (!list->head->next) {
        free(list->head);
        list->head = NULL;
        return;
    }
    LLNode* prev = list->head;
    LLNode* current = list->head->next;
    while (current->next) {
        prev = current;
        current = current->next;
    }
    prev->next = NULL;
    free(current);
}

// FUNCTION     : linkedlist_remove_by_index
// DESCRIPTION  :
//  Removes a node at the specified index from the linked list.
//// PARAMETERS   : list - pointer to the LinkedList structure
////              index - the index of the node to remove
//// // RETURNS      : none
void linkedlist_remove_by_index(LinkedList* list, int index) {
    if (!list || !list->head) return;
    if (index <= 0) {
        linkedlist_remove_front(list);
        return;
    }
    LLNode* prev = list->head;
    LLNode* current = list->head->next;
    int currentIndex = 1;
    while (current && currentIndex < index) {
        prev = current;
        current = current->next;
        currentIndex++;
    }
    if (current) {
        prev->next = current->next;
        free(current);
    }
}

// FUNCTION     : linkedlist_remove
// DESCRIPTION  :
//  Removes a node with the given value from the linked list.
// // PARAMETERS   : list - pointer to the LinkedList structure
////              value - the value to remove (as a string)
//// RETURNS      : none
void linkedlist_clear(LinkedList* list) {
    if (!list) return;
    while (list->head) {
        linkedlist_remove_front(list);
    }
}

// FUNCTION     : linkedlist_collect_data
// DESCRIPTION  :
//  Collects all data from the linked list into an array of strings.
//// PARAMETERS   : list - pointer to the LinkedList structure
////              count - pointer to store the number of nodes collected
//// RETURNS      : pointer to an array of strings representing the node 
char** linkedlist_collect_data(const LinkedList* list, int* count) {
    if (!list) {
        if (count) *count = 0;
        return NULL;
    }
    int counter = 0;
    LLNode* temp = list->head;
    while (temp) {
        counter++;
        temp = temp->next;
    }
    if (count) *count = counter;
    if (counter == 0) {
        return NULL;
    }
    char** array = (char**)malloc(sizeof(char*) * counter);
    if (!array) {
        if (count) *count = 0;
        return NULL;
    }
    temp = list->head;
    for (int i = 0; i < counter; i++) {
        array[i] = temp->data;
        temp = temp->next;
    }
    return array;
}

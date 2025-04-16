/*
* FILE : ds_circularlist.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a Circular Linked List data structure.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_circularlist.h"

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

// FUNCTION     : circularLinkedList_init
// DESCRIPTION  :
// Initializes the circular linked list.
// PARAMETERS   : list - pointer to the CircularList structure
//// RETURNS      : none
void circularLinkedList_init(CircularList* list) {
    if (!list) return;
    list->head = NULL;
}

// FUNCTION     : circularLinkedList_insert
// DESCRIPTION  :
// Inserts a new node with the given value at the end of the circular linked list.
//// PARAMETERS   : list - pointer to the CircularList structure
////              value - the value to insert (as a string)
//// RETURNS      : none
void circularLinkedList_insert(CircularList* list, const char* value) {
    if (!list) return;

    CircularListNode* node = (CircularListNode*)malloc(sizeof(CircularListNode));
    if (!node) return;
    safeCopy(node->data, value, CL_MAX_DATA_LEN);
    node->next = node;

    if (!list->head) {
        list->head = node;
    }
    else {
        CircularListNode* temp = list->head;
        while (temp->next != list->head) {
            temp = temp->next;
        }
        temp->next = node;
        node->next = list->head;
    }
}

// FUNCTION     : circularLinkedList_insert_by_index
// DESCRIPTION  :
// Inserts a new node with the given value at the specified index in the circular linked list.
//// PARAMETERS   : list - pointer to the CircularList structure
////              value - the value to insert (as a string)
////              index - the index at which to insert the new node
//// RETURNS      : none
void circularLinkedList_insert_by_index(CircularList* list, const char* value, int index) {
    if (!list) return;
    CircularListNode* newNode = (CircularListNode*)malloc(sizeof(CircularListNode));
    if (!newNode) return;
    safeCopy(newNode->data, value, CL_MAX_DATA_LEN);
    newNode->next = newNode; 

    if (!list->head) {
        list->head = newNode;
        return;
    }

    if (index <= 0) {
        CircularListNode* tail = list->head;
        while (tail->next != list->head) {
            tail = tail->next;
        }
        newNode->next = list->head;
        tail->next = newNode;
        list->head = newNode;
        return;
    }

    CircularListNode* current = list->head;
    int i = 0;
    while (i < index - 1 && current->next != list->head) {
        current = current->next;
        i++;
    }
    newNode->next = current->next;
    current->next = newNode;
}

// FUNCTION     : circularLinkedList_add_front
// DESCRIPTION  :
// Adds a new node with the given value at the front of the circular linked list.
// // PARAMETERS   : list - pointer to the CircularList structure
////              value - the value to insert (as a string)
//// RETURNS      : none
void circularLinkedList_add_front(CircularList* list, const char* value) {
    circularLinkedList_insert_by_index(list, value, 0);
}

// FUNCTION     : circularLinkedList_add_end
// DESCRIPTION  :
// Adds a new node with the given value at the end of the circular linked list.
//// PARAMETERS   : list - pointer to the CircularList structure
////              value - the value to insert (as a string)
//// RETURNS      : none
void circularLinkedList_add_end(CircularList* list, const char* value) {
    circularLinkedList_insert(list, value);
}

// FUNCTION     : circularLinkedList_remove
// DESCRIPTION  :
//  Removes a node with the given value from the circular linked list.
//// PARAMETERS   : list - pointer to the CircularList structure
////              value - the value to remove (as a string)
//// RETURNS      : none
void circularLinkedList_remove(CircularList* list, const char* value) {
    if (!list || !list->head) return;

    CircularListNode* current = list->head;
    CircularListNode* prev = NULL;

    if (strcmp(current->data, value) == 0) {
        if (current->next == list->head) {
            free(current);
            list->head = NULL;
            return;
        }
        CircularListNode* tail = list->head;
        while (tail->next != list->head) {
            tail = tail->next;
        }
        tail->next = current->next;
        list->head = current->next;
        free(current);
        return;
    }

    prev = current;
    current = current->next;
    while (current != list->head) {
        if (strcmp(current->data, value) == 0) {
            prev->next = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// FUNCTION     : circularLinkedList_remove_front
// DESCRIPTION  :
// Removes the front node from the circular linked list.
//// PARAMETERS   : list - pointer to the CircularList structure
//// RETURNS      : none
void circularLinkedList_remove_front(CircularList* list) {
    if (!list || !list->head) return;
    CircularListNode* head = list->head;

    if (head->next == head) {
        free(head);
        list->head = NULL;
        return;
    }

    CircularListNode* tail = head;
    while (tail->next != head) {
        tail = tail->next;
    }
    tail->next = head->next;
    list->head = head->next;
    free(head);
}

// FUNCTION     : circularLinkedList_remove_end
//  DESCRIPTION  :
//  Removes the last node from the circular linked list.
//// PARAMETERS   : list - pointer to the CircularList structure
////              value - the value to remove (as a string)
//// RETURNS      : none
void circularLinkedList_remove_end(CircularList* list) {
    if (!list || !list->head) return;
    CircularListNode* head = list->head;
    if (head->next == head) {
        free(head);
        list->head = NULL;
        return;
    }
    CircularListNode* current = head;
    CircularListNode* prev = NULL;
    while (current->next != head) {
        prev = current;
        current = current->next;
    }
    if (prev) {
        prev->next = head;
    }
    free(current);
}

// FUNCTION     : circularLinkedList_remove_by_index
// DESCRIPTION  :
// Removes a node at the specified index from the circular linked list.
//// PARAMETERS   : list - pointer to the CircularList structure
////              index - the index of the node to remove
//// RETURNS      : none
void circularLinkedList_remove_by_index(CircularList* list, int index) {
    if (!list || !list->head) return;
    if (index <= 0) {
        circularLinkedList_remove_front(list);
        return;
    }
    int len = 0;
    CircularListNode* temp = list->head;
    do {
        len++;
        temp = temp->next;
    } while (temp != list->head);

    if (index >= len) { 
        circularLinkedList_remove_end(list);
        return;
    }

    CircularListNode* current = list->head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }

    CircularListNode* toRemove = current->next;
    current->next = toRemove->next;
    free(toRemove);
}

// FUNCTION     : circularLinkedList_collect_data
// DESCRIPTION  :
// Collects all data from the circular linked list into an array of strings.
//// PARAMETERS   : list - pointer to the CircularList structure
////              count - pointer to store the number of nodes collected
//// RETURNS      : pointer to an array of strings representing the node values
char** circularLinkedList_collect_data(const CircularList* list, int* count) {
    if (!list || !list->head) {
        if (count) *count = 0;
        return NULL;
    }
    int counter = 0;
    CircularListNode* temp = list->head;
    do {
        counter++;
        temp = temp->next;
    } while (temp != list->head);

    if (count) *count = counter;
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

// FUNCTION     : circularLinkedList_clear
// DESCRIPTION  :
// Clears the circular linked list, freeing all nodes.
//// PARAMETERS   : list - pointer to the CircularList structure
//// RETURNS      : none
void circularLinkedList_clear(CircularList* list) {
    if (!list || !list->head) return;
    CircularListNode* current = list->head;
    CircularListNode* nextNode;
    // Break the circular link
    CircularListNode* tail = list->head;
    while (tail->next != list->head) {
        tail = tail->next;
    }
    tail->next = NULL;

    while (current) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    list->head = NULL;
}

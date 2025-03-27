#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_circularlist.h"


static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}


void cl_init(CircularList* list) {
    if (!list) return;
    list->head = NULL;
}


void cl_insert(CircularList* list, const char* value) {
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


void cl_insert_by_index(CircularList* list, const char* value, int index) {
    if (!list) return;
    CircularListNode* newNode = (CircularListNode*)malloc(sizeof(CircularListNode));
    if (!newNode) return;
    safeCopy(newNode->data, value, CL_MAX_DATA_LEN);
    newNode->next = newNode; 


    if (!list->head) {
        list->head = newNode;
        return;
    }

    // Insert at head if index <= 0
    if (index <= 0) {
        // Find tail node to update its next pointer
        CircularListNode* tail = list->head;
        while (tail->next != list->head) {
            tail = tail->next;
        }
        newNode->next = list->head;
        tail->next = newNode;
        list->head = newNode;
        return;
    }

    // Traverse list to find the correct insertion point.
    CircularListNode* current = list->head;
    int i = 0;
    while (i < index - 1 && current->next != list->head) {
        current = current->next;
        i++;
    }
    newNode->next = current->next;
    current->next = newNode;
}

/* Add front: same as inserting at index 0 */
void cl_add_front(CircularList* list, const char* value) {
    cl_insert_by_index(list, value, 0);
}

/* Add end: same as default insertion */
void cl_add_end(CircularList* list, const char* value) {
    cl_insert(list, value);
}

/* Remove the first node matching 'value' */
void cl_remove(CircularList* list, const char* value) {
    if (!list || !list->head) return;

    CircularListNode* current = list->head;
    CircularListNode* prev = NULL;

    /* Check if head needs removal */
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


void cl_remove_front(CircularList* list) {
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


void cl_remove_end(CircularList* list) {
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

/* Remove by index */
void cl_remove_by_index(CircularList* list, int index) {
    if (!list || !list->head) return;
    // If index is 0, remove front
    if (index <= 0) {
        cl_remove_front(list);
        return;
    }
    int len = 0;
    CircularListNode* temp = list->head;
    do {
        len++;
        temp = temp->next;
    } while (temp != list->head);

    if (index >= len) { 
        cl_remove_end(list);
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


char** cl_collect_data(const CircularList* list, int* count) {
    if (!list || !list->head) {
        if (count) *count = 0;
        return NULL;
    }
    int c = 0;
    CircularListNode* temp = list->head;
    do {
        c++;
        temp = temp->next;
    } while (temp != list->head);

    if (count) *count = c;
    char** array = (char**)malloc(sizeof(char*) * c);
    if (!array) {
        if (count) *count = 0;
        return NULL;
    }

    temp = list->head;
    for (int i = 0; i < c; i++) {
        array[i] = temp->data;
        temp = temp->next;
    }
    return array;
}

void cl_clear(CircularList* list) {
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

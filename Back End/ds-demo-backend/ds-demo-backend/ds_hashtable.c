/*
* FILE : ds_hashtable.c
* PROJECT : SENG1050 - Data Structures
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2025-03-15
* DESCRIPTION :
* This file contains the implementation of a simple hash table data structure.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_hashtable.h"

// FUNCTION     : ascii_hash
// DESCRIPTION  :
// Computes a simple hash value for a string using ASCII values.
//// PARAMETERS   : str - the string to hash
//// RETURNS      : the hash value (unsigned int)
static unsigned int ascii_hash(const char* str) {
    unsigned int sum = 0;
    while (*str) {
        sum += (unsigned char)(*str);
        str++;
    }
    return sum % HASH_CAPACITY;  /* capacity is 16 */
}

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

// FUNCTION     : hashTable_init
// DESCRIPTION  :
// Initializes the hash table.
//// PARAMETERS   : hashTable - pointer to the SimpleHashTable structure
//// RETURNS      : none
void hashTable_init(SimpleHashTable* hashTable) {
    if (!hashTable) return;
    for (int i = 0; i < HASH_CAPACITY; i++) {
        hashTable->buckets[i] = NULL;
    }
}

//FUNCTION     : createNode
//DESCRIPTION  :
//Creates a new hash table node with the given key and value.
//// PARAMETERS   : key - the key to store in the node
////              value - the value to store in the node
//// RETURNS      : pointer to the new node (NULL if allocation fails)
static HashNode* createNode(const char* key, const char* value) {
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (!node) return NULL;
    safeCopy(node->key, key, KEY_MAX_LEN);
    safeCopy(node->value, value, VAL_MAX_LEN);
    node->next = NULL;
    return node;
}

// FUNCTION     : hashTable_insert
// DESCRIPTION  :
// Inserts a key-value pair into the hash table.
//// If the key already exists, updates its value.
///// PARAMETERS   : hashTable - pointer to the SimpleHashTable structure
////              key - the key to insert (as a string)
////              value - the value to insert (as a string)
//// RETURNS      : none
void hashTable_insert(SimpleHashTable* hashTable, const char* key, const char* value) {
    if (!hashTable) return;
    unsigned int hashKey = ascii_hash(key);

    // Check if key is already present => update
    HashNode* current = hashTable->buckets[hashKey];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            safeCopy(current->value, value, VAL_MAX_LEN);
            return;
        }
        current = current->next;
    }

    
    HashNode* newN = createNode(key, value);
    newN->next = hashTable->buckets[hashKey];
    hashTable->buckets[hashKey] = newN;
}

// FUNCTION     : hashTable_remove
// DESCRIPTION  :
// Removes a key-value pair from the hash table.
//// PARAMETERS   : hashTable - pointer to the SimpleHashTable structure
////              key - the key to remove (as a string)
//// RETURNS      : none
void hashTable_remove(SimpleHashTable* hashTable, const char* key) {
    if (!hashTable) return;
    unsigned int hashKey = ascii_hash(key);
    HashNode* current = hashTable->buckets[hashKey];
    HashNode* prev = NULL;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                hashTable->buckets[hashKey] = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// FUNCTION     : hashTable_get
// DESCRIPTION  :
// Retrieves the value associated with a key in the hash table.
//// PARAMETERS   : hashTable - pointer to the SimpleHashTable structure
///              key - the key to search for (as a string)
//// RETURNS      : pointer to the value (as a string) or NULL if not found
const char* hashTable_get(SimpleHashTable* hashTable, const char* key) {
    if (!hashTable) return NULL;
    unsigned int hashKey = ascii_hash(key);
    HashNode* curr = hashTable->buckets[hashKey];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}
// FUNCTION     : hashTable_contains
// DESCRIPTION  :
// Checks if a key exists in the hash table.
//// PARAMETERS   : hashTable - pointer to the SimpleHashTable structure
////              key - the key to search for (as a string)
//// RETURNS      : 1 if the key exists, 0 otherwise
//// Returns 0 if the hash table is NULL.
int hashTable_contains(SimpleHashTable* hashTable, const char* key) {
    if (!hashTable) return 0;
    unsigned int hashKey = ascii_hash(key);
    HashNode* curr = hashTable->buckets[hashKey];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

// FUNCTION     : hashTable_collect
// DESCRIPTION  :
// Collects all key-value pairs from the hash table into an array of strings.
//// Each string is formatted as "key:value".
//// PARAMETERS   : hashTable - pointer to the SimpleHashTable structure
////              count - pointer to store the number of pairs collected
//// RETURNS      : pointer to an array of strings representing the key-value pairs
char** hashTable_collect(SimpleHashTable* hashTable, int* count) {
    if (!hashTable) {
        if (count) *count = 0;
        return NULL;
    }
    int capacity = 16;
    int counter = 0;
    char** arrayHolder = (char**)malloc(sizeof(char*) * capacity);

    for (int i = 0; i < HASH_CAPACITY; i++) {
        HashNode* current = hashTable->buckets[i];
        while (current) {
            if (counter >= capacity) {
                capacity *= 2;
                char** temp = (char**)realloc(arrayHolder, sizeof(char*) * capacity);
                if (temp) arrayHolder = temp;
            }
            // Build "key:value"
            char keyValue[KEY_MAX_LEN + VAL_MAX_LEN + 2];
            snprintf(keyValue, sizeof(keyValue), "%s:%s", current->key, current->value);

            arrayHolder[counter] = (char*)malloc(strlen(keyValue) + 1);
            strcpy(arrayHolder[counter], keyValue);
            counter++;
            current = current->next;
        }
    }
    if (count) *count = counter;
    return arrayHolder;
}

// FUNCTION     : hashTable_clear
// DESCRIPTION  :
// Clears the hash table, freeing all nodes.
//// PARAMETERS   : hashTable - pointer to the SimpleHashTable structure
//// RETURNS      : none
void hashTable_clear(SimpleHashTable* hashTable) {
    if (!hashTable) return;
    for (int i = 0; i < HASH_CAPACITY; i++) {
        HashNode* current = hashTable->buckets[i];
        while (current) {
            HashNode* temp = current;
            current = current->next;
            free(temp);
        }
        hashTable->buckets[i] = NULL;
    }
}

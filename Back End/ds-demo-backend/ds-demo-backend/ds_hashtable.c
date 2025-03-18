#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_hashtable.h"

/* ASCII-sum hashing: sum all character codes, then mod by HASH_CAPACITY=16 */
static unsigned int ascii_hash(const char* str) {
    unsigned int sum = 0;
    while (*str) {
        sum += (unsigned char)(*str);
        str++;
    }
    return sum % HASH_CAPACITY;  /* capacity is 16 */
}

/* Safely copy a string (like strncpy, but ensures null termination) */
static void safeCopy(char* dest, const char* src, int maxLen) {
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

/* Initialize the hash table: set all buckets to NULL */
void hash_init(SimpleHashTable* ht) {
    if (!ht) return;
    for (int i = 0; i < HASH_CAPACITY; i++) {
        ht->buckets[i] = NULL;
    }
}

/* Create a new hash node with the given key-value */
static HashNode* createNode(const char* key, const char* value) {
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (!node) return NULL;
    safeCopy(node->key, key, KEY_MAX_LEN);
    safeCopy(node->value, value, VAL_MAX_LEN);
    node->next = NULL;
    return node;
}

/* Insert or update a key-value pair */
void hash_insert(SimpleHashTable* ht, const char* key, const char* value) {
    if (!ht) return;
    unsigned int h = ascii_hash(key);

    // Check if key is already present => update
    HashNode* curr = ht->buckets[h];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            // Update value
            safeCopy(curr->value, value, VAL_MAX_LEN);
            return;
        }
        curr = curr->next;
    }

    // Not found => insert new node at front
    HashNode* newN = createNode(key, value);
    newN->next = ht->buckets[h];
    ht->buckets[h] = newN;
}

/* Remove a key from the table */
void hash_remove(SimpleHashTable* ht, const char* key) {
    if (!ht) return;
    unsigned int h = ascii_hash(key);
    HashNode* curr = ht->buckets[h];
    HashNode* prev = NULL;
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) {
                prev->next = curr->next;
            }
            else {
                ht->buckets[h] = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/* Return the value for a given key, or NULL if not found */
const char* hash_get(SimpleHashTable* ht, const char* key) {
    if (!ht) return NULL;
    unsigned int h = ascii_hash(key);
    HashNode* curr = ht->buckets[h];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}

/* Check if the key exists (1 if yes, 0 if no) */
int hash_contains(SimpleHashTable* ht, const char* key) {
    if (!ht) return 0;
    unsigned int h = ascii_hash(key);
    HashNode* curr = ht->buckets[h];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/* Collect all (key:value) pairs in an array of strings.
   The caller is responsible for freeing each string and the array. */
char** hash_collect(SimpleHashTable* ht, int* count) {
    if (!ht) {
        if (count) *count = 0;
        return NULL;
    }
    int capacity = 16;
    int c = 0;
    char** arr = (char**)malloc(sizeof(char*) * capacity);

    for (int i = 0; i < HASH_CAPACITY; i++) {
        HashNode* curr = ht->buckets[i];
        while (curr) {
            if (c >= capacity) {
                capacity *= 2;
                char** temp = (char**)realloc(arr, sizeof(char*) * capacity);
                if (temp) arr = temp;
            }
            // Build "key:value"
            char kv[KEY_MAX_LEN + VAL_MAX_LEN + 2];
            snprintf(kv, sizeof(kv), "%s:%s", curr->key, curr->value);

            arr[c] = (char*)malloc(strlen(kv) + 1);
            strcpy(arr[c], kv);
            c++;
            curr = curr->next;
        }
    }
    if (count) *count = c;
    return arr;
}

/* Clear all entries in the hash table */
void hash_clear(SimpleHashTable* ht) {
    if (!ht) return;
    for (int i = 0; i < HASH_CAPACITY; i++) {
        HashNode* curr = ht->buckets[i];
        while (curr) {
            HashNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
        ht->buckets[i] = NULL;
    }
}

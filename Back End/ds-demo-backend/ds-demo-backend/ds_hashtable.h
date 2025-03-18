#ifndef DS_HASHTABLE_H
#define DS_HASHTABLE_H

#define HASH_CAPACITY 16  /* Only 16 buckets now */
#define KEY_MAX_LEN 64
#define VAL_MAX_LEN 64

typedef struct HashNode {
    char key[KEY_MAX_LEN];
    char value[VAL_MAX_LEN];
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* buckets[HASH_CAPACITY];
} SimpleHashTable;

#ifdef __cplusplus
extern "C" {
#endif

    /* Initialize the hash table (clear all buckets) */
    void hash_init(SimpleHashTable* ht);

    /* Insert a key-value pair (ASCII-sum hashing) */
    void hash_insert(SimpleHashTable* ht, const char* key, const char* value);

    /* Remove a key from the hash table */
    void hash_remove(SimpleHashTable* ht, const char* key);

    /* Get the value for a given key (returns NULL if not found) */
    const char* hash_get(SimpleHashTable* ht, const char* key);

    /* Check if the key exists in the table (1 if yes, 0 if no) */
    int hash_contains(SimpleHashTable* ht, const char* key);

    /* Collect all (key:value) pairs in a dynamic array of strings.
       Caller must free each string plus the array itself. */
    char** hash_collect(SimpleHashTable* ht, int* count);

    /* Clear all entries in the table */
    void hash_clear(SimpleHashTable* ht);

#ifdef __cplusplus
}
#endif

#endif /* DS_HASHTABLE_H */

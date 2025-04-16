/*
* FILE : ds_hashtable.h
* PROJECT : FINAL PROJECT - DATA STRUCTURE - Data Structure Demo
* PROGRAMMER : Mohammad Mehdi Ebrahimzadeh
* FIRST VERSION : 2024-03-10
*
* DESCRIPTION :
* This file contains the declaration of the hash table data structure and its functions.
*/
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
    
	//Functions prototypes
    void hashTable_init(SimpleHashTable* hashTable);
    void hashTable_insert(SimpleHashTable* hashTable, const char* key, const char* value);
    void hashTable_remove(SimpleHashTable* hashTable, const char* key);
    const char* hashTable_get(SimpleHashTable* hashTable, const char* key);
    int hashTable_contains(SimpleHashTable* hashTable, const char* key);
    char** hashTable_collect(SimpleHashTable* hashTable, int* count);
    void hashTable_clear(SimpleHashTable* hashTable);

#ifdef __cplusplus
}
#endif

#endif /* DS_HASHTABLE_H */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h> 

/**
 * @struct HashTable
 * @brief Opaque structure representing a hash table.
 */
typedef struct HashTable HashTable;

typedef struct Node {
    const void* key;
    const void* value;
    struct Node* next;
} Node;

struct HashTable {
    Node** buckets;
    int capacity;
    int size;
    int (*compare)(const void*, const void*);
    unsigned long (*hash)(const void*);
};

int bucket_index2(const HashTable* ht, const void* key) ;

/**
 * @brief Creates a new empty hash table.
 *
 * @param compare  Pointer to a comparison function for keys.
 *             Must return:
 *             - zero if keys are equal
 *             - a negative value if key1 is lower than key2
 *             - a positive value if key1 is greater than key2
 *
 * @param hash Pointer to a hash function that maps a key to
 *             an unsigned long value.
 *
 * @return Pointer to the newly created hash table,
 *         or NULL if memory allocation fails.
 */
HashTable* hash_table_create(int (*f1)(const void*, const void*),
                             unsigned long (*f2)(const void*));

/**
 * @brief Inserts or updates a key-value pair in the hash table.
 *
 * If the key already exists, its associated value is replaced.
 *
 * @param table Pointer to the hash table.
 * @param key   Pointer to the key.
 * @param value Pointer to the value.
 */
void hash_table_put(HashTable*, const void* key, const void* value);

/**
 * @brief Retrieves the value associated with a given key.
 *
 * @param table Pointer to the hash table.
 * @param key   Pointer to the key.
 *
 * @return Pointer to the associated value,
 *         or NULL if the key is not present.
 */
void* hash_table_get(const HashTable*, const void* key);

/**
 * @brief Checks whether a key is present in the hash table.
 *
 * @param table Pointer to the hash table.
 * @param key   Pointer to the key.
 *
 * @return 1 if the key exists, 0 otherwise.
 */
int hash_table_contains_key(const HashTable*, const void* key);

/**
 * @brief Removes a key-value pair from the hash table.
 *
 * If the key is not present, the function does nothing.
 *
 * @param table Pointer to the hash table.
 * @param key   Pointer to the key.
 */
void hash_table_remove(HashTable*, const void* key);

/**
 * @brief Returns the number of elements stored in the table.
 *
 * @param table Pointer to the hash table.
 *
 * @return Number of key-value pairs currently stored.
 */
int hash_table_size(const HashTable*);

/**
 * @brief Returns an array containing all keys in the table.
 *
 * @param table Pointer to the hash table.
 *
 * @return Pointer to an array of keys,
 *         or NULL if the table is empty.
 */
void** hash_table_keyset(const HashTable*);

/**
 * @brief Frees all memory used by the hash table.
 *
 * This function releases internal structures.
 *
 * @param table Pointer to the hash table.
 */
void hash_table_free(HashTable*);

#endif
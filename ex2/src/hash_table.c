#include "hash_table.h"

#include <stdlib.h>
#include <stdint.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75

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

/* -------------------------------------------------------
 *  Hash Table helpers 
 * ------------------------------------------------------- */

// Returns the bucket index for a given key.
static int bucket_index(const HashTable* ht, const void* key) {
    return (int)(ht->hash(key) % (unsigned long)ht->capacity);
}

// Resizes the table and rehashes all entries.
static void rehash(HashTable* ht) {
    int old_capacity = ht->capacity;
    Node** old_buckets = ht->buckets;
    ht->capacity *= 2;
    ht->buckets = calloc(ht->capacity, sizeof(Node*));
    ht->size = 0;
    for (int i = 0; i < old_capacity; i++) {
        Node* node = old_buckets[i];

        while (node) {
            Node* next = node->next;
            int idx = bucket_index(ht, node->key);
            node->next = ht->buckets[idx];
            ht->buckets[idx] = node;
            ht->size++;
            node = next;
        }
    }
    free(old_buckets);
}

/* -------------------------------------------------------
 *  Public API
 * ------------------------------------------------------- */

HashTable* hash_table_create(int (*f1)(const void*, const void*), unsigned long (*f2)(const void*)) {
    if (!f1 || !f2) return NULL;

    HashTable* ht = malloc(sizeof(HashTable));
    ht->capacity = INITIAL_CAPACITY;
    ht->size = 0;
    ht->compare = f1;
    ht->hash = f2;
    ht->buckets = calloc(ht->capacity, sizeof(Node*));
    return ht;
}

void hash_table_put(HashTable* ht, const void* key, const void* value) {
    if (!ht || !key) return;

    int idx = bucket_index(ht, key);
    Node* node = ht->buckets[idx];
    while (node) {
        if (ht->compare(node->key, key) == 0) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    if ((double)ht->size / ht->capacity >= LOAD_FACTOR_THRESHOLD) {
        rehash(ht);
        idx = bucket_index(ht, key);
    }

    Node* new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = ht->buckets[idx];
    ht->buckets[idx] = new_node;
    ht->size++;
}

void* hash_table_get(const HashTable* ht, const void* key) {
    if (!ht || !key) return NULL;

    int idx = bucket_index(ht, key);
    Node* node = ht->buckets[idx];
    while (node) {
        if (ht->compare(node->key, key) == 0) {
            return (void*)node->value;
        }
        node = node->next;
    }
    return NULL;
}

int hash_table_contains_key(const HashTable* ht, const void* key) {
    if (!ht || !key) return 0;

    int idx = bucket_index(ht, key);
    Node* node = ht->buckets[idx];
    while (node) {
        if (ht->compare(node->key, key) == 0) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void hash_table_remove(HashTable* ht, const void* key) {
    if (!ht || !key) return;

    int idx = bucket_index(ht, key);
    Node* node = ht->buckets[idx];
    Node* prev = NULL;
    while (node) {
        if (ht->compare(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                ht->buckets[idx] = node->next;
            }
            free(node);
            ht->size--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

int hash_table_size(const HashTable* ht) {
    if (!ht) return -1;

    return ht->size;
}

void** hash_table_keyset(const HashTable* ht) {
    if (!ht) return NULL;

    void** keys = malloc((ht->size + 1) * sizeof(void*));
    if (!keys) return NULL;

    int k = 0;
    for (int i = 0; i < ht->capacity; i++) {
        Node* node = ht->buckets[i];
        while (node) {
            keys[k++] = (void*)node->key;
            node = node->next;
        }
    }
    keys[k] = NULL;
    return keys;
}

void hash_table_free(HashTable* ht) {
    if (!ht) return;

    for (int i = 0; i < ht->capacity; i++) {
        Node* node = ht->buckets[i];
        while (node) {
            Node* next = node->next;
            free(node);
            node = next;
        }
    }
    free(ht->buckets);
    free(ht);
}
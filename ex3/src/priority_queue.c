#include "priority_queue.h"
#include "lib/hash_table.h"

#include <stdlib.h>

#define INITIAL_CAP 8

struct PriorityQueue {
    void** heap;
    int size;
    int cap;
    int (*compare)(const void*, const void*);
    HashTable* pos_map;
};

/* -------------------------------------------------------
 *  Hash Table helpers 
 * ------------------------------------------------------- */

 // Sets the position of elem in the hash table to idx. Allocates a new entry if not already present.
 static int pos_set(PriorityQueue* pq, void* elem, int idx) {
    if (!pq || !elem) return -1;

    int *p = hash_table_get(pq->pos_map, elem); 
    if (p) {
        *p = idx;
    } else {
        p = malloc(sizeof(int)); 
        if (!p) return -1;
        *p = idx;
        hash_table_put(pq->pos_map, elem, p);
    }
    return 1;
}

// Frees the position entry for elem and removes it from the map.
static void pos_delete(PriorityQueue* pq, const void* elem) {
    int *p = hash_table_get(pq->pos_map, elem);
    if (p) {
        free(p);
        hash_table_remove(pq->pos_map, elem);
    }
}

/* -------------------------------------------------------
 *  Ternary heap helpers
 * ------------------------------------------------------- */

// Returns the index of the parent of node i
static int parent(int i) { 
    return (i - 1) / 3; 
}

// Returns the index of the first child of node i
static int child(int i) { 
    return 3 * i + 1; 
}

// Swaps two elements in the heap and updates their positions in the map.
static void swap(PriorityQueue* pq, int a, int b){ 
    if (a == b) return;

    int* pa = hash_table_get(pq->pos_map, pq->heap[a]);
    int* pb = hash_table_get(pq->pos_map, pq->heap[b]);
    if (pa) *pa = b;
    if (pb) *pb = a; 

    void* tmp = pq->heap[a];
    pq->heap[a] = pq->heap[b];
    pq->heap[b] = tmp;
}

// Moves node i up the heap until the heap property is restored.
static void sift_up(PriorityQueue* pq, int i) {
    while (i > 0) {
        int p = parent(i);
        if (pq->compare(pq->heap[i], pq->heap[p]) > 0) {
            swap(pq, i, p);
            i = p;
        } else {
            break;
        }
    }
}

// Moves node i down the heap until the heap property is restored. At each step, selects the highest-priority child among the three.
static void sift_down(PriorityQueue* pq, int i) {
    int n = pq->size;
    while (1) {
        int best = i;
        int c0 = child(i);

        for (int k = 0; k < 3; k++) {
            int c = c0 + k;
            if (c < n && pq->compare(pq->heap[c], pq->heap[best]) > 0) {
                best = c;
            }
        }
        if (best == i) break;
        swap(pq, i, best);
        i = best; 
    }
}

// Doubles the heap's internal buffer capacity.
static int heap_grow(PriorityQueue* pq) {
    int new_cap = pq->cap * 2;
    void** new_heap = realloc(pq->heap, (unsigned int)new_cap * sizeof(void*));
    if (!new_heap) return -1;
    pq->heap = new_heap;
    pq->cap = new_cap;
    return 0;
}

/* -------------------------------------------------------
 *  Public API
 * ------------------------------------------------------- */

PriorityQueue* priority_queue_create(int (*f1)(const void*,const void*), unsigned long (*f2)(const void*)) {
    if (!f1 || !f2) return NULL;

    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    if (!pq) return NULL;

    pq->heap = malloc(INITIAL_CAP * sizeof(void*));
    if (!pq->heap) { free(pq); return NULL; }

    pq->pos_map = hash_table_create(f1, f2);
    if (!pq->pos_map) { free(pq->heap); free(pq); return NULL; }

    pq->size = 0;
    pq->cap = INITIAL_CAP; 
    pq->compare = f1;

    return pq;
}

int priority_queue_push(PriorityQueue* pq, void* elem) {
    if (!pq || !elem) return 0;

    if (pq->size == pq->cap && heap_grow(pq) != 0) return 0;

    if (priority_queue_contains(pq, elem)) return 0;

    int idx = pq->size;
    pq->heap[idx] = elem;
    pq->size++;

    if (pos_set(pq, elem, idx) < 0) {
        pq->size--;
        return 0;
    }
    sift_up(pq, idx);
    return 1;
}

int priority_queue_contains(const PriorityQueue* pq, const void* elem) {
    if (!pq || !elem) return 0;
    
    return hash_table_contains_key(pq->pos_map, elem);
}


void* priority_queue_top(const PriorityQueue* pq) {
    if (!pq || pq->size == 0) return NULL;

    return pq->heap[0];
}

void priority_queue_pop(PriorityQueue* pq) {
    if (!pq || pq->size == 0) return;
    
    pos_delete(pq, pq->heap[0]); 

    pq->size--;
    if (pq->size == 0) return;

    pq->heap[0] = pq->heap[pq->size];
    int* p = hash_table_get(pq->pos_map, pq->heap[0]);
    if (p) *p = 0;

    sift_down(pq, 0); 
}

int priority_queue_remove(PriorityQueue* pq, const void* elem) {
    if (!pq || !elem) return 0;

    int* idx = hash_table_get(pq->pos_map, elem);
    if (!idx) return 0;

    int i = *idx;
    pos_delete(pq, elem);
    pq->size--;

    if (i == pq->size) return 1;

    pq->heap[i] = pq->heap[pq->size];
    int* p = hash_table_get(pq->pos_map, pq->heap[i]);
    if (p) *p = i;

    sift_up(pq, i);
    sift_down(pq, i);
    return 1;
}

int priority_queue_size(const PriorityQueue* pq) {
    if (!pq) return -1;

    return pq->size;
}

void priority_queue_free(PriorityQueue* pq) {
    if (!pq) return;

    for (int i = 0; i < pq->size; i++) {
        pos_delete(pq, pq->heap[i]);
    }
    hash_table_free(pq->pos_map);
    free(pq->heap);
    free(pq);
}
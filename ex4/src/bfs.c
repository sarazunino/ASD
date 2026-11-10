#include "../include/bfs.h"
# include <stdio.h>
# include <stdlib.h>

void printQueue(void** queue){
    int i =0;
    while(queue[i] != NULL){
        printf("%s -> \n ", (char*)queue[i++]);
    }
}

// remove prints please :)
void **breadth_first_visit(Graph gr, void *start, int (*compare)(const void *, const void *), unsigned long (*hash)(const void *)) {
    
    if (gr == NULL || start == NULL || compare == NULL || hash == NULL || !graph_contains_node(gr, start)) {
        printf("starting exception\n");
        return NULL;
    }

    int size = graph_num_nodes(gr);
    void **queue = malloc(sizeof(struct Node) * size+1);
    if (queue == NULL) {
        printf("queue malloc failed\n");
        return NULL;
    }
    
    HashTable *inserted = hash_table_create(compare, hash);
    if (inserted == NULL) {
        printf("hash malloc failed\n");
        return NULL;
    }

    int front = 0;
    int back = 0;
    queue[0] = start;
    
    hash_table_put(inserted, start, start);
    back++;
    
    void* curr = queue[front++];
    
    while (curr != NULL) {
        void** neighbour = graph_get_neighbours(gr, curr);
        int j = 0;
        void* curr_neigh = neighbour[j];
        while(curr_neigh != NULL) {
            // enqueue
            if(!hash_table_contains_key(inserted,curr_neigh)){
                hash_table_put(inserted, curr_neigh, curr_neigh);
                queue[back++] = curr_neigh;
            }
            curr_neigh = neighbour[j++];
        }
        curr = queue[front++];
    }
    queue[back] = NULL;

    return queue;
}
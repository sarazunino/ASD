# ifndef BFS_H
# define BFS_H

#include "../include/graph.h"
#include "../lib/hash_table.h"

/**
 * @brief           Function that visits a graph breadth-first.
 * 
 * @param gr        Pointer to the graph.
 * @param start     Starting node.
 * @param compare   Pointer to a comparison function.
 *                  Returns negative if first < second, zero if equal, positive if first > second.
 * @param hash      Pointer to a hash function that maps a key to an unsigned long value.
 * 
 * @return          Array of nodes in visiting order, NULL in case of failure.
 */
void **breadth_first_visit(Graph gr,
                            void *start,
                            int (*compare)(const void *, const void *),
                            unsigned long (*hash)(const void *));

# endif
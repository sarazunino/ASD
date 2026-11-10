#include "../include/graph.h"
#include "../lib/hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

# define SUCCESS 1
# define FAILURE 0
# define UNDIRECTED 0
# define DIRECTED 1
# define UNLABELLED 0
# define LABELLED 1

Graph graph_create(int labelled, int directed, int (*compare)(const void *, const void *), unsigned long (*hash)(const void *)) {
    if ((labelled != UNLABELLED && labelled != LABELLED)
        || (directed != DIRECTED && directed != UNDIRECTED)
        || compare == NULL
        || hash == NULL) return NULL;

    Graph graph = (Graph)malloc(sizeof(struct graph));
    if (graph == NULL) return NULL;

    graph->labelled = labelled;
    graph->directed = directed;
    graph->nodes = hash_table_create(compare, hash);
    graph->compare = compare;
    graph->hash = hash;
    if (graph->nodes == NULL) return NULL;
    graph->num_edges = 0;

    return graph;
}

int graph_is_directed(const Graph gr) {
    return gr == NULL ? FAILURE : gr->directed;
}

int graph_is_labelled(const Graph gr) {
    return gr == NULL ? FAILURE : gr->labelled;
}

int graph_add_node(Graph gr, const void *node) {
    if (gr == NULL || gr->nodes == NULL || node == NULL || graph_contains_node(gr, node) == SUCCESS) return FAILURE;
    
    HashTable* new_hash = hash_table_create(gr->compare, gr->hash);
    if (new_hash == NULL) return FAILURE;

    hash_table_put(gr->nodes, node, new_hash);
    return SUCCESS;
}

int graph_contains_node(const Graph gr, const void *node) {
    if (gr == NULL || gr->nodes == NULL || node == NULL) return FAILURE;

    return hash_table_contains_key(gr->nodes, node) ? SUCCESS : FAILURE;
}

int graph_remove_node(Graph gr, const void *node) {
    if (gr == NULL || gr->nodes == NULL || node == NULL || graph_contains_node(gr, node) == FAILURE) return FAILURE;

    gr->num_edges -= hash_table_size(hash_table_get(gr->nodes, node));

    hash_table_free(hash_table_get(gr->nodes, node));
    hash_table_remove(gr->nodes, node);

    Node** keyset = (Node**) hash_table_keyset(gr->nodes);
    if (keyset == NULL) return FAILURE;

    for(int i = 0; i < hash_table_size(gr->nodes); i++) {
        HashTable* h = (HashTable*) hash_table_get(gr->nodes, keyset[i]);
        if (h != NULL && hash_table_contains_key(h, node)) {
            hash_table_remove(h, node);
            gr->num_edges--;
        }
    }

    return SUCCESS;
}

int graph_num_nodes(const Graph gr) {
    if (gr == NULL || gr->nodes == NULL) return FAILURE;

    return hash_table_size(gr->nodes);
}

void **graph_get_nodes(const Graph gr) {
    if (gr == NULL || gr->nodes == NULL) return NULL;
    
    int n = graph_num_nodes(gr);
    if (n == 0) return NULL;
    
    void **values = malloc(sizeof(void *) * n);
    if (values == NULL) return NULL; 
    
    Node** keyset = (Node**) hash_table_keyset(gr->nodes);
    if (keyset == NULL) return NULL;

    int j = 0;

    for (int i = 0; i < hash_table_size(gr->nodes); i++){
        if (keyset[i] != NULL){
            values[j] = keyset[i];
            j++;
        }
    }

    return values;
}

Edge* graph_create_edge(const void* node1, const void* node2) {
    Edge *new_edge = (Edge *)malloc(sizeof(Edge));
    if (new_edge == NULL) return NULL;

    new_edge->source = (void*) node1;
    new_edge->dest = (void*) node2;
    new_edge->label = NULL;

    return new_edge;
}

int graph_add_edge(Graph gr, const void *node1, const void *node2, const void *label) {
    if (gr == NULL || gr->nodes == NULL || node1 == NULL || node2 == NULL
        || (label == NULL && gr->labelled == LABELLED)
        || (label != NULL && gr->labelled == UNLABELLED)
        || graph_contains_node(gr, node1) == FAILURE 
        || graph_contains_node(gr, node2) == FAILURE 
        || graph_contains_edge(gr, node1, node2) == SUCCESS
        || node1 == node2
        ) {
            return FAILURE;
    }

    Edge* new_edge = graph_create_edge(node1, node2);
    if (new_edge == NULL) return FAILURE;
    
    if (gr->labelled != 0) new_edge->label = (void *) label;

    HashTable* h1 = (HashTable*) hash_table_get(gr->nodes, node1);
    if (h1 == NULL) return FAILURE;

    hash_table_put(h1, node2, new_edge);
    if (gr->directed == UNDIRECTED){
        HashTable* h2 = (HashTable*) hash_table_get(gr->nodes, node2);
        if (h2 == NULL) return FAILURE;

        hash_table_put(h2, node1, new_edge);
    }

    gr->num_edges++;

    return SUCCESS;
}

int graph_contains_edge(const Graph gr, const void *node1, const void *node2) {
    if (gr == NULL || node1 == NULL || node2 == NULL) return FAILURE;

    HashTable* h1 = (HashTable*) hash_table_get(gr->nodes, node1);
    if (h1 == NULL) return FAILURE;

    HashTable* h2 = NULL;

    if (gr->directed == UNDIRECTED) {
        h2 = (HashTable*) hash_table_get(gr->nodes, node2);
        return (hash_table_contains_key(h1, node2) && hash_table_contains_key(h2, node1)) ? SUCCESS : FAILURE;
    }
    
    return hash_table_contains_key(h1, node2) ? SUCCESS : FAILURE;
}

int graph_remove_edge(Graph gr, const void *node1, const void *node2) {
    if (gr == NULL || node1 == NULL || node2 == NULL || graph_contains_edge(gr, node1, node2) == FAILURE) return FAILURE;

    HashTable* h1 = (HashTable*) hash_table_get(gr->nodes, node1);
    if (h1 == NULL) return FAILURE;

    hash_table_remove(h1, node2);
    gr->num_edges--;
    HashTable* h2 = NULL;
    
    if (gr->directed == UNDIRECTED) {
        h2 = (HashTable*) hash_table_get(gr->nodes, node2);
        if (h2 == NULL) return FAILURE;

        hash_table_remove(h2, node1);
    }
    
    return SUCCESS;
}

int graph_num_edges(const Graph gr) {
    if (gr == NULL || gr->nodes == NULL) return FAILURE;
    return gr->num_edges;
}

Edge **graph_get_edges(const Graph gr) {
    if (gr == NULL || gr->nodes == NULL || gr->num_edges <= 0) return NULL;
    
    HashTable* edges_hash_table = hash_table_create(gr->compare, gr->hash);
    if (edges_hash_table == NULL) return NULL;

    void** keyset = hash_table_keyset(gr->nodes);
    if (keyset == NULL) return NULL;
    
    for(int i = 0; i < hash_table_size(gr->nodes); i++) {
        HashTable* node_hash_table = (HashTable*) hash_table_get(gr->nodes, keyset[i]);
        if (node_hash_table == NULL) return NULL;
        
        void** nodes_keyset = hash_table_keyset(node_hash_table);
        if (nodes_keyset == NULL) return NULL;
        
        for(int j = 0; j < hash_table_size(node_hash_table); j++){
            Edge* edge = (Edge*)hash_table_get(node_hash_table, nodes_keyset[j]);
            if (edge == NULL) return NULL;
    
            if(!hash_table_contains_key(edges_hash_table, edge)) hash_table_put(edges_hash_table, edge, NULL);
        }
    }

    return (Edge**)hash_table_keyset(edges_hash_table);
}

void *graph_get_label(const Graph gr, const void *node1, const void *node2) {
    if (gr == NULL || gr->nodes == NULL || node1 == NULL || node2 == NULL || gr->labelled == UNLABELLED) return NULL;

    HashTable* h1 = (HashTable*) hash_table_get(gr->nodes, node1);
    if (h1 == NULL) return NULL;

    Edge* e1 = hash_table_get(h1, node2);
    if (e1 == NULL) return NULL;

    if (gr->directed == UNDIRECTED) {
        HashTable* h2 = (HashTable*) hash_table_get(gr->nodes, node2);
        if (h2 == NULL) return NULL;

        Edge* e2 = hash_table_get(h2, node1);
        if (e2 == NULL) return NULL;

        if (strcmp((char*) e1->label, (char*) e2->label) != 0) return NULL;
    }

    return e1->label;
}

void **graph_get_neighbours(const Graph gr, const void *node) {
    if (gr == NULL || node == NULL || graph_contains_node(gr, node) == FAILURE) return NULL;

    HashTable* h = hash_table_get(gr->nodes, node);
    if (h == NULL) return NULL;

    return hash_table_keyset(h);
}

int graph_num_neighbours(const Graph gr, const void *node) {
    if (gr == NULL || node == NULL || graph_contains_node(gr, node) == FAILURE) return FAILURE;

    HashTable* h = hash_table_get(gr->nodes, node);
    if (h == NULL) return FAILURE;

    return hash_table_size(h);
}

void graph_free(Graph gr) {
    if (gr != NULL) {
       
        void** keyset = hash_table_keyset(gr->nodes);
        
        for(int i = 0; i < hash_table_size(gr->nodes); i++) {
            HashTable* h = (HashTable*) hash_table_get(gr->nodes, keyset[i]);
            
            hash_table_free(h);
        }
        hash_table_free(gr->nodes);
        free(gr);
    }
}
# ifndef GRAPH_H
# define GRAPH_H

# include "../lib/hash_table.h"
# define SUCCESS 1
# define FAILURE 0
# define UNDIRECTED 0
# define DIRECTED 1
# define UNLABELLED 0
# define LABELLED 1

/**
 * @brief Structure representing a graph.
 */
typedef struct graph {
    int labelled;                               /**< Indicator of whether the graph's edges are labelled or not. */
    int directed;                               /**< Indicator of whether the graph's edges are directed or not. */
    HashTable* nodes;                           /**< HashTable containing the graph nodes. */
    int (*compare)(const void*, const void*);   /**< Pointer to a function that compares two values. */
    unsigned long (*hash)(const void*);         /**< Pointer to a hash function. */
    int num_edges;                              /**< Number of edges contained in the graph. */
} *Graph;

/**
 * @brief Structure representing an edge in a graph.
 */
typedef struct edge {
    void* source;   /**< Source node. */
    void* dest;     /**< Destination node. */
    void* label;    /**< Edge label. */
} Edge;

/**
 * @brief           Function that creates a graph.
 * 
 * @param labelled  1 if the graph is labelled, 0 otherwise.
 * @param directed  1 if the graph is directed, 0 otherwise.
 * @param compare   Pointer to a comparison function.
 *                  Returns negative if first < second, zero if equal, positive if first > second.
 * @param hash      Pointer to a hash function that maps a key to an unsigned long value.
 * 
 * @return          Pointer to the new graph in case of success, NULL otherwise.
 */
Graph graph_create(int labelled,
                  int directed,
                  int (*compare)(const void*, const void*),
                  unsigned long (*hash)(const void*));

/**
 * @brief       Function that tells if a graph is directed or not.
 * 
 * @param gr    Pointer to the graph.
 * 
 * @return      1 if the graph is directed, 0 otherwise
 */
int graph_is_directed(const Graph gr);

/**
 * @brief       Function that tells if a graph is labelled or not.
 * 
 * @param gr    Pointer to the graph.
 * 
 * @return      1 if the graph is labelled, 0 otherwise
 */
int graph_is_labelled(const Graph gr);

/**
 * @brief       Function that adds a node to the graph.
 * 
 * If the node is already present, the function does nothing.
 * 
 * @param gr    Pointer to the graph.
 * @param node  Pointer to the new node.
 * 
 * @return      1 if the insertion succeeds, 0 if it fails or if the node is already present.
 */
int graph_add_node(Graph gr, const void* node);

/**
 * @brief       Function that checks whether the graph contains a node or not.
 * 
 * @param gr    Pointer to the graph.
 * @param node  Pointer to the node to be checked.
 * 
 * @return      1 if the graph contains the node, 0 otherwise.
 */
int graph_contains_node(const Graph gr, const void* node);

/**
 * @brief       Function that removes a node from the graph.
 * 
 * If the node isn't in the graph, the function does nothing.
 * 
 * @param gr    Pointer to the graph.
 * @param node  Pointer to the node to be removed.
 * 
 * @return      1 if the removal succeeds, 0 if it fails or if the node isn't in the graph.
 */
int graph_remove_node(Graph gr, const void* node);

/**
 * @brief       Function that returns the number of nodes contained in the graph.
 * 
 * @param gr    Pointer to the graph.
 * 
 * @return      Number of nodes contained in the graph, 0 in case the graph doesn't exist.
 */
int graph_num_nodes(const Graph gr);

/**
 * @brief       Function that returns the array of nodes contained in the graph.
 * 
 * @param gr    Pointer to the graph.
 * 
 * @return      Array of nodes contained in the graph, NULL if the function fails.
 */
void** graph_get_nodes(const Graph gr);

/**
 * @brief           Function that adds an edge to the graph.
 * 
 * If the graph contains the edge already, the function does nothing.
 * 
 * @param gr        Pointer to the graph.
 * @param node1     Pointer to the edge's source node.
 * @param node2     Pointer to the edge's destination node.
 * @param label     Pointer to the edge's label (should be NULL when gr is unlabelled).
 * 
 * @return          1 if the insertion succeeds, 0 otherwise.
 */
int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label);

/**
 * @brief           Function that checks whether an edge is contained in the graph or not.
 * 
 * @param gr        Pointer to the graph.
 * @param node1     Pointer to the edge's source node.
 * @param node2     Pointer to the edge's destination node.
 * 
 * @return          1 if the graph contains the edge, 0 otherwise.
 */
int graph_contains_edge(const Graph gr, const void* node1, const void* node2);

/**
 * @brief           Function that removes an edge from the graph.
 * 
 * If the edge isn't present, the function does nothing.
 * 
 * @param gr        Pointer to the graph.
 * @param node1     Pointer to the edge's source node.
 * @param node2     Pointer to the edge's destination node.
 * 
 * @return          1 if the removel succeeds, 0 otherwise.
 */
int graph_remove_edge(Graph gr, const void* node1, const void* node2);

/**
 * @brief       Function that returns the number of edges in a graph.
 * 
 * @param gr    Pointer to the graph.
 * 
 * @return      Number of edges contained in the graph, 0 in case the graph doesn't exist.
 */
int graph_num_edges(const Graph gr);

/**
 * @brief       Function that returns the array of edges contained in the graph.
 * 
 * @param gr    Pointer to the graph.
 * 
 * @return      Array of edges contained in the graph, NULL if the function fails.
 */
Edge** graph_get_edges(const Graph gr);

/**
 * @brief           Function that returns an edge's label
 * 
 * @param gr        Pointer to the graph.
 * @param node1     Pointer to the edge's source node.
 * @param node2     Pointer to the edge's destination node.
 * 
 * @return          Pointer to the label in case of success, 0 otherwise or if the graph is unlabelled.
 */
void* graph_get_label(const Graph gr, const void* node1, const void* node2);

/**
 * @brief       Function that returns an array containing a node's neighbours.
 * 
 * @param gr    Pointer to the graph.
 * @param node  Pointer to the node.
 * 
 * @return      Array containing the given node's neighbour nodes, NULL if the node doesn't exist or has no neighbours.
 */
void** graph_get_neighbours(const Graph gr, const void* node);

/**
 * @brief       Function that returns the number of node's neighbours.
 * 
 * @param gr    Pointer to the graph.
 * @param node  Pointer to the node.
 * 
 * @return      Amount of the given node's neighbour nodes, 0 if the node doesn't exist.
 */
int graph_num_neighbours(const Graph gr, const void* node);

/**
 * @brief       Function that frees a graph and its internal structures.
 * 
 * @param gr    Graph to be freed.
 */
void graph_free(Graph gr);

/**
 * @brief Helper function that creates an edge.
 * 
 * @param node1     Pointer to the edge's source node.
 * @param node2     Pointer to the edge's destination node.
 * 
 * @returns Pointer to the new edge.
 */
Edge* graph_create_edge(const void* node1, const void* node2);

# endif
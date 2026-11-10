#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../include/graph.h"
# define END_TEST -999

int compare(const void *a_ptr, const void *b_ptr) {
    int a = *(int *)a_ptr;
    int b = *(int *)b_ptr;
    return a == b ? 0 : (a - b) / (abs(a - b));
}

unsigned long hash(const void *ptr) {
    unsigned long x = (unsigned long)(unsigned long *)ptr;

    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;

    return (unsigned long)x;
}

void test(int cond) {
    printf("%s\n", cond ? "TEST SUCCESS" : "TEST FAILED");
}

void test_graph_create() {
    Graph g_ul_ud = NULL, g_ul_d = NULL, g_l_ud = NULL, g_l_d = NULL, g5 = NULL, g6 = NULL;

    test((g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash)) != NULL);
    test((g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash)) != NULL);
    test((g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash)) != NULL);
    test((g_l_d = graph_create(LABELLED, DIRECTED, compare, hash)) != NULL);
    test((g5 = graph_create(-1, 634879534, compare, hash)) == NULL);
    test((g6 = graph_create(UNLABELLED, UNDIRECTED, NULL, NULL)) == NULL);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
    graph_free(g5);
    graph_free(g6);
}

void test_graph_is_directed() {
    test(graph_is_directed(graph_create(UNLABELLED, UNDIRECTED, compare, hash)) == UNDIRECTED);
    test(graph_is_directed(graph_create(UNLABELLED, DIRECTED, compare, hash)) == DIRECTED);
    test(graph_is_directed(graph_create(LABELLED, UNDIRECTED, compare, hash)) == UNDIRECTED);
    test(graph_is_directed(graph_create(LABELLED, DIRECTED, compare, hash)) == DIRECTED);
    test(graph_is_directed(NULL) == FAILURE);
}

void test_graph_is_labelled() {
    test(graph_is_labelled(graph_create(UNLABELLED, UNDIRECTED, compare, hash)) == UNLABELLED);
    test(graph_is_labelled(graph_create(UNLABELLED, DIRECTED, compare, hash)) == UNLABELLED);
    test(graph_is_labelled(graph_create(LABELLED, UNDIRECTED, compare, hash)) == LABELLED);
    test(graph_is_labelled(graph_create(LABELLED, DIRECTED, compare, hash)) == LABELLED);
    test(graph_is_labelled(NULL) == FAILURE);
}

void test_graph_add_node() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    test(graph_add_node(NULL, NULL) == FAILURE);
    test(graph_add_node(g_ul_ud, NULL) == FAILURE);
    test(graph_add_node(g_ul_d, NULL) == FAILURE);
    test(graph_add_node(g_l_ud, NULL) == FAILURE);
    test(graph_add_node(g_l_d, NULL) == FAILURE);

    int a = 5;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_add_node(g_ul_ud, &a) == SUCCESS);
    test(graph_add_node(g_ul_ud, &b) == SUCCESS);
    test(graph_add_node(g_ul_d, &a) == SUCCESS);
    test(graph_add_node(g_ul_d, &c) == SUCCESS);
    test(graph_add_node(g_l_ud, &b) == SUCCESS);
    test(graph_add_node(g_l_ud, &d) == SUCCESS);
    test(graph_add_node(g_l_d, &c) == SUCCESS);
    test(graph_add_node(g_l_d, &d) == SUCCESS);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_contains_node() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 5;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_contains_node(NULL, NULL) == FAILURE);
    test(graph_contains_node(NULL, &a) == FAILURE);
    test(graph_contains_node(g_ul_ud, NULL) == FAILURE);
    test(graph_contains_node(g_ul_d, NULL) == FAILURE);
    test(graph_contains_node(g_l_ud, NULL) == FAILURE);
    test(graph_contains_node(g_l_d, NULL) == FAILURE);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    test(graph_contains_node(g_ul_ud, &a) == SUCCESS);
    test(graph_contains_node(g_ul_ud, &d) == FAILURE);
    test(graph_contains_node(g_ul_d, &a) == SUCCESS);
    test(graph_contains_node(g_ul_d, &d) == FAILURE);
    test(graph_contains_node(g_l_ud, &b) == SUCCESS);
    test(graph_contains_node(g_l_ud, &c) == FAILURE);
    test(graph_contains_node(g_l_d, &c) == SUCCESS);
    test(graph_contains_node(g_l_d, &b) == FAILURE);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_remove_node() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 5;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_remove_node(NULL, NULL) == FAILURE);
    test(graph_remove_node(NULL, &a) == FAILURE);
    test(graph_remove_node(g_ul_ud, NULL) == FAILURE);
    test(graph_remove_node(g_ul_d, NULL) == FAILURE);
    test(graph_remove_node(g_l_ud, NULL) == FAILURE);
    test(graph_remove_node(g_l_d, NULL) == FAILURE);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    graph_remove_node(g_ul_ud, &a);
    graph_remove_node(g_ul_d, &a);
    graph_remove_node(g_l_ud, &b);
    graph_remove_node(g_l_d, &c);

    test(graph_contains_node(g_ul_ud, &a) == FAILURE);
    test(graph_contains_node(g_ul_d, &a) == FAILURE);
    test(graph_contains_node(g_l_ud, &b) == FAILURE);
    test(graph_contains_node(g_l_d, &c) == FAILURE);

    test(graph_remove_node(g_ul_ud, &d) == FAILURE);
    test(graph_remove_node(g_ul_d, &d) == FAILURE);
    test(graph_remove_node(g_l_ud, &c) == FAILURE);
    test(graph_remove_node(g_l_d, &b) == FAILURE);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_num_nodes() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 5;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_num_nodes(NULL) == FAILURE);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    test(graph_num_nodes(g_ul_ud) == 2);
    test(graph_num_nodes(g_ul_d) == 2);
    test(graph_num_nodes(g_l_ud) == 2);
    test(graph_num_nodes(g_l_d) == 2);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    test(graph_num_nodes(g_ul_ud) == 2);
    test(graph_num_nodes(g_ul_d) == 2);
    test(graph_num_nodes(g_l_ud) == 2);
    test(graph_num_nodes(g_l_d) == 2);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_get_nodes() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 5;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_get_nodes(NULL) == NULL);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    test(graph_get_nodes(g_ul_ud) != NULL);
    test(graph_get_nodes(g_ul_d) != NULL);
    test(graph_get_nodes(g_l_ud) != NULL);
    test(graph_get_nodes(g_l_d) != NULL);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void show_test_group(int tests[], char *group_name) {
    int i = 0;
    while (tests[i])
    {
        printf("%s %d:", group_name, i);
        test(tests[i++]);
    }
}

void test_graph_add_edge() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 6348796;
    int b = -2;
    int c = 7;
    int d = 10;
    int e = 0;
    int f = -8;
    int g = 22;
    int h = -14;

    // int tests_normal[] = {
    //     (graph_add_edge(NULL, &a, &c, "label") == FAILURE),
    //     (graph_add_edge(g_ul_ud, NULL, &a, "label") == FAILURE),
    //     (graph_add_edge(g_ul_ud, &a, NULL, "label") == FAILURE),
    //     (graph_add_edge(g_ul_ud, &a, &b, NULL) == FAILURE),
    //     (graph_add_edge(g_ul_ud, &b, &a, NULL) == FAILURE),
    //     (graph_add_edge(g_ul_d, NULL, &a, "label") == FAILURE),
    //     (graph_add_edge(g_ul_d, &a, NULL, "label") == FAILURE),
    //     (graph_add_edge(g_ul_d, &a, &c, NULL) == FAILURE),
    //     (graph_add_edge(g_ul_d, &c, &a, NULL) == FAILURE),
    //     (graph_add_edge(g_l_ud, NULL, &b, "label") == FAILURE),
    //     (graph_add_edge(g_l_ud, &b, NULL, "label") == FAILURE),
    //     (graph_add_edge(g_l_ud, &b, &d, NULL) == FAILURE),
    //     (graph_add_edge(g_l_ud, &d, &b, NULL) == FAILURE),
    //     (graph_add_edge(g_l_d, NULL, &c, "label") == FAILURE),
    //     (graph_add_edge(g_l_d, &c, NULL, "label") == FAILURE),
    //     (graph_add_edge(g_l_d, &c, &d, NULL) == FAILURE),
    //     (graph_add_edge(g_l_d, &d, &c, NULL) == FAILURE),
    //     END_TEST};

    // show_test_group(tests_normal, "test1");

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_ud, &c);
    graph_add_node(g_ul_ud, &d);
    graph_add_node(g_ul_ud, &e);
    graph_add_node(g_ul_ud, &f);
    graph_add_node(g_ul_ud, &g);
    graph_add_node(g_ul_ud, &h);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    int tests2[] = {
        (graph_add_edge(g_ul_ud, &a, &b, NULL) == SUCCESS),
        (graph_add_edge(g_ul_ud, &c, &d, NULL) == SUCCESS),
        (graph_add_edge(g_ul_ud, &e, &f, NULL) == SUCCESS),
        (graph_add_edge(g_ul_ud, &g, &h, NULL) == SUCCESS),
        (graph_add_edge(g_ul_ud, &a, &b, NULL) == FAILURE),
        (graph_add_edge(g_ul_ud, &c, &a, NULL) == SUCCESS),
        (graph_add_edge(g_ul_ud, &b, &b, NULL) == FAILURE),
        (graph_add_edge(g_ul_ud, &d, &b, NULL) == SUCCESS),
        (graph_add_edge(g_ul_ud, &b, &d, NULL) == FAILURE),
        (graph_add_edge(g_ul_ud, &d, &b, NULL) == FAILURE),
        (graph_add_edge(g_ul_ud, &d, &a, NULL) == SUCCESS),
        (graph_add_edge(g_ul_ud, &d, &c, NULL) == FAILURE),
        (graph_add_edge(g_ul_ud, &a, &b, "label") == FAILURE),
        (graph_add_edge(g_ul_d, &a, &c, NULL) == SUCCESS),
        (graph_add_edge(g_ul_d, &a, &c, "label") == FAILURE),
        (graph_add_edge(g_l_ud, &b, &d, NULL) == FAILURE),
        (graph_add_edge(g_l_ud, &b, &d, "label") == SUCCESS),
        (graph_add_edge(g_l_d, &c, &d, NULL) == FAILURE),
        (graph_add_edge(g_l_d, &c, &d, "label") == SUCCESS),
        END_TEST};

    show_test_group(tests2, NULL);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_contains_edge() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 50;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_contains_edge(NULL, &a, &b) == FAILURE);
    test(graph_contains_edge(g_ul_ud, NULL, &b) == FAILURE);
    test(graph_contains_edge(g_ul_ud, &a, NULL) == FAILURE);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    test(graph_contains_edge(g_ul_ud, &a, &b) == FAILURE);
    test(graph_contains_edge(g_ul_d, &a, &c) == FAILURE);
    test(graph_contains_edge(g_l_ud, &b, &d) == FAILURE);
    test(graph_contains_edge(g_l_d, &c, &d) == FAILURE);

    graph_add_edge(g_ul_ud, &a, &b, NULL);
    graph_add_edge(g_ul_d, &a, &c, NULL);
    graph_add_edge(g_l_ud, &b, &d, "label");
    graph_add_edge(g_l_d, &c, &d, "label");

    test(graph_contains_edge(g_ul_ud, &a, &b) == SUCCESS);
    test(graph_contains_edge(g_ul_d, &a, &c) == SUCCESS);
    test(graph_contains_edge(g_l_ud, &b, &d) == SUCCESS);
    test(graph_contains_edge(g_l_d, &c, &d) == SUCCESS);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_remove_edge() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 50;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_remove_edge(NULL, &a, &c) == FAILURE);
    test(graph_remove_edge(g_ul_ud, NULL, &c) == FAILURE);
    test(graph_remove_edge(g_ul_ud, &a, NULL) == FAILURE);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    test(graph_remove_edge(g_ul_ud, &a, &b) == FAILURE);
    test(graph_remove_edge(g_ul_d, &a, &c) == FAILURE);
    test(graph_remove_edge(g_l_ud, &b, &d) == FAILURE);
    test(graph_remove_edge(g_l_d, &c, &d) == FAILURE);

    graph_add_edge(g_ul_ud, &a, &b, NULL);
    graph_add_edge(g_ul_d, &a, &c, NULL);
    graph_add_edge(g_l_ud, &b, &d, "label");
    graph_add_edge(g_l_d, &c, &d, "label");

    test(graph_remove_edge(g_ul_ud, &a, &b) == SUCCESS);
    test(graph_remove_edge(g_ul_d, &a, &c) == SUCCESS);
    test(graph_remove_edge(g_l_ud, &b, &d) == SUCCESS);
    test(graph_remove_edge(g_l_d, &c, &d) == SUCCESS);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_num_edges() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 50;
    int b = -2;
    int c = 7;
    int d = 10;
    

    test(graph_num_edges(NULL) == FAILURE);
    test(graph_num_edges(g_ul_ud) == 0);
    test(graph_num_edges(g_ul_d) == 0); 
    test(graph_num_edges(g_l_ud) == 0);
    test(graph_num_edges(g_l_d) == 0);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    test(graph_num_edges(g_ul_ud) == 0);
    test(graph_num_edges(g_ul_d) == 0);
    test(graph_num_edges(g_l_ud) == 0);
    test(graph_num_edges(g_l_d) == 0);

    graph_add_edge(g_ul_ud, &a, &b, NULL);
    graph_add_edge(g_ul_d, &a, &c, NULL);
    graph_add_edge(g_l_ud, &b, &d, "label");
    graph_add_edge(g_l_d, &c, &d, "label");

    test(graph_num_edges(g_ul_ud) == 1);
    test(graph_num_edges(g_ul_d) == 1);
    test(graph_num_edges(g_l_ud) == 1);
    test(graph_num_edges(g_l_d) == 1);

    graph_remove_edge(g_ul_ud, &a, &b);
    graph_remove_edge(g_ul_d, &a, &c);
    graph_remove_edge(g_l_ud, &b, &d);
    graph_remove_edge(g_l_d, &c, &d);

    test(graph_num_edges(g_ul_ud) == 0);
    test(graph_num_edges(g_ul_d) == 0);
    test(graph_num_edges(g_l_ud) == 0);
    test(graph_num_edges(g_l_d) == 0);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_get_edges() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 50;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_get_edges(NULL) == NULL);
    test(graph_get_edges(g_ul_ud) == NULL);
    test(graph_get_edges(g_ul_d) == NULL);
    test(graph_get_edges(g_l_ud) == NULL);
    test(graph_get_edges(g_l_d) == NULL);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    graph_add_edge(g_ul_ud, &a, &b, NULL);
    graph_add_edge(g_ul_d, &a, &c, NULL);
    graph_add_edge(g_l_ud, &b, &d, "label");
    graph_add_edge(g_l_d, &c, &d, "label");

    test(graph_get_edges(g_ul_ud) != NULL);
    test(graph_get_edges(g_ul_d) != NULL);
    test(graph_get_edges(g_l_ud) != NULL);
    test(graph_get_edges(g_l_d) != NULL);

    graph_remove_edge(g_ul_ud, &a, &b);
    graph_remove_edge(g_ul_d, &a, &c);
    graph_remove_edge(g_l_ud, &b, &d);
    graph_remove_edge(g_l_d, &c, &d);

    test(graph_get_edges(g_ul_ud) == NULL);
    test(graph_get_edges(g_ul_d) == NULL);
    test(graph_get_edges(g_l_ud) == NULL);
    test(graph_get_edges(g_l_d) == NULL);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_get_label() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 50;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_get_label(NULL, &a, &b) == NULL);
    test(graph_get_label(g_ul_ud, NULL, &b) == NULL);
    test(graph_get_label(g_ul_ud, &a, NULL) == NULL);
    test(graph_get_label(g_ul_d, NULL, &c) == NULL);
    test(graph_get_label(g_ul_d, &a, NULL) == NULL);
    test(graph_get_label(g_l_ud, NULL, &d) == NULL);
    test(graph_get_label(g_l_ud, &b, NULL) == NULL);
    test(graph_get_label(g_l_d, NULL, &d) == NULL);
    test(graph_get_label(g_l_d, &c, NULL) == NULL);

    test(graph_get_label(g_ul_ud, &a, &b) == NULL);
    test(graph_get_label(g_ul_d, &a, &c) == NULL);
    test(graph_get_label(g_l_ud, &b, &d) == NULL);
    test(graph_get_label(g_l_d, &c, &d) == NULL);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    graph_add_edge(g_ul_ud, &a, &b, NULL);
    graph_add_edge(g_ul_d, &a, &c, NULL);
    graph_add_edge(g_l_ud, &b, &d, "label");
    graph_add_edge(g_l_d, &c, &d, "label");

    test(graph_get_label(g_ul_ud, &a, &b) == NULL);
    test(graph_get_label(g_ul_d, &a, &c) == NULL);
    test(graph_get_label(g_l_ud, &b, &d) == "label"); // issue here
    test(graph_get_label(g_l_d, &c, &d) == "label");

    graph_remove_edge(g_ul_ud, &a, &b);
    graph_remove_edge(g_ul_d, &a, &c);
    graph_remove_edge(g_l_ud, &b, &d);
    graph_remove_edge(g_l_d, &c, &d);

    test(graph_get_label(g_ul_ud, &a, &b) == NULL);
    test(graph_get_label(g_ul_d, &a, &c) == NULL);
    test(graph_get_label(g_l_ud, &b, &d) == NULL);
    test(graph_get_label(g_l_d, &c, &d) == NULL);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_get_neighbours() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 50;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_get_neighbours(NULL, &a) == NULL);
    test(graph_get_neighbours(g_ul_ud, &a) == NULL);
    test(graph_get_neighbours(g_ul_d, &a) == NULL);
    test(graph_get_neighbours(g_l_ud, &a) == NULL);
    test(graph_get_neighbours(g_l_d, &a) == NULL);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);
    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);
    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);

    graph_add_edge(g_ul_ud, &a, &b, NULL);
    graph_add_edge(g_ul_d, &a, &c, NULL);
    graph_add_edge(g_l_ud, &b, &d, "label");
    graph_add_edge(g_l_d, &c, &d, "label");

    test(graph_get_neighbours(g_ul_ud, &a) != NULL);
    test(graph_get_neighbours(g_ul_ud, &b) != NULL);
    test(graph_get_neighbours(g_ul_d, &a) != NULL);
    test(graph_get_neighbours(g_ul_d, &c) != NULL);
    test(graph_get_neighbours(g_l_ud, &b) != NULL);
    test(graph_get_neighbours(g_l_ud, &d) != NULL);
    test(graph_get_neighbours(g_l_d, &c) != NULL);
    test(graph_get_neighbours(g_l_d, &d) != NULL);

    graph_remove_edge(g_ul_ud, &a, &b);
    graph_remove_edge(g_ul_d, &a, &c);
    graph_remove_edge(g_l_ud, &b, &d);
    graph_remove_edge(g_l_d, &c, &d);

    test(graph_get_neighbours(g_ul_ud, &a) != NULL);
    test(graph_get_neighbours(g_ul_d, &a) != NULL);
    test(graph_get_neighbours(g_l_ud, &b) != NULL);
    test(graph_get_neighbours(g_l_d, &c) != NULL);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

void test_graph_num_neighbours() {
    Graph g_ul_ud = graph_create(UNLABELLED, UNDIRECTED, compare, hash);
    Graph g_ul_d = graph_create(UNLABELLED, DIRECTED, compare, hash);
    Graph g_l_ud = graph_create(LABELLED, UNDIRECTED, compare, hash);
    Graph g_l_d = graph_create(LABELLED, DIRECTED, compare, hash);

    int a = 50;
    int b = -2;
    int c = 7;
    int d = 10;

    test(graph_num_neighbours(NULL, &a) == FAILURE);
    test(graph_num_neighbours(g_ul_ud, NULL) == FAILURE);
    test(graph_num_neighbours(g_ul_ud, &a) == FAILURE);
    test(graph_num_neighbours(g_ul_d, &a) == FAILURE);
    test(graph_num_neighbours(g_l_ud, &a) == FAILURE);
    test(graph_num_neighbours(g_l_d, &a) == FAILURE);

    graph_add_node(g_ul_ud, &a);
    graph_add_node(g_ul_ud, &b);
    graph_add_node(g_ul_ud, &c);
    graph_add_node(g_ul_ud, &d);

    graph_add_node(g_ul_d, &a);
    graph_add_node(g_ul_d, &c);

    graph_add_node(g_l_ud, &b);
    graph_add_node(g_l_ud, &d);

    graph_add_node(g_l_d, &c);
    graph_add_node(g_l_d, &d);
    graph_add_node(g_l_d, &a);

    graph_add_edge(g_ul_ud, &a, &b, NULL);
    // printf("\n\n%d\t\n",graph_num_neighbours(g_ul_ud, &a) );
    // print_hash("edges", g_ul_ud->edges, printEdge);

    graph_add_edge(g_ul_ud, &c, &a, NULL);
    // printf("\n\n%d\t\n",graph_num_neighbours(g_ul_ud, &a) );
    // print_hash("edges", g_ul_ud->edges, printEdge);

    graph_add_edge(g_ul_ud, &a, &d, NULL);
    // printf("\n\n%d\t\n",graph_num_neighbours(g_ul_ud, &a) );
    // print_hash("edges", g_ul_ud->edges, printEdge);

    graph_add_edge(g_ul_d, &a, &c, NULL);

    graph_add_edge(g_l_ud, &b, &d, "label");

    graph_add_edge(g_l_d, &c, &d, "label");
    graph_add_edge(g_l_d, &a, &d, "label");
    graph_add_edge(g_l_d, &d, &a, "label");

    // printf("\n\n\n");
    test(graph_num_neighbours(g_ul_ud, &a) == 3);

    test(graph_num_neighbours(g_ul_d, &c) == 0);
    // printf("%d\t",graph_num_neighbours(g_ul_d, &a) );
    // print_hash("ul ud", g_ul_d->adjacent, printAdj);

    test(graph_num_neighbours(g_l_ud, &b) == 1);
    // printf("%d\t",graph_num_neighbours(g_l_ud, &a) );
    // print_hash("ul ud", g_l_ud->adjacent, printAdj);

    test(graph_num_neighbours(g_l_d, &d) == 1);
    // printf("%d\t",graph_num_neighbours(g_l_d, &a) );
    // print_hash("ul ud", g_l_d->adjacent, printAdj);

    graph_remove_edge(g_ul_ud, &a, &b);
    graph_remove_edge(g_ul_ud, &a, &c);
    graph_remove_edge(g_ul_ud, &a, &d);
    graph_remove_edge(g_ul_d, &a, &c);
    graph_remove_edge(g_l_ud, &b, &d);
    graph_remove_edge(g_l_d, &c, &d);
    graph_remove_edge(g_l_d, &a, &d);
    graph_remove_edge(g_l_d, &d, &a);

    test(graph_num_neighbours(g_ul_ud, &a) == 0);
    test(graph_num_neighbours(g_ul_d, &c) == 0);
    test(graph_num_neighbours(g_l_ud, &b) == 0);
    test(graph_num_neighbours(g_l_d, &d) == 0);

    graph_free(g_ul_ud);
    graph_free(g_ul_d);
    graph_free(g_l_ud);
    graph_free(g_l_d);
}

typedef struct test_struct {
    char *name;
    void (*test)();
    int show;
} TestStruct;

int main(void) {
    TestStruct tests[] = {
        {"create", test_graph_create, 1},
        {"directed", test_graph_is_directed, 1},
        {"labelled", test_graph_is_labelled, 1},
        {"added_node", test_graph_add_node, 1},
        {"contains_node", test_graph_contains_node, 1},
        {"remove_node", test_graph_remove_node, 1},
        {"num_nodes", test_graph_num_nodes, 1},
        {"get_nodes", test_graph_get_nodes, 1},
        {"add_edge", test_graph_add_edge, 1},
        {"contains_edge", test_graph_contains_edge, 1},
        {"remove_edge", test_graph_remove_edge, 1},
        {"num_edges", test_graph_num_edges, 1},
        {"get_edges", test_graph_get_edges, 1},
        {"get_label", test_graph_get_label, 1},
        {"get_neighbours", test_graph_get_neighbours, 1},
        {"num_neighbours", test_graph_num_neighbours, 1},
        {NULL, NULL, 1}
    };

    int i = 0;
    TestStruct curr = tests[i];
    while (curr.name != NULL) {
        if (curr.show) {
            printf("%s\n", curr.name);
            curr.test();
        }
        curr = tests[++i];
    }
    
}
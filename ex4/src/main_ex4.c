# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
# include "../include/graph.h"
# include "../include/bfs.h"

int compare(const void *a_ptr, const void *b_ptr) {
    char* a = (char *)a_ptr;
    char* b = (char *)b_ptr;
    int res = strcmp(a,b);
    return res;
}

unsigned long hash(const void *ptr) {

    unsigned char* str = (unsigned char*) ptr;
    unsigned long hash = 5381;
    int c;
    while((c = *str++)){
        hash = ((hash << 5) + hash) +c;
    }

    return hash;
}

void printNode(void* node){
    if(node == NULL){ 
        printf("Null node\n");
        return;
    }
    
    printf("%s: ", (char*)node);
}

void printEdge(void* edge){
    if(edge == NULL){
        printf("NULL EDGE");
        return;
    }

    Edge* e = (Edge*)edge;
    printNode(e->source);
    printf(" - ");
    printNode(e->dest);
    printf(" as %f",*(float*)e->label);
}

void print_hash(HashTable* h){
    Node** keyset = (Node**) hash_table_keyset(h);
    for(int i = 0; i < hash_table_size(h); i++) {
        printf("\t\t(");
        printEdge(hash_table_get(h, keyset[i]));
        printf("\n");

    }   
    
}

void print_graph(Graph gr){
    if(gr == NULL || gr->nodes == NULL){ 
        printf("\nNUll graph\n");
        return;
    }
    printf("\n(n:%d, e:%d){\n", graph_num_nodes(gr), gr->num_edges);
     Node** keyset = (Node**) hash_table_keyset(gr->nodes);
     for(int i = 0; i < hash_table_size(gr->nodes); i++) {
        
        HashTable* h = (HashTable*) hash_table_get(gr->nodes, keyset[i]);
        if (h != NULL) {
             printf("   ");
            printNode(keyset[i]);
            printf("(%d)[\n", hash_table_size(h));

            print_hash(h);
            printf("]\n");
        }
        
    }
    printf("}\n");
    fflush(stdout); // removing this causes seg fault
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("input error. please insert: file_name input_file starting_node output_file\n");
        return -1;
    }
    
    FILE* f = fopen(argv[1], "r");
    if (f == NULL) return -1;

    char base[1000];
    char* rest = base;

    int i = 0;
    Graph gr = graph_create(LABELLED, UNDIRECTED, compare, hash);
    int keep = 1;

    while(keep != EOF ){
        char* place1 = (char*) malloc(sizeof(char) * 100);
        char* place2 = (char*) malloc(sizeof(char) * 100);
        float* distance = (float*)malloc(sizeof(float));
        keep = fscanf(f, "%[^,],%[^,],%f\n", place1, place2, distance);

        graph_add_node(gr, place1);
        graph_add_node(gr, place2);
        graph_add_edge(gr, place1, place2, distance);
    }

    fclose(f);

    void** res = breadth_first_visit(gr, argv[2], compare, hash);

    graph_free(gr);

    FILE* f_out = fopen(argv[3], "w");
    if (f_out == NULL) return -1;

    int j = 0;
    while (res[j] != NULL) {
        fprintf(f_out, "%s\n", res[j]);
        j++;
    }

    fclose(f_out);
    return 0;

}
#include "priority_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef struct {
    int id;
    int start;
    int length;
    int priority;
} Task;

static int compare(const void* a_ptr, const void* b_ptr) {
    if (a_ptr == b_ptr) return 0;  

    const Task *a = (const Task *)a_ptr;
    const Task *b = (const Task *)b_ptr;

    if (a->priority != b->priority) {
        return a->priority > b->priority ? 1 : -1;
    }
    
    if(a->start != b->start) {
        return a->start < b->start ? 1 : -1;
    }

    return a->id - b->id;
}

static unsigned long hash(const void* elem) {
    return (unsigned long)(uintptr_t)elem;
}

static int read_tasks(FILE *file, Task *task) {
    if(!file || !task) return -1;

    char line[256];
    while(fgets(line, sizeof(line), file)) {
        if (line[0] != '\n' && line[0] != '\r' && sscanf(line, "%d,%d,%d,%d", 
            &task->id, &task->start,  &task->length, &task->priority) == 4) {
            return 1;
        }
    }
    return feof(file) ? 0 : -1;
}

static Task *load_tasks(FILE *file, int *count) {
    if (!file) return NULL;

    int cap = 64, n = 0;
    Task *tasks = malloc((unsigned)cap * sizeof(Task));
    if (!tasks) return NULL;
 
    Task t;
    while (read_tasks(file, &t)) {
        if (n == cap) {
            cap *= 2;
            Task *tmp = realloc(tasks, (unsigned)cap * sizeof(Task));
            if (!tmp) { free(tasks); return NULL; }
            tasks = tmp;
        }
        tasks[n++] = t;
    }
 
    *count = n;
    return tasks;
}

static void write_results(FILE *file, const int *id, const int *start_time, const int *end_time, int n) {
    fprintf(file, "ID, StartTime, EndTime\n");
    for (int i = 0; i < n; i++) {
        fprintf(file, "%d, %d, %d\n", id[i], start_time[i], end_time[i]);
    }
}

static void scheduling(Task *tasks, int n, int *id, int *start_time, int *end_time) {
    PriorityQueue *pq = priority_queue_create(compare, hash);
    if (!pq) {
        fprintf(stderr, "Failed to create priority queue\n");
        return;
    }

    int t = 0, next = 0, done = 0;

    while (done < n) {
        while(next < n && tasks[next].start <= t) {
            priority_queue_push(pq, &tasks[next]);
            next++;
        }

        if (priority_queue_size(pq) == 0) {
            t++;
        } else {
            Task *chosen = (Task *)priority_queue_top(pq);
            priority_queue_pop(pq);

            id[done] = chosen->id;
            start_time[done] = t;
            end_time[done] = t + chosen->length;
            t = end_time[done];
            done++;
        }
    }
    priority_queue_free(pq);
}

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if(!in) { 
        fprintf(stderr, "Cannot open input file '%s'\n", argv[1]);
        return 1;
    }
    FILE *out = fopen(argv[2], "w");
    if(!out) { 
        fprintf(stderr, "Cannot open output file '%s'\n", argv[2]);
        return 1;
    }

    int n = 0;
    Task *tasks = load_tasks(in, &n);
    fclose(in);

    if (!tasks) {
        fprintf(stderr, "Failed to load tasks\n");
        fclose(out);
        return 1;
    }
    if (n == 0) {
        fprintf(stderr, "No tasks found\n");
        free(tasks);
        fclose(out);
        return 1;
    }

    printf("Loaded %d tasks from '%s'\n", n, argv[1]);

    int *id = malloc((unsigned)n * sizeof(int));
    int *start_time = malloc((unsigned)n * sizeof(int));
    int *end_time = malloc((unsigned)n * sizeof(int));
    if (!id || !start_time || !end_time) {
        free(tasks); free(id); free(start_time); free(end_time);
        fclose(out);
        return 1;
    }

    clock_t t0 = clock();

    scheduling(tasks, n, id, start_time, end_time);

    clock_t t1 = clock();
    printf("Scheduling: %.3f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

    write_results(out, id, start_time, end_time, n);
    fclose(out);

    printf("Simulation completed. Results written to '%s'\n", argv[2]);

    free(tasks);
    free(id);
    free(start_time);
    free(end_time);

    return 0;
}
#include "hybridsort.h"

#include <string.h>
#include <stdlib.h>

/* -------------------------------------------------------
 *  Sorting helpers
 * ------------------------------------------------------- */

// Swap two memory blocks
static void swap(void *a, void *b, size_t size){
    if (a == b || size == 0) return;

    unsigned char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

// Select median of three as pivot (placed at arr[hi])
static void median_of_three(void *base, size_t lo, size_t hi, size_t size, int (*compar)(const void *, const void *)) {
    if (hi <= lo) return;

    unsigned char *arr = (unsigned char *)base;
    size_t mid = lo + (hi - lo) / 2;

    if (compar(arr + mid * size, arr + lo * size) < 0) {
        swap(arr + mid * size, arr + lo * size, size);
    }
    if (compar(arr + hi * size, arr + lo * size) < 0) {
        swap(arr + hi * size, arr + lo * size, size);
    }
    if (compar(arr + hi * size, arr + mid * size) < 0) {
        swap(arr + hi * size, arr + mid * size, size);
    }
    swap(arr + mid * size, arr + hi * size, size);
}

// Select median of five as pivot (placed at arr[hi])
static void median_of_five(void *base,size_t lo, size_t hi,size_t size,int (*compar)(const void *, const void *)) {
    if (hi <= lo) return;

    unsigned char *arr = (unsigned char *)base;
    size_t step = (hi - lo) / 4;

    size_t idx[5] = {lo, lo + step, lo + 2 * step, lo + 3 * step, hi};

    for (size_t i = 1; i < 5; i++) {
        for (size_t j = i; j > 0; j--) {
            unsigned char *a = arr + idx[j - 1] * size;
            unsigned char *b = arr + idx[j] * size;

            if (compar(a, b) > 0) {
                size_t tmp = idx[j];
                idx[j] = idx[j - 1];
                idx[j - 1] = tmp;
            }
        }
    }

    swap(arr + idx[2] * size, arr + hi * size, size);
}

// 3-way partition (Dijkstra)
// Result: [< pivot] [== pivot] [> pivot]
static void partition_3way(void *base, size_t lo, size_t hi, size_t size, int (*compar)(const void *, const void *), size_t *out_lt, size_t *out_gt) {
    if (hi <= lo) {
        *out_lt = lo;
        *out_gt = hi;
        return;
    }
    unsigned char *arr = (unsigned char *)base;
    size_t n = hi - lo + 1;
    if (n >= 5) {
        median_of_five(base, lo, hi, size, compar);
    } else {
        median_of_three(base, lo, hi, size, compar);
    }

    unsigned char pivot[size];
    memcpy(pivot, arr + hi * size, size);

    size_t lt = lo;
    size_t i = lo;
    size_t gt = hi;

    while (i <= gt) {
        int c = compar(arr + i * size, pivot);
        if (c < 0){
            swap(arr + lt * size, arr + i * size, size);
            lt++; 
            i++;
        } else if (c > 0) {
            swap(arr + i * size, arr + gt * size, size);
            gt--;
        } else {
            i++;
        }
    }

    *out_lt = lt;
    *out_gt = gt;
}

/* -------------------------------------------------------
 *  Public API  
 * ------------------------------------------------------- */

void selection_sort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *)){
    if(!base || size == 0 || !compar || nitems < 2) return;

    unsigned char *arr = (unsigned char *)base;

    for (size_t i = 0; i < nitems - 1; i++) {
        size_t min_idx = i;

        for (size_t j = i + 1; j < nitems; j++) {
            void *elem_j = arr + j * size;
            void *elem_min = arr + min_idx * size;

            if (compar(elem_j, elem_min) < 0) {
                min_idx = j;
            }
        }

        if (min_idx != i) {
            swap(arr + i * size, arr + min_idx * size, size);
        }
    }
}

void quick_sort(void *base, size_t lo, size_t hi, size_t size, size_t k, int (*compar)(const void *, const void *)) {
    if (hi <= lo) return;

    size_t count = hi - lo + 1;

    if (k > 0 && count < k) {
        unsigned char *arr = (unsigned char *)base;
        selection_sort(arr + lo * size, count, size, compar);
        return;
    }

    size_t lt, gt;
    partition_3way(base, lo, hi, size, compar, &lt, &gt);

    if (lt > lo) {
        quick_sort(base, lo, lt - 1, size, k, compar);
    } 
    if (gt < hi) {
        quick_sort(base, gt + 1, hi, size, k, compar);
    } 
}

void hybridsort(void *base, size_t nitems, size_t size, size_t k, int (*compar)(const void *, const void *)) {
    if(!base || size == 0 || !compar || nitems < 2) return;

    if (nitems < k){
        selection_sort(base, nitems, size, compar);
    } else {
        quick_sort(base, 0, nitems - 1, size, k, compar);
    }
}
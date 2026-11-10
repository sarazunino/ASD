#ifndef HYBRIDSORT_H
#define HYBRIDSORT_H

#include <stddef.h>

/**
 * @brief Sorts an array using the Selection Sort algorithm.
 * 
 * This function sorts an array using the Selection 
 * Sort algorithm with O(n²) time complexity.
 * 
 * @param base   Pointer to the first element of the array.
 * @param nitems Number of elements in the array.
 * @param size   Size in bytes of each element.
 * @param compar Comparison function.
 *              Return:
 *             - zero if keys are equal
 *             - a negative value if key1 is lower than key2
 *             - a positive value if key1 is greater than key2
 */
void selection_sort(void *base, 
                   size_t nitems, 
                   size_t size,
                   int (*compar)(const void *, const void *));

/**
 * @brief Sorts an array using the Quick Sort algorithm.
 * 
 * This function applies Quick Sort to a portion of an array.
 * The algorithm may switch to Selection Sort for small subarrays
 * depending on the value @p k.
 * 
 * @param base   Pointer to the first element of the array
 * @param lo     Lower index of subarray
 * @param hi     Upper index of subarray
 * @param size   Size in bytes of each element
 * @param k      Threshold for switching to SelectionSort
 * @param compar Comparison function.
 *               Returns negative if first < second,
 *               zero if equal, positive if first > second.
 */
void quick_sort(void *base, 
                size_t lo,
                size_t hi, 
                size_t size, 
                size_t k, 
                int (*compar)(const void *, const void *));

/**
 * @brief Hybrid QuickSort-SelectionSort algorithm.
 *
 * This function sorts a generic array using:
 * - QuickSort for subarrays of size >= k
 * - SelectionSort for subarrays of size <  k
 * 
 * The switch between the two algorithms happens automatically
 * during the QuickSort recursion.
 *
 * @param base   Pointer to the first element of the array.
 * @param nitems Number of elements in the array.
 * @param size   Size in bytes of each element.
 * @param k      Threshold for switching to SelectionSort
 * @param compar Comparison function.
 *               Returns negative if first < second,
 *               zero if equal, positive if first > second.
 */
void hybridsort(void *base,
                size_t nitems,
                size_t size,
                size_t k,
                int (*compar)(const void *, const void *));

#endif
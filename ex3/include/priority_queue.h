#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

/**
 * @struct PriorityQueue
 * @brief Opaque structure representing the priority queue.
 */
typedef struct PriorityQueue PriorityQueue;

/**
 * @brief Create and initialise an empty priority queue.
 *
 * @param f1 Comparison function between two elements. Must return:
 *           - a positive value if the first argument has higher priority,
 *           - zero if both arguments have equal priority,
 *           - a negative value if the first argument has lower priority.
 * @param f2 Hash function for elements stored in the queue.
 *
 * @return Pointer to the newly created queue or NULL if @p f1 or @p f2
 *         are NULL or if a memory allocation fails.
 */
PriorityQueue* priority_queue_create(int (*f1)(const void*,const void*), unsigned long (*f2)(const void*));

/**
 * @brief Insert an element into the priority queue.
 *
 * @param pq   Pointer to the priority queue.
 * @param elem Pointer to the element to insert.
 *
 * @return 1 on success, or 0 if @p pq or @p elem are NULL, if @p elem
 *         is already present, or if a memory allocation fails.
 */
int priority_queue_push(PriorityQueue* pq, void* elem);

/**
 * @brief Test whether an element is present in the priority queue.
 *
 * @param pq   Pointer to the priority queue.
 * @param elem Pointer to the element to search for.
 *
 * @return 1 if the element is present, 0 otherwise (including if @p pq or @p elem are NULL).
 */
int priority_queue_contains(const PriorityQueue* pq, const void* elem);

/**
 * @brief Return the element with the highest priority without removing it.
 *
 * @param pq Pointer to the priority queue.
 *
 * @return Pointer to the highest-priority element, or NULL if @p pq is NULL or the queue is empty.
 */
void* priority_queue_top(const PriorityQueue* pq);

/**
 * @brief Remove the element with the highest priority from the queue.
 *
 * @param pq Pointer to the priority queue.
 */
void priority_queue_pop(PriorityQueue* pq);

/**
 * @brief Remove an arbitrary element from the priority queue.
 *
 * @param pq   Pointer to the priority queue.
 * @param elem Pointer to the element to remove.
 *
 * @return 1 on success, or 0 if @p pq or @p elem are NULL or if
 *         @p elem is not present in the queue.
 */
int priority_queue_remove(PriorityQueue* pq, const void* elem);

/**
 * @brief Return the number of elements currently in the priority queue.
 *
 * @param pq Pointer to the priority queue.
 *
 * @return Number of elements in the queue, or -1 if @p pq is NULL.
 */
int priority_queue_size(const PriorityQueue* pq);

/**
 * @brief Destroy the priority queue and release all internal memory.
 *
 * @param pq Pointer to the priority queue to destroy.
 */
void priority_queue_free(PriorityQueue* pq);

#endif
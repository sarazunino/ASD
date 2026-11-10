#include "priority_queue.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------
 *  Unity hooks
 * ------------------------------------------------------- */

void setUp(void) {}
void tearDown(void) {}

/* -------------------------------------------------------
 *  Helpers
 * ------------------------------------------------------- */

static int compar_int(const void* a_ptr, const void* b_ptr) {
    int a = *((int *) a_ptr);
    int b = *((int *) b_ptr);
	return (a > b ? 1 : (a < b ? -1 : 0));
}

static int compar_float(const void * a_ptr, const void * b_ptr) {
    float a = *((float *) a_ptr);
    float b = *((float *) b_ptr);
    return (a > b ? 1 : (a < b ? -1 : 0));
}

static int compar_string(const void * a_ptr, const void * b_ptr) {
    const char *a = *(const char* const*)a_ptr;
    const char *b = *(const char* const*)b_ptr;
    return strcmp(a, b);
}

static int compar_long(const void * a_ptr, const void * b_ptr) {
    long a = *((long *) a_ptr);
    long b = *((long *) b_ptr);
    return (a > b ? 1 : (a < b ? -1 : 0));
}

static int compar_double(const void * a_ptr, const void * b_ptr) {
    double a = *((double *) a_ptr);
    double b = *((double *) b_ptr);
    return (a > b ? 1 : (a < b ? -1 : 0));
}

static unsigned long hash_int(const void* p) {
    return (unsigned long)(*(int*)p);
}

static unsigned long hash_float(const void* p) {
    unsigned long h = 0;
    memcpy(&h, p, sizeof(float));
    return h;
}

static unsigned long hash_string(const void* p) {
    const unsigned char* s = (const unsigned char*)(*(const char* const*)p);
    unsigned long h = 5381;
    while (*s) h = h * 33 ^ *s++;
    return h;
}

static unsigned long hash_long(const void* p) {
    return (unsigned long)(*(long*)p);
}

static unsigned long hash_double(const void* p) {
    unsigned long long bits = 0;
    memcpy(&bits, p, sizeof(double));
    return (unsigned long)(bits ^ (bits >> 32));
}

/* -------------------------------------------------------
 *  Priority Queue Tests
 * ------------------------------------------------------- */

void test_create(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    TEST_ASSERT_NOT_NULL(pq);
    priority_queue_free(pq);
}

void test_create_null_cmp(void) {
    TEST_ASSERT_NULL(priority_queue_create(NULL, hash_int));
}

void test_create_null_hash(void) {
    TEST_ASSERT_NULL(priority_queue_create(compar_int, NULL));
}

void test_create_both_null(void) {
    TEST_ASSERT_NULL(priority_queue_create(NULL, NULL));
}

void test_create_initial_size(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    TEST_ASSERT_EQUAL_INT(0, priority_queue_size(pq));
    priority_queue_free(pq);
}

void test_push_new_elem(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 1;
    TEST_ASSERT_TRUE(priority_queue_push(pq, &a));
    priority_queue_free(pq);
}

void test_push_duplicate(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 7;
    priority_queue_push(pq, &a);
    TEST_ASSERT_FALSE(priority_queue_push(pq, &a));
    priority_queue_free(pq);
}

void test_push_null_elem(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    TEST_ASSERT_FALSE(priority_queue_push(pq, NULL));
    priority_queue_free(pq);
}

void test_push_size(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 1;
    priority_queue_push(pq, &a);
    TEST_ASSERT_EQUAL_INT(1, priority_queue_size(pq));
    priority_queue_free(pq);
}

void test_push_duplicate_size(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 7;
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &a);
    TEST_ASSERT_EQUAL_INT(1, priority_queue_size(pq));
    priority_queue_free(pq);
}

void test_top_empty(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    TEST_ASSERT_NULL(priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_top_int(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 1, b = 5, c = 3;
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    TEST_ASSERT_EQUAL_PTR(&b, priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_top_float(void) {
    PriorityQueue* pq = priority_queue_create(compar_float, hash_float);
    float a = 1.1f, b = 3.3f, c = 2.2f;
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    TEST_ASSERT_EQUAL_PTR(&b, priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_top_double(void) {
    PriorityQueue* pq = priority_queue_create(compar_double, hash_double);
    double a = 1.1, b = 3.3, c = 2.2;
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    TEST_ASSERT_EQUAL_PTR(&b, priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_top_long(void) {
    PriorityQueue* pq = priority_queue_create(compar_long, hash_long);
    long a = 100L, b = 999L, c = 500L;
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    TEST_ASSERT_EQUAL_PTR(&b, priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_top_string(void) {
    PriorityQueue* pq = priority_queue_create(compar_string, hash_string);
    const char* a = "apple";
    const char* b = "zebra";
    const char* c = "mango";
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    TEST_ASSERT_EQUAL_STRING("zebra", *(const char**)priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_pop_empty(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    priority_queue_pop(pq);
    TEST_ASSERT_EQUAL_INT(0, priority_queue_size(pq));
    priority_queue_free(pq);
}

void test_pop_decreases_size(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 1, b = 2;
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_pop(pq);
    TEST_ASSERT_EQUAL_INT(1, priority_queue_size(pq));
    priority_queue_free(pq);
}

void test_pop_int(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 1, b = 5, c = 3;
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    priority_queue_pop(pq);
    TEST_ASSERT_EQUAL_PTR(&c, priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_pop_order_int(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int vals[] = {4, 1, 7, 2, 9, 3};
    for (int i = 0; i < 6; i++) priority_queue_push(pq, &vals[i]);

    int prev = *(int*)priority_queue_top(pq);
    priority_queue_pop(pq);
    while (priority_queue_size(pq) > 0) {
        int cur = *(int*)priority_queue_top(pq);
        TEST_ASSERT_GREATER_OR_EQUAL_INT(cur, prev);
        prev = cur;
        priority_queue_pop(pq);
    }

    priority_queue_free(pq);
}

void test_contains_present_elem(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 42;
    priority_queue_push(pq, &a);
    TEST_ASSERT_TRUE(priority_queue_contains(pq, &a));
    priority_queue_free(pq);
}

void test_contains_absent_elem(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 42, b = 99;
    priority_queue_push(pq, &a);
    TEST_ASSERT_FALSE(priority_queue_contains(pq, &b));
    priority_queue_free(pq);
}

void test_remove_present_elem(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int a = 1;
    priority_queue_push(pq, &a);
    TEST_ASSERT_TRUE(priority_queue_remove(pq, &a));
    priority_queue_free(pq);
}

void test_remove_absent_elem(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int x = 42;
    TEST_ASSERT_FALSE(priority_queue_remove(pq, &x));
    priority_queue_free(pq);
}

void test_remove_string(void) {
    PriorityQueue* pq = priority_queue_create(compar_string, hash_string);
    const char* a = "apple";
    const char* b = "zebra";
    const char* c = "mango";
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    priority_queue_remove(pq, &b);
    TEST_ASSERT_EQUAL_STRING("mango", *(const char**)priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_remove_long(void) {
    PriorityQueue* pq = priority_queue_create(compar_long, hash_long);
    long a = 100L, b = 999L, c = 500L;
    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    priority_queue_remove(pq, &b);
    TEST_ASSERT_EQUAL_PTR(&c, priority_queue_top(pq));
    priority_queue_free(pq);
}

void test_capacity(void) {
    PriorityQueue* pq = priority_queue_create(compar_int, hash_int);
    int vals[20];
    for (int i = 0; i < 20; i++) {
        vals[i] = i;
        priority_queue_push(pq, &vals[i]);
    }

    TEST_ASSERT_EQUAL_INT(20, priority_queue_size(pq));
    TEST_ASSERT_EQUAL_INT(19, *(int*)priority_queue_top(pq));

    priority_queue_free(pq);
}

/* -------------------------------------------------------
 *  Main
 * ------------------------------------------------------- */

int main(void) {
    UNITY_BEGIN();

    // create
    RUN_TEST(test_create);
    RUN_TEST(test_create_null_cmp);
    RUN_TEST(test_create_null_hash);
    RUN_TEST(test_create_both_null);
    RUN_TEST(test_create_initial_size);

    // push
    RUN_TEST(test_push_new_elem);
    RUN_TEST(test_push_duplicate);
    RUN_TEST(test_push_null_elem);
    RUN_TEST(test_push_size);
    RUN_TEST(test_push_duplicate_size);

    // top
    RUN_TEST(test_top_empty);
    RUN_TEST(test_top_int);
    RUN_TEST(test_top_float);
    RUN_TEST(test_top_double);
    RUN_TEST(test_top_long);
    RUN_TEST(test_top_string);

    // pop
    RUN_TEST(test_pop_empty);
    RUN_TEST(test_pop_decreases_size);
    RUN_TEST(test_pop_int);
    RUN_TEST(test_pop_order_int);

    // contains
    RUN_TEST(test_contains_present_elem);
    RUN_TEST(test_contains_absent_elem);

    // remove
    RUN_TEST(test_remove_present_elem);
    RUN_TEST(test_remove_absent_elem);
    RUN_TEST(test_remove_string);
    RUN_TEST(test_remove_long);

    // heap grow
    RUN_TEST(test_capacity);

    return UNITY_END();
}
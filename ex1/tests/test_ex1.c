#include "hybridsort.h"
#include "unity.h"

#include <string.h>
#include <stddef.h>

/* -------------------------------------------------------
 *  Unity hooks
 * ------------------------------------------------------- */

void setUp(void) {}
void tearDown(void) {}

/* -------------------------------------------------------
 *  Comparators
 * ------------------------------------------------------- */

static int compar_int(const void * a_ptr, const void * b_ptr) {
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

/* -------------------------------------------------------
 *  Helpers
 * ------------------------------------------------------- */

static void assert_sorted(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *)) {
   const unsigned char *arr = (const unsigned char *)base;
    for (size_t i = 1; i < nitems; i++) {
        const void *prev = arr + (i - 1) * size;
        const void *curr = arr + i * size;
        TEST_ASSERT_TRUE_MESSAGE(compar(prev, curr) <= 0, "Array is not sorted");
    }
}

/* -------------------------------------------------------
 *  SelectionSort Tests
 * ------------------------------------------------------- */

void test_selection_sort_empty(void) {
    selection_sort(NULL, 0, sizeof(int), compar_int);
    TEST_PASS();
}

void test_selection_sort_single_element(void) {
    int a[1] = {7};
    selection_sort(a, 1, sizeof(int), compar_int);
    TEST_ASSERT_EQUAL_INT(7, a[0]);
}

void test_selection_sort_already_sorted(void){
  int a[6] = {1, 2, 3, 4, 5, 6}; 
  selection_sort(a, 6, sizeof(int), compar_int);
  assert_sorted(a, 6, sizeof(int), compar_int);
}

void test_selection_sort_reversed(void){
  int a[6] = {6, 5, 4, 3, 2, 1};
  selection_sort(a, 6, sizeof(int), compar_int);
  assert_sorted(a, 6, sizeof(int), compar_int);
}

void test_selection_sort_duplicated(void){
  int a[10] = {3, 3, 6, 5, 5, 7, 1, 1, 1, 1};
  selection_sort(a, 10, sizeof(int), compar_int);
  assert_sorted(a, 10, sizeof(int), compar_int);
}

void test_selection_sort_long(void) {
    long a[6] = {1, 2, 3, 4, 5, 6};
    selection_sort(a, 6, sizeof(long), compar_long);
    assert_sorted(a, 6, sizeof(long), compar_long);
}

void test_selection_sort_float(void) {
    float a[6] = {1.0f, 2.5f, 2.5f, -1.0f, 3.0f, 0.0f};
    selection_sort(a, 6, sizeof(float), compar_float);
    assert_sorted(a, 6, sizeof(float), compar_float);
}

void test_selection_sort_double(void) {
  double a[7] = {3.14, -2.0, 0.0, 1.5, 1.5, 100.0, -0.25};
  selection_sort(a, 7, sizeof(double), compar_double);
  assert_sorted(a, 7, sizeof(double), compar_double);
}

void test_selection_sort_string(void) {
    const char *a[] = {"apple", "banana", "strawberry", "Apricot", "cherry"};
    selection_sort((void*)a, 5, sizeof(char*), compar_string);
    assert_sorted(a, 5, sizeof(char*), compar_string);
}

/* -------------------------------------------------------
 *  QuickSort Tests
 * ------------------------------------------------------- */

void test_quick_sort_empty(void) {
  quick_sort(NULL, 0, 0, sizeof(int), 0, compar_int);
  TEST_PASS();
}

void test_quick_sort_single_element(void) {
  int a[1] = {7};
  quick_sort(a, 0, 0, sizeof(int), 0, compar_int);
  TEST_ASSERT_EQUAL_INT(7, a[0]);
}

void test_quick_sort_already_sorted(void){
  int a[6] = {1, 2, 3, 4, 5, 6}; 
  quick_sort(a, 0, 6, sizeof(int), 0, compar_int);
  assert_sorted(a, 6, sizeof(int), compar_int);
}

void test_quick_sort_reversed(void){
  int a[6] = {6, 5, 4, 3, 2, 1};
  quick_sort(a, 0, 6, sizeof(int), 0, compar_int);
  assert_sorted(a, 6, sizeof(int), compar_int);
}

void test_quick_sort_duplicated(void){
  int a[10] = {3, 3, 6, 5, 5, 7, 1, 1, 1, 1};
  quick_sort(a, 0, 10, sizeof(int), 0, compar_int);
  assert_sorted(a, 10, sizeof(int), compar_int);
}

void test_quick_sort_k_middle(void) {
    int a[7] = {4, 1, 6, 2, 0, 3, 5};
    quick_sort(a, 0, 7, sizeof(int), 4, compar_int);
    assert_sorted(a, 7, sizeof(int), compar_int);
}

void test_quick_sort_long(void) {
    long a[6] = {6, 5, 4, 3, 2, 1};
    quick_sort(a, 0, 6, sizeof(long), 0, compar_long);
    assert_sorted(a, 6, sizeof(long), compar_long);
}

void test_quick_sort_float(void) {
    float a[6] = {1.0f, 2.5f, 2.5f, -1.0f, 3.0f, 0.0f};
    quick_sort(a, 0, 6, sizeof(float), 0, compar_float);
    assert_sorted(a, 6, sizeof(float), compar_float);
}

void test_quick_sort_double(void) {
    double a[7] = {3.14, -2.0, 0.0, 1.5, 1.5, 100.0, -0.25};
    quick_sort(a, 0, 7, sizeof(double), 0, compar_double);
    assert_sorted(a, 7, sizeof(double), compar_double);
}

void test_quick_sort_string(void) {
    const char *a[] = {"apple", "banana", "strawberry", "Apricot", "cherry"};
    quick_sort((void*)a, 0, 5, sizeof(char*), 0, compar_string);
    assert_sorted(a, 5, sizeof(char*), compar_string);
}

/* -------------------------------------------------------
 *  HybridSort Tests
 * ------------------------------------------------------- */

void test_hybridsort_null_base(void){
  hybridsort(NULL, 10, sizeof(int), 3, compar_int);
  TEST_PASS();
}

void test_hybridsort_null_compar(void){
  int a[3] = {3,2,1};
  hybridsort(a, 3, sizeof(int), 3, NULL);
  TEST_PASS();
}

void test_hybridsort_zero_size(void){
  int a[3] = {3,2,1};
  hybridsort(a, 3, 0, 3, compar_int);
  TEST_PASS();
}

void test_hybridsort_empty_array(void){
  int *a = NULL;
  hybridsort(a, 0, sizeof(int), 3, compar_int);
  TEST_PASS();
}

void test_hybridsort_single_element(void){
  int a[1] = {23};
  hybridsort(a, 1, sizeof(int), 3, compar_int);
  TEST_ASSERT_EQUAL_INT(23, a[0]);
}

void test_hybridsort_already_sorted(void){
  int a[6] = {1, 2, 3, 4, 5, 6}; 
  hybridsort(a, 6, sizeof(int), 3, compar_int);
  assert_sorted(a, 6, sizeof(int), compar_int);
}

void test_hybridsort_reversed(void){
  int a[6] = {6, 5, 4, 3, 2, 1};
  hybridsort(a, 6, sizeof(int), 3, compar_int);
  assert_sorted(a, 6, sizeof(int), compar_int);
}

void test_hybridsort_duplicated(void){
  int a[10] = {3, 3, 6, 5, 5, 7, 1, 1, 1, 1};
  hybridsort(a, 10, sizeof(int), 3, compar_int);
  assert_sorted(a, 10, sizeof(int), compar_int);
}

void test_hybridsort_most_duplicated(void){
  int a[10] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 1};
  hybridsort(a, 10, sizeof(int), 3, compar_int);
  assert_sorted(a, 10, sizeof(int), compar_int);
}

void test_hybridsort_k_0(void){
  int a[7] = {4,1,6,2,0,3,5};
  hybridsort(a, 7, sizeof(int), 0, compar_int);
  assert_sorted(a, 7, sizeof(int), compar_int);
}

void test_hybridsort_k_1(void){
    int a[7] = {4,1,6,2,0,3,5};
    hybridsort(a, 7, sizeof(int), 1, compar_int);
    assert_sorted(a, 7, sizeof(int), compar_int);
}

void test_hybridsort_k_equal(void){
    int a[7] = {4,1,6,2,0,3,5};
    hybridsort(a, 7, sizeof(int), 7, compar_int);  
    assert_sorted(a, 7, sizeof(int), compar_int);
}

void test_hybridsort_k_greater(void){
    int a[7] = {4,1,6,2,0,3,5};
    hybridsort(a, 7, sizeof(int), 100, compar_int); 
    assert_sorted(a, 7, sizeof(int), compar_int);
}

void test_hybridsort_long(void) {
    long a[6] = {6, 5, 4, 3, 2, 1};
    hybridsort(a, 6, sizeof(long), 3, compar_long);
    assert_sorted(a, 6, sizeof(long), compar_long);
}

void test_hybridsort_float(void) {
    float a[6] = {1.0f, 2.5f, 2.5f, -1.0f, 3.0f, 0.0f};
    hybridsort(a, 6, sizeof(float), 3, compar_float);
    assert_sorted(a, 6, sizeof(float), compar_float);
}

void test_hybridsort_double(void) {
    double a[7] = {3.14, -2.0, 0.0, 1.5, 1.5, 100.0, -0.25};
    hybridsort(a, 7, sizeof(double), 3, compar_double);
    assert_sorted(a, 7, sizeof(double), compar_double);
}

void test_hybridsort_string(void) {
    const char *a[] = {"apple", "banana", "strawberry", "Apricot", "cherry"};
    hybridsort((void*)a, 5, sizeof(char*), 3, compar_string);
    assert_sorted(a, 5, sizeof(char*), compar_string);
}

/* -------------------------------------------------------
 *  Main
 * ------------------------------------------------------- */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_selection_sort_empty);
    RUN_TEST(test_selection_sort_single_element);
    RUN_TEST(test_selection_sort_already_sorted);
    RUN_TEST(test_selection_sort_reversed);
    RUN_TEST(test_selection_sort_duplicated);
    RUN_TEST(test_selection_sort_long);
    RUN_TEST(test_selection_sort_float);
    RUN_TEST(test_selection_sort_double);
    RUN_TEST(test_selection_sort_string);
    
    RUN_TEST(test_quick_sort_empty);
    RUN_TEST(test_quick_sort_single_element);
    RUN_TEST(test_quick_sort_already_sorted);
    RUN_TEST(test_quick_sort_reversed);
    RUN_TEST(test_quick_sort_duplicated);
    RUN_TEST(test_quick_sort_k_middle);
    RUN_TEST(test_quick_sort_long);
    RUN_TEST(test_quick_sort_float);
    RUN_TEST(test_quick_sort_double);
    RUN_TEST(test_quick_sort_string);

    RUN_TEST(test_hybridsort_null_base);
    RUN_TEST(test_hybridsort_null_compar);
    RUN_TEST(test_hybridsort_zero_size);
    RUN_TEST(test_hybridsort_empty_array);
    RUN_TEST(test_hybridsort_single_element);
    RUN_TEST(test_hybridsort_already_sorted);
    RUN_TEST(test_hybridsort_reversed);
    RUN_TEST(test_hybridsort_duplicated);
    RUN_TEST(test_hybridsort_most_duplicated);
    RUN_TEST(test_hybridsort_k_0);
    RUN_TEST(test_hybridsort_k_1);
    RUN_TEST(test_hybridsort_k_equal);
    RUN_TEST(test_hybridsort_k_greater);
    RUN_TEST(test_hybridsort_long);
    RUN_TEST(test_hybridsort_float);
    RUN_TEST(test_hybridsort_double);
    RUN_TEST(test_hybridsort_string);

    return UNITY_END();
}
#include "hash_table.h"
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

static int int_compare(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

static unsigned long int_hash(const void* key) {
    return (unsigned long)(*(int*)key);
}

static int string_compare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

static unsigned long string_hash(const void* key) {
    const unsigned char* str = (const unsigned char*)key;
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/* -------------------------------------------------------
 *  HashTable Tests
 * ------------------------------------------------------- */

void test_create_not_null(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    TEST_ASSERT_NOT_NULL(ht);
    hash_table_free(ht);
}

void test_initial_size_zero(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(ht));
    hash_table_free(ht);
}

void test_put_and_size(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int k = 1; int v = 100;
    hash_table_put(ht, &k, &v);
    TEST_ASSERT_EQUAL_INT(1, hash_table_size(ht));
    hash_table_free(ht);
}

void test_get_existing_key(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int k = 42; int v = 999;
    hash_table_put(ht, &k, &v);
    int* result = hash_table_get(ht, &k);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_INT(999, *result);
    hash_table_free(ht);
}

void test_get_nonexistent_key(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int k = 7;
    void* result = hash_table_get(ht, &k);
    TEST_ASSERT_NULL(result);
    hash_table_free(ht);
}

void test_contains_key_present(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int k = 5; int v = 50;
    hash_table_put(ht, &k, &v);
    TEST_ASSERT_EQUAL_INT(1, hash_table_contains_key(ht, &k));
    hash_table_free(ht);
}

void test_contains_key_absent(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int k = 5;
    TEST_ASSERT_EQUAL_INT(0, hash_table_contains_key(ht, &k));
    hash_table_free(ht);
}

void test_update_existing_key(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int k = 3; int v1 = 10; int v2 = 20;
    hash_table_put(ht, &k, &v1);
    hash_table_put(ht, &k, &v2);
    TEST_ASSERT_EQUAL_INT(1, hash_table_size(ht));
    int* result = hash_table_get(ht, &k);
    TEST_ASSERT_EQUAL_INT(20, *result);
    hash_table_free(ht);
}

void test_remove_existing_key(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int k = 8; int v = 80;
    hash_table_put(ht, &k, &v);
    hash_table_remove(ht, &k);
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(ht));
    TEST_ASSERT_NULL(hash_table_get(ht, &k));
    hash_table_free(ht);
}

void test_remove_nonexistent_key(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int k = 99;
    hash_table_remove(ht, &k);
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(ht));
    hash_table_free(ht);
}

void test_keyset_contains_all_keys(void) {
    HashTable* ht = hash_table_create(string_compare, string_hash);
    hash_table_put(ht, "apple", "red");
    hash_table_put(ht, "banana", "yellow");
    hash_table_put(ht, "cherry", "red");

    void** keys = hash_table_keyset(ht);
    int found_apple = 0, found_banana = 0, found_cherry = 0;
    for (int i = 0; keys[i] != NULL; i++) {
        if (strcmp(keys[i], "apple") == 0) found_apple = 1;
        if (strcmp(keys[i], "banana") == 0) found_banana = 1;
        if (strcmp(keys[i], "cherry") == 0) found_cherry = 1;
    }
    TEST_ASSERT_EQUAL_INT(1, found_apple);
    TEST_ASSERT_EQUAL_INT(1, found_banana);
    TEST_ASSERT_EQUAL_INT(1, found_cherry);
    free(keys);
    hash_table_free(ht);
}

void test_many_insertions_trigger_rehash(void) {
    HashTable* ht = hash_table_create(int_compare, int_hash);
    int keys[100];
    int vals[100];
    for (int i = 0; i < 100; i++) {
        keys[i] = i;
        vals[i] = i * 2;
        hash_table_put(ht, &keys[i], &vals[i]);
    }
    TEST_ASSERT_EQUAL_INT(100, hash_table_size(ht));
    for (int i = 0; i < 100; i++) {
        int* v = hash_table_get(ht, &keys[i]);
        TEST_ASSERT_NOT_NULL(v);
        TEST_ASSERT_EQUAL_INT(i * 2, *v);
    }
    hash_table_free(ht);
}

void test_collision_chaining(void) {
    HashTable* ht = hash_table_create(int_compare,
        (unsigned long(*)(const void*))int_hash);

    int keys[10]; int vals[10];
    for (int i = 0; i < 10; i++) {
        keys[i] = i * 16; 
        vals[i] = i;
        hash_table_put(ht, &keys[i], &vals[i]);
    }
    for (int i = 0; i < 10; i++) {
        int* v = hash_table_get(ht, &keys[i]);
        TEST_ASSERT_NOT_NULL(v);
        TEST_ASSERT_EQUAL_INT(i, *v);
    }
    hash_table_free(ht);
}

void test_string_keys_and_values(void) {
    HashTable* ht = hash_table_create(string_compare, string_hash);
    hash_table_put(ht, "hello", "world");
    char* result = hash_table_get(ht, "hello");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("world", result);
    hash_table_free(ht);
}

void test_keyset_size_matches(void) {
    HashTable* ht = hash_table_create(string_compare, string_hash);
    hash_table_put(ht, "a", "1");
    hash_table_put(ht, "b", "2");
    void** keys = hash_table_keyset(ht);
    int count = 0;
    while (keys[count] != NULL) count++;
    TEST_ASSERT_EQUAL_INT(hash_table_size(ht), count);
    free(keys);
    hash_table_free(ht);
}

/* -------------------------------------------------------
 *  Main
 * ------------------------------------------------------- */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_create_not_null);
    RUN_TEST(test_initial_size_zero);
    RUN_TEST(test_put_and_size);
    RUN_TEST(test_get_existing_key);
    RUN_TEST(test_get_nonexistent_key);
    RUN_TEST(test_contains_key_present);
    RUN_TEST(test_contains_key_absent);
    RUN_TEST(test_update_existing_key);
    RUN_TEST(test_remove_existing_key);
    RUN_TEST(test_remove_nonexistent_key);
    RUN_TEST(test_keyset_contains_all_keys);
    RUN_TEST(test_many_insertions_trigger_rehash);
    RUN_TEST(test_collision_chaining);
    RUN_TEST(test_string_keys_and_values);
    RUN_TEST(test_keyset_size_matches);
    
    return UNITY_END();
}
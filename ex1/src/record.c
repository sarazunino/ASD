#include "record.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 1024

/* -------------------------------------------------------
 *  Record I/O functions 
 * ------------------------------------------------------- */

int read_record(FILE *file, Record *record) {
    if (!file || !record) return -1;

    if (fread(&record->id, sizeof(uint64_t), 1, file) != 1) {
        return feof(file) ? 0 : -1;
    }
    if (fread(&record->field1, sizeof(float), 1, file) != 1) return -1;
    if (fread(&record->field2, sizeof(int64_t), 1, file) != 1) return -1;
    if (fread(record->field3, FIELD3_LEN, 1, file) != 1) return -1;

    return 1;
}

int write_record(FILE *file, const Record *record) {
    if (!file || !record) return -1;

    if (fwrite(&record->id, sizeof(uint64_t), 1, file) != 1) return -1;
    if (fwrite(&record->field1, sizeof(float), 1, file) != 1) return -1;
    if (fwrite(&record->field2, sizeof(int64_t), 1, file) != 1) return -1;
    if (fwrite(record->field3, FIELD3_LEN, 1, file) != 1) return -1;

    return 1;
}

int load_records_from_file(FILE *file, Record **records, size_t *count) {
    if (!file || !records || !count) return -1; 

    rewind(file);

    size_t capacity = INITIAL_CAPACITY, n = 0;

    Record *arr = malloc(capacity * sizeof(Record));
    if (!arr) { 
        return -1; 
    }

    int ret;
    while ((ret = read_record(file, &arr[n])) == 1) {
        n++;

        if (n == capacity) {
            capacity *= 2;
            Record *tmp = realloc(arr, capacity * sizeof(Record));
            if (!tmp) { 
                free(arr); 
                return -1; 
            }
            arr = tmp;
        }
    }

    if (ret == -1) { 
        free(arr); 
        return -1;
    }
    *records = arr;
    *count = n;

    return 0;
}

int save_records_to_file(FILE *file, const Record *records, size_t count) {
    if (!file || !records) return -1;

    for (size_t i = 0; i < count; i++) {
        if (write_record(file, &records[i]) != 1) return -1;
    }

    return 0;
}

void print_record(const Record *record) {
    if (!record) return;
    printf("id=%-20llu  field1=%-15.6f  field2=%-20lld  field3=\"%s\"\n",
           (unsigned long long)record->id,
           (double)record->field1,
           (long long)record->field2,
           record->field3);
}

/* -------------------------------------------------------
 *  Comparison functions  
 * ------------------------------------------------------- */

int compar_by_id(const void *a_ptr, const void *b_ptr) {
    const Record *a = (const Record *)a_ptr;
    const Record *b = (const Record *)b_ptr;
    return (a->id > b->id ? 1 : (a->id < b->id ? -1 : 0));
}

int compar_by_field1(const void *a_ptr, const void *b_ptr) {
    const Record *a = (const Record *)a_ptr;
    const Record *b = (const Record *)b_ptr;
    return (a->field1 > b->field1 ? 1 : (a->field1 < b->field1 ? -1 : 0));
}

int compar_by_field2(const void *a_ptr, const void *b_ptr) {
    const Record *a = (const Record *)a_ptr;
    const Record *b = (const Record *)b_ptr;
    return (a->field2 > b->field2 ? 1 : (a->field2 < b->field2 ? -1 : 0));
}

int compar_by_field3(const void *a_ptr, const void *b_ptr) {
    const Record *a = (const Record *)a_ptr;
    const Record *b = (const Record *)b_ptr;
    return memcmp(a->field3, b->field3, FIELD3_LEN);
}

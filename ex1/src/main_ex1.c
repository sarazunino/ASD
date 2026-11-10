#include "hybridsort.h"
#include "record.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

void sort_records(FILE *infile, FILE *outfile, size_t field, size_t k) {
    int (*compar)(const void *, const void *) = NULL;

    if (field == 1) {
        compar = compar_by_id;
    } else if (field == 2) {
        compar = compar_by_field1;
    } else if (field == 3) {
        compar = compar_by_field2;
    } else if (field == 4) {
        compar = compar_by_field3;
    } else {
        printf("invalid field (expected 1..4)\n");
        return;
    }

    Record *records = NULL;
    size_t count = 0;

    if (load_records_from_file(infile, &records, &count) != 0) {
        printf("error loading records\n");
        return;
    }

    hybridsort(records, count, sizeof(Record), k, compar);

    if (save_records_to_file(outfile, records, count) != 0) {
        printf("error saving records\n");
        free(records);
        return;
    }

    free(records);
}

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input> <output> <field 1..4> <k>\n", argv[0]);
        return 1;
    }
    FILE *in  = fopen(argv[1], "rb");
    if (!in) {
        fprintf(stderr, "Cannot open input file '%s': %s\n", argv[1], strerror(errno));
        return 1;
    }
    FILE *out = fopen(argv[2], "wb");
    if (!out) {
        fprintf(stderr, "Cannot open output file '%s': %s\n", argv[2], strerror(errno));
        fclose(in);
        return 1;
    }

    size_t field = (size_t)atoi(argv[3]);
    size_t k = (size_t)atoi(argv[4]);

    clock_t start = clock();
    
    sort_records(in, out, field, k);

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Sorting completed in %.3f seconds\n", elapsed);

    fclose(in);
    fclose(out);

    return 0;
}
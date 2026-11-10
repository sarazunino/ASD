#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

static void to_lower(char* s) {
    for (; *s; s++) {
        *s = (char)tolower((unsigned char)*s);
    }
}

static void count_token(HashTable* ht, const char* token, int tlen) {
    void* val = hash_table_get(ht, token);
    if (val) {
        (*(int*)val)++;
    } else {
        char* key = malloc(tlen + 1);
        memcpy(key, token, tlen + 1);
        int* cnt = malloc(sizeof(int));
        *cnt = 1;
        hash_table_put(ht, key, cnt);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_path> <min_word_length>\n", argv[0]);
        return 1;
    }

    const char* filepath = argv[1];
    int min_len = atoi(argv[2]);
    if (min_len < 1) {
        fprintf(stderr, "min_word_length must be >= 1\n");
        return 1;
    }

    FILE* f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, "Cannot open file: %s\n", filepath);
        return 1;
    }

    HashTable* ht = hash_table_create(string_compare, string_hash);

    char word[1024];
    char token[1024];

    while (fscanf(f, "%1023s", word) == 1) {
        int wlen = (int)strlen(word);
        int tstart = 0;
        for (int i = 0; i <= wlen; i++) {
            if (i == wlen || !isalpha((unsigned char)word[i])) {
                int tlen = i - tstart;
                if (tlen >= min_len) {
                    memcpy(token, word + tstart, tlen);
                    token[tlen] = '\0';
                    to_lower(token);
                    count_token(ht, token, tlen);
                }
                tstart = i + 1;
            }
        }
    }
    fclose(f);

    void** keys = hash_table_keyset(ht);
    const char* best_word = NULL;
    int best_count = 0;

    for (int i = 0; keys[i] != NULL; i++) {
        const char* k = (const char*)keys[i];
        int* cnt = (int*)hash_table_get(ht, k);
        if (*cnt > best_count) {
            best_count = *cnt;
            best_word = k;
        }
    }
    free(keys);

    if (best_word) {
        printf("Most frequent word (length %d): \"%s\" (%d occurrences)\n",
               min_len, best_word, best_count);
    } else {
        printf("No words found with length %d.\n", min_len);
    }

    void** all_keys = hash_table_keyset(ht);
    for (int i = 0; all_keys[i] != NULL; i++) {
        void* cnt = hash_table_get(ht, all_keys[i]);
        free(cnt);
        free(all_keys[i]);
    }
    free(all_keys);
    hash_table_free(ht);

    return 0;
}
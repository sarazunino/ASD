# Exercise 2 – Hash Table with Chaining

## Implementation

This project implements a generic hash table library using separate chaining for collision resolution. 
The implementation was developed with the support of a Large Language Model (LLM), as required by the assignment.
We developed to compute the most frequent word (with length ≥ a given minimum) from a text file using the hash table. 

### Initial Prompt

> Implement in C a generic library for a hash table with chaining. The data structure must be capable of storing a set of key–value pairs {<key₁, value₁>, ..., <keyₙ, valueₙ>}.  
> The hash table must accept keys and values of generic types (all keys share the same type, and all values share the same type, but keys and values may be of different types from each other).  
> To compare keys, the hash table must accept a comparison function of type `int (*compare)(const void*, const void*)`. It must also accept a hash function of type `unsigned long (*hash)(const void*)`.  
>
> It must provide the following operations:
> ```
> HashTable* hash_table_create(int (*compare)(const void*, const void*), unsigned long (*hash)(const void*));
> void hash_table_put(HashTable*, const void*, const void*);
> void* hash_table_get(const HashTable*, const void*);
> int hash_table_contains_key(const HashTable*, const void*);
> void hash_table_remove(HashTable*, const void*);
> int hash_table_size(const HashTable*);
> void** hash_table_keyset(const HashTable*);
> void hash_table_free(HashTable*);
> ```

### Issues Found

From the first response, the LLM gave a solution that met the required time complexities, which was a key requirement of the assignment. However, some functions were not very readable and required simplification.

An issue also appeared in the tokenizer used for the word-frequency program. The initial version only removed non-alphabetic characters from the edges of whitespace-separated tokens, so `"aegis-bearing"` was treated as a single 12-character word. This led to incorrect results for higher minimum lengths (`min_len = 13` returned `"aegis-bearing"` instead of `"understanding"`).
The fix was to split each token at every non-alphabetic character, so `"aegis-bearing"` is correctly processed as two separate words: `"aegis"` and `"bearing"`.

Overall, only about three iterations were required to:  
- simplify the structure of the code,
- and fix the issue with hyphenated strings.

### Reflections

LLM assistance was effective in generating a correct structure and respecting the required computational complexity from the start.

However, human supervision was still needed to improve readability, identify subtle issues (such as tokenization errors), and ensure full compliance with the assignment requirements.

The development process was iterative but limited. With only a few targeted refinements, the final implementation became both readable and algorithmically correct.

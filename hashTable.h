/***********************************/
/* A shared header file for hash   */
/* table implementation.           */
/***********************************/

/* Include guard to prevent multiple definitions of the same header. */
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/* Maximum size of the hash table. */
#define MAX_HASH_SIZE 10000

/* Structure to represent the hash table. */
typedef struct hash_table_t {
    int size;  // Hash table size
    int count; // Number of elements
    long long* keys;  // Array of keys.
    int* values;  // Array of values.
} HashTable;

/* Function declarations for initializing and freeing the hash table. */
void init_hashtable(HashTable* hashtable);
void free_hashtable(HashTable* hashtable);

/* Function declarations for adding and looking up elements in the hash table. */
int hashtable_add(HashTable* hashtable, long long key, int value);
int hashtable_lookup(HashTable* hashtable, long long key);

/* End of include guard. */
#endif /* HASH_TABLE_H */
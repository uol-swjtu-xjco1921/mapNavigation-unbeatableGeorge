// This program includes header files for standard input/output streams and commonly used utility libraries.
#include <stdio.h>
#include <stdlib.h>

// These are custom header files that are included
#include "hashTable.h" // Header file for hash table implementation
#include "mapError.h" // Header file for error handling in map operations.


// Initialize hash table with default size and allocate memory for keys and values arrays
void init_hashtable(HashTable* hashtable)
{
    hashtable->size = MAX_HASH_SIZE;
    hashtable->count = 0;
    hashtable->keys = (long long*)calloc(hashtable->size, sizeof(long long));
    hashtable->values = (int*)calloc(hashtable->size, sizeof(int));
    if (hashtable->keys == NULL || hashtable->values == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for hash table\n");
        exit(ERROR_MALLOC_FAILED);
    }
}

// Free memory used by keys and values arrays in hash table
void free_hashtable(HashTable* hashtable)
{
    if (hashtable->keys != NULL)
        free(hashtable->keys);
    if (hashtable->values != NULL)
        free(hashtable->values);
}

// Add key-value pair to the hash table
int hashtable_add(HashTable* hashtable, long long key, int value)
{
    // Check if hash table is already at maximum size
    if (hashtable->count == MAX_HASH_SIZE)
        return ERROR_HASH_ADD;

    // Convert key to positive long long for hashing purposes
    key = key + 2147483647LL;

    // Get index of slot in hash table using modulo operator
    int index = key % hashtable->size;

    // Handle collisions using linear probing
    while (hashtable->keys[index] != 0 && hashtable->keys[index] != key)
    {
        index = (index + 1) % hashtable->size;
    }

    // Add new key-value pair to hash table
    if (hashtable->keys[index] == 0)
    {
        hashtable->keys[index] = key;
        hashtable->count++;
    }
    hashtable->values[index] = value;
    return FUNCTION_SUCCESS;
}

// Look up the value associated with a given key in the hash table
int hashtable_lookup(HashTable* hashtable, long long key)
{
    // Convert key to positive long long for hashing purposes
    key = key + 2147483647LL;

    // Get index of slot in hash table using modulo operator
    int index = key % hashtable->size;

    // Handle collisions using linear probing
    while (hashtable->keys[index] != 0)
    {
        if (hashtable->keys[index] == key)
        {
            return hashtable->values[index];
        }
        index = (index + 1) % hashtable->size;
    }

    // Key not found in hash table
    return -1;
}
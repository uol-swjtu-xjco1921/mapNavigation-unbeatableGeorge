#include <stdio.h>
#include <stdlib.h>

#include "hashTable.h"

void init_hashtable(HashTable* hashtable) 
{
    hashtable->size = MAX_HASH_SIZE;
    hashtable->count = 0;
    hashtable->keys = (long long*)calloc(hashtable->size, sizeof(long long));
    hashtable->values = (int*)calloc(hashtable->size, sizeof(int));
    if (hashtable->keys == NULL || hashtable->values == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for hash table\n");
        exit(1);
    }
}

void free_hashtable(HashTable* hashtable) 
{
    if (hashtable->keys != NULL)
        free(hashtable->keys);
    if (hashtable->values != NULL)
        free(hashtable->values);
}

void hashtable_add(HashTable* hashtable, long long key, int value) 
{
    key = key + 2147483647LL;
    int index = key % hashtable->size;
    while (hashtable->keys[index] != 0 && hashtable->keys[index] != key) 
    {
        index = (index + 1) % hashtable->size;
    }
    if (hashtable->keys[index] == 0) 
    {
        hashtable->keys[index] = key;
        hashtable->count++;
    }
    hashtable->values[index] = value;
}

int hashtable_lookup(HashTable* hashtable, long long key) 
{
    key = key + 2147483647LL;
    int index = key % hashtable->size;
    while (hashtable->keys[index] != 0) 
    {
        if (hashtable->keys[index] == key) 
        {
            return hashtable->values[index];
        }
        index = (index + 1) % hashtable->size;
    }
    return -1;
}
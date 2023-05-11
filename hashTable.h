#define MAX_HASH_SIZE 10000

typedef struct hash_table_t {
    int size;  // Hash table size
    int count; // Number of elements
    long long* keys;
    int* values;
} HashTable;

void init_hashtable(HashTable* hashtable);
void free_hashtable(HashTable* hashtable);
int hashtable_add(HashTable* hashtable, long long key, int value);
int hashtable_lookup(HashTable* hashtable, long long key);
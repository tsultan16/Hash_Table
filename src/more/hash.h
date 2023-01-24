

// hash table struct
typedef struct hashtable_st
{
    uint64_t * capacity;       // max number of items
    uint64_t * nvals_per_item; // number of values per item
    uint64_t ** items;         // array of pointers 
    
} hashtable_t;


// function prototypes 
uint64_t hash(const uint64_t key, const uint64_t table_capacity);

void init_hash_table(hashtable_t *table, const void *base_ptr, const uint64_t table_capacity, const uint64_t nvals_per_item);

bool insert_item(hashtable_t *table,  const uint64_t key, const uint64_t *values);

int64_t lookup_item(const hashtable_t *table, const uint64_t key);

bool delete_item(const hashtable_t *table, const uint64_t key);

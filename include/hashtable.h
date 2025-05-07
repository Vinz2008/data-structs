#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// TODO : macros

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

#define HASHTABLE_DEF(value_type) \
    typedef struct { \
        const char* key;  /*key is NULL if this slot is empty*/ \
        value_type value; \
    } hashtable_entry_##value_type; \
    \
    typedef struct { \
        hashtable_entry_##value_type* entries;  /*hash slots*/ \
        int capacity;    /*size of _entries array*/ \
        int length;      /*number of items in hash table*/ \
    } hashtable_##value_type##_t; \
    \
    hashtable_##value_type##_t hashtable_##value_type##_create(int capacity){ \
        return (hashtable_##value_type##_t){ \
            .length = 0, \
            .capacity = capacity, \
            .entries = calloc(capacity, sizeof(hashtable_entry_##value_type)) \
        }; \
    } \
    \
    value_type* hashtable_##value_type##_get(hashtable_##value_type##_t table, const char* key){ \
        uint64_t hash = hash_key(key); \
        size_t index = (size_t)(hash & (uint64_t)(table.capacity - 1)); \
        \
        while (table.entries[index].key != NULL){ \
            if (strcmp(key, table.entries[index].key) == 0){ \
                return &table.entries[index].value; \
            } \
            index++; \
            if (index >= table.capacity){ \
                /* wrap around*/ \
                index = 0; \
            } \
        } \
        \
        return NULL; \
    } \
    \
    static const char* hashtable_##value_type##_set_entry(hashtable_entry_##value_type* entries, size_t capacity, const char* key, value_type value, int* table_length){ \
        uint64_t hash = hash_key(key); \
        size_t index = (size_t)(hash & (uint64_t)(capacity - 1)); \
        while (entries[index].key != NULL){ \
            if (strcmp(key, entries[index].key) == 0){ \
                entries[index].value = value; \
                return entries[index].key; \
            } \
            index++; \
            if (index >= capacity){ \
                index = 0; \
            } \
        } \
        if (table_length){ \
            key = strdup(key); \
            (*table_length)++; \
        } \
        entries[index].key = key; \
        entries[index].value = value; \
        return key; \
    } \
    \
    static bool hashtable_##value_type##_expand(hashtable_##value_type##_t* table) { \
        /* Allocate new entries array.*/ \
        size_t new_capacity = table->capacity * 2; \
        if (new_capacity < table->capacity){ \
            /*overflow*/ \
            return false; \
        } \
        \
        hashtable_entry_##value_type* new_entries = calloc(new_capacity, sizeof(hashtable_entry_##value_type)); \
        \
        /*Iterate entries, move all non-empty ones to new table's entries.*/ \
        for (size_t i = 0; i < table->capacity; i++) { \
            hashtable_entry_##value_type entry = table->entries[i]; \
            if (entry.key != NULL) { \
                hashtable_##value_type##_set_entry(new_entries, new_capacity, entry.key, entry.value, NULL); \
            } \
        } \
        \
        /*Free old entries array and update this table's details.*/ \
        free(table->entries); \
        table->entries = new_entries; \
        table->capacity = new_capacity; \
        return true; \
    } \
    \
    /*return the key*/ \
    const char* hashtable_##value_type##_set(hashtable_##value_type##_t* table, const char* key, value_type value){ \
        assert(key != NULL); \
        if (table->length >= table->capacity / 2){ \
            if (!hashtable_##value_type##_expand(table)){ \
                return NULL; \
            } \
        } \
        return hashtable_##value_type##_set_entry(table->entries, table->capacity, key, value, &table->length); \
    } \
    \
    /*TODO : remove from hashtable*/ \
    \
    void hashtable_##value_type##_destroy(hashtable_##value_type##_t table){ \
        for (size_t i = 0; i < table.capacity; i++){ \
            free((void*)table.entries[i].key); \
        } \
        \
        free(table.entries); \
    } \
    
// TODO : hashtable iterator

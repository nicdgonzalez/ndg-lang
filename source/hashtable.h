#ifndef NDGLANG_HASHTABLE_H_
# define NDGLANG_HASHTABLE_H_

# include <stdint.h>

struct Bucket {
  struct Bucket *previous;
  struct Bucket *next;
  const char *key;
  void *value;
};

struct HashTable {
  uint64_t capacity;
  uint64_t count;
  struct Bucket *table;
};

void hashtable_init(struct HashTable *);
void hashtable_insert(struct HashTable *, const char *, void *);
void * hashtable_search(struct HashTable *, const char *);
void hashtable_remove(struct HashTable *, const char *);

#endif  // !NDGLANG_HASHTABLE_H_
